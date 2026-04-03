/*!
 * \file  tests/Material/FiniteStrainBehaviourTangentOperator7.cxx
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
#include "TFEL/Math/ST2toST2/ConvertSpatialModuliToKirchhoffJaumanRateModuli.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

struct FiniteStrainBehaviourTangentOperator7 final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator7()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator7") {
  }  // end of FiniteStrainBehaviourTangentOperator7
  tfel::tests::TestResult execute() override {
    this->check();
    return this->result;
  }  // end of execute
 private:
  // check that the perturbation approximation is ok
  void check() {
    using TangentOperator =
        tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    using real = double;
    using stensor = tfel::math::stensor<3u, real>;
    using tensor = tfel::math::tensor<3u, real>;
    using st2tost2 = tfel::math::st2tost2<3u, real>;
    using size_type = tfel::math::stensor<3u, real>::size_type;
    const real l0 = 1.09465e+11;
    const real m0 = 5.6391e+10;
    const real eps = 1.e-10 * m0;
    auto nhb = [&l0, &m0](const tfel::math::tensor<3u, real>& F) -> stensor {
      const auto J = tfel::math::det(F);
      const auto B = tfel::math::computeLeftCauchyGreenTensor(F);
      const auto Id = stensor::Id();
      return (l0 * std::log(J) * Id + m0 * (B - Id)) / J;
    };
    // spatial moduli
    const auto computeCs = [&l0, &m0](const tensor& F) -> st2tost2 {
      const auto J = tfel::math::det(F);
      const auto m = m0 - l0 * std::log(J);
      return l0 * st2tost2::IxI() + 2 * m * st2tost2::Id();
    };
    const auto computeCSE = [&l0, &m0](const tensor& F) -> st2tost2 {
      auto index = [](size_type i, size_type j) -> size_type {
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
          for (size_type k = 0; k != 3; ++k) {
            for (size_type l = 0; l != 3; ++l) {
              const auto rij = index(i, j);
              const auto rkl = index(k, l);
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
    for (const tensor& F :
         {tensor::Id(), tensor{1.03, 0.98, 1.09, 0.03, -0.012, 0.04, -0.028,
                               -0.015, 0.005}}) {
      const tensor F0 = tensor::Id();
      const auto s = nhb(F);
      const auto Cs = computeCs(F);
      const auto CSE = computeCSE(F);
      st2tost2 Cs2;
      tfel::material::convert<TangentOperator::SPATIAL_MODULI,
                              TangentOperator::DS_DEGL>(Cs2, CSE, F0, F, s);
      const auto Cs3 =
          tfel::material::convert<TangentOperator::SPATIAL_MODULI,
                                  TangentOperator::DS_DEGL>(CSE, F0, F, s);
      for (size_type i = 0; i != 6u; ++i) {
        for (size_type j = 0; j != 6u; ++j) {
          if (std::abs(Cs(i, j) - Cs2(i, j)) > eps) {
            std::cout << i << " " << j << " " << Cs(i, j) << " " << Cs2(i, j)
                      << " " << Cs(i, j) - Cs2(i, j) << " " << eps << std::endl;
          }
          if (std::abs(Cs(i, j) - Cs3(i, j)) > eps) {
            std::cout << i << " " << j << " " << Cs(i, j) << " " << Cs3(i, j)
                      << " " << Cs(i, j) - Cs3(i, j) << " " << eps << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(Cs(i, j) - Cs2(i, j)) < eps);
          TFEL_TESTS_ASSERT(std::abs(Cs(i, j) - Cs3(i, j)) < eps);
        }
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator7,
                          "FiniteStrainBehaviourTangentOperator7");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator7.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
