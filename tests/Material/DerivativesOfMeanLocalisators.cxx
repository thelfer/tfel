/*!
 * \file   tests/Material/DerivativesOfMeanLocalisators.cxx
 * \brief
 * \author Antoine MARTIN
 * \date   16/09/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif NDEBUG

#include <limits>
#include <cstdlib>
#include <iostream>
#include "TFEL/Config/TFELTypes.hxx"

#include "TFEL/Material/IsotropicModuli.hxx"
#include "TFEL/Material/OrientationAverages.hxx"
#include "TFEL/Material/PhaseDescription.hxx"
#include "TFEL/Math/General/DerivativeType.hxx"

#include "TFEL/Math/qt.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct DerivativesOfMeanLocalisatorsTest final : public tfel::tests::TestCase {
  DerivativesOfMeanLocalisatorsTest()
      : tfel::tests::TestCase("TFEL/Material",
                              "DerivativesOfMeanLocalisators") {
  }  // end of DerivativesOfMeanLocalisatorsTest

  tfel::tests::TestResult execute() override {
    this->template test_loc_A2A4_derivative<double, false>(1e-6, 1e-9,10);
    this->template test_loc_A2A4_derivative<double, true>(1e-6, 1e-9,10);

    this->template test_loc_A2A4_derivative<double, false>(1e-6, 1e-9,0.5);
    this->template test_loc_A2A4_derivative<double, true>(1e-6, 1e-9,0.5);

    return this->result;
  }

  template <typename NumericType, bool use_qt>
  void test_loc_A2A4_derivative(const NumericType h, const NumericType eps,const NumericType e) {
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using real = typename tfel::config::Types<1u, NumericType, use_qt>::real;
    using length =
        typename tfel::config::Types<1u, NumericType, use_qt>::length;
    using compliance =
        typename tfel::config::Types<1u, NumericType, use_qt>::compliance;

    using namespace tfel::material;
    const auto k0 = stress(2);
    const auto mu0 = stress(1);
    const auto ki = stress(200);
    const auto mui = stress(100);
    const auto KG0 = KGModuli<stress>(k0, mu0);
    const auto KGi = KGModuli<stress>(ki, mui);
    const std::array<real, 4> dk0 = {1., 0., 0., 0.};
    const std::array<real, 4> dmu0 = {0., 1., 0., 0.};
    const std::array<real, 4> dki = {0., 0., 1., 0.};
    const std::array<real, 4> dmui = {0., 0., 0., 1.};
    tfel::math::tvector<3u, real> n_a = {std::sqrt(2) / 2., std::sqrt(2) / 2.,
                                         0.};
    tfel::math::tvector<3u, real> n_b = {-std::sqrt(2) / 2., std::sqrt(2) / 2.,
                                         0.};
    const length a = length(1.);
    const length b = a/e;
    using namespace tfel::material::homogenization::elasticity;

    tfel::math::stensor<3u, real> A2 = tfel::math::stensor<3u, real>::Id();
    A2(1) = real(0.5);
    A2(0) = real(0.5);
    tfel::math::st2tost2<3u, real> A4 =
        2. / 13 * tfel::math::st2tost2<3u, real>::J() +
        1. / 4 * tfel::math::st2tost2<3u, real>::K();
    A4(0, 0) = real(0.1);
    A4(1, 1) = real(0.1);

    auto func = [&e,&A2,&A4](const stress& k0_, const stress& mu0_, const stress& ki_,
                    const stress& mui_) {
      return EllipsoidMeanLocalisator<3u, stress>::
          UserDefinedDistributionOfSpheroids(KGModuli<stress>(k0_, mu0_),
                                             KGModuli<stress>(ki_, mui_),
                                             length(e), length(1), A2, A4);
    };

    std::array<std::array<real, 4>, 4> dt_ = {dk0, dmu0, dki, dmui};
    std::array<stress, 4> k_ = {k0, mu0, ki, mui};

    for (int i = 0; i < 4; i++) {
      const auto dAv_dt =
          DerivativesOfMeanLocalisator<3u, stress>(KG0, KGi, e, A2, A4, dt_[i]);

      std::array<stress, 4> kk_ = {k0, mu0, ki, mui};
      auto func2 = [&func,&kk_,&i](const stress& _k_) {
        kk_[i] = _k_;
        return func(kk_[0], kk_[1], kk_[2], kk_[3]);
      };

      auto dADF_dt =
          tfel::math::computeNumericalDerivative(func2, k_[i], stress(h));

      for (int ii : {0, 1, 2, 3, 4, 5}) {
        for (int jj : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(my_abs(dAv_dt(ii, jj) - dADF_dt(ii, jj)) <
                            compliance(eps));
        }
      }

      auto isod = IsotropicDistribution(Ellipsoid(a, b, b), 0.1, KGi);
      const auto dAvIso_dt =
          isod.computeDerivativesOfMeanLocalisator(KG0, dt_[i]);

      auto funcIso = [&a,&b](const stress& k0_, const stress& mu0_,
                         const stress& ki_, const stress& mui_) {
        auto A = EllipsoidMeanLocalisator<3u, stress>::Isotropic(
            KGModuli<stress>(k0_, mu0_), KGModuli<stress>(ki_, mui_),
            {a, b, b});
        return A;
      };

      kk_ = {k0, mu0, ki, mui};
      auto funcIso2 = [&funcIso,&kk_,&i](const stress& _k_) {
        kk_[i] = _k_;
        return funcIso(kk_[0], kk_[1], kk_[2], kk_[3]);
      };

      auto dAIsoDF_dt =
          tfel::math::computeNumericalDerivative(funcIso2, k_[i], stress(h));
      for (int ii : {0, 1, 2, 3, 4, 5}) {
        for (int jj : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(my_abs(dAvIso_dt(ii, jj) - dAIsoDF_dt(ii, jj)) <
                            compliance(eps));
        }
      }

      short unsigned int index = 0;
      auto tisod = TransverseIsotropicDistribution<stress>(
          Ellipsoid(a, b, b), 0.1, KGi, n_a, index);
      const auto dAvTiso_dt =
          tisod.computeDerivativesOfMeanLocalisator(KG0, dt_[i]);

      auto funcTiso = [&n_a,&a,&b](const stress& k0_, const stress& mu0_,
                          const stress& ki_, const stress& mui_) {
        auto A = EllipsoidMeanLocalisator<3u, stress>::TransverseIsotropic(
            KGModuli<stress>(k0_, mu0_), KGModuli<stress>(ki_, mui_), n_a, a, b,
            b);
        return A;
      };
      kk_ = {k0, mu0, ki, mui};
      auto funcTiso2 = [&funcTiso,&kk_,&i](const stress& _k_) {
        kk_[i] = _k_;
        return funcTiso(kk_[0], kk_[1], kk_[2], kk_[3]);
      };
      auto dATisoDF_dt =
          tfel::math::computeNumericalDerivative(funcTiso2, k_[i], stress(h));

      for (int ii : {0, 1, 2, 3, 4, 5}) {
        for (int jj : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(my_abs(dAvTiso_dt(ii, jj) - dATisoDF_dt(ii, jj)) <
                            compliance(eps));
        }
      }

      auto od =
          OrientedDistribution<stress>(Ellipsoid(a, b, b), 0.1, KGi, n_b, n_a);
      const auto dAvOr_dt = od.computeDerivativesOfMeanLocalisator(KG0, dt_[i]);
      auto funcOr = [&n_a,&n_b,&a,&b](const stress& k0_, const stress& mu0_,
                        const stress& ki_, const stress& mui_) {
        auto A = EllipsoidMeanLocalisator<3u, stress>::Oriented(
            KGModuli<stress>(k0_, mu0_), KGModuli<stress>(ki_, mui_), n_b, a,
            n_a, b, b);
        return A;
      };
      kk_ = {k0, mu0, ki, mui};
      auto funcOr2 = [&funcOr,&kk_,&i](const stress& _k_) {
        kk_[i] = _k_;
        return funcOr(kk_[0], kk_[1], kk_[2], kk_[3]);
      };
      auto dAOrDF_dt =
          tfel::math::computeNumericalDerivative(funcOr2, k_[i], stress(h));
      for (int ii : {0, 1, 2, 3, 4, 5}) {
        for (int jj : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(my_abs(dAvOr_dt(ii, jj) - dAOrDF_dt(ii, jj)) <
                            compliance(eps));
        }
      }
    }

  }  // end of test_loc_derivative

};  // end of struct DerivativesOfMeanLocalisatorsTest

TFEL_TESTS_GENERATE_PROXY(DerivativesOfMeanLocalisatorsTest,
                          "DerivativesOfMeanLocalisators");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DerivativesOfMeanLocalisators.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
