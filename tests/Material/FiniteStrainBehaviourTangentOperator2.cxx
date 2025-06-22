/*!
 * \file  tests/Material/FiniteStrainBehaviourTangentOperator2.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 17 août 2016
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
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

struct FiniteStrainBehaviourTangentOperator2 final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator2()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator2") {
  }  // end of FiniteStrainBehaviourTangentOperator2
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
    using t2tot2 = tfel::math::t2tot2<N, real>;
    const real l0 = 1.09465e+11;
    const real m0 = 5.6391e+10;
    const real eps = 1.e-10 * m0;
    auto nhb = [&l0, &m0](const tfel::math::tensor<N, real>& F) -> stensor {
      const auto J = tfel::math::det(F);
      const auto B = tfel::math::computeLeftCauchyGreenTensor(F);
      const auto Id = stensor::Id();
      return (l0 * std::log(J) * Id + m0 * (B - Id)) / J;
    };
    // spatial moduli
    const auto Cs = [&l0,
                     &m0](const tfel::math::tensor<N, real>& F) -> st2tost2 {
      const auto J = tfel::math::det(F);
      const auto m = m0 - l0 * std::log(J);
      return l0 * st2tost2::IxI() + 2 * m * st2tost2::Id();
    };
    // dtau_dF
    const auto D = [&Cs,
                    &nhb](const tfel::math::tensor<N, real>& F) -> t2tost2 {
      // not supported by gcc 4.7.2: auto toT2toST2=[](const t2tot2& s) ->
      // t2tost2{
      auto toT2toST2 = [](t2tot2 s) -> t2tost2 {
        t2tost2 r;
        for (unsigned short i = 0; i != 3; ++i) {
          for (unsigned short j = 0;
               j != tfel::math::TensorDimeToSize<N>::value; ++j) {
            r(i, j) = s(i, j);
          }
        }
        for (unsigned short i = 0;
             i != tfel::math::StensorDimeToSize<N>::value - 3; ++i) {
          for (unsigned short j = 0;
               j != tfel::math::TensorDimeToSize<N>::value; ++j) {
            r(3 + i, j) =
                (s(3 + 2 * i, j) + s(3 + 2 * i + 1, j)) / std::sqrt(2);
          }
        }
        return r;
      };
      const auto s = nhb(F);
      const auto t = s * det(F);
      const auto tus = unsyme(t);
      const auto dD = tfel::math::computeRateOfDeformationDerivative(F);
      const auto dW = tfel::math::computeSpinRateDerivative(F);
      const auto CtJ =
          tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Cs(F), t);
      return CtJ * dD + toT2toST2((t2tot2::tpld(tus) - t2tot2::tprd(tus)) * dW);
    };
    for (const tensor& F :
         {tensor::Id(), tensor{1.03, 0.98, 1.09, 0.03, -0.012, 0.04, -0.028,
                               -0.015, 0.005}}) {
      const t2tost2 nD = this->getNumericalApproximation(nhb, F, 1.e-5);
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
      tfel::math::tensor<N, real> Fp = F;
      tfel::math::tensor<N, real> Fm = F;
      Fp(j) += e;
      Fm(j) -= e;
      const tfel::math::stensor<N, real> tp = b(Fp) * det(Fp);
      const tfel::math::stensor<N, real> tm = b(Fm) * det(Fm);
      const tfel::math::stensor<N, real> dt = (tp - tm) / (2 * e);
      for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value;
           ++i) {
        r(i, j) = dt(i);
      }
    }
    return r;
  }
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator2,
                          "FiniteStrainBehaviourTangentOperator2");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator2.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
