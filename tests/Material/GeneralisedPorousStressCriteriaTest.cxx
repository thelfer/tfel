/*!
 * \file   tests/Material/GeneralisedPorousStressCriteriaTest.cxx
 * \brief  This file tests the generalised Gurson-Tvergaard-Needleman and
 * Rousselier-Tanguy-Besson stress criteria.
 * \author Tristan Chenaille
 * \date   14/11/2026
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
#include <tuple>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/Hosford1972YieldCriterion.hxx"
#include "TFEL/Material/GeneralisedGursonTvergaardNeedlemanStressCriterion.hxx"
#include "TFEL/Material/GeneralisedRousselierTanguyBessonStressCriterion.hxx"

struct GeneralisedPorousStressCriteriaTest final
    : public tfel::tests::TestCase {
  GeneralisedPorousStressCriteriaTest()
      : tfel::tests::TestCase("TFEL/Material",
                              "GeneralisedPorousStressCriteriaTest") {
  }  // end of GeneralisedPorousStressCriteriaTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::material;
    const auto gtn =
        GursonTvergaardNeedleman1982StressCriterionParameters<Stensor>{
            .f_c = 0.05, .f_r = 0.2, .q_1 = 1.5, .q_2 = 1, .q_3 = 2.25};
    auto rtb = RousselierTanguyBesson2002StressCriterionParameters<Stensor>{};
    rtb.DR = 2;
    rtb.qR = 1.5;
    // generalised criteria as functions of the equivalent stress
    const auto ggtn = [&gtn](const Stensor& sig, const double seq,
                             const Stensor& n, const Stensor4& dn,
                             const double f) {
      return computeGeneralisedGursonTvergaardNeedlemanStressSecondDerivative(
          sig, seq, n, dn, f, gtn, seps);
    };
    const auto grtb = [&rtb](const Stensor& sig, const double seq,
                             const Stensor& n, const Stensor4& dn,
                             const double f) {
      return computeGeneralisedRousselierTanguyBessonStressSecondDerivative(
          sig, seq, n, dn, f, rtb, seps);
    };
    // the porosities are chosen below and above the coalescence porosity of
    // the Gurson-Tvergaard-Needleman criterion
    for (const auto f : {0.01, 0.08}) {
      for (const auto& sig : {Stensor{150, 20, -30, 40, -25, 10},
                              Stensor{420, 300, 310, 15, 0, -20},
                              Stensor{-250, -310, -300, 0, 12, 5}}) {
        this->compareToVonMisesBasedCriterion(
            ggtn,
            computeGursonTvergaardNeedleman1982StressSecondDerivative(
                sig, f, gtn, seps),
            sig, f);
        this->compareToVonMisesBasedCriterion(
            grtb,
            computeRousselierTanguyBesson2002StressSecondDerivative(sig, f, rtb,
                                                                    seps),
            sig, f);
        this->checkDerivatives(ggtn, sig, f);
        this->checkDerivatives(grtb, sig, f);
      }
    }
    this->checkStressAndNormal();
    return this->result;
  }  // end of execute

 private:
  using Stensor = tfel::math::stensor<3u, double>;
  using Stensor4 = tfel::math::st2tost2<3u, double>;
  //! \brief threshold for the equivalent stress
  static constexpr double seps = 1e-10;
  /*!
   * \brief check that the generalised criterion is the original one when the
   * equivalent stress is the von Mises stress
   */
  template <typename Criterion, typename Results>
  void compareToVonMisesBasedCriterion(const Criterion& c,
                                       const Results& r,
                                       const Stensor& sig,
                                       const double f) {
    const auto seq = tfel::math::sigmaeq(sig);
    const auto nvm = Stensor{3 * tfel::math::deviator(sig) / (2 * seq)};
    const auto dnvm = Stensor4{(Stensor4::M() - (nvm ^ nvm)) / seq};
    const auto [s, n, ds_df, dn, dn_df] = c(sig, seq, nvm, dnvm, f);
    const auto& [s0, n0, ds0_df, dn0, dn0_df] = r;
    TFEL_TESTS_ASSERT(std::abs(s - s0) < 1e-12 * s0);
    TFEL_TESTS_ASSERT(std::abs(ds_df - ds0_df) < 1e-10 * std::abs(ds0_df));
    for (unsigned short i = 0; i != 6; ++i) {
      TFEL_TESTS_ASSERT(std::abs(n[i] - n0[i]) < 1e-10);
      TFEL_TESTS_ASSERT(std::abs(dn_df[i] - dn0_df[i]) <
                        1e-10 * std::max(std::abs(dn0_df[i]), 1.));
      for (unsigned short j = 0; j != 6; ++j) {
        TFEL_TESTS_ASSERT(std::abs(dn(i, j) - dn0(i, j)) < 1e-10 / s0);
      }
    }
  }  // end of compareToVonMisesBasedCriterion
  /*!
   * \brief check the derivatives of the generalised criterion by centred
   * finite differences, the equivalent stress being the Hosford stress
   */
  template <typename Criterion>
  void checkDerivatives(const Criterion& c,
                        const Stensor& sig,
                        const double f) {
    constexpr auto a = 6.;
    const auto criterion = [&c](const Stensor& s, const double p) {
      const auto [seq, n, dn] =
          tfel::material::computeHosfordStressSecondDerivative(s, a, seps);
      return c(s, seq, n, dn, p);
    };
    const auto [s, n, ds_df, dn, dn_df] = criterion(sig, f);
    constexpr auto ds = 1e-3;
    for (unsigned short j = 0; j != 6; ++j) {
      auto sig_p = sig;
      auto sig_m = sig;
      sig_p[j] += ds;
      sig_m[j] -= ds;
      const auto rp = criterion(sig_p, f);
      const auto rm = criterion(sig_m, f);
      const auto nj = (std::get<0>(rp) - std::get<0>(rm)) / (2 * ds);
      TFEL_TESTS_ASSERT(std::abs(n[j] - nj) < 1e-7);
      for (unsigned short i = 0; i != 6; ++i) {
        const auto dnij = (std::get<1>(rp)[i] - std::get<1>(rm)[i]) / (2 * ds);
        TFEL_TESTS_ASSERT(std::abs(dn(i, j) - dnij) < 1e-7 / s);
      }
    }
    constexpr auto df = 1e-6;
    const auto rp = criterion(sig, f + df);
    const auto rm = criterion(sig, f - df);
    const auto ds_df_fd = (std::get<0>(rp) - std::get<0>(rm)) / (2 * df);
    TFEL_TESTS_ASSERT(std::abs(ds_df - ds_df_fd) < 1e-6 * std::abs(ds_df));
    for (unsigned short i = 0; i != 6; ++i) {
      const auto dni_df = (std::get<1>(rp)[i] - std::get<1>(rm)[i]) / (2 * df);
      TFEL_TESTS_ASSERT(std::abs(dn_df[i] - dni_df) <
                        1e-6 * std::max(std::abs(dn_df[i]), 1.));
    }
  }  // end of checkDerivatives
  /*!
   * \brief check that the three functions associated with a criterion
   * return the same stress and the same normal
   */
  void checkStressAndNormal() {
    using namespace tfel::material;
    const auto gtn =
        GursonTvergaardNeedleman1982StressCriterionParameters<Stensor>{
            .f_c = 0.05, .f_r = 0.2, .q_1 = 1.5, .q_2 = 1, .q_3 = 2.25};
    auto rtb = RousselierTanguyBesson2002StressCriterionParameters<Stensor>{};
    rtb.DR = 2;
    rtb.qR = 1.5;
    const auto sig = Stensor{150, 20, -30, 40, -25, 10};
    const auto f = 0.01;
    const auto [seq, n, dn] =
        computeHosfordStressSecondDerivative(sig, 6., seps);
    {
      const auto s1 = computeGeneralisedGursonTvergaardNeedlemanStress(
          sig, seq, f, gtn, seps);
      const auto [s2, n2, ds2_df] =
          computeGeneralisedGursonTvergaardNeedlemanStressNormal(sig, seq, n, f,
                                                                 gtn, seps);
      const auto [s3, n3, ds3_df, dn3, dn3_df] =
          computeGeneralisedGursonTvergaardNeedlemanStressSecondDerivative(
              sig, seq, n, dn, f, gtn, seps);
      TFEL_TESTS_ASSERT(std::abs(s2 - s1) < 1e-14 * s1);
      TFEL_TESTS_ASSERT(std::abs(s3 - s1) < 1e-14 * s1);
      TFEL_TESTS_ASSERT(std::abs(ds3_df - ds2_df) < 1e-14 * std::abs(ds2_df));
      for (unsigned short i = 0; i != 6; ++i) {
        TFEL_TESTS_ASSERT(std::abs(n3[i] - n2[i]) < 1e-14);
      }
    }
    {
      const auto s1 = computeGeneralisedRousselierTanguyBessonStress(
          sig, seq, f, rtb, seps);
      const auto [s2, n2, ds2_df] =
          computeGeneralisedRousselierTanguyBessonStressNormal(sig, seq, n, f,
                                                               rtb, seps);
      const auto [s3, n3, ds3_df, dn3, dn3_df] =
          computeGeneralisedRousselierTanguyBessonStressSecondDerivative(
              sig, seq, n, dn, f, rtb, seps);
      TFEL_TESTS_ASSERT(std::abs(s2 - s1) < 1e-14 * s1);
      TFEL_TESTS_ASSERT(std::abs(s3 - s1) < 1e-14 * s1);
      TFEL_TESTS_ASSERT(std::abs(ds3_df - ds2_df) < 1e-14 * std::abs(ds2_df));
      for (unsigned short i = 0; i != 6; ++i) {
        TFEL_TESTS_ASSERT(std::abs(n3[i] - n2[i]) < 1e-14);
      }
    }
  }  // end of checkStressAndNormal
};

TFEL_TESTS_GENERATE_PROXY(GeneralisedPorousStressCriteriaTest,
                          "GeneralisedPorousStressCriteriaTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("GeneralisedPorousStressCriteria.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
