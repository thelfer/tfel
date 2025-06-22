/*!
 * \file   stensor_isotropic_function2.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25 juin 2018
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

struct StensorIsotropicFunctionDerivativeTest2 final
    : public tfel::tests::TestCase {
  StensorIsotropicFunctionDerivativeTest2()
      : tfel::tests::TestCase("TFEL/Math",
                              "StensorIsotropicFunctionDerivativeTest2") {
  }  // end of StensorIsotropicFunctionDerivativeTest

  tfel::tests::TestResult execute() override {
    this->test<3>({-0.000733594, 0.000273806, 0.000273806, 0., 0., 0.});
    this->test<3>({-0.002, 0.000746479, 0.000746479, 0., 0., 3.74031e-28});
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void test(const tfel::math::stensor<N, double>& v) {
    using size_type = typename tfel::math::stensor<N, double>::size_type;
    constexpr const auto jes =
        tfel::math::stensor<N, double>::FSESJACOBIEIGENSOLVER;
    constexpr const double eps = 1.e-8;
#if (defined __INTEL_COMPILER)
    const auto K = tfel::math::st2tost2<N, double>::K();
#else
    TFEL_CONSTEXPR const auto K = tfel::math::st2tost2<N, double>::K();
#endif
    constexpr const auto E = 150e9;
    constexpr const auto n = 0.3;
    constexpr const auto m = E / (2 * (1 + n));
    constexpr const auto rm = 0.6;
    constexpr const double prec = 4.e-9 * E;
    auto f = [](const double x) -> double { return x > 0 ? x : 0; };
    auto df = [](const double x) -> double {
      return std::abs(x) < 1.e-12 ? 0.5 : ((x < 0) ? 0 : 1);
    };
    const auto s = deviator(v);
    const auto fdf =
        s.template computeIsotropicFunctionAndDerivative<jes>(f, df, 1.e-13);
    const auto d = 2 * m * (K + (rm - 1) * fdf.second * K);
    decltype(fdf.second) nd;
    for (size_type i = 0; i != v.size(); ++i) {
      auto v2 = v;
      v2[i] += eps;
      const auto s2_p = deviator(v2);
      const auto nf_f =
          2 * m *
          (s2_p + (rm - 1) * s2_p.template computeIsotropicFunction<jes>(f));
      v2[i] -= 2 * eps;
      const auto s2_m = deviator(v2);
      const auto nf_b =
          2 * m *
          (s2_m + (rm - 1) * s2_m.template computeIsotropicFunction<jes>(f));
      for (size_type j = 0; j != v.size(); ++j) {
        nd(j, i) = (nf_f[j] - nf_b[j]) / (2 * eps);
      }
    }
    for (size_type i = 0; i != v.size(); ++i) {
      for (size_type j = 0; j != v.size(); ++j) {
        if (std::abs(d(i, j) - nd(i, j)) > prec) {
          std::cout << i << " " << j << " " << d(i, j) << " " << nd(i, j) << " "
                    << std::abs(d(i, j) - nd(i, j)) << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(d(i, j) - nd(i, j)) < prec);
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(StensorIsotropicFunctionDerivativeTest2,
                          "StensorIsotropicFunctionDerivativeTest2");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StensorIsotropicFunctionDerivative2.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
