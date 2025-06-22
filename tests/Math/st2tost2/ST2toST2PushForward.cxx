/*!
 * \file   ST2toST2PushForward.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   16 avril 2016
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

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Material/Lame.hxx"

struct ST2toST2PushForwardTest : public tfel::tests::TestCase {
  ST2toST2PushForwardTest()
      : tfel::tests::TestCase("TFEL/Math", "ST2toST2PushForward") {
  }  // end of ST2toST2PushForward

  tfel::tests::TestResult execute() {
    this->test<1>();
    this->test<2>();
    this->test<3>();
    this->test2();
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
    const auto F = tensor<N, double>::Id();
    const auto C = push_forward(D, F);
    for (size_type i = 0; i != StensorDimeToSize<N>::value; ++i) {
      for (size_type j = 0; j != StensorDimeToSize<N>::value; ++j) {
        TFEL_TESTS_ASSERT(std::abs(C(i, j) - D(i, j)) < eps * E);
      }
    }
    const auto C2 = pull_back(C, F);
    for (size_type i = 0; i != StensorDimeToSize<N>::value; ++i) {
      for (size_type j = 0; j != StensorDimeToSize<N>::value; ++j) {
        TFEL_TESTS_ASSERT(std::abs(C2(i, j) - D(i, j)) < eps * E);
      }
    }
  }
  static int row_index(size_type i, size_type j) {
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
  }
  /*!
   * In this test, we use the relation between the tangent moduli of
   * the Neo Hookean behaviour to check our implementation of the
   * push_forward operation
   */
  void test2() {
    using namespace tfel::math;
    const double eps = 1.e-14;
    const double E = 150e9;
    const double nu = 0.33;
    const double lambda = tfel::material::computeLambda(E, nu);
    const double mu = tfel::material::computeMu(E, nu);
    const st2tost2<3u, double> D = lambda * st2tost2<3u, double>::IxI() +
                                   2 * mu * st2tost2<3u, double>::Id();
    const auto F = []() {
      tensor<3u, double> Fi;
      for (size_type i = 0; i != Fi.size(); ++i) {
        if (i < 3) {
          Fi[i] = 1 + 0.1 * i * i;
        } else {
          Fi[i] = 0.03 * (i - 2);
        }
      }
      return Fi;
    }();
    auto set = [](st2tost2<3u, double>& s, const size_type i, const size_type j,
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
    auto get = [](const stensor<3u, double>& s, const size_type i) {
      const double cste = std::sqrt(2.);
      if (i > 2) {
        return s(i) / cste;
      }
      return s(i);
    };
    const auto C = computeRightCauchyGreenTensor(F);
    const auto iC = invert(C);
    st2tost2<3u, double> C_SE;
    for (size_type i = 0; i != 3; ++i) {
      for (size_type j = 0; j != 3; ++j) {
        for (size_type k = 0; k != 3; ++k) {
          for (size_type l = 0; l != 3; ++l) {
            const auto rij = row_index(i, j);
            const auto rkl = row_index(k, l);
            const auto rik = row_index(i, k);
            const auto rjl = row_index(j, l);
            const auto ril = row_index(i, l);
            const auto rkj = row_index(k, j);
            set(C_SE, rij, rkl,
                lambda * get(iC, rij) * get(iC, rkl) +
                    mu * (get(iC, rik) * get(iC, rjl) +
                          get(iC, ril) * get(iC, rkj)));
          }
        }
      }
    }
    const auto D2 = push_forward(C_SE, F);
    for (size_type i = 0; i != StensorDimeToSize<3u>::value; ++i) {
      for (size_type j = 0; j != StensorDimeToSize<3u>::value; ++j) {
        TFEL_TESTS_ASSERT(std::abs(D(i, j) - D2(i, j)) < eps * E);
      }
    }
    const auto C_SE2 = pull_back(D, F);
    for (size_type i = 0; i != StensorDimeToSize<3u>::value; ++i) {
      for (size_type j = 0; j != StensorDimeToSize<3u>::value; ++j) {
        TFEL_TESTS_ASSERT(std::abs(C_SE2(i, j) - C_SE(i, j)) < eps * E);
      }
    }
  }
};  // end of ST2toST2PushForwardTest

TFEL_TESTS_GENERATE_PROXY(ST2toST2PushForwardTest, "ST2toST2PushForwardTest");

int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("ST2toST2PushForward.xml");
  const auto r = manager.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
