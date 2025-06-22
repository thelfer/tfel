/*!
 * \file   test-cxx
 * \author Helfer Thomas
 * \date   16 jan 2010
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

#ifdef __CYGWIN__
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif /* __CYGWIN__ */

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "ErrnoHandlingCheck-cxx.hxx"
#include "VanadiumAlloy_YoungModulus_SRMA-cxx.hxx"
#include "VanadiumAlloy_PoissonRatio_SRMA-cxx.hxx"

struct CxxMaterialPropertyInterfaceTest final : public tfel::tests::TestCase {
  CxxMaterialPropertyInterfaceTest()
      : tfel::tests::TestCase("TFEL/Math", "CxxMaterialPropertyInterfaceTest") {
  }  // end of CxxMaterialPropertyInterfaceTest

  virtual tfel::tests::TestResult execute() override {
    const auto y = [](const double T) {
      return 127.8e9 * (1. - 7.825e-5 * (T - 293.15));
    };
    const auto n = [](const double T) {
      return 0.3272 * (1. - 3.056e-5 * (T - 293.15));
    };
    const auto mp_y = mfront::VanadiumAlloy_YoungModulus_SRMA();
    const auto mp_n = mfront::VanadiumAlloy_PoissonRatio_SRMA();
    TFEL_TESTS_ASSERT(std::abs(mp_y(900) - y(900)) < 1.e-14 * y(900));
    TFEL_TESTS_ASSERT(std::abs(mp_n(900) - n(900)) < 1.e-14 * n(900));
    TFEL_TESTS_CHECK_THROW(mp_y(-900), std::range_error);
    TFEL_TESTS_CHECK_THROW(mp_n(-900), std::range_error);
#if (!defined _WIN32)
    const auto e_c = mfront::ErrnoHandlingCheck();
    unsetenv("OUT_OF_BOUNDS_POLICY");
    TFEL_TESTS_ASSERT(std::abs(mp_y(50) - y(50)) < 1.e-14 * y(50));
    setenv("OUT_OF_BOUNDS_POLICY", "NONE", 1);
    TFEL_TESTS_ASSERT(std::abs(mp_y(50) - y(50)) < 1.e-14 * y(50));
    setenv("OUT_OF_BOUNDS_POLICY", "WARNING", 1);
    TFEL_TESTS_ASSERT(std::abs(mp_y(50) - y(50)) < 1.e-14 * y(50));
    setenv("OUT_OF_BOUNDS_POLICY", "STRICT", 1);
    TFEL_TESTS_CHECK_THROW(mp_y(50), std::range_error);
    TFEL_TESTS_CHECK_THROW(mp_n(50), std::range_error);
    TFEL_TESTS_CHECK_THROW(e_c(-2), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(e_c(2), std::runtime_error);
#endif
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(CxxMaterialPropertyInterfaceTest,
                          "CxxMaterialPropertyInterfaceTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("CxxMaterialPropertyInterface.xml");
  const auto r = manager.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
