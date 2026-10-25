/*!
 * \file   tests/Material/StressCriteriaTest.cxx
 * \brief  This file tests some stress criteria in particular stress states
 * \date   26/10/2026
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
#include <tuple>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Material/GursonTvergaardNeedleman1982StressCriterion.hxx"

struct StressCriteriaTest final : public tfel::tests::TestCase {
  StressCriteriaTest()
      : tfel::tests::TestCase("TFEL/Material", "StressCriteriaTest") {
  }  // end of StressCriteriaTest
  tfel::tests::TestResult execute() override {
    this->testGursonTvergaardNeedleman1982();
    return this->result;
  }  // end of execute
 private:
  using Stensor = tfel::math::stensor<3u, double>;
  //! \brief the GTN criterion is even in the mean stress
  void testGursonTvergaardNeedleman1982() {
    using namespace tfel::material;
    const auto params =
        GursonTvergaardNeedleman1982StressCriterionParameters<Stensor>{
            .f_c = 0.05, .f_r = 0.2, .q_1 = 1.5, .q_2 = 1, .q_3 = 2.25};
    const auto [s1, n1, ds1_df] =
        computeGursonTvergaardNeedleman1982StressNormal(
            Stensor{100, 100, 100, 0, 0, 0}, 0.01, params, 1e-8);
    const auto [s2, n2, ds2_df] =
        computeGursonTvergaardNeedleman1982StressNormal(
            Stensor{-100, -100, -100, 0, 0, 0}, 0.01, params, 1e-8);
    TFEL_TESTS_ASSERT(std::abs(s2 - s1) < 1e-12 * s1);
    TFEL_TESTS_ASSERT(std::abs(n2[0] + n1[0]) < 1e-12);
    TFEL_TESTS_ASSERT(std::abs(ds2_df - ds1_df) < 1e-12 * ds1_df);
  }  // end of testGursonTvergaardNeedleman1982
};

TFEL_TESTS_GENERATE_PROXY(StressCriteriaTest, "StressCriteriaTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StressCriteriaTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
