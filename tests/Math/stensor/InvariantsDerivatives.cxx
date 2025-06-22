/*!
 * \file   InvariantsDerivatives.cxx
 * \brief
 * \author Thomas Helfer
 * \date   23 déc. 2016
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

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <cassert>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/power.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/Stensor/DecompositionInPositiveAndNegativeParts.hxx"

template <unsigned short N, typename F, typename real>
static tfel::math::stensor<N, real> getNumericalApproximation(
    const F& f, const tfel::math::stensor<N, real>& C, const real e) {
  tfel::math::stensor<N, real> r;
  for (unsigned short j = 0; j != C.size(); ++j) {
    auto Cp = C;
    auto Cm = C;
    Cp(j) += e;
    Cm(j) -= e;
    const auto Fp = f(Cp);
    const auto Fm = f(Cm);
    const auto dF = (Fp - Fm) / (2 * e);
    r(j) = dF;
  }
  return r;
}  // end of getNumericalApproximation

template <unsigned short N, typename F, typename real>
static tfel::math::st2tost2<N, real> getNumericalApproximation2(
    const F& f, const tfel::math::stensor<N, real>& C, const real e) {
  tfel::math::st2tost2<N, real> r;
  const auto size = tfel::math::StensorDimeToSize<N>::value;
  for (unsigned short j = 0; j != size; ++j) {
    auto Cp = C;
    auto Cm = C;
    Cp(j) += e;
    Cm(j) -= e;
    const auto Fp = f(Cp);
    const auto Fm = f(Cm);
    const auto dF = (Fp - Fm) / (2 * e);
    for (unsigned short i = 0; i != size; ++i) {
      r(i, j) = dF(i);
    }
  }
  return r;
}  // end of getNumericalApproximation2

struct InvariantsDerivatives final : public tfel::tests::TestCase {
  InvariantsDerivatives()
      : tfel::tests::TestCase("TFEL/Math", "InvariantsDerivatives") {
  }  // end of InvariantsDerivatives
  tfel::tests::TestResult execute() override {
    const double sqrt2 = std::sqrt(2.);
    const double v0[6] = {0, 0, 0, 0, 0, 0};
    const double v1[6] = {8.2e-2, -4.5e-2, 7.2e-2, 2.3e-2 * sqrt2, 0, 0.};
    const double v2[6] = {-8.2e-2, 4.5e-2, 7.2e-2, 0e-2, 2.3e-2 * sqrt2, 0.};
    const double v3[6] = {8.2e-2, 4.5e-2, -7.2e-2, 0e-2, 0.e-2, 2.3e-2 * sqrt2};
    const double v4[6] = {8.2e-2,          4.5e-2,           -7.2e-2,
                          3.14e-2 * sqrt2, -6.42e-2 * sqrt2, 2.3e-2 * sqrt2};
    this->check(tfel::math::stensor<1u, double>(v0));
    this->check(tfel::math::stensor<2u, double>(v0));
    this->check(tfel::math::stensor<3u, double>(v0));
    this->check(tfel::math::stensor<1u, double>(v1));
    this->check(tfel::math::stensor<2u, double>(v1));
    this->check(tfel::math::stensor<3u, double>(v1));
    this->check(tfel::math::stensor<1u, double>(v2));
    this->check(tfel::math::stensor<2u, double>(v2));
    this->check(tfel::math::stensor<3u, double>(v2));
    this->check(tfel::math::stensor<1u, double>(v3));
    this->check(tfel::math::stensor<2u, double>(v3));
    this->check(tfel::math::stensor<3u, double>(v3));
    this->check(tfel::math::stensor<1u, double>(v4));
    this->check(tfel::math::stensor<2u, double>(v4));
    this->check(tfel::math::stensor<3u, double>(v4));
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void check(const tfel::math::stensor<N, double>& s1) {
    using stensor = tfel::math::stensor<N, double>;
    using st2tost2 = tfel::math::st2tost2<N, double>;
    TFEL_CONSTEXPR const auto eps = 1e-2;
    TFEL_CONSTEXPR const auto prec = 1e-13;
    TFEL_CONSTEXPR const auto eps2 = 5e-6;
    TFEL_CONSTEXPR const auto prec2 = 1e-10;
    auto I1 = [](const stensor& v) { return trace(v); };
    auto I2 = [](const stensor& v) {
      const auto I1v = trace(v);
      return (I1v * I1v - trace(square(v))) / 2;
    };
    auto I3 = [](const stensor& v) { return det(v); };
    auto iJb = [&I3](const stensor& v) { return 1 / std::cbrt(I3(v)); };
    auto dI1 = [](const stensor&) -> stensor { return stensor::Id(); };
    auto dI2 = [&I1](const stensor& v) -> stensor {
      const auto id = stensor::Id();
      return I1(v) * id - v;
    };
    auto dI3 = [&I1, &I2](const stensor& v) -> stensor {
      const auto id = stensor::Id();
      return square(v) - I1(v) * v + I2(v) * id;
    };
    auto diJb = [&I3, &dI3, &iJb](const stensor& v) -> stensor {
      const auto iJb4 = tfel::math::power<4>(iJb(v));
      const auto diJb_dI3 = -iJb4 / 3;
      return diJb_dI3 * dI3(v);
    };
    const auto id = stensor::Id();
    const stensor s = id + s1;
    const auto dI1_ds = dI1(s);
    const auto dI2_ds = dI2(s);
    const auto dI3_ds = dI3(s);
    const auto diJb_ds = diJb(s);
    const auto d2I1_ds2 = st2tost2(double(0));
    const auto d2I2_ds2 = (id ^ id) - st2tost2::Id();
    const auto d2I3_ds2 = computeDeterminantSecondDerivative(s);
    const auto diJb_dI3 = -tfel::math::power<4>(iJb(s)) / 3;
    const auto d2iJb_dI32 = 4 * tfel::math::power<7>(iJb(s)) / 9;
    const auto d2iJb_ds2 = d2iJb_dI32 * (dI3_ds ^ dI3_ds) + diJb_dI3 * d2I3_ds2;
    const auto ndI1_ds = getNumericalApproximation(I1, s, eps);
    const auto ndI2_ds = getNumericalApproximation(I2, s, eps);
    const auto ndI3_ds = getNumericalApproximation(I3, s, eps);
    const auto ndiJb_ds = getNumericalApproximation(iJb, s, eps2);
    const auto nd2I1_ds2 = getNumericalApproximation2(dI1, s, eps);
    const auto nd2I2_ds2 = getNumericalApproximation2(dI2, s, eps);
    const auto nd2I3_ds2 = getNumericalApproximation2(dI3, s, eps);
    const auto nd2iJb_ds2 = getNumericalApproximation2(diJb, s, eps2);
    for (unsigned short i = 0; i != s.size(); ++i) {
      if (std::abs(dI1_ds(i) - ndI1_ds(i)) > prec) {
        std::cout << "dI1_ds : " << i << " " << dI1_ds(i) << " " << ndI1_ds(i)
                  << " " << std::abs(dI1_ds(i) - ndI1_ds(i)) << std::endl;
      }
      if (std::abs(dI2_ds(i) - ndI2_ds(i)) > prec) {
        std::cout << "dI2_ds : " << i << " " << dI2_ds(i) << " " << ndI2_ds(i)
                  << " " << std::abs(dI2_ds(i) - ndI2_ds(i)) << std::endl;
      }
      if (std::abs(dI3_ds(i) - ndI3_ds(i)) > prec) {
        std::cout << "dI3_ds : " << i << " " << dI3_ds(i) << " " << ndI3_ds(i)
                  << " " << std::abs(dI3_ds(i) - ndI3_ds(i)) << std::endl;
      }
      if (std::abs(diJb_ds(i) - ndiJb_ds(i)) > prec2) {
        std::cout << "diJb_ds : " << i << " " << diJb_ds(i) << " "
                  << ndiJb_ds(i) << " " << std::abs(diJb_ds(i) - ndiJb_ds(i))
                  << std::endl;
      }
      TFEL_TESTS_ASSERT(std::abs(dI1_ds(i) - ndI1_ds(i)) < prec);
      TFEL_TESTS_ASSERT(std::abs(dI2_ds(i) - ndI2_ds(i)) < prec);
      TFEL_TESTS_ASSERT(std::abs(dI3_ds(i) - ndI3_ds(i)) < prec);
      TFEL_TESTS_ASSERT(std::abs(diJb_ds(i) - ndiJb_ds(i)) < prec2);
      for (unsigned short j = 0; j != s.size(); ++j) {
        if (std::abs(d2I1_ds2(i, j) - nd2I1_ds2(i, j)) > prec) {
          std::cout << "d2I1_ds2 : " << i << " " << j << " " << d2I1_ds2(i, j)
                    << " " << nd2I1_ds2(i, j) << " "
                    << std::abs(d2I1_ds2(i, j) - nd2I1_ds2(i, j)) << std::endl;
        }
        if (std::abs(d2I2_ds2(i, j) - nd2I2_ds2(i, j)) > prec) {
          std::cout << "d2I2_ds2 : " << i << " " << j << " " << d2I2_ds2(i, j)
                    << " " << nd2I2_ds2(i, j) << " "
                    << std::abs(d2I2_ds2(i, j) - nd2I2_ds2(i, j)) << std::endl;
        }
        if (std::abs(d2I3_ds2(i, j) - nd2I3_ds2(i, j)) > prec) {
          std::cout << "d2I3_ds2 : " << i << " " << j << " " << d2I3_ds2(i, j)
                    << " " << nd2I3_ds2(i, j) << " "
                    << std::abs(d2I3_ds2(i, j) - nd2I3_ds2(i, j)) << std::endl;
        }
        if (std::abs(d2iJb_ds2(i, j) - nd2iJb_ds2(i, j)) > prec2) {
          std::cout << "d2iJb_ds2 : " << i << " " << j << " " << d2iJb_ds2(i, j)
                    << " " << nd2iJb_ds2(i, j) << " "
                    << std::abs(d2iJb_ds2(i, j) - nd2iJb_ds2(i, j))
                    << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(d2I1_ds2(i, j) - nd2I1_ds2(i, j)) < prec);
        TFEL_TESTS_ASSERT(std::abs(d2I2_ds2(i, j) - nd2I2_ds2(i, j)) < prec);
        TFEL_TESTS_ASSERT(std::abs(d2I3_ds2(i, j) - nd2I3_ds2(i, j)) < prec);
        TFEL_TESTS_ASSERT(std::abs(d2iJb_ds2(i, j) - nd2iJb_ds2(i, j)) < prec2);
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(InvariantsDerivatives, "InvariantsDerivatives");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("InvariantsDerivatives.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
