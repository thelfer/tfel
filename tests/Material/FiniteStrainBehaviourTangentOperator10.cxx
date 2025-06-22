/*!
 * \file   FiniteStrainBehaviourTangentOperator10.cxx
 *
 * \brief This test verify that the abaqus tangent operator
 *        is correctly computed by comparing its value to
 *        an numerical perturbation
 *
 * Numerical Approximation of Tangent Moduli for Finite Element
 * Implementations of Nonlinear Hyperelastic Material Models
 *
 * Sun, Wei and Chaikof, Elliot L and Levenston, Marc E
 * Journal of biomechanical engineering, 6
 * 2008-12
 *
 * \author THOMAS HELFER
 * \date   13 août 2016
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
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

struct FiniteStrainBehaviourTangentOperator10 final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator10()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator10") {
  }  // end of FiniteStrainBehaviourTangentOperator10
  virtual tfel::tests::TestResult execute() override {
    this->check1<1u>();
    this->check1<2u>();
    this->check1<3u>();
    return this->result;
  }

 private:
  // check that the perturbation approximation is ok
  template <unsigned short N>
  void check1() {
    using real = double;
    using TangentOperator =
        tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    using stensor = tfel::math::stensor<N, real>;
    using tensor = tfel::math::tensor<N, real>;
    using st2tost2 = tfel::math::st2tost2<N, real>;
    using size_type = typename tfel::math::stensor<N, real>::size_type;
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
    // Kirchhoff Jaumann moduli from the spatial moduli
    const auto CJ = [nhb,
                     &Cs](const tfel::math::tensor<N, real>& F) -> st2tost2 {
      return tfel::material::convert<TangentOperator::C_TAU_JAUMANN,
                                     TangentOperator::SPATIAL_MODULI>(
          Cs(F), tensor::Id(), F, nhb(F));
    };
    const real v1[9u] = {1.03, 0.98,   1.09,   0.03, -0.012,
                         0.04, -0.028, -0.015, 0.005};
    const real v2[9u] = {0.70, 1.125, 1.32,  -0.24, -0.32,
                         0.15, -0.14, -0.05, 0.08};
    for (const tensor& F : {tensor::Id(), tensor{v1}, tensor{v2}}) {
      const auto nC = getD(nhb, F, 1.e-5);
      const auto aC = CJ(F);
      for (size_type i = 0; i != tfel::math::StensorDimeToSize<N>::value; ++i) {
        for (size_type j = 0; j != tfel::math::StensorDimeToSize<N>::value;
             ++j) {
          if (std::abs(aC(i, j) - nC(i, j)) > eps) {
            std::cout << i << " " << j << " " << nC(i, j) << " "
                      << " " << aC(i, j) << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(aC(i, j) - nC(i, j)) < eps);
        }
      }
    }
  }
  template <unsigned short N, typename Behaviour, typename real>
  tfel::math::st2tost2<N, real> getD(const Behaviour& b,
                                     const tfel::math::tensor<N, real>& F,
                                     const real e) {
    tfel::math::st2tost2<N, real> D;
    for (unsigned short j = 0; j != tfel::math::StensorDimeToSize<N>::value;
         ++j) {
      const auto dF = getDeformationGradientPerturbation(F, j, e);
      const auto Fp = F + dF;
      const auto sp = b(Fp) * det(Fp);
      const auto Fm = F - dF;
      const auto sm = b(Fm) * det(Fm);
      const auto ds = (sp - sm) / (2 * e);
      for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value;
           ++i) {
        D(i, j) = ds(i);
      }
    }
    return D;
  }
  template <unsigned short N, typename real>
  inline tfel::math::tensor<N, real> getDeformationGradientPerturbation(
      const tfel::math::tensor<N, real>& F,
      const unsigned short idx,
      const real e) {
    const auto c = [idx]() -> std::pair<unsigned short, unsigned short> {
      if ((idx == 0) || (idx == 1) || (idx == 2)) {
        return {idx, idx};
      }
      return {2 * idx - 3, 2 * idx - 2};
    }();
    tfel::math::tensor<N, real> dF;
    tfel::math::tensor<N, real> eiej(real(0));
    const auto v = (idx > 2 ? std::sqrt(2) : real(1)) * e / 2;
    eiej(c.first) += v;
    eiej(c.second) += v;
    return eiej * F;
  }  // end of getDeformationGradientPerturbation
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator10,
                          "FiniteStrainBehaviourTangentOperator10");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator10.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
