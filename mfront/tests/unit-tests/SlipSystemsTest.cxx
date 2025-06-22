/*!
 * \file   mfront/tests/unit-tests/SlipSystemsTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15 juin 2017
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
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/Tensor/TensorConceptIO.hxx"
#include "TFEL/Material/MetallicCFCSlidingSystems.hxx"
#include "MFront/BehaviourDescription.hxx"

struct SlipSystemsTest final : public tfel::tests::TestCase {
  SlipSystemsTest()
      : tfel::tests::TestCase("MFront", "SlipSystemsTest") {
  }  // end of SlipSystemsTest

  tfel::tests::TestResult execute() override {
    this->bcc();
    return this->result;
  }  // end of execute

 private:
  void bcc() {
    using real = long double;
    using namespace tfel::material;
    using system = SlipSystemsDescription::system;
    using system3d = SlipSystemsDescription::system3d;
    using vec3d = SlipSystemsDescription::vec3d;
    const auto& ss = MetallicCFCSlidingSystems<real>::getSlidingSystems();
    auto make_vec = [](const int v0, const int v1, const int v2) {
      auto v = vec3d();
      v[0] = v0;
      v[1] = v1;
      v[2] = v2;
      return v;
    };
    auto make_sys = [](const vec3d& d, const vec3d& p) {
      auto s = system3d();
      s.plane = p;
      s.burgers = d;
      return system(s);
    };
    const auto gs = make_sys(make_vec(1, -1, 0), make_vec(1, 1, 1));
    mfront::BehaviourDescription bd;
    bd.setSymmetryType(mfront::ORTHOTROPIC);
    bd.setCrystalStructure(CrystalStructure::BCC);
    bd.setSlipSystems(std::vector<system>(1u, gs));
    const auto o = bd.getSlipSystems().getOrientationTensors(0);
    for (const auto& t : o) {
      std::cout << "\nt: " << t[0] << " " << t[1] << " " << t[2] << " " << t[3]
                << " " << t[4] << " " << t[5] << " " << t[6] << " " << t[7]
                << " " << t[8] << '\n';
      const auto b = [&ss, &t] {
        for (const auto& mu : ss.mu) {
          std::cout << "mu: " << mu << '\n';
          auto e1 = real{};
          auto e2 = real{};
          for (decltype(t.size()) i = 0; i != t.size(); ++i) {
            e1 += std::abs(t[i] - mu[i]);
            e2 += std::abs(t[i] - mu[i]);
          }
          if ((e1 < 1.e-14) || (e2 < 1.e-14)) {
            return true;
          }
        }
        return false;
      }();
      // TFEL_TESTS_ASSERT(b);
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(SlipSystemsTest, "SlipSystemsTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("SlipSystemsTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
