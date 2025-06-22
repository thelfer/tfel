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

struct CxxTokenizerTest2 final : public tfel::tests::TestCase {
  CxxTokenizerTest2()
      : tfel::tests::TestCase("TFEL/Utilities", "CxxTokenizerTest2") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    this->check("a+b", {"a", "+", "b"});
    this->check("a+=b", {"a", "+=", "b"});
    this->check("a-b", {"a", "-", "b"});
    this->check("a-=b", {"a", "-=", "b"});
    this->check("a%b", {"a", "%", "b"});
    this->check("a%=b", {"a", "%=", "b"});
    this->check("a|b", {"a", "|", "b"});
    this->check("a|=b", {"a", "|=", "b"});
    this->check("a>b", {"a", ">", "b"});
    this->check("a>=b", {"a", ">=", "b"});
    this->check("a<b", {"a", "<", "b"});
    this->check("a<=b", {"a", "<=", "b"});
    this->check("a:b", {"a", ":", "b"});
    this->check("a::b", {"a", "::", "b"});
    this->check2();
    this->check3();
    return this->result;
  }  // end of execute()
 private:
  using Token = tfel::utilities::Token;
  using CxxTokenizer = tfel::utilities::CxxTokenizer;
  void check(const std::string& s, const std::vector<const char*>& values) {
    using size_type = std::vector<const char*>::size_type;
    CxxTokenizer t;
    t.parseString(s);
    TFEL_TESTS_ASSERT(t.size() == values.size());
    if (t.size() != values.size()) {
      return;
    }
    for (size_type i = 0; i != t.size(); ++i) {
      TFEL_TESTS_ASSERT(t[i].value == values[i]);
      TFEL_TESTS_ASSERT(t[i].flag == Token::Standard);
    }
  }
  void check2() {
    CxxTokenizer t;
    TFEL_TESTS_CHECK_THROW(t.parseString("'a"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString("'\a"), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(t.parseString("'\ab'"), std::runtime_error);
    t.parseString("'\a'");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "'\a'");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Char);
    t.parseString("'a'");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].value == "'a'");
    TFEL_TESTS_ASSERT(t[0].flag == Token::Char);
  }
  void check3() {
    CxxTokenizer t;
    TFEL_TESTS_CHECK_THROW(t.parseString("a\\n"), std::runtime_error);
    //    t.parseString("a\\nb"),std::runtime_error);
  }
};

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerTest2, "CxxTokenizer2");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CxxTokenizer2.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
