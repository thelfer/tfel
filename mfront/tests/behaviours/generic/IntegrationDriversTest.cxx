/*!
 * \file   mfront/tests/behaviours/generic/IntegrationDriversTest.cxx
 * \brief  tests of the integration drivers of the `generic` interface which
 * can't be checked by `MTest`: integrations returning a time step scaling
 * factor lower than one, post-processings and initialize functions.
 * \date   20/10/2026
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
#include <array>
#include <string>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/GenericBehaviour/BehaviourData.h"

/*!
 * \brief state of a material point for the tridimensional modelling
 * hypothesis. The arrays are large enough for small and finite strain
 * behaviours.
 */
struct MaterialPoint {
  MaterialPoint() {
    this->d.error_message = this->error_message.data();
    this->d.dt = 1;
    this->d.K = this->K.data();
    this->d.rdt = &(this->rdt);
    this->d.speed_of_sound = &(this->speed_of_sound);
    this->d.s0.gradients = this->g0.data();
    this->d.s0.thermodynamic_forces = this->th0.data();
    this->d.s0.mass_density = &(this->mass_density);
    this->d.s0.material_properties = this->mps.data();
    this->d.s0.internal_state_variables = this->isvs0.data();
    this->d.s0.stored_energy = &(this->stored_energy0);
    this->d.s0.dissipated_energy = &(this->dissipated_energy0);
    this->d.s0.external_state_variables = this->esvs.data();
    this->d.s1.gradients = this->g1.data();
    this->d.s1.thermodynamic_forces = this->th1.data();
    this->d.s1.mass_density = &(this->mass_density);
    this->d.s1.material_properties = this->mps.data();
    this->d.s1.internal_state_variables = this->isvs1.data();
    this->d.s1.stored_energy = &(this->stored_energy1);
    this->d.s1.dissipated_energy = &(this->dissipated_energy1);
    this->d.s1.external_state_variables = this->esvs.data();
  }
  MaterialPoint(MaterialPoint&&) = delete;
  MaterialPoint(const MaterialPoint&) = delete;
  MaterialPoint& operator=(MaterialPoint&&) = delete;
  MaterialPoint& operator=(const MaterialPoint&) = delete;
  //! \brief gradients at the beginning of the time step
  std::array<mfront_gb_real, 9> g0 = {1, 1, 1, 0, 0, 0, 0, 0, 0};
  //! \brief gradients at the end of the time step
  std::array<mfront_gb_real, 9> g1 = {1, 1, 1, 0, 0, 0, 0, 0, 0};
  //! \brief thermodynamic forces at the beginning of the time step
  std::array<mfront_gb_real, 9> th0 = {};
  //! \brief thermodynamic forces at the end of the time step
  std::array<mfront_gb_real, 9> th1 = {};
  //! \brief tangent operator
  std::array<mfront_gb_real, 81> K = {};
  //! \brief material properties
  std::array<mfront_gb_real, 3> mps = {};
  //! \brief internal state variables at the beginning of the time step
  std::array<mfront_gb_real, 1> isvs0 = {};
  //! \brief internal state variables at the end of the time step
  std::array<mfront_gb_real, 1> isvs1 = {};
  //! \brief external state variables (temperature)
  std::array<mfront_gb_real, 1> esvs = {293.15};
  mfront_gb_real mass_density = 1;
  mfront_gb_real rdt = 1;
  mfront_gb_real speed_of_sound = 0;
  mfront_gb_real stored_energy0 = 0;
  mfront_gb_real stored_energy1 = 0;
  mfront_gb_real dissipated_energy0 = 0;
  mfront_gb_real dissipated_energy1 = 0;
  std::array<char, 512> error_message = {};
  mfront_gb_BehaviourData d;
};  // end of struct MaterialPoint

struct IntegrationDriversTest final : public tfel::tests::TestCase {
  IntegrationDriversTest()
      : tfel::tests::TestCase("MFront/GenericBehaviour",
                              "IntegrationDriversTest"),
        library([] {
          const auto e =
              ::getenv("MFrontGenericIntegrationDriversBehavioursBuildPath");
          return e == nullptr ? std::string{} : e;
        }()) {}  // end of IntegrationDriversTest

  tfel::tests::TestResult execute() override {
    this->checkTimeStepScalingFactor();
    this->checkPostProcessing();
    this->checkInitializeFunctions();
    return this->result;
  }  // end of execute

 private:
  /*!
   * \brief when the behaviour proposes a time step scaling factor lower than
   * one after the integration, the integration succeeds (the returned value
   * is 0): the stress and the tangent operator must be the ones obtained
   * with a time step scaling factor equal to one.
   */
  void checkTimeStepScalingFactor() {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto f = elm.getGenericBehaviourFunction(
        this->library, "HenckyTimeStepScalingFactorTest_Tridimensional");
    const auto integrate = [&f](MaterialPoint& m, const mfront_gb_real a) {
      m.g1 = {1.02, 0.99, 1.01, 0.03, -0.01, 0.02, 0, -0.02, 0.01};
      m.mps = {150e9, 0.3, a};
      std::fill(m.th1.begin(), m.th1.end(), -999);
      std::fill(m.K.begin(), m.K.end(), -999);
      // consistent tangent operator, Cauchy stress, derivative of the Cauchy
      // stress with respect to the deformation gradient
      m.K[0] = 4;
      m.K[1] = 0;
      m.K[2] = 0;
      return f(&(m.d));
    };
    const auto are_equal = [](const auto& v1, const auto& v2) {
      return std::equal(v1.begin(), v1.end(), v2.begin(),
                        [](const mfront_gb_real a, const mfront_gb_real b) {
                          return std::abs(a - b) < 1e-12 * (1 + std::abs(a));
                        });
    };
    MaterialPoint m1;
    MaterialPoint m2;
    TFEL_TESTS_ASSERT(integrate(m1, 1) == 1);
    TFEL_TESTS_ASSERT(integrate(m2, 0.5) == 0);
    TFEL_TESTS_ASSERT(std::abs(m2.rdt - 0.5) < 1e-14);
    this->registerResult("time step scaling factor: stress",
                         are_equal(m1.th1, m2.th1));
    this->registerResult("time step scaling factor: tangent operator",
                         are_equal(m1.K, m2.K));
  }  // end of checkTimeStepScalingFactor
  /*!
   * \brief a post-processing must see the internal state variables at the
   * end of the time step.
   */
  void checkPostProcessing() {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto b = std::string{"InternalStateVariablePostProcessingTest"};
    const auto f =
        elm.getGenericBehaviourFunction(this->library, b + "_Tridimensional");
    const auto pp = elm.getGenericBehaviourPostProcessingFunction(
        this->library, b, "Tridimensional", "InternalStateVariable");
    MaterialPoint m;
    std::fill(m.g0.begin(), m.g0.end(), 0);
    std::fill(m.g1.begin(), m.g1.end(), 0);
    m.isvs0 = {0.25};
    // integration without tangent operator: p is increased by one
    m.K[0] = 0;
    TFEL_TESTS_ASSERT(f(&(m.d)) == 1);
    TFEL_TESTS_ASSERT(std::abs(m.isvs1[0] - 1.25) < 1e-14);
    auto p_end = mfront_gb_real{-1};
    TFEL_TESTS_ASSERT(pp(&p_end, &(m.d)) == 0);
    this->registerResult(
        "post-processing: value of p at the end of the "
        "time step",
        std::abs(p_end - 1.25) < 1e-14);
  }  // end of checkPostProcessing
  /*!
   * \brief the initialize functions of finite strain behaviours must
   * compile and return the stress they compute in the stress measure of the
   * caller, here the first Piola-Kirchhoff stress, using the deformation
   * gradient at the beginning of the time step.
   */
  void checkInitializeFunctions() {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    // pressure given to the initialize functions
    const auto pr = mfront_gb_real{100};
    // diagonal of the deformation gradient at the beginning of the time step
    const auto F0 = std::array<mfront_gb_real, 3>{1.2, 0.9, 1.1};
    const auto J = F0[0] * F0[1] * F0[2];
    const auto check = [&](const std::string& b, const auto& expected) {
      const auto f = elm.getGenericBehaviourInitializeFunction(
          this->library, b, "Tridimensional", "StressFromPressure");
      MaterialPoint m;
      m.g0 = {F0[0], F0[1], F0[2], 0, 0, 0, 0, 0, 0};
      // the deformation gradient at the end of the time step must not be
      // used by the initialize function
      m.g1 = {1.3, 0.8, 1.05, 0.1, -0.05, 0.02, 0.03, -0.01, 0.04};
      std::fill(m.th1.begin(), m.th1.end(), -999);
      // first Piola-Kirchhoff stress
      m.K[1] = 2;
      TFEL_TESTS_ASSERT(f(&(m.d), &pr) == 0);
      // the expected stress is diagonal
      auto ok = true;
      for (unsigned short i = 0; i != 3; ++i) {
        ok = ok && (std::abs(m.th1[i] - expected(F0[i], J, pr)) < 1e-12 * pr);
      }
      for (unsigned short i = 3; i != 9; ++i) {
        ok = ok && (std::abs(m.th1[i]) < 1e-12 * pr);
      }
      this->registerResult(b + ": first Piola-Kirchhoff stress", ok);
    };
    // expected components (i,i) of the first Piola-Kirchhoff stress
    check("FiniteStrainInitializeFunctionTest",  // Cauchy stress p I
          [](const mfront_gb_real F, const mfront_gb_real Je,
             const mfront_gb_real p) { return Je * p / F; });
    check("GreenLagrangeInitializeFunctionTest",  // PK2 stress p I
          [](const mfront_gb_real F, const mfront_gb_real,
             const mfront_gb_real p) { return p * F; });
    check("HenckyInitializeFunctionTest",  // Kirchhoff stress p I
          [](const mfront_gb_real F, const mfront_gb_real,
             const mfront_gb_real p) { return p / F; });
  }  // end of checkInitializeFunctions
  //! \brief path to the library
  const std::string library;
};  // end of struct IntegrationDriversTest

TFEL_TESTS_GENERATE_PROXY(IntegrationDriversTest, "IntegrationDriversTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("IntegrationDriversTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
