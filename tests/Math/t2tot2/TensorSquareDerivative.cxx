/*!
 * \file   TensorSquareDerivative.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15 nov. 2016
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
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/t2tot2.hxx"

struct TensorSquareDerivativeTest final : public tfel::tests::TestCase {
  TensorSquareDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math/T2toT2",
                              "TensorSquareDerivativeTest") {
  }  // end of TensorSquareDerivativeTest

  tfel::tests::TestResult execute() override {
    const double Id[9u] = {1., 1., 1., 0., 0., 0., 0., 0., 0.};
    const double v[9u] = {
        1.27453824166446,    0.77207083708966,     0.24525337568425,
        -1.7790370858361e-4, 3.613971630283e-3,    -1.7873236537153e-02,
        3.613971630283e-3,   -1.7873236537153e-02, -1.7790370858361e-1};
    const double v2[9u] = {1.02, 0.85,  1.78,    0.18,   -0.16,
                           0.05, -0.08, -0.0243, 0.07654};
    test<1u>(Id);
    test<2u>(Id);
    test<3u>(Id);
    test<1u>(v);
    test<2u>(v);
    test<3u>(v);
    test<1u>(v2);
    test<2u>(v2);
    test<3u>(v2);
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void test(const double* values) {
    using namespace tfel::math;
    using Tensor = tensor<N, double>;
    using size_type = typename tensor<N, double>::size_type;
    const double eps = 1.e-5;
    const double prec = 1.e-10;
    const Tensor s(values);
    const auto D = t2tot2<N, double>::tpld(s) + t2tot2<N, double>::tprd(s);
    t2tot2<N, double> nD;
    for (size_type i = 0; i != s.size(); ++i) {
      tensor<N> s_e(s);
      s_e[i] += eps;
      const tensor<N> s2p = s_e * s_e;
      s_e[i] -= 2 * eps;
      const tensor<N> s2m = s_e * s_e;
      for (size_type j = 0; j != s.size(); ++j) {
        nD(j, i) = (s2p(j) - s2m(j)) / (2 * eps);
      }
    }
    for (size_type i = 0; i != s.size(); ++i) {
      for (size_type j = 0; j != s.size(); ++j) {
        TFEL_TESTS_ASSERT(std::abs(nD(i, j) - D(i, j)) < prec);
        if (std::abs(nD(i, j) - D(i, j)) > prec) {
          std::cout << "Error " << N << " (" << i << ", " << j << ") "
                    << "[" << i * TensorDimeToSize<N>::value + j
                    << "]: " << nD(i, j) << " vs " << D(i, j) << " "
                    << std::abs(nD(i, j) - D(i, j)) << std::endl;
        }
      }
    }
  }
};  // end of TensorSquareDerivativeTest

TFEL_TESTS_GENERATE_PROXY(TensorSquareDerivativeTest,
                          "TensorSquareDerivativeTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TensorSquareDerivative.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
