/*!
 * \file   CxxTokenizerTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date 24/08/2015
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
    t.clear();
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
    t.clear();
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
    TFEL_TESTS_ASSERT(t[0].comment == "a comment \"with string\"");
    t.clear();
    t.parseString("//!< a comment \"with string\"");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "a comment \"with string\"");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    // C style comments
    t.clear();
    t.parseString("/*< a comment \"with string\" */");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "< a comment \"with string\"");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    t.clear();
    t.parseString("/*< a comment // C++ comment*/");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "< a comment // C++ comment");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    t.clear();
    t.parseString("/*!< a comment // C++ comment */");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "a comment // C++ comment");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    t.clear();
    t.parseString("test /*! a comment // C++ comment */");
    TFEL_TESTS_ASSERT(t.size() == 2u);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[1].value == "a comment // C++ comment");
    TFEL_TESTS_ASSERT(t[1].flag == Token::DoxygenComment);
    t.stripComments();
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    t.clear();
    t.parseString("test /*!< a comment // C++ comment */");
    TFEL_TESTS_ASSERT(t.size() == 2u);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[1].value == "a comment // C++ comment");
    TFEL_TESTS_ASSERT(t[1].flag == Token::DoxygenBackwardComment);
    t.stripComments();
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[0].value == "test");
    TFEL_TESTS_ASSERT(t[0].comment == "a comment // C++ comment");
    /* strings */
    t.clear();
    t.parseString("a b c d");
    TFEL_TESTS_ASSERT(t.size() == 4u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Standard);
    TFEL_TESTS_ASSERT(t[0].value == "a");
    TFEL_TESTS_ASSERT(t[1].value == "b");
    TFEL_TESTS_ASSERT(t[2].value == "c");
    TFEL_TESTS_ASSERT(t[3].value == "d");
    // unfinished string
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("\"test"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("'test"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("'te'"), std::runtime_error);
    t.clear();
    t.parseString("\"string test\"");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t.begin()->flag == Token::String);
    TFEL_TESTS_ASSERT(t.begin()->value == "\"string test\"");
    t.treatCharAsString(true);
    t.clear();
    t.parseString("'string test'");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::String);
    TFEL_TESTS_ASSERT(t[0].value == "'string test'");
    t.treatCharAsString(false);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("'string test'"), std::runtime_error);
    t.clear();
    t.parseString("'c'");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Char);
    TFEL_TESTS_ASSERT(t[0].value == "'c'");
    t.clear();
    t.parseString("'\\''");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Char);
    TFEL_TESTS_ASSERT(t[0].value == "'\\''");
    // the escape sequences of a character literal: an escape sequence is a
    // single character of the literal although it holds several characters of
    // the source file, so that `'\x27'` is the `'` character and not an
    // unterminated literal, as `'\''` is. The value of the escape sequence is
    // not checked against that of the character type: `'\400'` is out of the
    // range of a `signed char` and is a diagnostic of a compiler, not an error
    // of the grammar
    for (const auto* const s :
         {"'\\n'", "'\\0'", "'\\\"'", "'\\\\'", "'\\a'", "'\\?'", "'\\q'",
          "'\\x41'", "'\\xff'", "'\\7'", "'\\77'", "'\\101'", "'\\u0041'",
          "'\\u00e9'", "'\\U00000041'"}) {
      t.clear();
      t.parseString(s);
      TFEL_TESTS_ASSERT(t.size() == 1u);
      TFEL_TESTS_ASSERT(t[0].flag == Token::Char);
      TFEL_TESTS_ASSERT(t[0].value == s);
    }
    // a hexadecimal escape sequence without any digit, as `'\x'`, and an
    // incomplete universal character name, as `'\u00zz'`, are rejected
    // although the tokenizer used to accept the first one and to reject both
    // with a message about an unexpected token
    for (const auto* const s :
         {"''", "'\\x'", "'\\xG'", "'\\u00zz'", "'\\08'", "'ab'", "'\\ab'"}) {
      t.clear();
      TFEL_TESTS_CHECK_THROW(t.parseString(s), std::runtime_error);
    }
    // the backslash which introduces an escape sequence is itself introduced by
    // a backslash: the value of the token holds the four characters of the
    // literal
    t.clear();
    t.parseString("'\\\\'");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Char);
    TFEL_TESTS_ASSERT(t[0].value == "'\\\\'");
    // t.clear();
    // t.parseString("'\\\\"'");
    // TFEL_TESTS_ASSERT(t.size()==1u);
    // TFEL_TESTS_ASSERT(t[0].flag==Token::Char);
    // TFEL_TESTS_ASSERT(t[0].value=="'\"'");
    // t.clear();
    //    t.parseString("\"\\\"\"");
    // TFEL_TESTS_ASSERT(t.size()==1u);
    // TFEL_TESTS_ASSERT(t[0].flag==Token::Char);
    // TFEL_TESTS_ASSERT(t[0].value=="\"\\\"\"");
    // numbers
    // an identifier which follows a numeric literal is not a part of this
    // literal: the maximal munch rule of the preprocessing tokens, which states
    // that a token is the longest sequence of characters which can build one,
    // is not applied as strictly as `C++` does it since the tokenizer is also
    // used for the free form texts, as the `@Description` blocks of `MFront`,
    // see issue #1460
    t.clear();
    t.parseString("12.3a");
    TFEL_TESTS_ASSERT(t.size() == 2u);
    TFEL_TESTS_ASSERT(t[0].value == "12.3");
    TFEL_TESTS_ASSERT(t[1].value == "a");
    t.clear();
    t.parseString("12.3a+");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[0].value == "12.3");
    TFEL_TESTS_ASSERT(t[1].value == "a");
    TFEL_TESTS_ASSERT(t[2].value == "+");
    t.clear();
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
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("0.3LL"), std::runtime_error);
    t.clear();
    t.parseString("a=2.e-5");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[0].value == "a");
    TFEL_TESTS_ASSERT(t[1].value == "=");
    TFEL_TESTS_ASSERT(t[2].value == "2.e-5");
    // C++14 user defined litterals
    t.clear();
    t.parseString("2.e-5_kg");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5_kg");
    t.clear();
    t.parseString("2.e-5l_kg");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5l_kg");
    t.clear();
    t.parseString("2.e-5L_kg");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5L_kg");
    t.clear();
    t.parseString("2.e-5f_kg");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5f_kg");
    t.clear();
    t.parseString("2.e-5F_kg");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5F_kg");
    t.clear();
    t.parseString("2.e-5F_banana+3.5_potato");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[0].value == "2.e-5F_banana");
    TFEL_TESTS_ASSERT(t[1].value == "+");
    TFEL_TESTS_ASSERT(t[2].value == "3.5_potato");
    // t.clear();
    // t.parseString("-.");
    // TFEL_TESTS_ASSERT(t.size()==2u);
    // TFEL_TESTS_ASSERT(t[0].value=="-");
    // TFEL_TESTS_ASSERT(t[1].value==".");
    TFEL_TESTS_CHECK_THROW(t.parseString("0.3.3"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString(".3.3"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString(".3f.3"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString(".3F.3"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString(".3l.3"), std::runtime_error);
    t.clear();
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
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("-2U"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("-2UL"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("-2ULL"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("-2LU"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("-2LLU"), std::runtime_error);
    // binary integer
    this->check("0b0101");
    this->check("0b0101u");
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("0b"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("0b2"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("0b020"), std::runtime_error);
    // hexadecimal integer
    this->check("0x1F");
    this->check("0X1fUL");
    // hexadecimal floating point literal, which is supported since C++11: the
    // exponent, which is introduced by `p`, is required, see issue #1460
    this->check("0x1p3");
    this->check("0x1.8p-3");
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("0x1.8"), std::runtime_error);
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
