/*!
 * \file   AbaqusTangentOperator2.cxx
 * \author Thomas Helfer
 * \date   13 août 2016
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

struct AbaqusTangentOperator2 final : public tfel::tests::TestCase {
  AbaqusTangentOperator2()
      : tfel::tests::TestCase("TFEL/Material", "AbaqusTangentOperator2") {
  }  // end of AbaqusTangentOperator2
  tfel::tests::TestResult execute() override {
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
    // total langrangian moduli
    const auto CSE = [&l0, &m0](const tensor& F) -> st2tost2 {
      auto index = [](size_type i, size_type j) -> size_type {
        if ((i == j) && (i < 3)) {
          return i;
        } else if (((i == 0) && (j == 1)) || ((i == 1) && (j == 0))) {
          return 3;
        } else if (((i == 0) && (j == 2)) || ((i == 2) && (j == 0))) {
          return 4;
        }
        return 5;
      };
      auto set = [](st2tost2& s, const size_type i, const size_type j,
                    const double v) {
        const double cste = std::sqrt(2.);
        if (((i > 2) && (j <= 2)) || ((j > 2) && (i <= 2))) {
          s(i, j) = v * cste;
        } else if ((i > 2) && (j > 2)) {
          s(i, j) = v * 2;
        } else {
          s(i, j) = v;
        }
      };
      // not supported by gcc 4.7.2:  auto get = [](const stensor& s,...
      auto get = [](stensor s, const size_type i) {
        const double cste = std::sqrt(2.);
        if (i >= s.size()) {
          return double(0);
        }
        if (i > 2) {
          return s(i) / cste;
        }
        return s(i);
      };
      const auto C = computeRightCauchyGreenTensor(F);
      const auto iC = invert(C);
      const auto J = tfel::math::det(F);
      const auto m = m0 - l0 * std::log(J);
      st2tost2 C_SE;
      for (size_type i = 0; i != 3; ++i) {
        for (size_type j = 0; j != 3; ++j) {
          if (((i == 1) && (j == 0)) || ((i == 2) && (j == 1))) {
            continue;
          }
          const auto rij = index(i, j);
          if (rij >= iC.size()) {
            continue;
          }
          for (size_type k = 0; k != 3; ++k) {
            for (size_type l = 0; l != 3; ++l) {
              const auto rkl = index(k, l);
              if (rkl >= tfel::math::StensorDimeToSize<N>::value) {
                continue;
              }
              const auto rik = index(i, k);
              const auto rjl = index(j, l);
              const auto ril = index(i, l);
              const auto rkj = index(k, j);
              set(C_SE, rij, rkl,
                  l0 * get(iC, rij) * get(iC, rkl) +
                      m * (get(iC, rik) * get(iC, rjl) +
                           get(iC, ril) * get(iC, rkj)));
            }
          }
        }
      }
      return C_SE;
    };
    // abaqus operator from the spatial moduli
    const auto Ca = [nhb,
                     &Cs](const tfel::math::tensor<N, real>& F) -> st2tost2 {
      return tfel::material::convert<TangentOperator::ABAQUS,
                                     TangentOperator::SPATIAL_MODULI>(
          Cs(F), tensor::Id(), F, nhb(F));
    };
    // abaqus operator from CSE
    const auto Ca2 = [nhb,
                      &CSE](const tfel::math::tensor<N, real>& F) -> st2tost2 {
      return tfel::material::convert<TangentOperator::ABAQUS,
                                     TangentOperator::DS_DEGL>(
          CSE(F), tensor::Id(), F, nhb(F));
    };
    const real v1[9u] = {1.03, 0.98,   1.09,   0.03, -0.012,
                         0.04, -0.028, -0.015, 0.005};
    const real v2[9u] = {0.70, 1.125, 1.32,  -0.24, -0.32,
                         0.15, -0.14, -0.05, 0.08};
    for (const tensor& F : {tensor::Id(), tensor(v1), tensor(v2)}) {
      const auto nC = getD(nhb, F, 1.e-5);
      const auto aC = Ca(F);
      for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value;
           ++i) {
        for (unsigned short j = 0; j != tfel::math::StensorDimeToSize<N>::value;
             ++j) {
          if (std::abs(aC(i, j) - nC(i, j)) > eps) {
            std::cout << i << " " << j << " " << nC(i, j) << " "
                      << " " << aC(i, j) << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(aC(i, j) - nC(i, j)) < eps);
        }
      }
      const auto aC2 = Ca2(F);
      for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value;
           ++i) {
        for (unsigned short j = 0; j != tfel::math::StensorDimeToSize<N>::value;
             ++j) {
          if (std::abs(aC2(i, j) - nC(i, j)) > eps) {
            std::cout << i << " " << j << " " << nC(i, j) << " "
                      << " " << aC2(i, j) << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(aC2(i, j) - nC(i, j)) < eps);
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

TFEL_TESTS_GENERATE_PROXY(AbaqusTangentOperator2, "AbaqusTangentOperator2");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("AbaqusTangentOperator2.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
