/*!
 * \file   ConvertToTangentModuliTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Material/Lame.hxx"
#include "TFEL/Math/ST2toST2/ConvertToTangentModuli.hxx"

struct ConvertToTangentModuliTest : public tfel::tests::TestCase {
  ConvertToTangentModuliTest()
      : tfel::tests::TestCase("TFEL/Math", "ConvertToTangentModuli") {
  }  // end of ConvertToTangentModuli

  tfel::tests::TestResult execute() override {
    this->test<1>();
    this->test<2>();
    this->test<3>();
    return this->result;
  }  // end of execute

 private:
  // simple alias
  using size_type = tfel::math::t2tost2<1u, double>::size_type;
  /*!
   */
  template <unsigned short N>
  void test() {
    using namespace tfel::math;
    const double eps = 1.e-14;
    const double E = 150e9;
    const double nu = 0.33;
    const double lambda = tfel::material::computeLambda(E, nu);
    const double mu = tfel::material::computeMu(E, nu);
    const st2tost2<N, double> D = lambda * st2tost2<N, double>::IxI() +
                                  2 * mu * st2tost2<N, double>::Id();
    tensor<N, double> F;
    for (size_type i = 0; i != F.size(); ++i) {
      if (i < 3) {
        F[i] = 1 + 0.1 * i * i;
      } else {
        F[i] = 0.03 * (i - 2);
      }
    }
    const auto CG = computeRightCauchyGreenTensor(F);
    const auto e_gl = 0.5 * (CG - stensor<N, double>::Id());
    const auto S = eval(D * e_gl);
    const auto dC = t2tost2<N, double>::dCdF(F);
    const auto dS = 0.5 * D * dC;
    t2tost2<N, double> K;
    computePushForwardDerivative(K, dS, S, F);
    // real work begin here
    t2tot2<N, double> C1;
    st2tost2<N, double> C2;
    for (size_type i = 0; i != 3u; ++i) {
      for (size_type j = 0; j != 3u; ++j) {
        const auto rij = index(i, j, N);
        const auto rij2 = index2(i, j, N);
        if (rij == -1) {
          continue;
        }
        for (size_type k = 0; k != 3u; ++k) {
          for (size_type l = 0; l != 3u; ++l) {
            const auto rkl = index(k, l, N);
            if (rkl == -1) {
              continue;
            }
            C1(rij, rkl) = double{0};
            for (size_type m = 0; m != 3u; ++m) {
              const auto rkm = index(k, m, N);
              const auto rlm = index(l, m, N);
              if (rkm == -1) {
                continue;
              }
              if (rlm == -1) {
                continue;
              }
              C1(rij, rkl) += K(rij2, rkm) * F(rlm);
            }
            if (i != j) {
              C1(rij, rkl) /= std::sqrt(2);
            }
          }
        }
      }
    }
    for (size_type i = 0; i != 3u; ++i) {
      for (size_type j = 0; j != 3u; ++j) {
        const auto rij = index2(i, j, N);
        if (rij == -1) {
          continue;
        }
        const auto rij1 = index(i, j, N);
        const auto rij2 = index(j, i, N);
        for (size_type k = 0; k != 3u; ++k) {
          for (size_type l = 0; l != 3u; ++l) {
            const auto rkl = index2(k, l, N);
            if (rkl == -1) {
              continue;
            }
            const auto rkl1 = index(k, l, N);
            const auto rkl2 = index(l, k, N);
            C2(rij, rkl) = (C1(rij1, rkl1) + C1(rij1, rkl2) + C1(rij2, rkl1) +
                            C1(rij2, rkl2)) /
                           4;
          }
        }
      }
    }
    for (size_type i = 3; i != StensorDimeToSize<N>::value; ++i) {
      for (size_type j = 0; j != StensorDimeToSize<N>::value; ++j) {
        C2(i, j) *= std::sqrt(2);
      }
    }
    for (size_type i = 0; i != StensorDimeToSize<N>::value; ++i) {
      for (size_type j = 3; j != StensorDimeToSize<N>::value; ++j) {
        C2(i, j) /= std::sqrt(2);
      }
    }
    const auto C = convertToTangentModuli(K, F);
    for (size_type i = 0; i != StensorDimeToSize<N>::value; ++i) {
      for (size_type j = 0; j != StensorDimeToSize<N>::value; ++j) {
        TFEL_TESTS_ASSERT(std::abs(C(i, j) - C2(i, j)) < E * eps);
      }
    }
  }
  static int index(const size_type i, const size_type j, const size_type N) {
    if (i == j) {
      return i;
    }
    if (N == 1) {
      return -1;
    }
    if ((i == 0) && (j == 1)) {
      return 3;
    }
    if ((i == 1) && (j == 0)) {
      return 4;
    }
    if ((i == 0) && (j == 2) && (N == 3u)) {
      return 5;
    }
    if ((i == 2) && (j == 0) && (N == 3u)) {
      return 6;
    }
    if ((i == 1) && (j == 2) && (N == 3u)) {
      return 7;
    }
    if ((i == 2) && (j == 1) && (N == 3u)) {
      return 8;
    }
    return -1;
  }
  static int index2(const size_type i, const size_type j, const size_type N) {
    if (i == j) {
      return i;
    }
    if (N == 1) {
      return -1;
    }
    if (((i == 0) && (j == 1)) || ((i == 1) && (j == 0))) {
      return 3;
    }
    if ((((i == 0) && (j == 2)) || ((i == 2) && (j == 0))) && (N == 3u)) {
      return 4;
    }
    if ((((i == 1) && (j == 2)) || ((i == 2) && (j == 1))) && (N == 3u)) {
      return 5;
    }
    return -1;
  }
};  // end of ConvertToTangentModuliTest

TFEL_TESTS_GENERATE_PROXY(ConvertToTangentModuliTest,
                          "ConvertToTangentModuliTest");

int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("ConvertToTangentModuli.xml");
  const auto r = manager.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
