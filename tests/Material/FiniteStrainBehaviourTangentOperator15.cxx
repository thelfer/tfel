/*!
 * \file  tests/Material/FiniteStrainBehaviourTangentOperator15.cxx
 * \brief direct conversions of DTAU_DF and DTAU_DDF to DPK1_DF, compared to
 * the conversions through the derivative of the Cauchy stress
 * \date 21/11/2026
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
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

struct FiniteStrainBehaviourTangentOperator15 final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator15()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator15") {
  }  // end of FiniteStrainBehaviourTangentOperator15
  tfel::tests::TestResult execute() override {
    this->check<1u>();
    this->check<2u>();
    this->check<3u>();
    return this->result;
  }  // end of execute

 private:
  template <unsigned short N>
  void check() {
    using namespace tfel::material;
    using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
    using real = double;
    using stensor = tfel::math::stensor<N, real>;
    using tensor = tfel::math::tensor<N, real>;
    using st2tost2 = tfel::math::st2tost2<N, real>;
    using t2tot2 = tfel::math::t2tot2<N, real>;
    const real l0 = 1.09465e+11;
    const real m0 = 5.6391e+10;
    const real eps = 1.e-12 * m0;
    // Cauchy stress and spatial moduli of a neo-Hookean behaviour
    const auto sigma = [&l0, &m0](const tensor& F) -> stensor {
      const auto J = tfel::math::det(F);
      const auto B = tfel::math::computeLeftCauchyGreenTensor(F);
      const auto Id = stensor::Id();
      return (l0 * std::log(J) * Id + m0 * (B - Id)) / J;
    };
    const auto Cs = [&l0, &m0](const tensor& F) -> st2tost2 {
      const auto J = tfel::math::det(F);
      const auto m = m0 - l0 * std::log(J);
      return l0 * st2tost2::IxI() + 2 * m * st2tost2::Id();
    };
    const auto check_equal = [this, &eps](const t2tot2& a, const t2tot2& b) {
      for (unsigned short i = 0; i != tfel::math::TensorDimeToSize<N>::value;
           ++i) {
        for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value;
             ++j) {
          const auto e = std::abs(a(i, j) - b(i, j));
          if (e > eps) {
            std::cout << a(i, j) << " " << b(i, j) << " " << e << " " << eps
                      << std::endl;
          }
          TFEL_TESTS_ASSERT(e < eps);
        }
      }
    };
    const real v0[9u] = {1.01, 0.995,  1.02,  0.012, -0.007,
                         0.02, -0.011, 0.004, -0.009};
    const real v1[9u] = {1.03, 0.98,   1.09,   0.03, -0.012,
                         0.04, -0.028, -0.015, 0.005};
    const auto F0s = {tensor::Id(), tensor::Id(), tensor{v0}};
    const auto F1s = {tensor::Id(), tensor{v1}, tensor{v1}};
    auto pF0 = F0s.begin();
    for (const auto& F1 : F1s) {
      const auto& F0 = *pF0;
      ++pF0;
      const auto s = sigma(F1);
      const auto dtau =
          convert<TangentOperator::DTAU_DF, TangentOperator::SPATIAL_MODULI>(
              Cs(F1), F0, F1, s);
      // reference: conversion through the derivative of the Cauchy stress
      const auto dsig =
          convert<TangentOperator::DSIG_DF, TangentOperator::DTAU_DF>(dtau, F0,
                                                                      F1, s);
      const auto dP =
          convert<TangentOperator::DPK1_DF, TangentOperator::DSIG_DF>(dsig, F0,
                                                                      F1, s);
      // direct conversion of DTAU_DF
      check_equal(convert<TangentOperator::DPK1_DF, TangentOperator::DTAU_DF>(
                      dtau, F0, F1, s),
                  dP);
      // direct conversion of DTAU_DDF
      const auto dtau_ddF =
          convert<TangentOperator::DTAU_DDF, TangentOperator::DTAU_DF>(
              dtau, F0, F1, s);
      check_equal(convert<TangentOperator::DPK1_DF, TangentOperator::DTAU_DDF>(
                      dtau_ddF, F0, F1, s),
                  dP);
    }
  }  // end of check
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator15,
                          "FiniteStrainBehaviourTangentOperator15");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator15.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
