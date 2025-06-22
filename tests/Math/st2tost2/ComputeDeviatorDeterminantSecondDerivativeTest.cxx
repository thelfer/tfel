/*!
 * \file tests/Math/st2tost2/ComputeDeviatorDeterminantSecondDerivativeTest.cxx
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
#include "TFEL/Math/st2tost2.hxx"

struct ComputeDeviatorDeterminantSecondDerivativeTest final
    : public tfel::tests::TestCase {
  ComputeDeviatorDeterminantSecondDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "ComputeDeviatorDeterminantSecondDerivative") {
  }  // end of ComputeDeviatorDeterminantSecondDerivativeTest

  tfel::tests::TestResult execute() override {
    const double sqrt2 = tfel::math::Cste<double>::sqrt2;
    const double v1[6] = {8.2e-2, -4.5e-2, 7.2e-2, 2.3e-2 * sqrt2, 0, 0.};
    const double v2[6] = {-8.2e-2, 4.5e-2, 7.2e-2, 0e-2, 2.3e-2 * sqrt2, 0.};
    const double v3[6] = {8.2e-2, 4.5e-2, -7.2e-2, 0e-2, 0.e-2, 2.3e-2 * sqrt2};
    const double v4[6] = {8.2e-2,          4.5e-2,           -7.2e-2,
                          3.14e-2 * sqrt2, -6.42e-2 * sqrt2, 2.3e-2 * sqrt2};
    const double v5[6] = {1.27453824166446,  0.77207083708966,
                          0.24525337568425,  -1.7790370858361e-4,
                          3.613971630283e-3, -1.7873236537153e-02};
    for (const auto v : {v1, v2, v3, v4, v5}) {
      test<1u>(v);
      test<2u>(v);
      test<3u>(v);
    }
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
    using Stensor = stensor<N, double>;
    using size_type = typename stensor<N, double>::size_type;
    const double eps = 1.e-5;
    const double prec = 1.e-10;
    const Stensor s(values);
    const auto dJ3 = computeDeviatorDeterminantSecondDerivative(s);
    st2tost2<N, double> ndJ3;
    for (size_type i = 0; i != s.size(); ++i) {
      stensor<N> s_e(s);
      s_e[i] += eps;
      const stensor<N> dJp = computeDeviatorDeterminantDerivative(s_e);
      s_e[i] -= 2 * eps;
      const stensor<N> dJm = computeDeviatorDeterminantDerivative(s_e);
      for (size_type j = 0; j != s.size(); ++j) {
        ndJ3(j, i) = (dJp(j) - dJm(j)) / (2 * eps);
      }
    }
    for (size_type i = 0; i != s.size(); ++i) {
      for (size_type j = 0; j != s.size(); ++j) {
        TFEL_TESTS_ASSERT(std::abs(ndJ3(i, j) - dJ3(i, j)) < prec);
        if (std::abs(ndJ3(i, j) - dJ3(i, j)) > prec) {
          std::cout << "Error " << N << " (" << i << ", " << j << ") "
                    << "[" << i * StensorDimeToSize<N>::value + j
                    << "]: " << ndJ3(i, j) << " vs " << dJ3(i, j) << " "
                    << std::abs(ndJ3(i, j) - dJ3(i, j)) << std::endl;
        }
      }
    }
  }
  template <unsigned short N>
  void test3(const double* values) {
    using namespace tfel::math;
    using Stensor = stensor<N, double>;
    using Stensor4 = st2tost2<N, double>;
    using size_type = typename stensor<N, double>::size_type;
    const auto id = Stensor::Id();
    const double prec = 1.e-10;
    const stensor<N, double> s(values);
    const auto i1 = trace(s);
    const auto di2 = i1 * id - s;
    const auto d2i2 = (id ^ id) - Stensor4::Id();
    const auto d2i3 = computeDeterminantSecondDerivative(s);
    const auto dJ3 = computeDeviatorDeterminantSecondDerivative(s);
    const st2tost2<N, double> dJ3_2 =
        (4 * i1 / 9) * (id ^ id) - ((id ^ di2) + (di2 ^ id) + i1 * d2i2) / 3 +
        d2i3;
    for (size_type i = 0; i != s.size(); ++i) {
      for (size_type j = 0; j != s.size(); ++j) {
        TFEL_TESTS_ASSERT(std::abs(dJ3_2(i, j) - dJ3(i, j)) < prec);
        if (std::abs(dJ3_2(i, j) - dJ3(i, j)) > prec) {
          std::cout << "Error " << N << " (" << i << ", " << j << ") "
                    << "[" << i * StensorDimeToSize<N>::value + j
                    << "]: " << dJ3_2(i, j) << " vs " << dJ3(i, j) << " "
                    << std::abs(dJ3_2(i, j) - dJ3(i, j)) << std::endl;
        }
      }
    }
  }
};  // end of ComputeDeviatorDeterminantSecondDerivativeTest

TFEL_TESTS_GENERATE_PROXY(ComputeDeviatorDeterminantSecondDerivativeTest,
                          "ComputeDeviatorDeterminantSecondDerivativeTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ComputeDeviatorDeterminantSecondDerivative.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
