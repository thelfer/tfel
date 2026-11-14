/*!
 * \file   tests/Material/MicrostructureLocalisationTensors.cxx
 * \brief  This file checks that the mean strain localisation tensors and
 * the effective polarisation returned by the homogenization schemes are
 * consistent with the homogenized stiffness
 * \date 25/10/2026
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
#include <vector>
#include <cstdlib>
#include <iostream>
#include "TFEL/Material/MicrostructureLinearHomogenization.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

struct MicrostructureLocalisationTensorsTest final
    : public tfel::tests::TestCase {
  MicrostructureLocalisationTensorsTest()
      : tfel::tests::TestCase("TFEL/Material",
                              "MicrostructureLocalisationTensors") {
  }  // end of MicrostructureLocalisationTensorsTest
  tfel::tests::TestResult execute() override {
    this->test_particulate();
    this->test_poly();
    return this->result;
  }  // end of execute

 private:
  using stensor = tfel::math::stensor<3u, double>;
  using st2tost2 = tfel::math::st2tost2<3u, double>;
  //! \brief criterion value
  static constexpr auto eps = 1e-12;
  /*!
   * \brief check that the sum of the localisation tensors weighted by the
   * fractions of the phases is the identity, that the homogenized
   * stiffness is consistent with the localisation tensors and that the
   * effective polarisation is equal to the polarisation imposed in all
   * the phases
   * \param[in] h: homogenization scheme
   * \param[in] f: fractions of the phases
   * \param[in] C: elasticities of the phases
   * \param[in] tau: polarisation imposed in all the phases
   */
  void check(const tfel::material::homogenization::elasticity::
                 HomogenizationScheme<3u, double>& h,
             const std::vector<double>& f,
             const std::vector<st2tost2>& C,
             const stensor& tau) {
    const auto& A = h.mean_strain_localisation_tensors;
    TFEL_TESTS_ASSERT(A.size() == f.size());
    if (A.size() != f.size()) {
      return;
    }
    auto sA = st2tost2::zero();
    auto sCA = st2tost2::zero();
    for (std::size_t r = 0; r != f.size(); ++r) {
      sA += f[r] * A[r];
      sCA += f[r] * C[r] * A[r];
    }
    const auto Id = st2tost2::Id();
    for (unsigned short i = 0; i != 6; ++i) {
      TFEL_TESTS_ASSERT(std::abs(h.effective_polarisation[i] - tau[i]) < eps);
      for (unsigned short j = 0; j != 6; ++j) {
        TFEL_TESTS_ASSERT(std::abs(sA(i, j) - Id(i, j)) < eps);
        TFEL_TESTS_ASSERT(std::abs(sCA(i, j) - h.homogenized_stiffness(i, j)) <
                          eps);
      }
    }
  }  // end of check
  void test_particulate() {
    using namespace tfel::material;
    using namespace tfel::material::homogenization::elasticity;
    ParticulateMicrostructure<3u, double> micro(KGModuli<double>(2., 1.));
    SphereDistribution<double> d1(Sphere<double>(), 0.2,
                                  KGModuli<double>(50., 30.));
    SphereDistribution<double> d2(Sphere<double>(), 0.15,
                                  KGModuli<double>(0.5, 0.2));
    std::ignore = micro.addInclusionPhase(d1);
    std::ignore = micro.addInclusionPhase(d2);
    const auto f = std::vector<double>{micro.getMatrixFraction(), 0.2, 0.15};
    const auto C = std::vector<st2tost2>{
        micro.getMatrixElasticity(),
        (*(micro.getInclusionPhase(0))).getElasticityOfPhase(),
        (*(micro.getInclusionPhase(1))).getElasticityOfPhase()};
    const auto tau = stensor{1., -2., 0.5, 0.3, 0., 0.};
    const auto polarisations = std::vector<stensor>{tau, tau, tau};
    this->check(computeDilute<3u, double>(micro, 12, polarisations), f, C, tau);
    this->check(computeMoriTanaka<3u, double>(micro, 12, polarisations), f, C,
                tau);
    this->check(computeAsymmetricSelfConsistent<3u, double>(micro, 1e-12, true,
                                                            8, polarisations),
                f, C, tau);
    this->check(computeAsymmetricSelfConsistent<3u, double>(
                    micro, 1e-12, true, 8, polarisations, true),
                f, C, tau);
  }  // end of test_particulate
  void test_poly() {
    using namespace tfel::material;
    using namespace tfel::material::homogenization::elasticity;
    const auto n_a = tfel::math::tvector<3u, double>{1., 0., 0.};
    const auto n_b = tfel::math::tvector<3u, double>{0., 1., 0.};
    const auto KG1 = KGModuli<double>(2., 1.);
    const auto KG2 = KGModuli<double>(50., 30.);
    Grain<double> grain1(Sphere<double>(), 0.6, KG1, n_a, n_b);
    Grain<double> grain2(Sphere<double>(), 0.4, KG2, n_a, n_b);
    Polycrystal<double> poly;
    std::ignore = poly.addGrain(grain1);
    std::ignore = poly.addGrain(grain2);
    const auto f = std::vector<double>{0.6, 0.4};
    const auto C = std::vector<st2tost2>{grain1.getElasticityOfPhase(),
                                         grain2.getElasticityOfPhase()};
    const auto tau = stensor{1., -2., 0.5, 0.3, 0., 0.};
    const auto polarisations = std::vector<stensor>{tau, tau};
    this->check(computeSelfConsistent<3u, double>(poly, 1e-12, C[0], true, 8,
                                                  polarisations),
                f, C, tau);
  }  // end of test_poly
};

TFEL_TESTS_GENERATE_PROXY(MicrostructureLocalisationTensorsTest,
                          "MicrostructureLocalisationTensors");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MicrostructureLocalisationTensors.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
