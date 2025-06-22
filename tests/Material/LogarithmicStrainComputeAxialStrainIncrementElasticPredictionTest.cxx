/*!
 * \file   LogarithmicStrainComputeAxialStrainIncrementElasticPredictionTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   14/05/2018
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

#include <cstdlib>
#include <utility>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/st2tot2.hxx"
#include "TFEL/Material/Lame.hxx"
#include "TFEL/Material/LogarithmicStrainComputeAxialStrainIncrementElasticPrediction.hxx"

struct LogarithmicStrainComputeAxialStrainIncrementElasticPredictionTest final
    : public tfel::tests::TestCase {
  LogarithmicStrainComputeAxialStrainIncrementElasticPredictionTest()
      : tfel::tests::TestCase(
            "TFEL/Material",
            "LogarithmicStrainComputeAxialStrainIncrementElasticPrediction") {
  }  // end of LogarithmicStrainComputeAxialStrainIncrementElasticPredictionTest
  tfel::tests::TestResult execute() override {
    test1();
    test2();
    return this->result;
  }

 private:
  void test1() {
    using namespace tfel::math;
    using namespace tfel::material;
    using real = double;
    constexpr const auto eps = real(1.e-4);
    const auto l = computeLambda(150e9, 0.3);
    const auto m = computeMu(150e9, 0.3);
    auto eel = stensor<1u, real>{0.453e-3, -0.32e-3, 0.55e-3};
    auto deto = stensor<1u, real>{-0.22e-3, 0., -0.24e-3};
    auto eto_zz = real(-0.67e-3);
    auto P_zz = real(87e6);
    const auto de = computeAxialStrainIncrementElasticPrediction(
        l, l + 2 * m, l, eel, eto_zz, deto, P_zz, 1., 1.e-12);
    const auto Tzz =
        l * (eel(0) + deto(0) + eel(2) + deto(2)) + (2 * m + l) * (eel(1) + de);
    TFEL_TESTS_ASSERT(std::abs(Tzz / (exp(eto_zz + de)) - P_zz) < eps);
  }
  void test2() {
    using namespace tfel::math;
    using namespace tfel::material;
    using real = double;
    constexpr const auto eps = real(1.e-4);
    constexpr const auto theta = real(0.64);
    const auto l = computeLambda(150e9, 0.3);
    const auto m = computeMu(150e9, 0.3);
    auto eel = stensor<1u, real>{0.453e-3, -0.32e-3, 0.55e-3};
    auto deto = stensor<1u, real>{-0.22e-3, 0., -0.24e-3};
    auto eto_zz = real(-0.67e-3);
    const auto P_zz = real(87e6);
    const auto dP_zz = real(15e6);
    const auto de = computeAxialStrainIncrementElasticPrediction(
        l, l + 2 * m, l, eel, eto_zz, deto, P_zz + theta * dP_zz, theta,
        1.e-12);
    const auto Tzz = l * (eel(0) + theta * deto(0) + eel(2) + theta * deto(2)) +
                     (2 * m + l) * (eel(1) + theta * de);
    TFEL_TESTS_ASSERT(std::abs(Tzz / (exp(eto_zz + theta * de)) - P_zz -
                               theta * dP_zz) < eps);
  }
};

TFEL_TESTS_GENERATE_PROXY(
    LogarithmicStrainComputeAxialStrainIncrementElasticPredictionTest,
    "LogarithmicStrainComputeAxialStrainIncrementElasticPrediction");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput(
      "LogarithmicStrainComputeAxialStrainIncrementElasticPredictionTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
