/*!
 * \file   tests/Math/symmetric_product.cxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cmath>
#include <limits>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

struct SymmetricProduct final : public tfel::tests::TestCase {
  SymmetricProduct()
      : tfel::tests::TestCase("TFEL/Math", "SymmetricProduct") {
  }  // end of SymmetricProduct
  tfel::tests::TestResult execute() override {
    this->test1<1u, float>();
    this->test1<1u, double>();
    this->test1<1u, long double>();
    this->test1<2u, float>();
    this->test1<2u, double>();
    this->test1<2u, long double>();
    this->test1<3u, float>();
    this->test1<3u, double>();
    this->test1<3u, long double>();
    //
    this->test2<1u, float>();
    this->test2<1u, double>();
    this->test2<1u, long double>();
    this->test2<2u, float>();
    this->test2<2u, double>();
    this->test2<2u, long double>();
    this->test2<3u, float>();
    this->test2<3u, double>();
    this->test2<3u, long double>();
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N, typename T>
  void test1() {
    using namespace tfel::math;
    constexpr auto eps = 10 * std::numeric_limits<T>::epsilon();
    constexpr auto id = stensor<N, T>::Id();
    constexpr auto s = symmetric_product(id, id);
    TFEL_TESTS_STATIC_ASSERT(constexpr_fct::abs(s(0) - 1) < eps);
    TFEL_TESTS_STATIC_ASSERT(constexpr_fct::abs(s(1) - 1) < eps);
    TFEL_TESTS_STATIC_ASSERT(constexpr_fct::abs(s(2) - 1) < eps);
    TFEL_TESTS_STATIC_ASSERT(abs(s - id) < eps);
  }
  template <unsigned short N, typename T>
  void test2() {
    using namespace tfel::math;
    constexpr auto eps = 10 * std::numeric_limits<T>::epsilon();
    constexpr auto s = []() constexpr->stensor<N, T> {
      if constexpr (N == 1) {
        return stensor<N, T>{1, -0.2, -0.5};
      } else if constexpr (N == 2) {
        return stensor<N, T>{1, -0.2, -0.5, 0.25};
      } else {
        return stensor<N, T>{1, -0.2, -0.5, 0.25, 0.15, -0.33};
      }
    }
    ();
    constexpr auto s2 = []() constexpr->stensor<N, T> {
      if constexpr (N == 1) {
        return stensor<N, T>{0.7, 0.4, -1.2};
      } else if constexpr (N == 2) {
        return stensor<N, T>{0.7, 0.4, -1.2, 0.45};
      } else {
        return stensor<N, T>{0.7, 0.4, -1.2, 0.45, -0.17, -0.78};
      }
    }
    ();
    constexpr auto s3 = symmetric_product(s, s2);
    const auto s4 = syme(s * s2);
    TFEL_TESTS_ASSERT(abs(s3 - s4) < eps);
  }
};

TFEL_TESTS_GENERATE_PROXY(SymmetricProduct, "SymmetricProduct");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("symmetric_product.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
