/*!
 * \file  tests/Material/FiniteStrainBehaviourTangentOperator6.cxx
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

struct FiniteStrainBehaviourTangentOperator6 final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator6()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator6") {
  }  // end of FiniteStrainBehaviourTangentOperator6
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
    using size_type = unsigned short;
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
      auto convert = [this, &F](const t2tost2& DJ) {
        auto index = [](const size_type i, const size_type j) -> size_type {
          // i,j are valid for the space dimension considered
          if ((i == j) && (i < 3)) {
            return i;
          } else if ((i == 0) && (j == 1)) {
            return 3;
          } else if ((i == 1) && (j == 0)) {
            return 3;
          } else if ((i == 0) && (j == 2)) {
            return 4;
          } else if ((i == 2) && (j == 0)) {
            return 4;
          } else if ((i == 1) && (j == 2)) {
            return 5;
          }
          return 5;
        };
        st2tost2 r;
        for (size_type k = 0; k != 3; ++k) {
          for (size_type l = 0; l != 3; ++l) {
            const auto rkl = index(k, l);
            if (rkl >= tfel::math::StensorDimeToSize<N>::value) {
              continue;
            }
            const auto s = DJ * getDeformationGradient(F, rkl);
            for (size_type i = 0; i != tfel::math::StensorDimeToSize<N>::value;
                 ++i) {
              r(i, rkl) = s(i);
            }
          }
        }
        return r;
      };
      t2tost2 Dt;
      const auto s = svk(F);
      const auto S =
          tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s, F);
      const auto CSE = l0 * st2tost2::IxI() + 2 * m0 * st2tost2::Id();
      const auto DS = (CSE * t2tost2::dCdF(F)) / 2;
      tfel::math::computePushForwardDerivative(Dt, DS, S, F);
      const auto J = det(F);
      const auto t = s * J;
      const auto CJ = convert(Dt);
      const stensor mt = -1 * t;
      return convertSpatialModuliToKirchhoffJaumanRateModuli(CJ, mt);
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
  template <unsigned short N, typename real>
  inline tfel::math::tensor<N, real> getDeformationGradient(
      const tfel::math::tensor<N, real>& F, const unsigned short idx) {
    const auto c = [idx]() -> std::pair<unsigned short, unsigned short> {
      if ((idx == 0) || (idx == 1) || (idx == 2)) {
        return {idx, idx};
      }
      return {2 * idx - 3, 2 * idx - 2};
    }();
    tfel::math::tensor<N, real> dF;
    tfel::math::tensor<N, real> eiej(real(0));
    const auto v = (idx > 2 ? std::sqrt(2) : real(1)) / 2;
    eiej(c.first) += v;
    eiej(c.second) += v;
    return eiej * F;
  }  // end of getDeformationGradient
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator6,
                          "FiniteStrainBehaviourTangentOperator6");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator6.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
