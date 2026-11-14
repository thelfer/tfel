/*!
 * \file   tests/Material/ChuNeedleman1980NucleationModelsTest.cxx
 * \brief  This file compares the derivatives computed by the nucleation
 * models of Chu and Needleman and by the power law nucleation models to
 * their finite difference approximations
 * \date   13/10/2026
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
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Material/ChuNeedleman1980StrainBasedNucleationModel.hxx"
#include "TFEL/Material/ChuNeedleman1980StressBasedNucleationModel.hxx"
#include "TFEL/Material/PowerLawStrainBasedNucleationModel.hxx"
#include "TFEL/Material/PowerLawStressBasedNucleationModel.hxx"

struct ChuNeedleman1980NucleationModelsTest final
    : public tfel::tests::TestCase {
  ChuNeedleman1980NucleationModelsTest()
      : tfel::tests::TestCase("TFEL/Material",
                              "ChuNeedleman1980NucleationModelsTest") {
  }  // end of ChuNeedleman1980NucleationModelsTest
  tfel::tests::TestResult execute() override {
    this->testStrainBasedModel();
    this->testStressBasedModel();
    this->testPowerLawStrainBasedModel();
    this->testPowerLawStressBasedModel();
    return this->result;
  }  // end of execute
 private:
  //! \brief strain based model, before and after the mean value
  void testStrainBasedModel() {
    using namespace tfel::material;
    const auto params =
        ChuNeedleman1980StrainBasedNucleationModelParameters<double>{
            .fn = 0.04, .en = 0.3, .sn = 0.1};
    constexpr auto h = 1e-7;
    constexpr auto eps = 1e-6;
    for (const auto p : {0.12, 0.25, 0.41}) {
      // derivative of the porosity rate factor with respect to the
      // equivalent plastic strain
      const auto [fr, dfr_dp] =
          computeChuNeedleman1980StrainBasedNucleationModelPorosityRateFactorAndDerivative(
              params, p);
      const auto frp =
          computeChuNeedleman1980StrainBasedNucleationModelPorosityRateFactor(
              params, p + h);
      const auto frm =
          computeChuNeedleman1980StrainBasedNucleationModelPorosityRateFactor(
              params, p - h);
      TFEL_TESTS_ASSERT(std::abs(dfr_dp - (frp - frm) / (2 * h)) <
                        eps * std::abs(fr / params.sn));
      // derivative of the porosity increment with respect to the increment
      // of the equivalent plastic strain
      for (const auto dp : {0.01, 0.05, 0.2}) {
        const auto [df, ddf_ddp] =
            computeChuNeedleman1980StrainBasedNucleationModelPorosityIncrementAndDerivative(
                params, p, dp);
        const auto dfp =
            computeChuNeedleman1980StrainBasedNucleationModelPorosityIncrement(
                params, p, dp + h);
        const auto dfm =
            computeChuNeedleman1980StrainBasedNucleationModelPorosityIncrement(
                params, p, dp - h);
        TFEL_TESTS_ASSERT(std::abs(df - (dfp + dfm) / 2) < 1e-10);
        TFEL_TESTS_ASSERT(std::abs(ddf_ddp - (dfp - dfm) / (2 * h)) <
                          eps * params.fn / params.sn);
      }
    }
  }  // end of testStrainBasedModel
  //! \brief stress based model, before and after the mean value
  void testStressBasedModel() {
    using namespace tfel::material;
    using Stensor = tfel::math::stensor<3u, double>;
    const auto params =
        ChuNeedleman1980StressBasedNucleationModelParameters<Stensor>{
            .fn = 0.04, .sigm = 200, .sn = 50};
    constexpr auto h = 1e-5;
    constexpr auto eps = 1e-6;
    for (const auto s : {120., 230., 290.}) {
      const auto sig = Stensor{s, 0.6 * s, -0.2 * s, 10., 0., 5.};
      const auto [An, dAn_dsig] =
          computeChuNeedleman1980StressBasedNucleationModelPorosityRateFactorAndDerivative(
              params, sig, 1.);
      for (unsigned short i = 0; i != 6; ++i) {
        auto sigp = sig;
        auto sigm = sig;
        sigp[i] += h;
        sigm[i] -= h;
        const auto Anp =
            computeChuNeedleman1980StressBasedNucleationModelPorosityRateFactor(
                params, sigp, 1.);
        const auto Anm =
            computeChuNeedleman1980StressBasedNucleationModelPorosityRateFactor(
                params, sigm, 1.);
        TFEL_TESTS_ASSERT(std::abs(dAn_dsig[i] - (Anp - Anm) / (2 * h)) <
                          eps * std::abs(An / params.sn));
      }
    }
  }  // end of testStressBasedModel
  //! \brief power law strain based model
  void testPowerLawStrainBasedModel() {
    using namespace tfel::material;
    const auto params = PowerLawStrainBasedNucleationModelParameters<double>{
        .fn = 0.04, .en = 0.1, .m = 1.5};
    constexpr auto h = 1e-7;
    constexpr auto eps = 1e-6;
    for (const auto p : {0.15, 0.3}) {
      const auto [An, dAn_dp] =
          computePowerLawStrainBasedNucleationModelPorosityRateFactorAndDerivative(
              params, p);
      const auto Anp =
          computePowerLawStrainBasedNucleationModelPorosityRateFactor(params,
                                                                      p + h);
      const auto Anm =
          computePowerLawStrainBasedNucleationModelPorosityRateFactor(params,
                                                                      p - h);
      TFEL_TESTS_ASSERT(std::abs(dAn_dp - (Anp - Anm) / (2 * h)) <
                        eps * An / params.en);
      for (const auto dp : {0.01, 0.1}) {
        const auto [df, ddf_ddp] =
            computePowerLawStrainBasedNucleationModelPorosityIncrementAndDerivative(
                params, p, dp);
        const auto dfp =
            computePowerLawStrainBasedNucleationModelPorosityIncrement(
                params, p, dp + h);
        const auto dfm =
            computePowerLawStrainBasedNucleationModelPorosityIncrement(
                params, p, dp - h);
        TFEL_TESTS_ASSERT(std::abs(df - (dfp + dfm) / 2) < 1e-10);
        TFEL_TESTS_ASSERT(std::abs(ddf_ddp - (dfp - dfm) / (2 * h)) <
                          eps * params.fn);
      }
    }
  }  // end of testPowerLawStrainBasedModel
  //! \brief power law stress based model
  void testPowerLawStressBasedModel() {
    using namespace tfel::material;
    using Stensor = tfel::math::stensor<3u, double>;
    const auto params = PowerLawStressBasedNucleationModelParameters<Stensor>{
        .fn = 0.04, .sn = 100, .m = 1.5, .pmin = 0};
    constexpr auto h = 1e-5;
    constexpr auto eps = 1e-6;
    for (const auto s : {150., 300.}) {
      const auto sig = Stensor{s, 0.6 * s, -0.2 * s, 10., 0., 5.};
      const auto [An, dAn_dsig] =
          computePowerLawStressBasedNucleationModelPorosityRateFactorAndDerivative(
              params, sig, 1.);
      for (unsigned short i = 0; i != 6; ++i) {
        auto sigp = sig;
        auto sigm = sig;
        sigp[i] += h;
        sigm[i] -= h;
        const auto Anp =
            computePowerLawStressBasedNucleationModelPorosityRateFactor(
                params, sigp, 1.);
        const auto Anm =
            computePowerLawStressBasedNucleationModelPorosityRateFactor(
                params, sigm, 1.);
        TFEL_TESTS_ASSERT(std::abs(dAn_dsig[i] - (Anp - Anm) / (2 * h)) <
                          eps * An / params.sn);
      }
    }
  }  // end of testPowerLawStressBasedModel
};

TFEL_TESTS_GENERATE_PROXY(ChuNeedleman1980NucleationModelsTest,
                          "ChuNeedleman1980NucleationModelsTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ChuNeedleman1980NucleationModelsTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
