/*!
 * \file  tests/Material/FiniteStrainBehaviourTangentOperator11.cxx
 * \brief
 * \author Thomas Helfer
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
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

template <unsigned short N, typename Behaviour, typename real>
static tfel::math::t2tost2<N, real> getNumericalApproximation(
    const Behaviour& b, const tfel::math::tensor<N, real>& F, const real e) {
  tfel::math::t2tost2<N, real> r;
  for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value; ++j) {
    tfel::math::tensor<N, real> Fp = F;
    tfel::math::tensor<N, real> Fm = F;
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
}  // end of getNumericalApproximation

struct FiniteStrainBehaviourTangentOperator11 final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator11()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator11") {
  }  // end of FiniteStrainBehaviourTangentOperator11
  tfel::tests::TestResult execute() override {
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
    using t2tost2 = tfel::math::t2tost2<N, real>;
    const real l0 = 1.09465e+11;
    const real m0 = 5.6391e+10;
    const real eps = 1.e-9 * m0;
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
    // dsig_dF
    const auto D = [&Cs,
                    &nhb](const tfel::math::tensor<N, real>& F) -> t2tost2 {
      using namespace tfel::material;
      using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
      const auto s = nhb(F);
      const auto dtau =
          convert<TangentOperator::DTAU_DF, TangentOperator::SPATIAL_MODULI>(
              Cs(F), F, F, s);
      return convert<TangentOperator::DSIG_DF, TangentOperator::DTAU_DF>(
          dtau, F, F, s);
    };
    const real v1[9u] = {1.03, 0.98,   1.09,   0.03, -0.012,
                         0.04, -0.028, -0.015, 0.005};
    for (const tensor& F : {tensor::Id(), tensor{v1}}) {
      const auto ndP = ::getNumericalApproximation(nhb, F, 1.e-5);
      const auto dP = D(F);
      for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value;
           ++i) {
        for (unsigned short j = 0; j != tfel::math::TensorDimeToSize<N>::value;
             ++j) {
          const auto e = std::abs(dP(i, j) - ndP(i, j));
          if (e > eps) {
            std::cout << dP(i, j) << " " << ndP(i, j) << " " << e << " " << eps
                      << std::endl;
          }
          TFEL_TESTS_ASSERT(e < eps);
        }
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator11,
                          "FiniteStrainBehaviourTangentOperator11");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator11.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
