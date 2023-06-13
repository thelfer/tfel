/*!
 * \file   tests/Utilities/CxxTokenizerTest3.cxx
 * \brief
 * \date   09/03/2023
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

struct CxxTokenizerTest3 final : public tfel::tests::TestCase {
  CxxTokenizerTest3()
      : tfel::tests::TestCase("TFEL/Utilities", "CxxTokenizerTest3") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    this->checkInteger("1000", 1000);
    this->checkInteger("1'000", 1000);
    return this->result;
  }  // end of execute()
 private:
  void checkInteger(const std::string& s, const int v) {
    tfel::utilities::CxxTokenizer t;
    t.parseString(s);
    TFEL_TESTS_ASSERT(t.size() == 1);
    if (t.size() != 1u) {
      return;
    }
    auto p = t.begin();
    TFEL_TESTS_ASSERT(t.readInt(p, t.end()) == v);
  }
};

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerTest3, "CxxTokenizer3");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CxxTokenizer3.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
