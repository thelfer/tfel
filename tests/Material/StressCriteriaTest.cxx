/*!
 * \file   tests/Material/StressCriteriaTest.cxx
 * \brief  This file tests some stress criteria in particular stress states
 * \date   26/10/2026
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
#include "TFEL/Material/GursonTvergaardNeedleman.hxx"
#include "TFEL/Material/GursonTvergaardNeedleman1982StressCriterion.hxx"
#include "TFEL/Material/RousselierTanguyBesson2002StressCriterion.hxx"
#include "TFEL/Material/Cazacu2001YieldCriterion.hxx"
#include "TFEL/Material/Cazacu2004IsotropicYieldCriterion.hxx"
#include "TFEL/Material/Cazacu2004OrthotropicYieldCriterion.hxx"
#include "TFEL/Material/IsotropicPlasticity.hxx"
#include "TFEL/Material/Drucker1949YieldCriterion.hxx"
#include "TFEL/Material/MohrCoulombYieldCriterion.hxx"

struct StressCriteriaTest final : public tfel::tests::TestCase {
  StressCriteriaTest()
      : tfel::tests::TestCase("TFEL/Material", "StressCriteriaTest") {
  }  // end of StressCriteriaTest
  tfel::tests::TestResult execute() override {
    this->testGursonTvergaardNeedleman1982();
    this->testFlowStressGTN();
    this->testCazacu2001();
    this->testCazacu2004();
    this->testDrucker1949();
    this->testMohrCoulomb();
    this->testSinglePrecision();
    this->testPorousCriteriaInSinglePrecision();
    return this->result;
  }  // end of execute
 private:
  using Stensor = tfel::math::stensor<3u, double>;
  //! \brief the GTN criterion is even in the mean stress
  void testGursonTvergaardNeedleman1982() {
    using namespace tfel::material;
    const auto params =
        GursonTvergaardNeedleman1982StressCriterionParameters<Stensor>{
            .f_c = 0.05, .f_r = 0.2, .q_1 = 1.5, .q_2 = 1, .q_3 = 2.25};
    const auto [s1, n1, ds1_df] =
        computeGursonTvergaardNeedleman1982StressNormal(
            Stensor{100, 100, 100, 0, 0, 0}, 0.01, params, 1e-8);
    const auto [s2, n2, ds2_df] =
        computeGursonTvergaardNeedleman1982StressNormal(
            Stensor{-100, -100, -100, 0, 0, 0}, 0.01, params, 1e-8);
    TFEL_TESTS_ASSERT(std::abs(s2 - s1) < 1e-12 * s1);
    TFEL_TESTS_ASSERT(std::abs(n2[0] + n1[0]) < 1e-12);
    TFEL_TESTS_ASSERT(std::abs(ds2_df - ds1_df) < 1e-12 * ds1_df);
  }  // end of testGursonTvergaardNeedleman1982
  //! \brief flow stress of the GTN model for a nearly hydrostatic stress
  void testFlowStressGTN() {
    using namespace tfel::material;
    const auto params =
        GursonTvergaardNeedleman1982StressCriterionParameters<Stensor>{
            .f_c = 0.05, .f_r = 0.2, .q_1 = 1.5, .q_2 = 1, .q_3 = 2.25};
    const auto sig = Stensor{100.1, 100, 99.9, 0, 0, 0};
    const auto s1 = computeFlowStressGTN(tfel::math::sigmaeq(sig),
                                         tfel::math::trace(sig), 0.01, 1.5, 1.);
    const auto s2 =
        computeGursonTvergaardNeedleman1982Stress(sig, 0.01, params, 1e-8);
    TFEL_TESTS_ASSERT(std::abs(s1 - s2) < 1e-10 * s2);
    TFEL_TESTS_ASSERT(
        std::isfinite(computeFlowStressGTN(0.1f, 300.f, 0.01f, 1.5f, 1.f)));
  }  // end of testFlowStressGTN
  //! \brief derivative of the normal of the Cazacu 2001 criterion for c != 1
  void testCazacu2001() {
    using namespace tfel::material;
    const J2OCoefficients<Stensor> a = {0.586, 1.05, 0.823, 0.96, 1, 1};
    const J3OCoefficients<Stensor> b = {1.44, 0.061, -1.302, -0.281, -0.375, 1,
                                        1,    1,     1,      0.445,  1};
    this->checkNormalDerivative([&a, &b](const Stensor& s) {
      return computeCazacu2001StressCriterionSecondDerivative(s, a, b, 1.285,
                                                              1e-12);
    });
  }  // end of testCazacu2001
  //! \brief derivatives of the normals of the Cazacu 2004 criteria for a
  //! zero deviatoric stress
  void testCazacu2004() {
    using namespace tfel::material;
    const J2OCoefficients<Stensor> a = {0.586, 1.05, 0.823, 0.96, 1, 1};
    const J3OCoefficients<Stensor> b = {1.44, 0.061, -1.302, -0.281, -0.375, 1,
                                        1,    1,     1,      0.445,  1};
    for (const auto& sig : {Stensor(0.), Stensor{100, 100, 100, 0, 0, 0}}) {
      const auto dn1 =
          std::get<2>(computeCazacu2004IsotropicStressCriterionSecondDerivative(
              sig, -1.056, 1e-8));
      const auto dn2 = std::get<2>(
          computeCazacu2004OrthotropicStressCriterionSecondDerivative(
              sig, a, b, -1.056, 1e-8));
      for (unsigned short i = 0; i != 6; ++i) {
        for (unsigned short j = 0; j != 6; ++j) {
          TFEL_TESTS_ASSERT(std::isfinite(dn1(i, j)));
          TFEL_TESTS_ASSERT(std::isfinite(dn2(i, j)));
        }
      }
    }
  }  // end of testCazacu2004
  //! \brief derivative of the normal of the Drucker 1949 criterion for c != 1
  void testDrucker1949() {
    using namespace tfel::material;
    this->checkNormalDerivative([](const Stensor& s) {
      return computeDrucker1949StressCriterionSecondDerivative(s, 1.285, 1e-12);
    });
  }  // end of testDrucker1949
  //! \brief derivative of the normal of the Mohr-Coulomb criterion in pure
  //! shear (vanishing third invariant) and on a triaxial axis
  void testMohrCoulomb() {
    using namespace tfel::material;
    using Parameters = MohrCoulombParameters<Stensor>;
    const auto p =
        makeMohrCoulombParameters<Stensor, Parameters::DEGREE>(3, 30, 29, 1);
    for (const auto& sig :
         {Stensor{0, 0, 0, 100, 0, 0}, Stensor{-100, -20, -20, 0, 0, 0}}) {
      this->checkNormalDerivative(
          [&p](const Stensor& s) {
            return computeMohrCoulombStressCriterionSecondDerivative(p, s);
          },
          sig);
    }
  }  // end of testMohrCoulomb
  //! \return the Drucker 1949 and Cazacu 2001 criteria for a stress in Pa
  template <typename real>
  static auto computeCriteriaInPa() {
    using namespace tfel::material;
    using StressStensor = tfel::math::stensor<3u, real>;
    const auto sig = StressStensor{150e6, -40e6, 25e6, 30e6, -12e6, 18e6};
    const J2OCoefficients<StressStensor> a = {0.586, 1.05, 0.823, 0.96, 1, 1};
    const J3OCoefficients<StressStensor> b = {
        1.44, 0.061, -1.302, -0.281, -0.375, 1, 1, 1, 1, 0.445, 1};
    return std::make_tuple(computeDrucker1949StressCriterionSecondDerivative(
                               sig, real(1.285), real(0.2)),
                           computeCazacu2001StressCriterionSecondDerivative(
                               sig, a, b, real(1.285), real(0.2)));
  }  // end of computeCriteriaInPa
  //! \brief criteria in single precision with stresses in Pa
  void testSinglePrecision() {
    const auto [druckerf, cazacuf] = computeCriteriaInPa<float>();
    const auto [druckerd, cazacud] = computeCriteriaInPa<double>();
    this->compare(druckerf, druckerd);
    this->compare(cazacuf, cazacud);
  }  // end of testSinglePrecision
  //! \return the GTN and Rousselier equivalent stresses for a stress in Pa
  template <typename real>
  static auto computePorousCriteriaInPa(const Stensor& sig) {
    using namespace tfel::material;
    using StressStensor = tfel::math::stensor<3u, real>;
    auto s = StressStensor{};
    for (unsigned short i = 0; i != 6; ++i) {
      s[i] = static_cast<real>(sig[i]);
    }
    const auto pg =
        GursonTvergaardNeedleman1982StressCriterionParameters<StressStensor>{
            .f_c = 0.01, .f_r = 0.1, .q_1 = 1.5, .q_2 = 1, .q_3 = 2.25};
    auto pr =
        RousselierTanguyBesson2002StressCriterionParameters<StressStensor>{};
    pr.DR = 2;
    pr.qR = 1.5;
    return std::make_tuple(
        computeGursonTvergaardNeedleman1982Stress(s, real(0.01), pg, real(0.2)),
        computeRousselierTanguyBesson2002Stress(s, real(0.01), pr, real(0.2)));
  }  // end of computePorousCriteriaInPa
  //! \brief GTN and Rousselier criteria in single precision with stresses in
  //! Pa
  void testPorousCriteriaInSinglePrecision() {
    for (const auto& sig : {Stensor{150e6, -40e6, 25e6, 30e6, -12e6, 18e6},
                            Stensor{73.8e6, 10.9e6, -17.1e6, 10e6, 0, 0}}) {
      const auto [gtnf, rousselierf] = computePorousCriteriaInPa<float>(sig);
      const auto [gtnd, rousselierd] = computePorousCriteriaInPa<double>(sig);
      TFEL_TESTS_ASSERT(std::abs(static_cast<double>(gtnf) - gtnd) <
                        1e-5 * gtnd);
      TFEL_TESTS_ASSERT(std::abs(static_cast<double>(rousselierf) -
                                 rousselierd) < 1e-5 * rousselierd);
    }
  }  // end of testPorousCriteriaInSinglePrecision
  /*!
   * \brief compare the equivalent stress, the normal and its derivative
   * computed in single precision to the ones computed in double precision
   */
  template <typename SinglePrecisionResults, typename DoublePrecisionResults>
  void compare(const SinglePrecisionResults& rf,
               const DoublePrecisionResults& rd) {
    const auto seq = std::get<0>(rd);
    TFEL_TESTS_ASSERT(std::abs(static_cast<double>(std::get<0>(rf)) - seq) <
                      1e-5 * seq);
    for (unsigned short i = 0; i != 6; ++i) {
      TFEL_TESTS_ASSERT(std::abs(static_cast<double>(std::get<1>(rf)[i]) -
                                 std::get<1>(rd)[i]) < 1e-5);
      for (unsigned short j = 0; j != 6; ++j) {
        TFEL_TESTS_ASSERT(std::abs(static_cast<double>(std::get<2>(rf)(i, j)) -
                                   std::get<2>(rd)(i, j)) *
                              seq <
                          1e-4);
      }
    }
  }  // end of compare
  /*!
   * \brief compare the derivative of the normal to its finite difference
   * approximation
   * \param[in] f: function returning the equivalent stress, the normal and
   * its derivative
   * \param[in] sig: stress
   */
  template <typename Function>
  void checkNormalDerivative(const Function& f,
                             const Stensor& sig = {150, -40, 25, 30, -12, 18}) {
    const auto dn = std::get<2>(f(sig));
    constexpr auto h = 1e-4;
    for (unsigned short j = 0; j != 6; ++j) {
      auto sigp = sig;
      auto sigm = sig;
      sigp[j] += h;
      sigm[j] -= h;
      const auto np = std::get<1>(f(sigp));
      const auto nm = std::get<1>(f(sigm));
      for (unsigned short i = 0; i != 6; ++i) {
        TFEL_TESTS_ASSERT(std::abs(dn(i, j) - (np[i] - nm[i]) / (2 * h)) <
                          1e-8);
      }
    }
  }  // end of checkNormalDerivative
};

TFEL_TESTS_GENERATE_PROXY(StressCriteriaTest, "StressCriteriaTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StressCriteriaTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
