/*!
 * \file   IsotropicCazacuYieldTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10/01/2018
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

#include "TFEL/Math/power.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

struct IsotropicCazacuYieldTest final : public tfel::tests::TestCase {
  IsotropicCazacuYieldTest()
      : tfel::tests::TestCase("TFEL/Material", "IsotropicCazacuYieldTest") {
  }  // end of IsotropicCazacuYieldTest
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
      // test<1u>(tfel::math::stensor<1u,double>(v));
      // test<2u>(tfel::math::stensor<2u,double>(v));
      // test<3u>(tfel::math::stensor<3u,double>(v));
      test2<1u>(tfel::math::stensor<1u, double>(v));
      // test2<2u>(tfel::math::stensor<2u,double>(v));
      // test2<3u>(tfel::math::stensor<3u,double>(v));
    }
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void test(const tfel::math::stensor<N, double>& t) {
    using tfel::math::power;
    using stensor = tfel::math::stensor<N, double>;
    constexpr const double c = -1.058;
    constexpr const double e = 1.e-6;
    constexpr const double prec = 2.e-10;
    auto fseq = [](const stensor& sig) {
      const auto s = deviator(sig);
      const auto J2 = (s | s) / 2;
      const auto J3 = det(s);
      return cbrt(power<3>(sqrt(J2)) - c * J3);
    };
    auto normal = [fseq](const stensor& sig) -> stensor {
      const auto s = deviator(sig);
      const auto J2 = (s | s) / 2;
      const auto seq = fseq(sig);
      const auto iseq = 1 / seq;
      const auto dJ2 = s;
      const auto dJ3 = computeDeviatorDeterminantDerivative(sig);
      const auto nb = (sqrt(J2) / 2) * dJ2 - (c / 3) * dJ3;
      return iseq * iseq * nb;
    };
    const auto n = normal(t);
    stensor nn;
    for (unsigned short j = 0; j != t.size(); ++j) {
      auto sig_p = t;
      auto sig_m = t;
      sig_p(j) += e;
      sig_m(j) -= e;
      const auto seq_p = fseq(sig_p);
      const auto seq_m = fseq(sig_m);
      nn(j) = (seq_p - seq_m) / (2 * e);
    }
    for (unsigned short i = 0; i != t.size(); ++i) {
      const auto error = std::abs(n(i) - nn(i));
      if (error > prec) {
        std::cout << "n : " << i << " " << n(i) << " " << nn(i) << " " << error
                  << std::endl;
      }
      TFEL_TESTS_ASSERT(error < prec);
    }
  }
  template <unsigned short N>
  void test2(const tfel::math::stensor<N, double>& t) {
    using tfel::math::power;
    using stensor = tfel::math::stensor<N, double>;
    using st2tost2 = tfel::math::st2tost2<N, double>;
    using size_type = unsigned short;
    constexpr const double c = -1.058;
    constexpr const double e = 1.e-7;
    constexpr const double prec = 1.e-8;
    auto normal = [c](const stensor& sig) -> stensor {
      const auto s = deviator(sig);
      const auto J2 = (s | s) / 2;
      const auto J3 = det(s);
      const auto seq = cbrt(power<3>(sqrt(J2)) - c * J3);
      const auto iseq = 1 / seq;
      const auto dJ2 = s;
      const auto dJ3 = computeDeviatorDeterminantDerivative(sig);
      const auto nb = (sqrt(J2) / 2) * dJ2 - (c / 3) * dJ3;
      return iseq * iseq * nb;
    };
    const auto id = stensor::Id();
    const auto id4 = st2tost2::Id();
    const auto s = deviator(t);
    const auto J2 = (s | s) / 2;
    const auto J3 = det(s);
    const auto seq = cbrt(power<3>(sqrt(J2)) - c * J3);
    const auto iseq = 1 / seq;
    const auto dJ2 = s;
    const auto d2J2 = eval(id4 - (id ^ id) / 3);
    const auto dJ3 = computeDeviatorDeterminantDerivative(t);
    const auto d2J3 = computeDeviatorDeterminantSecondDerivative(t);
    const auto nb = (sqrt(J2) / 2) * dJ2 - (c / 3) * dJ3;
    const auto dnb = 1 / (4 * sqrt(J2)) * (dJ2 ^ dJ2) + (sqrt(J2) / 2) * d2J2 -
                     (c / 3) * d2J3;
    const auto n = eval(iseq * iseq * nb);
    const auto dn = iseq * (-2 * (n ^ n) + iseq * dnb);
    st2tost2 ndn;
    for (unsigned short i = 0; i != t.size(); ++i) {
      auto sig_p = t;
      auto sig_m = t;
      sig_p(i) += e;
      sig_m(i) -= e;
      const auto n_p = normal(sig_p);
      const auto n_m = normal(sig_m);
      for (unsigned short j = 0; j != t.size(); ++j) {
        ndn(j, i) = (n_p(j) - n_m(j)) / (2 * e);
      }
    }
    for (size_type i = 0; i != s.size(); ++i) {
      for (size_type j = 0; j != s.size(); ++j) {
        const auto error = std::abs(ndn(i, j) - dn(i, j));
        TFEL_TESTS_ASSERT(error < prec);
        if (error > prec) {
          std::cout << "Error " << N << " (" << i << ", " << j << ") "
                    << ndn(i, j) << " vs " << dn(i, j) << " " << error
                    << std::endl;
        }
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(IsotropicCazacuYieldTest, "IsotropicCazacuYieldTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("IsotropicCazacuYieldTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
