/*!
 * \file   mfront/tests/behaviours/generic/IntegrationDriversTest.cxx
 * \brief  tests of the integration drivers of the `generic` interface which
 * can't be checked by `MTest`.
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
          const auto e = ::getenv("MFrontGenericBehaviours4BuildPath");
          return e == nullptr ? std::string{} : e;
        }()) {}  // end of IntegrationDriversTest

  tfel::tests::TestResult execute() override {
    this->checkTimeStepScalingFactor();
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
    const auto integrate = [&f](MaterialPoint& m, const int sm, const int to,
                                const mfront_gb_real a) {
      m.g1 = {1.02, 0.99, 1.01, 0.03, -0.01, 0.02, 0, -0.02, 0.01};
      m.mps = {150e9, 0.3, a};
      std::fill(m.th1.begin(), m.th1.end(), -999);
      std::fill(m.K.begin(), m.K.end(), -999);
      // consistent tangent operator, stress measure and tangent operator
      m.K[0] = 4;
      m.K[1] = sm;
      m.K[2] = to;
      return f(&(m.d));
    };
    const auto are_equal = [](const auto& v1, const auto& v2) {
      return std::equal(v1.begin(), v1.end(), v2.begin(),
                        [](const mfront_gb_real a, const mfront_gb_real b) {
                          return std::abs(a - b) < 1e-12 * (1 + std::abs(a));
                        });
    };
    const char* const stress_measures[] = {"CAUCHY", "PK2", "PK1"};
    const char* const tangent_operators[] = {"DSIG_DF", "DS_DEGL", "DPK1_DF",
                                             "DTAU_DDF"};
    for (const auto sm : {0, 1, 2}) {
      for (const auto to : {0, 1, 2, 3}) {
        const auto c = std::string{stress_measures[sm]} + ", " +
                       tangent_operators[to] + ": ";
        MaterialPoint m1;
        MaterialPoint m2;
        this->registerResult(c + "integration (rdt=1)",
                             integrate(m1, sm, to, 1) == 1);
        this->registerResult(c + "integration (rdt=0.5)",
                             integrate(m2, sm, to, 0.5) == 0);
        this->registerResult(c + "rdt", std::abs(m2.rdt - 0.5) < 1e-14);
        this->registerResult(c + "stress", are_equal(m1.th1, m2.th1));
        this->registerResult(c + "tangent operator", are_equal(m1.K, m2.K));
      }
    }
  }  // end of checkTimeStepScalingFactor
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
