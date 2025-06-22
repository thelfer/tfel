/*!
 * \file   CxxTokenizerTest.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   24 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"

struct CxxTokenizerTest final : public tfel::tests::TestCase {
  CxxTokenizerTest()
      : tfel::tests::TestCase("TFEL/Utilities", "CxxTokenizerTest") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::utilities;
    CxxTokenizer t;
    // C++ style comments
    t.parseString("///*");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "/*");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    t.parseString("test //! a comment \"with string\"");
    TFEL_TESTS_ASSERT(t.size() == 2u);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[1].value == "a comment \"with string\"");
    TFEL_TESTS_ASSERT(t[1].flag == Token::DoxygenComment);
    t.stripComments();
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    t.parseString("test //!< a comment \"with string\"");
    TFEL_TESTS_ASSERT(t.size() == 2u);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[1].value == "a comment \"with string\"");
    TFEL_TESTS_ASSERT(t[1].flag == Token::DoxygenBackwardComment);
    t.stripComments();
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    t.parseString("//!< a comment \"with string\"");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "a comment \"with string\"");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    // C style comments
    t.parseString("/*< a comment \"with string\" */");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "< a comment \"with string\" ");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    t.parseString("/*< a comment // C++ comment */");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "< a comment // C++ comment ");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    t.parseString("/*!< a comment // C++ comment */");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "a comment // C++ comment ");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    t.parseString("test /*! a comment // C++ comment */");
    TFEL_TESTS_ASSERT(t.size() == 2u);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[1].value == "a comment // C++ comment ");
    TFEL_TESTS_ASSERT(t[1].flag == Token::DoxygenComment);
    t.stripComments();
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    t.parseString("test /*!< a comment // C++ comment */");
    TFEL_TESTS_ASSERT(t.size() == 2u);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[1].value == "a comment // C++ comment ");
    TFEL_TESTS_ASSERT(t[1].flag == Token::DoxygenBackwardComment);
    t.stripComments();
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    /* strings */
    t.parseString("a b c d");
    TFEL_TESTS_ASSERT(t.size() == 4u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[0].value == "a");
    TFEL_TESTS_ASSERT(t[1].value == "b");
    TFEL_TESTS_ASSERT(t[2].value == "c");
    TFEL_TESTS_ASSERT(t[3].value == "d");
    // unfinished string
    TFEL_TESTS_CHECK_THROW(t.parseString("\"test"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString("'test"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString("'te'"), std::runtime_error);
    t.parseString("\"string test\"");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t.begin()->flag == Token::String);
    TFEL_TESTS_ASSERT(t.begin()->value == "\"string test\"");
    t.treatCharAsString(true);
    t.parseString("'string test'");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::String);
    TFEL_TESTS_ASSERT(t[0].value == "'string test'");
    t.treatCharAsString(false);
    TFEL_TESTS_CHECK_THROW(t.parseString("'string test'"), std::runtime_error);
    t.parseString("'c'");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Char);
    TFEL_TESTS_ASSERT(t[0].value == "'c'");
    t.parseString("'\\''");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Char);
    TFEL_TESTS_ASSERT(t[0].value == "'\\''");
    // t.parseString("'\\\\'");
    // TFEL_TESTS_ASSERT(t.size()==1u);
    // TFEL_TESTS_ASSERT(t[0].flag==Token::Char);
    // TFEL_TESTS_ASSERT(t[0].value=="'\\'");
    // t.parseString("'\\\\"'");
    // TFEL_TESTS_ASSERT(t.size()==1u);
    // TFEL_TESTS_ASSERT(t[0].flag==Token::Char);
    // TFEL_TESTS_ASSERT(t[0].value=="'\"'");
    // t.parseString("\"\\\"\"");
    // TFEL_TESTS_ASSERT(t.size()==1u);
    // TFEL_TESTS_ASSERT(t[0].flag==Token::Char);
    // TFEL_TESTS_ASSERT(t[0].value=="\"\\\"\"");
    // numbers
    t.parseString("12.3a");
    TFEL_TESTS_ASSERT(t.size() == 2u);
    TFEL_TESTS_ASSERT(t[0].value == "12.3");
    TFEL_TESTS_ASSERT(t[1].value == "a");
    t.parseString("12.3a+");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[0].value == "12.3");
    TFEL_TESTS_ASSERT(t[1].value == "a");
    TFEL_TESTS_ASSERT(t[2].value == "+");
    t.parseString("12.3+a");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[0].value == "12.3");
    TFEL_TESTS_ASSERT(t[1].value == "+");
    TFEL_TESTS_ASSERT(t[2].value == "a");
    this->check(".3");
    this->check(".3f");
    this->check(".3e+11");
    this->check(".3e-11");
    this->check(".3e-11f");
    this->check(".3e-11F");
    this->check(".3e-11l");
    this->check(".3e-11L");
    TFEL_TESTS_CHECK_THROW(t.parseString("0.3LL"), std::runtime_error);
    t.parseString("a=2.e-5");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[0].value == "a");
    TFEL_TESTS_ASSERT(t[1].value == "=");
    TFEL_TESTS_ASSERT(t[2].value == "2.e-5");
    // C++14 user defined litterals
    t.parseString("2.e-5_kg");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5_kg");
    t.parseString("2.e-5l_kg");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5l_kg");
    t.parseString("2.e-5L_kg");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5L_kg");
    t.parseString("2.e-5f_kg");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5f_kg");
    t.parseString("2.e-5F_kg");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5F_kg");
    t.parseString("2.e-5F_banana+3.5_potato");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5F_banana");
    TFEL_TESTS_ASSERT(t[1].value == "+");
    TFEL_TESTS_ASSERT(t[2].value == "3.5_potato");
    // t.parseString("-.");
    // TFEL_TESTS_ASSERT(t.size()==2u);
    // TFEL_TESTS_ASSERT(t[0].value=="-");
    // TFEL_TESTS_ASSERT(t[1].value==".");
    TFEL_TESTS_CHECK_THROW(t.parseString("0.3.3"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString(".3.3"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString(".3f.3"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString(".3F.3"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString(".3l.3"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString(".3L.3"), std::runtime_error);
    // integers
    this->check("2");
    this->check("-2");
    this->check("2l");
    this->check("-2L");
    this->check("2ll");
    this->check("-2LL");
    this->check("2u");
    // invalid unsigned integers
    TFEL_TESTS_CHECK_THROW(t.parseString("-2U"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString("-2UL"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString("-2ULL"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString("-2LU"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString("-2LLU"), std::runtime_error);
    // binary integer
    this->check("0b0101");
    TFEL_TESTS_CHECK_THROW(t.parseString("0b"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString("0b2"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString("0b020"), std::runtime_error);
    return this->result;
  }  // end of execute()
 private:
  void check(const char* const s) {
    tfel::utilities::CxxTokenizer t;
    t.parseString(s);
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == s);
  }
};

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerTest, "CxxTokenizer");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("CxxTokenizer.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
