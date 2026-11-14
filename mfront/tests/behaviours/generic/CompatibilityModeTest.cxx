/*!
 * \file   mfront/tests/behaviours/generic/CompatibilityModeTest.cxx
 * \brief  tests of the integration drivers of the `generic` interface which
 * can't be checked by `MTest`: integrations returning a time step scaling
 * factor lower than one, post-processings and initialize functions.
 * \date   20/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <array>
#include <string>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"

struct CompatibilityModeTest final : public tfel::tests::TestCase {
  CompatibilityModeTest()
      : tfel::tests::TestCase("TFEL/System/ExternalLibraryManager",
                              "CompatibilityModeTest"),
        library([] {
          const auto e = ::getenv("MFrontGenericBehavioursBuildPath");
          return e == nullptr ? std::string{} : e;
        }()) {}  // end of CompatibilityModeTest

  tfel::tests::TestResult execute() override {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto v1 =
        elm.getCompatibilityMode(this->library, "PhysicalConstants_v5_2_0");
    TFEL_TESTS_CHECK_EQUAL(v1, "5.2.0");
    const auto v2 =
        elm.getCompatibilityMode(this->library, "PhysicalConstants_v6_0_0");
    TFEL_TESTS_CHECK_EQUAL(v2, "future");
    return this->result;
  }  // end of execute

 private:
  //! \brief path to the library
  const std::string library;
};  // end of struct CompatibilityModeTest

TFEL_TESTS_GENERATE_PROXY(CompatibilityModeTest, "CompatibilityModeTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CompatibilityModeTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
