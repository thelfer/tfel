/*!
 * \file   tests/Material/MicrostructureDerivatives.cxx
 * \brief
 * \author Antoine Martin
 * \date   23 June 2026
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
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
#include <typeinfo>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Material/MicrostructureLinearHomogenization.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct MicrostructureDerivativesTest final : public tfel::tests::TestCase {
  MicrostructureDerivativesTest()
      : tfel::tests::TestCase("TFEL/Material", "MicrostructureDerivatives") {
  }  // end of MicrostructureDerivativesTest

  tfel::tests::TestResult execute() override {
    using real = double;

    using stress = typename tfel::config::Types<1u, real, true>::stress;
    using length = typename tfel::config::Types<1u, real, true>::length;

    this->template test_particulate<real, stress, length>(1e-6, 1e-7, 10);
    this->template test_particulate<real, real, real>(1e-6, 1e-7, 10);

    this->template test_particulate<real, stress, length>(1e-6, 1e-7, 0.5);
    this->template test_particulate<real, real, real>(1e-6, 1e-7, 0.5);

    this->template test_poly<real, stress, length>(1e-6, 1e-7, 3);
    this->template test_poly<real, real, real>(1e-6, 1e-7, 3);

    this->template test_poly<real, stress, length>(1e-6, 1e-7, 1./3);
    this->template test_poly<real, real, real>(1e-6, 1e-7, 1./3);

    return this->result;
  }

  template <typename real, typename stress, typename length>
  void test_func(auto func,
                 const real h,
                 const real eps,
                 auto dC_dkr,
                 auto dC_dmur,
                 stress k1,
                 stress mu1,
                 stress k2,
                 stress mu2,
                 stress k3,
                 stress mu3,
                 stress k4,
                 stress mu4) {
    using namespace tfel::material;
    const auto nr = int(4);
    std::array<stress, nr> k_ = {k1, k2, k3, k4};
    std::array<stress, nr> m_ = {mu1, mu2, mu3, mu4};
    for (int r = 0; r < nr; r++) {
      std::array<stress, nr> kk_ = {k1, k2, k3, k4};
      std::array<stress, nr> mm_ = {mu1, mu2, mu3, mu4};
      auto funck = [&kk_,&r,&mm_,&func](const stress& _k_) {
        kk_[r] = _k_;
        return func(kk_[0], mm_[0], kk_[1], mm_[1], kk_[2], mm_[2], kk_[3],
                    mm_[3]);
      };
      auto funcm = [&kk_,&r,&mm_,&func](const stress& _m_) {
        mm_[r] = _m_;
        return func(kk_[0], mm_[0], kk_[1], mm_[1], kk_[2], mm_[2], kk_[3],
                    mm_[3]);
      };

      auto dC_dkr_DF =
          tfel::math::computeNumericalDerivative(funck, k_[r], stress(h));
      kk_ = {k1, k2, k3, k4};
      mm_ = {mu1, mu2, mu3, mu4};
      auto dC_dmur_DF =
          tfel::math::computeNumericalDerivative(funcm, m_[r], stress(h));
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(my_abs(dC_dkr[r](i, j) - dC_dkr_DF(i, j)) <
                            real(eps));
          TFEL_TESTS_ASSERT(my_abs(dC_dmur[r](i, j) - dC_dmur_DF(i, j)) <
                            real(eps));
        }
      }
    }
  }

 private:
  template <typename real, typename stress, typename length>
  void test_particulate(const real h, const real eps,const real e) {
    using namespace tfel::material::homogenization::elasticity;
    using namespace tfel::material;
    length a = length(10);
    length b = a/e;
    tfel::math::tvector<3u, real> n_a = {std::sqrt(2) / 2., std::sqrt(2) / 2.,
                                         0.};
    tfel::math::tvector<3u, real> n_b = {-std::sqrt(2) / 2., std::sqrt(2) / 2.,
                                         0.};
    const auto k0 = stress(1.);
    const auto mu0 = stress(0.5);
    const auto ki = stress(100.);
    const auto mui = stress(40.);
    const auto KG0 = tfel::material::KGModuli<stress>(k0, mu0);
    const auto KGi = tfel::material::KGModuli<stress>(ki, mui);
    Spheroid<length> spheroid1(a, b);
    IsotropicDistribution<stress> distrib1(spheroid1, real(0.2), KGi);
    unsigned short int index = 0;
    TransverseIsotropicDistribution<stress> distrib2(spheroid1, real(0.2), KGi,
                                                     n_a, index);
    OrientedDistribution<stress> distrib3(spheroid1, real(0.2), KGi, n_a, n_b);
    ParticulateMicrostructure<3u, stress> micro1(KG0);
    micro1.addInclusionPhase(distrib1);
    micro1.addInclusionPhase(distrib2);
    micro1.addInclusionPhase(distrib3);

    // Dilute scheme
    auto h_DS = computeDilute<3u, stress>(micro1, 0, {}, true);
    auto dCDS_dkr = h_DS.derivative_of_homogenized_stiffness_wrt_kr;
    auto dCDS_dmur = h_DS.derivative_of_homogenized_stiffness_wrt_mur;
    auto funcDS = [&micro1](const stress& k0_, const stress& mu0_, const stress& ki1_,
                      const stress& mui1_, const stress& ki2_,
                      const stress& mui2_, const stress& ki3_,
                      const stress& mui3_) {
      micro1.changeElasticityOfMatrixPhase(KGModuli<stress>(k0_, mu0_));
      micro1.changeElasticityOfInclusionPhase(0, KGModuli<stress>(ki1_, mui1_));
      micro1.changeElasticityOfInclusionPhase(1, KGModuli<stress>(ki2_, mui2_));
      micro1.changeElasticityOfInclusionPhase(2, KGModuli<stress>(ki3_, mui3_));
      const auto hh = computeDilute<3u, stress>(micro1, 0, {}, false);
      return hh.homogenized_stiffness;
    };

    test_func<real, stress, length>(funcDS, h, eps, dCDS_dkr, dCDS_dmur, k0,
                                    mu0, ki, mui, ki, mui, ki, mui);

    // Mori-Tanaka scheme
    ParticulateMicrostructure<3u, stress> micro2(KG0);
    micro2.addInclusionPhase(distrib1);
    micro2.addInclusionPhase(distrib2);
    micro2.addInclusionPhase(distrib3);

    auto h_MT = computeMoriTanaka<3u, stress>(micro2, 0, {}, true);
    auto dCMT_dkr = h_MT.derivative_of_homogenized_stiffness_wrt_kr;
    auto dCMT_dmur = h_MT.derivative_of_homogenized_stiffness_wrt_mur;
    auto funcMT = [&micro2](const stress& k0_, const stress& mu0_, const stress& ki1_,
                      const stress& mui1_, const stress& ki2_,
                      const stress& mui2_, const stress& ki3_,
                      const stress& mui3_) {
      micro2.changeElasticityOfMatrixPhase(KGModuli<stress>(k0_, mu0_));
      micro2.changeElasticityOfInclusionPhase(0, KGModuli<stress>(ki1_, mui1_));
      micro2.changeElasticityOfInclusionPhase(1, KGModuli<stress>(ki2_, mui2_));
      micro2.changeElasticityOfInclusionPhase(2, KGModuli<stress>(ki3_, mui3_));
      const auto hh = computeMoriTanaka<3u, stress>(micro2, 0, {}, false);
      return hh.homogenized_stiffness;
    };

    test_func<real, stress, length>(funcMT, h, eps, dCMT_dkr, dCMT_dmur, k0,
                                    mu0, ki, mui, ki, mui, ki, mui);

    // Asymmetric Self-consistent scheme
    ParticulateMicrostructure<3u, stress> micro3(KG0);
    micro3.addInclusionPhase(distrib1);
    micro3.addInclusionPhase(distrib2);
    micro3.addInclusionPhase(distrib3);
    auto h_ASC = computeAsymmetricSelfConsistent<3u, stress>(micro3, 1e-5, true,
                                                             0, {}, true);
    auto dCASC_dkr = h_ASC.derivative_of_homogenized_stiffness_wrt_kr;
    auto dCASC_dmur = h_ASC.derivative_of_homogenized_stiffness_wrt_mur;
    auto funcASC = [&micro3](const stress& k0_, const stress& mu0_,
                       const stress& ki1_, const stress& mui1_,
                       const stress& ki2_, const stress& mui2_,
                       const stress& ki3_, const stress& mui3_) {
      micro3.changeElasticityOfMatrixPhase(KGModuli<stress>(k0_, mu0_));
      micro3.changeElasticityOfInclusionPhase(0, KGModuli<stress>(ki1_, mui1_));
      micro3.changeElasticityOfInclusionPhase(1, KGModuli<stress>(ki2_, mui2_));
      micro3.changeElasticityOfInclusionPhase(2, KGModuli<stress>(ki3_, mui3_));
      const auto hh = computeAsymmetricSelfConsistent<3u, stress>(
          micro3, 1e-5, true, 0, {}, false);
      return hh.homogenized_stiffness;
    };

    test_func<real, stress, length>(funcASC, h, eps, dCASC_dkr, dCASC_dmur, k0,
                                    mu0, ki, mui, ki, mui, ki, mui);

  }  // end of test_particulate

  template <typename real, typename stress, typename length>
  void test_poly(const real h, const real eps,const real e) {
    using namespace tfel::material;
    using namespace tfel::material::homogenization::elasticity;
    length a = length(3.);
    length b = a/e;
    tfel::math::tvector<3u, real> n_a = {std::sqrt(2) / 2., std::sqrt(2) / 2.,
                                         0.};
    tfel::math::tvector<3u, real> n_b = {-std::sqrt(2) / 2., std::sqrt(2) / 2.,
                                         0.};
    tfel::math::tvector<3u, real> n_a_1 = {1., 0., 0.};
    tfel::math::tvector<3u, real> n_b_1 = {0., 1., 0.};
    tfel::math::tvector<3u, real> n_a_2 = {0., 1., 0.};
    tfel::math::tvector<3u, real> n_b_2 = {0., 0., 1.};
    const auto k0 = stress(1.);
    const auto mu0 = stress(0.5);
    const auto ki = stress(100.);
    const auto mui = stress(40.);
    const auto ki3 = stress(10.);
    const auto mui3 = stress(10.);
    const auto ki4 = stress(50.);
    const auto mui4 = stress(20.);
    const auto KGi1 = tfel::material::KGModuli<stress>(k0, mu0);
    const auto KGi2 = tfel::material::KGModuli<stress>(ki, mui);
    const auto KGi3 = tfel::material::KGModuli<stress>(ki3, mui3);
    const auto KGi4 = tfel::material::KGModuli<stress>(ki4, mui4);
    auto C_0 = 3 * k0 * tfel::math::st2tost2<3u, real>::J() +
               2 * mu0 * tfel::math::st2tost2<3u, real>::K();

    Spheroid<length> spheroid1(a, b);
    Grain<stress> grain1(spheroid1, real(0.2), KGi1, n_a_1, n_b_1);
    Grain<stress> grain2(spheroid1, real(0.3), KGi2, n_a, n_b);
    Grain<stress> grain3(spheroid1, real(0.25), KGi3, n_a_2, n_b_2);
    Grain<stress> grain4(spheroid1, real(0.25), KGi4, n_a, n_b);
    Polycrystal<stress> poly1;
    poly1.addGrain(grain1);
    poly1.addGrain(grain2);
    poly1.addGrain(grain3);
    poly1.addGrain(grain4);

    // Self-consistent scheme
    auto h_SC =
        computeSelfConsistent<3u, stress>(poly1, 1e-6, C_0, true, 0, {}, true);
    auto dCSC_dkr = h_SC.derivative_of_homogenized_stiffness_wrt_kr;
    auto dCSC_dmur = h_SC.derivative_of_homogenized_stiffness_wrt_mur;

    auto funcSC = [&poly1,&C_0](const stress& k0_, const stress& mu0_, const stress& ki1_,
                      const stress& mui1_, const stress& ki2_,
                      const stress& mui2_, const stress& ki3_,
                      const stress& mui3_) {
      poly1.changeElasticityOfGrain(0, KGModuli<stress>(k0_, mu0_));
      poly1.changeElasticityOfGrain(1, KGModuli<stress>(ki1_, mui1_));
      poly1.changeElasticityOfGrain(2, KGModuli<stress>(ki2_, mui2_));
      poly1.changeElasticityOfGrain(3, KGModuli<stress>(ki3_, mui3_));
      const auto hh = computeSelfConsistent<3u, stress>(poly1, 1e-6, C_0, true,
                                                        0, {}, false);
      return hh.homogenized_stiffness;
    };
    test_func<real, stress, length>(funcSC, h, eps, dCSC_dkr, dCSC_dmur, k0,
                                    mu0, ki, mui, ki3, mui3, ki4, mui4);

  }  // end of test_poly
};   // end of struct MicrostructureDerivativesTest

TFEL_TESTS_GENERATE_PROXY(MicrostructureDerivativesTest,
                          "MicrostructureDerivatives");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MicrostructureDerivatives.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
