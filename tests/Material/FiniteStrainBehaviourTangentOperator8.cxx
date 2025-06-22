/*!
 * \file  tests/Material/FiniteStrainBehaviourTangentOperator8.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 août 2016
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
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tot2.hxx"
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Math/T2toT2/T2toT2ConceptIO.hxx"
#include "TFEL/Math/T2toST2/T2toST2ConceptIO.hxx"
#include "TFEL/Math/ST2toST2/ConvertSpatialModuliToKirchhoffJaumanRateModuli.hxx"
#include "TFEL/Math/T2toST2/ConvertKirchhoffStressJaumanRateModuliToKirchhoffStressDerivative.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

struct FiniteStrainBehaviourTangentOperator8 final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator8()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator8") {
  }  // end of FiniteStrainBehaviourTangentOperator8
  virtual tfel::tests::TestResult execute() override {
    this->check<1u>();
    this->check<2u>();
    this->check<3u>();
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void check() {
    using real = double;
    using stensor = tfel::math::stensor<N, real>;
    using tensor = tfel::math::tensor<N, real>;
    using st2tost2 = tfel::math::st2tost2<N, real>;
    using size_type = unsigned short;
    using TangentOperator =
        tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    const real l0 = 1.09465e+11;
    const real m0 = 5.6391e+10;
    const real eps = 5.e-10 * m0;
    auto svk = [&l0, &m0](const tfel::math::tensor<N, real>& F) -> stensor {
      const auto e = computeGreenLagrangeTensor(F);
      const auto S = l0 * trace(e) * stensor::Id() + 2 * m0 * e;
      return convertSecondPiolaKirchhoffStressToCauchyStress(S, F);
    };
    // spatial moduli
    const auto Cs = [&l0,
                     &m0](const tfel::math::tensor<N, real>& F) -> st2tost2 {
      const auto CSE = l0 * st2tost2::IxI() + 2 * m0 * st2tost2::Id();
      return tfel::math::push_forward(CSE, F);
    };
    // compute the spatial moduli through the Kirchhoff stress derivative
    const auto D = [this, &svk, &l0,
                    &m0](const tfel::math::tensor<N, real>& F) -> st2tost2 {
      const auto s = svk(F);
      const auto CSE = l0 * st2tost2::IxI() + 2 * m0 * st2tost2::Id();
      const auto DS_DC = tfel::material::convert<TangentOperator::DS_DC,
                                                 TangentOperator::DS_DEGL>(
          CSE, tensor::Id(), F, s);
      const auto DS_DF = tfel::material::convert<TangentOperator::DS_DF,
                                                 TangentOperator::DS_DC>(
          DS_DC, tensor::Id(), F, s);
      const auto Dt = tfel::material::convert<TangentOperator::DTAU_DF,
                                              TangentOperator::DS_DF>(
          DS_DF, tensor::Id(), F, s);
      return tfel::material::convert<TangentOperator::SPATIAL_MODULI,
                                     TangentOperator::DTAU_DF>(Dt, tensor::Id(),
                                                               F, s);
    };
    for (const tensor& F :
         {tensor::Id(), tensor{1.03, 0.98, 1.09, 0.03, -0.012, 0.04, -0.028,
                               -0.015, 0.005}}) {
      const auto nD = Cs(F);
      const auto aD = D(F);
      for (size_type i = 0; i != tfel::math::StensorDimeToSize<N>::value; ++i) {
        for (size_type j = 0; j != tfel::math::StensorDimeToSize<N>::value;
             ++j) {
          if (std::abs(aD(i, j) - nD(i, j)) > eps) {
            std::cout << i << " " << j << " " << aD(i, j) << " " << nD(i, j)
                      << " " << aD(i, j) - nD(i, j) << " " << eps << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(aD(i, j) - nD(i, j)) < eps);
        }
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator8,
                          "FiniteStrainBehaviourTangentOperator8");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator8.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
