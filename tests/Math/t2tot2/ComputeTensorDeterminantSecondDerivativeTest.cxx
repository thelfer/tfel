/*!
 * \file   tests/Math/t2tot2/ComputeTensorDeterminantSecondDerivativeTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   14/11/2016
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

struct ComputeTensorDeterminantSecondDerivativeTest final
    : public tfel::tests::TestCase {
  ComputeTensorDeterminantSecondDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math/T2toT2",
                              "ComputeTensorDeterminantSecondDerivative") {
  }  // end of ComputeTensorDeterminantSecondDerivativeTest

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
    this->template test2<N>(values);
    this->template test3<N>(values);
  }
  template <unsigned short N>
  void test2(const double* values) {
    using namespace tfel::math;
    using Tensor = tensor<N, double>;
    using size_type = typename tensor<N, double>::size_type;
    const double eps = 1.e-5;
    const double prec = 1.e-10;
    const Tensor s(values);
    const auto dJ2 = computeDeterminantSecondDerivative(s);
    t2tot2<N, double> ndJ2;
    for (size_type i = 0; i != s.size(); ++i) {
      tensor<N> s_e(s);
      s_e[i] += eps;
      const tensor<N> dJp = det(s_e) * invert(s_e);
      s_e[i] -= 2 * eps;
      const tensor<N> dJm = det(s_e) * invert(s_e);
      for (size_type j = 0; j != s.size(); ++j) {
        ndJ2(j, i) = (dJp(j) - dJm(j)) / (2 * eps);
      }
    }
    for (size_type i = 0; i != s.size(); ++i) {
      for (size_type j = 0; j != s.size(); ++j) {
        TFEL_TESTS_ASSERT(std::abs(ndJ2(i, j) - dJ2(i, j)) < prec);
        if (std::abs(ndJ2(i, j) - dJ2(i, j)) > prec) {
          std::cout << "Error " << N << " (" << i << ", " << j << ") "
                    << "[" << i * TensorDimeToSize<N>::value + j
                    << "]: " << ndJ2(i, j) << " vs " << dJ2(i, j) << " "
                    << std::abs(ndJ2(i, j) - dJ2(i, j)) << std::endl;
        }
      }
    }
  }
  template <unsigned short N>
  void test3(const double* values) {
    using namespace tfel::math;
    using Tensor = tensor<N, double>;
    using Tensor4 = t2tot2<N, double>;
    using size_type = typename tensor<N, double>::size_type;
    const double prec = 1.e-10;
    const tensor<N, double> s(values);
    const Tensor s2 = s * s;
    const Tensor s3 = s2 * s;
    const auto i1 = trace(s);
    const auto i2 = (i1 * i1 - trace(s * s)) / 2;
    const auto i3 = det(s);
    const Tensor c = s3 - i1 * s2 + i2 * s - i3 * Tensor::Id();
    const auto dJ2 = computeDeterminantSecondDerivative(s);
    const Tensor id = Tensor::Id();
    const t2tot2<N, double> dJ2_2 = Tensor4::tpld(s) + Tensor4::tprd(s) -
                                    (s ^ id) - i1 * Tensor4::Id() +
                                    (id ^ (i1 * id - transpose(s)));
    for (size_type i = 0; i != s.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(c(i)) <
                        100 * std::numeric_limits<double>::epsilon());
      for (size_type j = 0; j != s.size(); ++j) {
        TFEL_TESTS_ASSERT(std::abs(dJ2_2(i, j) - dJ2(i, j)) < prec);
        if (std::abs(dJ2_2(i, j) - dJ2(i, j)) > prec) {
          std::cout << "Error " << N << " (" << i << ", " << j << ") "
                    << "[" << i * TensorDimeToSize<N>::value + j
                    << "]: " << dJ2_2(i, j) << " vs " << dJ2(i, j) << " "
                    << std::abs(dJ2_2(i, j) - dJ2(i, j)) << std::endl;
        }
      }
    }
  }
};  // end of ComputeTensorDeterminantSecondDerivativeTest

TFEL_TESTS_GENERATE_PROXY(ComputeTensorDeterminantSecondDerivativeTest,
                          "ComputeTensorDeterminantSecondDerivativeTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ComputeTensorDeterminantSecondDerivative.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
