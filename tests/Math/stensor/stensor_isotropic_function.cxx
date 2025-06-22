/*!
 * \file   stensor_isotropic_function.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   09 févr. 2016
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

#include <cmath>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

struct StensorIsotropicFunctionDerivativeTest final
    : public tfel::tests::TestCase {
  StensorIsotropicFunctionDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "StensorIsotropicFunctionDerivativeTest") {
  }  // end of StensorIsotropicFunctionDerivativeTest

  virtual tfel::tests::TestResult execute() override {
    this->test1<1>({1., 1., 1.});
    this->test1<2>({1., 1., 1., 0.});
    this->test1<3>({1., 1., 1., 0., 0., 0.});
    this->test1<1>({1., 1.2, 0.8});
    this->test1<2>({0.9, 1.06, 0.7, 0.33});
    this->test1<3>({0.9, 1.06, 0.7, 0.33, 0.46, 0.2});
    this->test2<1>({1., 1., 1.});
    this->test2<2>({1., 1., 1., 0.});
    this->test2<3>({1., 1., 1., 0., 0., 0.});
    this->test2<1>({1., 1.2, 0.8});
    this->test2<2>({0.9, 1.06, 0.7, 0.33});
    this->test2<3>({0.9, 1.06, 0.7, 0.33, 0.46, 0.2});
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N, typename Function, typename FunctionDerivative>
  void test(const tfel::math::stensor<N, double>& v,
            const Function f,
            const FunctionDerivative df) {
    using size_type = typename tfel::math::stensor<N, double>::size_type;
    constexpr const double eps = 1.e-7;
    constexpr const double prec = 5.e-8;
    const auto fdf = v.computeIsotropicFunctionAndDerivative(f, df, 1.e-12);
    const auto& d = fdf.second;
    decltype(fdf.second) nd;
    for (size_type i = 0; i != v.size(); ++i) {
      auto v2 = v;
      v2[i] += eps;
      const auto nf_f = v2.computeIsotropicFunction(f);
      v2[i] -= 2 * eps;
      const auto nf_b = v2.computeIsotropicFunction(f);
      for (size_type j = 0; j != v.size(); ++j) {
        nd(j, i) = (nf_f[j] - nf_b[j]) / (2 * eps);
      }
    }
    for (size_type i = 0; i != v.size(); ++i) {
      for (size_type j = 0; j != v.size(); ++j) {
        // if(std::abs(d(i,j)-nd(i,j))>prec){
        //   std::cout << i << " " << j << " " << d(i,j)
        //   << " " << nd(i,j) << " " << std::abs(d(i,j)-nd(i,j))
        //   << std::endl;
        // }
        TFEL_TESTS_ASSERT(std::abs(d(i, j) - nd(i, j)) < prec);
      }
    }
  }
  template <unsigned short N>
  void test1(const tfel::math::stensor<N, double>& v) {
    const auto e = [](const double x) { return std::exp(x); };
    this->test(v, e, e);
  }
  template <unsigned short N>
  void test2(const tfel::math::stensor<N, double>& v) {
    this->test(
        v, [](const double x) { return std::abs(x); },
        [](const double x) {
          return std::abs(x) < 1.e-12 ? 0.5 : ((x < 0) ? -1 : 1);
        });
  }
};

TFEL_TESTS_GENERATE_PROXY(StensorIsotropicFunctionDerivativeTest,
                          "StensorIsotropicFunctionDerivativeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("StensorIsotropicFunctionDerivative.xml");
  const auto r = manager.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
