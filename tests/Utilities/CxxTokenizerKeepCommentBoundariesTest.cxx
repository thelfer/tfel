/*!
 * \file   CxxTokenizerKeepCommentBoundariesTest.cxx
 * \brief
 * \author Thomas Helfer
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

#include <tuple>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"

struct CxxTokenizerKeepCommentBoundariesTest final
    : public tfel::tests::TestCase {
  CxxTokenizerKeepCommentBoundariesTest()
      : tfel::tests::TestCase("TFEL/Utilities",
                              "CxxTokenizerKeepCommentBoundariesTest") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    this->check("/* test */", "/* test */", true);
    this->check("/* test */", "test", false);
    return this->result;
  }  // end of execute()
 private:
  //! a simple alias
  using size_type = tfel::utilities::Token::size_type;
  using Token = tfel::utilities::Token;
  using flags = Token::TokenFlag;
  /*!
   * \param[in] s: string to be parsed
   * \param[in] r: expected result
   * \param[in] b: keep comment boundaries
   */
  void check(const char* const s, const std::string r, const bool b) {
    tfel::utilities::CxxTokenizer tokenizer;
    tokenizer.keepCommentBoundaries(b);
    tokenizer.parseString(s);
    TFEL_TESTS_ASSERT(tokenizer.size() == 1u);
    if (tokenizer.size() != 1u) {
      return;
    }
    const auto& t = *(tokenizer.begin());
    std::cout << "'" << t.value << "'\n"
              << "'" << r << "'\n";
    TFEL_TESTS_ASSERT(t.value == r);
    TFEL_TESTS_ASSERT(t.flag == tfel::utilities::Token::Comment);
  }
};

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerKeepCommentBoundariesTest,
                          "CxxTokenizerKeepCommentBoundaries");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CxxTokenizer.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
