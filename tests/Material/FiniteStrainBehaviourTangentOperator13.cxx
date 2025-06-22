/*!
 * \file   FiniteStrainBehaviourTangentOperator13.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 avril 2017
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

#include <cfenv>
#include <cstdlib>
#include <utility>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx"

#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

struct FiniteStrainBehaviourTangentOperator13 final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator13()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator13") {
  }  // end of FiniteStrainBehaviourTangentOperator13
  tfel::tests::TestResult execute() override {
    for (const auto& r :
         {FE_DOWNWARD, FE_UPWARD, FE_TONEAREST, FE_TOWARDZERO}) {
      this->check1<1u, double>(r);
      this->check2<1u, double>(r);
      this->check1<2u, double>(r);
      this->check2<2u, double>(r);
      this->check1<3u, double>(r);
      this->check2<3u, double>(r);
    }
    for (const auto& r :
         {FE_DOWNWARD, FE_UPWARD, FE_TONEAREST, FE_TOWARDZERO}) {
      this->check1<1u, long double>(r);
      this->check2<1u, long double>(r);
      this->check1<2u, long double>(r);
      this->check2<2u, long double>(r);
      this->check1<3u, long double>(r);
      this->check2<3u, long double>(r);
    }
    return this->result;
  }

 private:
  using TangentOperator =
      tfel::material::FiniteStrainBehaviourTangentOperatorBase;
  // check that the perturbation approximation is ok
  template <unsigned short N, typename real>
  void check1(const int r) {
    using namespace tfel::math;
    using namespace tfel::material;
    using stensor = tfel::math::stensor<N, real>;
    using tensor = tfel::math::tensor<N, real>;
    using st2tost2 = tfel::math::st2tost2<N, real>;
    using size_type = unsigned short;
    const real l0 = 1.09465e+11;
    const real m0 = 5.6391e+12;
    const real eps = 2.e-4 * (2 * m0);
    // changing the rounding mode
    std::fesetround(r);
    // Hencky-Biot law with Cauchy result
    auto hb = [&l0, &m0](const tensor& F) -> stensor {
      const auto C = computeRightCauchyGreenTensor(F);
      auto e = stensor{};
      auto p = st2tost2{};
      const auto f = [](const real x) { return std::log1p(x - 1) / 2; };
      const auto df = [](const real x) { return 1 / (2 * x); };
      std::tie(e, p) = C.template computeIsotropicFunctionAndDerivative<
          stensor::FSESJACOBIEIGENSOLVER>(f, df, 1.e-14);
      const auto T = eval(l0 * trace(e) * stensor::Id() + 2 * m0 * e);
      const auto S = eval(2 * (T | p));
      return convertSecondPiolaKirchhoffStressToCauchyStress(S, F);
    };
    const real v1[9u] = {1.03, 0.98,   1.09,   0.03, -0.012,
                         0.04, -0.028, -0.015, 0.005};
    const real v2[9u] = {0.70, 1.125, 1.32,  -0.24, -0.32,
                         0.15, -0.14, -0.05, 0.08};
    const real v3[9u] = {1.70, 0.625, 0.625, 0., 0., 0., 0., 0., 0.};
    const real v4[9u] = {0.625, 1.70, 0.625, 0., 0., 0., 0., 0., 0.};
    const real v5[9u] = {0.625, 0.625, 1.70, 0., 0., 0., 0., 0., 0.};
    const real v6[9u] = {0.625, 0.625, 1.70, 0., 0., 0., 0., 0., 0.};
    const auto F0 = tensor::Id();
    for (const tensor& F1 : {F0, tensor{v1}, tensor{v2}, tensor{v3}, tensor{v4},
                             tensor{v5}, tensor{v6}}) {
      const auto sig = hb(F1);
      const auto D = eval(l0 * st2tost2::IxI() + 2 * m0 * st2tost2::Id());
      const auto nD = getD(hb, F1, real(5.e-7));
      const auto D2 =
          tfel::material::convert<TangentOperator::SPATIAL_MODULI,
                                  TangentOperator::DT_DELOG>(D, F0, F1, sig);
      const auto aD = tfel::material::convert<TangentOperator::ABAQUS,
                                              TangentOperator::SPATIAL_MODULI>(
          D2, F0, F1, sig);
      for (size_type i = 0; i != tfel::math::StensorDimeToSize<N>::value; ++i) {
        for (size_type j = 0; j != tfel::math::StensorDimeToSize<N>::value;
             ++j) {
          if (std::abs(aD(i, j) - nD(i, j)) > eps) {
            std::cout << i << " " << j << " " << nD(i, j) << " "
                      << " " << aD(i, j) << " " << std::abs(aD(i, j) - nD(i, j))
                      << " " << std::abs(aD(i, j) - nD(i, j)) / (2 * m0)
                      << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(aD(i, j) - nD(i, j)) < eps);
        }
      }
    }
  }
  template <unsigned short N, typename real>
  void check2(const int r) {
    using namespace tfel::math;
    using namespace tfel::material;
    using stensor = tfel::math::stensor<N, real>;
    using tensor = tfel::math::tensor<N, real>;
    using st2tost2 = tfel::math::st2tost2<N, real>;
    using size_type = unsigned short;
    const real l0 = 1.09465e+11;
    const real m0 = 5.6391e+12;
    const real eps = 2.e-4 * (2 * m0);
    auto D = eval(l0 * st2tost2::IxI() + 2 * m0 * st2tost2::Id());
    for (size_type i = 0; i != StensorDimeToSize<N>::value; ++i) {
      for (size_type j = i + 1; j != StensorDimeToSize<N>::value; ++j) {
        D(i, j) *= 2;
      }
    }
    // changing the rounding mode
    std::fesetround(r);
    // Hencky-Biot law with Cauchy result
    auto hb = [&D](const tensor& F) -> stensor {
      const auto C = computeRightCauchyGreenTensor(F);
      auto e = stensor{};
      auto p = st2tost2{};
      const auto f = [](const real x) { return std::log1p(x - 1) / 2; };
      const auto df = [](const real x) { return 1 / (2 * x); };
      std::tie(e, p) = C.template computeIsotropicFunctionAndDerivative<
          stensor::FSESJACOBIEIGENSOLVER>(f, df, 1.e-14);
      const auto T = D * e;
      const auto S = eval(2 * (T | p));
      return convertSecondPiolaKirchhoffStressToCauchyStress(S, F);
    };
    const real v1[9u] = {1.03, 0.98,   1.09,   0.03, -0.012,
                         0.04, -0.028, -0.015, 0.005};
    const real v2[9u] = {0.70, 1.125, 1.32,  -0.24, -0.32,
                         0.15, -0.14, -0.05, 0.08};
    const real v3[9u] = {1.70, 0.625, 0.625, 0., 0., 0., 0., 0., 0.};
    const real v4[9u] = {0.625, 1.70, 0.625, 0., 0., 0., 0., 0., 0.};
    const real v5[9u] = {0.625, 0.625, 1.70, 0., 0., 0., 0., 0., 0.};
    const real v6[9u] = {0.625, 0.625, 1.70, 0., 0., 0., 0., 0., 0.};
    const auto F0 = tensor::Id();
    for (const tensor& F1 : {F0, tensor{v1}, tensor{v2}, tensor{v3}, tensor{v4},
                             tensor{v5}, tensor{v6}}) {
      const auto sig = hb(F1);
      const auto nD = getD(hb, F1, real(5.e-7));
      const auto D2 =
          tfel::material::convert<TangentOperator::SPATIAL_MODULI,
                                  TangentOperator::DT_DELOG>(D, F0, F1, sig);
      const auto aD = tfel::material::convert<TangentOperator::ABAQUS,
                                              TangentOperator::SPATIAL_MODULI>(
          D2, F0, F1, sig);
      for (size_type i = 0; i != tfel::math::StensorDimeToSize<N>::value; ++i) {
        for (size_type j = 0; j != tfel::math::StensorDimeToSize<N>::value;
             ++j) {
          if (std::abs(aD(i, j) - nD(i, j)) > eps) {
            std::cout << i << " " << j << " " << nD(i, j) << " "
                      << " " << aD(i, j) << " " << std::abs(aD(i, j) - nD(i, j))
                      << " " << std::abs(aD(i, j) - nD(i, j)) / (2 * m0)
                      << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(aD(i, j) - nD(i, j)) < eps);
        }
      }
    }
  }
  template <unsigned short N, typename Behaviour, typename real>
  tfel::math::st2tost2<N, real> getD(const Behaviour& b,
                                     const tfel::math::tensor<N, real>& F,
                                     const real e) {
    tfel::math::st2tost2<N, real> D;
    const auto J = det(F);
    for (unsigned short j = 0; j != tfel::math::StensorDimeToSize<N>::value;
         ++j) {
      const auto dF = getDeformationGradientPerturbation(F, j, e);
      const auto Fp = F + dF;
      const auto sp = b(Fp) * det(Fp);
      const auto Fm = F - dF;
      const auto sm = b(Fm) * det(Fm);
      const auto ds = (sp - sm) / (2 * e * J);
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
    tfel::math::tensor<N, real> eiej(real(0));
    const auto v = (idx > 2 ? std::sqrt(2) : real(1)) * e / 2;
    eiej(c.first) += v;
    eiej(c.second) += v;
    return eiej * F;
  }  // end of getDeformationGradientPerturbation
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator13,
                          "FiniteStrainBehaviourTangentOperator13");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator13.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
