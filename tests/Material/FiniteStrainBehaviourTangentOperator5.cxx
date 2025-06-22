/*!
 * \file  tests/Material/FiniteStrainBehaviourTangentOperator5.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 août 2016
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

struct FiniteStrainBehaviourTangentOperator5 final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator5()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator5") {
  }  // end of FiniteStrainBehaviourTangentOperator5
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
    using t2tost2 = tfel::math::t2tost2<N, real>;
    using st2tost2 = tfel::math::st2tost2<N, real>;
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
    const auto D = [&Cs,
                    &svk](const tfel::math::tensor<N, real>& F) -> t2tost2 {
      const auto J = det(F);
      const auto s = svk(F);
      const auto t = s * J;
      const auto CtJ =
          tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Cs(F), t);
      const auto Dt = tfel::math::
          ConvertKirchhoffStressJaumanRateModuliToKirchhoffStressDerivative<
              N, double>::exe(CtJ, F, t);
      const auto dJ = tfel::math::computeDeterminantDerivative(F);
      return (Dt - (s ^ dJ)) / J;
    };
    for (const tensor& F :
         {tensor::Id(), tensor{1.03, 0.98, 1.09, 0.03, -0.012, 0.04, -0.028,
                               -0.015, 0.005}}) {
      const t2tost2 nD = this->getNumericalApproximation(svk, F, 1.e-5);
      const t2tost2 aD = D(F);
      for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value;
           ++i) {
        for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value;
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
  template <unsigned short N, typename Behaviour, typename real>
  tfel::math::t2tost2<N, real> getNumericalApproximation(
      const Behaviour& b, const tfel::math::tensor<N, real>& F, const real e) {
    tfel::math::t2tost2<N, real> r;
    for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value;
         ++j) {
      auto Fp = F;
      auto Fm = F;
      Fp(j) += e;
      Fm(j) -= e;
      const auto sp = b(Fp);
      const auto sm = b(Fm);
      const auto ds = (sp - sm) / (2 * e);
      for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value;
           ++i) {
        r(i, j) = ds(i);
      }
    }
    return r;
  }
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator5,
                          "FiniteStrainBehaviourTangentOperator5");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator5.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
