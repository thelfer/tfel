/*!
 * \file   tests/Utilities/UnicodeSupportTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   23 janv. 2017
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

#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"

struct UnicodeSupportTest final : public tfel::tests::TestCase {
  UnicodeSupportTest()
      : tfel::tests::TestCase("TFEL/UnicodeSupport", "UnicodeSupportTest") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute()
 private:
  void test1() {
    using namespace tfel::unicode;
    for (const auto& ucd : getSupportedUnicodeCharactersDescriptions()) {
      TFEL_TESTS_ASSERT(getMangledString(ucd.uc) == ucd.m);
    }
  }
  void test2() {
    using namespace tfel::unicode;
    TFEL_TESTS_ASSERT(getMangledString("εᵗᵒ") ==
                      "tum_03B5__tum_1D57__tum_1D52__");
    TFEL_TESTS_ASSERT(getMangledString("εᵉˡ") ==
                      "tum_03B5__tum_1D49__tum_02E1__");
  };
};

TFEL_TESTS_GENERATE_PROXY(UnicodeSupportTest, "UnicodeSupport");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("UnicodeSupport.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
