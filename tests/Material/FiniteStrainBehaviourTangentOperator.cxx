/*!
 * \file  tests/Material/FiniteStrainBehaviourTangentOperator.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 juin 2014
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

template <unsigned short N, typename Behaviour, typename real>
static tfel::math::t2tot2<N, real> getNumericalApproximation(
    const Behaviour& b, const tfel::math::tensor<N, real>& F, const real e) {
  tfel::math::t2tot2<N, real> r;
  for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value; ++j) {
    tfel::math::tensor<N, real> Fp = F;
    tfel::math::tensor<N, real> Fm = F;
    Fp(j) += e;
    Fm(j) -= e;
    const tfel::math::tensor<N, real> tp = unsyme(b(Fp)) * det(Fp);
    const tfel::math::tensor<N, real> tm = unsyme(b(Fm)) * det(Fm);
    const tfel::math::tensor<N, real> dt = (tp - tm) / (2 * e);
    for (unsigned short i = 0; i != tfel::math::TensorDimeToSize<N>::value;
         ++i) {
      r(i, j) = dt(i);
    }
  }
  return r;
}

struct FiniteStrainBehaviourTangentOperator final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator") {
  }  // end of FiniteStrainBehaviourTangentOperator
  virtual tfel::tests::TestResult execute() override {
    this->check<1u>();
    this->check<2u>();
    this->check<3u>();
    return this->result;
  }  // end of execute
 private:
  // check that the perturbation approximation is ok
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
    const auto D = [&Cs, &nhb](const tfel::math::tensor<N, real>& F) -> t2tot2 {
      auto toT2toT2 = [](t2tost2 s) {
        t2tot2 r;
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
            r(3 + 2 * i, j) = r(3 + 2 * i + 1, j) = s(3 + i, j) / std::sqrt(2);
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
      return toT2toT2(CtJ * dD) + (t2tot2::tpld(tus) - t2tot2::tprd(tus)) * dW;
    };
    const real v1[9u] = {1.03, 0.98,   1.09,   0.03, -0.012,
                         0.04, -0.028, -0.015, 0.005};
    for (const tensor& F : {tensor::Id(), tensor{v1}}) {
      const t2tot2 nD = ::getNumericalApproximation(nhb, F, 1.e-5);
      const t2tot2 aD = D(F);
      for (unsigned short i = 0; i != tfel::math::TensorDimeToSize<N>::value;
           ++i) {
        for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value;
             ++j) {
          if (std::abs(aD(i, j) - nD(i, j)) > eps) {
            std::cout << aD(i, j) << " " << nD(i, j) << " "
                      << aD(i, j) - nD(i, j) << " " << eps << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(aD(i, j) - nD(i, j)) < eps);
        }
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator,
                          "FiniteStrainBehaviourTangentOperator");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
