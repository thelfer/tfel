/*!
 * \file   SlipSystemsDescriptionTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12 juin 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/FSAlgorithm/equal.hxx"
#include "TFEL/Material/SlipSystemsDescription.hxx"

struct SlipSystemsDescriptionTest final : public tfel::tests::TestCase {
  SlipSystemsDescriptionTest()
      : tfel::tests::TestCase("TFEL/Material", "SlipSystemsDescriptionTest") {
  }  // end of SlipSystemsDescription
  tfel::tests::TestResult execute() override {
    this->checkFCC();
    this->checkHCP();
    return this->result;
  }
  ~SlipSystemsDescriptionTest() override = default;

 private:
  using SlipSystemsDescription = tfel::material::SlipSystemsDescription;

  template <size_t N>
  static bool equal(const std::array<int, N>& v1,
                    const std::array<int, N>& v2) {
    return tfel::fsalgo::equal<N>::exe(v1.begin(), v2.begin());
  }  // end of equal
  template <size_t N>
  static bool equal(const SlipSystemsDescription::system& g1,
                    const std::array<int, N>& b,
                    const std::array<int, N>& p) {
    using system_type =
        typename std::conditional<N == 3, SlipSystemsDescription::system3d,
                                  SlipSystemsDescription::system4d>::type;
    if (!g1.template is<system_type>()) {
      return false;
    }
    const auto& g1b = g1.template get<system_type>();
    return (equal(g1b.burgers, b)) && (equal(g1b.plane, p));
  }  // end of equal
  template <size_t N>
  static bool contains(const std::vector<SlipSystemsDescription::system>& gs,
                       const std::array<int, N>& b,
                       const std::array<int, N>& p) {
    for (const auto& g : gs) {
      if (equal(g, b, p)) {
        return true;
      }
    }
    return false;
  }
  //! \brief tests related to FCC systems
  void checkFCC() {
    using namespace tfel::material;
    using vec3d = SlipSystemsDescription::vec3d;
    using system3d = SlipSystemsDescription::system3d;
    auto ssd = SlipSystemsDescription(CrystalStructure::FCC);
    auto make_vec = [](const int v0, const int v1, const int v2) {
      auto v = vec3d();
      v[0] = v0;
      v[1] = v1;
      v[2] = v2;
      return v;
    };
    /* slip systems */
    ssd.addSlipSystemsFamily(make_vec(1, 1, 0), make_vec(1, -1, 1));
    TFEL_TESTS_CHECK_EQUAL(ssd.getNumberOfSlipSystemsFamilies(), 1u);
    TFEL_TESTS_CHECK_THROW(ssd.getSlipSystemFamily(1), std::runtime_error);
    const auto& gsf = ssd.getSlipSystemFamily(0);
    TFEL_TESTS_ASSERT(gsf.is<system3d>());
    const auto ssf0 = gsf.get<system3d>();
    TFEL_TESTS_ASSERT(equal(ssf0.burgers, make_vec(1, 1, 0)));
    TFEL_TESTS_ASSERT(equal(ssf0.plane, make_vec(1, -1, 1)));
    const auto gs = ssd.getSlipSystems();
    TFEL_TESTS_CHECK_EQUAL(gs.size(), 1u);
    const auto& gs0 = gs[0];
    TFEL_TESTS_CHECK_EQUAL(gs0.size(), 12u);
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(0, 1, -1), make_vec(1, 1, 1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(1, 0, -1), make_vec(1, 1, 1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(1, -1, 0), make_vec(1, 1, 1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(0, 1, 1), make_vec(1, 1, -1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(1, 0, 1), make_vec(1, 1, -1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(1, -1, 0), make_vec(1, 1, -1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(0, 1, -1), make_vec(1, -1, -1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(1, 0, 1), make_vec(1, -1, -1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(1, 1, 0), make_vec(1, -1, -1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(0, 1, 1), make_vec(1, -1, 1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(1, 0, -1), make_vec(1, -1, 1)));
    TFEL_TESTS_ASSERT(contains(gs0, make_vec(1, 1, 0), make_vec(1, -1, 1)));
    /* interaction matrix */
    const auto im = ssd.getInteractionMatrixStructure();
    TFEL_TESTS_CHECK_EQUAL(im.rank(), 7u);
    TFEL_TESTS_CHECK_EQUAL(
        im.getRank(system3d{make_vec(0, 1, -1), make_vec(1, 1, 1)},
                   system3d{make_vec(0, 1, -1), make_vec(1, 1, 1)}),
        0u);
  }
  //! \brief tests related to HCP systems
  void checkHCP() {
    using namespace tfel::material;
    using vec4d = SlipSystemsDescription::vec4d;
    using system4d = SlipSystemsDescription::system4d;
    auto ssd = SlipSystemsDescription(CrystalStructure::HCP);
    auto make_vec = [](const int v0, const int v1, const int v2, const int v3) {
      auto v = vec4d();
      v[0] = v0;
      v[1] = v1;
      v[2] = v2;
      v[3] = v3;
      return v;
    };
    /* slip systems */
    ssd.addSlipSystemsFamily(make_vec(-2, 1, 1, 0), make_vec(0, 0, 0, 1));
    ssd.addSlipSystemsFamily(make_vec(1, 1, -2, 0), make_vec(1, -1, 0, 0));
    ssd.addSlipSystemsFamily(make_vec(1, 1, -2, 0), make_vec(1, -1, 0, 1));
    ssd.addSlipSystemsFamily(make_vec(-2, 1, 1, 3), make_vec(1, -1, 0, 1));
    TFEL_TESTS_CHECK_EQUAL(ssd.getNumberOfSlipSystemsFamilies(), 4u);
    TFEL_TESTS_CHECK_THROW(ssd.getSlipSystemFamily(4), std::runtime_error);
    const auto gs = ssd.getSlipSystems();
    TFEL_TESTS_CHECK_EQUAL(gs.size(), 4u);
    // first gliding system family
    const auto& gsf = ssd.getSlipSystemFamily(0);
    TFEL_TESTS_ASSERT(gsf.is<system4d>());
    const auto ssf0 = gsf.get<system4d>();
    TFEL_TESTS_ASSERT(equal(ssf0.burgers, make_vec(-2, 1, 1, 0)));
    TFEL_TESTS_ASSERT(equal(ssf0.plane, make_vec(0, 0, 0, 1)));
    // gliding systems of the first family
    const auto& gs0 = gs[0];
    TFEL_TESTS_CHECK_EQUAL(gs0.size(), 3u);
    TFEL_TESTS_ASSERT(
        contains(gs0, make_vec(-2, 1, 1, 0), make_vec(0, 0, 0, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs0, make_vec(1, -2, 1, 0), make_vec(0, 0, 0, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs0, make_vec(1, 1, -2, 0), make_vec(0, 0, 0, 1)));
    // gliding systems of the second family
    const auto& gs1 = gs[1];
    TFEL_TESTS_CHECK_EQUAL(gs1.size(), 3u);
    TFEL_TESTS_ASSERT(
        contains(gs1, make_vec(1, -2, 1, 0), make_vec(-1, 0, 1, 0)));
    TFEL_TESTS_ASSERT(
        contains(gs1, make_vec(1, 1, -2, 0), make_vec(-1, 1, 0, 0)));
    TFEL_TESTS_ASSERT(
        contains(gs1, make_vec(-2, 1, 1, 0), make_vec(0, -1, 1, 0)));
    // gliding systems of the third family
    const auto& gs2 = gs[2];
    TFEL_TESTS_CHECK_EQUAL(gs2.size(), 6u);
    TFEL_TESTS_ASSERT(
        contains(gs2, make_vec(1, -2, 1, 0), make_vec(-1, 0, 1, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs2, make_vec(1, 1, -2, 0), make_vec(-1, 1, 0, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs2, make_vec(-2, 1, 1, 0), make_vec(0, -1, 1, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs2, make_vec(-2, 1, 1, 0), make_vec(0, 1, -1, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs2, make_vec(1, 1, -2, 0), make_vec(1, -1, 0, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs2, make_vec(1, -2, 1, 0), make_vec(1, 0, -1, 1)));
    // gliding systems of the fourth family
    const auto& gs3 = gs[3];
    TFEL_TESTS_CHECK_EQUAL(gs3.size(), 12u);
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(-2, 1, 1, -3), make_vec(-1, 0, 1, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(1, 1, -2, 3), make_vec(-1, 0, 1, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(-2, 1, 1, -3), make_vec(-1, 1, 0, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(1, -2, 1, 3), make_vec(-1, 1, 0, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(1, -2, 1, -3), make_vec(0, -1, 1, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(1, 1, -2, 3), make_vec(0, -1, 1, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(1, -2, 1, 3), make_vec(0, 1, -1, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(1, 1, -2, -3), make_vec(0, 1, -1, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(-2, 1, 1, 3), make_vec(1, -1, 0, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(1, -2, 1, -3), make_vec(1, -1, 0, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(-2, 1, 1, 3), make_vec(1, 0, -1, 1)));
    TFEL_TESTS_ASSERT(
        contains(gs3, make_vec(1, 1, -2, -3), make_vec(1, 0, -1, 1)));
  }
};

TFEL_TESTS_GENERATE_PROXY(SlipSystemsDescriptionTest,
                          "SlipSystemsDescriptionTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("SlipSystemsDescription.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
