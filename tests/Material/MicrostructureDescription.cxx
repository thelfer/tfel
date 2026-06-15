/*!
 * \file   tests/Material/MicrostructureDescription.cxx
 * \brief
 * \author Antoine Martin
 * \date   23 January 2025
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
#include "TFEL/Material/MicrostructureDescription.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
#include "TFEL/Math/ST2toST2/WalpoleBasis.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct MicrostructureDescriptionTest final : public tfel::tests::TestCase {
  MicrostructureDescriptionTest()
      : tfel::tests::TestCase("TFEL/Material", "MicrostructureDescription") {
  }  // end of MicrostructureDescriptionTest

  tfel::tests::TestResult execute() override {
    using real = double;

    using stress = typename tfel::config::Types<1u, real, true>::stress;
    using length = typename tfel::config::Types<1u, real, true>::length;

    this->template test_particulate<real, stress, length>();
    this->template test_particulate<real, real, real>();
    this->template user_defined_distribution<real, stress, length>();
    this->template user_defined_distribution<real, real, real>();

    return this->result;
  }

 private:
  template <typename real, typename stress, typename length>
  void test_particulate() {
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    using namespace tfel::material::homogenization::elasticity;
    length a = length(10);
    length b = length(1);
    length c = length(1);
    tfel::math::tvector<3u, real> n_a = {1., 0., 0.};
    tfel::math::tvector<3u, real> n_b = {0., 1., 0.};

    const auto young0 = stress{1e9};
    const auto nu0 = real(0.2);
    const auto youngi = stress{10e9};
    const auto nui = real(0.3);

    tfel::math::st2tost2<3u, stress> C_0;
    static constexpr auto value =
        tfel::material::StiffnessTensorAlterationCharacteristic::UNALTERED;
    tfel::material::computeIsotropicStiffnessTensorII<3u, value, stress, real>(
        C_0, young0, nu0);
    tfel::math::st2tost2<3u, stress> C_i;
    tfel::material::computeIsotropicStiffnessTensorII<3u, value, stress, real>(
        C_i, youngi, nui);
    const auto Enui = tfel::material::YoungNuModuli<stress>(youngi, nui);
    const auto KG0 = tfel::material::computeKGModuli<stress>(C_0);

    Ellipsoid<length> ellipsoid1(a, b, c);
    Spheroid<length> spheroid1(a, b);
    IsotropicDistribution<stress> distrib1(ellipsoid1, real(0.5), Enui);
    IsotropicDistribution<stress> distrib2(spheroid1, real(0.5), Enui);
    const auto sph = Sphere<length>();
    SphereDistribution<stress> distrib21(sph, real(0.5), Enui);
    const auto A_iso21 = distrib21.computeMeanLocalisator(C_0);
    const auto A_iso22 = distrib21.computeMeanLocalisator(KG0);
    
    TFEL_TESTS_ASSERT(tfel::material::relative_error(A_iso21,A_iso22)/young0<10*eps/stress(1));
 
    const auto A_iso1 = distrib1.computeMeanLocalisator(KG0);
    const auto A_iso2 = distrib2.computeMeanLocalisator(KG0);

    TFEL_TESTS_ASSERT(tfel::material::relative_error(A_iso1,A_iso2)/young0<eps/stress(1));

    unsigned short int index = 0;
    TransverseIsotropicDistribution<stress> distrib3(spheroid1, real(0.5), Enui,
                                                     n_b, index);
    OrientedDistribution<stress> distrib4(ellipsoid1, real(0.5), C_i, n_b, n_a);

    const auto A_Or_1 = distrib3.computeMeanLocalisator(KG0);
    const auto A_Or_2 = distrib4.computeMeanLocalisator(KG0);

    TFEL_TESTS_ASSERT(tfel::material::relative_error(A_Or_1,A_Or_2)/young0<eps/stress(1));

    unsigned short int index2 = 1;
    TransverseIsotropicDistribution<stress> distrib5(ellipsoid1, real(0.5),
                                                     Enui, n_a, index2);
    TransverseIsotropicDistribution<stress> distrib6(spheroid1, real(0.5), Enui,
                                                     n_a, index2);

    const auto A_TI_1 = distrib5.computeMeanLocalisator(KG0);
    const auto A_TI_2 = distrib6.computeMeanLocalisator(KG0);

    TFEL_TESTS_ASSERT(tfel::material::relative_error(A_TI_1,A_TI_2)/young0<eps/stress(1));

    ParticulateMicrostructure<3u, stress> micro1(C_0);
    micro1.addInclusionPhase(distrib1);
    TFEL_TESTS_ASSERT(not(micro1.isIsotropicMatrix()));
    micro1.changeElasticityOfMatrixPhase(KG0);
    TFEL_TESTS_ASSERT(micro1.isIsotropicMatrix());
    micro1.changeElasticityOfInclusionPhase(0,10*C_0);
    auto phasei=micro1.getInclusionPhase(0);
    auto Ci=(*phasei).getElasticityOfPhase();
    tfel::math::st2tost2<3u,stress> C=10*C_0;
    TFEL_TESTS_ASSERT(tfel::material::relative_error(Ci,C)<eps);
    auto iso=(*phasei).isIsotropic();
    TFEL_TESTS_ASSERT(not(iso));
    micro1.changeElasticityOfInclusionPhase(0,KG0);
    phasei=micro1.getInclusionPhase(0);
    Ci=(*phasei).getElasticityOfPhase();
    TFEL_TESTS_ASSERT(tfel::material::relative_error(Ci,C_0)<eps);
    iso=(*phasei).isIsotropic();
    TFEL_TESTS_ASSERT(iso);
    micro1.changeFractionOfInclusionPhase(0,real(0.2));
    TFEL_TESTS_ASSERT(my_abs(micro1.getMatrixFraction()-real(0.8))<eps);
    phasei=micro1.getInclusionPhase(0);
    auto fr=(*phasei).fraction;
    TFEL_TESTS_ASSERT(my_abs(fr-real(0.2))<eps);
  }
  
  
  template <typename real, typename stress, typename length>
  void user_defined_distribution() {
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    using namespace tfel::material::homogenization::elasticity;
    length a = length(100);
    length b = length(1);
    tfel::math::tvector<3u, real> n_a = {1., 0., 0.};
    tfel::math::tvector<3u, real> n_b = {0., 1., 0.};

    const auto k0 = stress{1.};
    const auto mu0 = stress(0.5);
    const auto ki = stress{100.};
    const auto mui = stress(60.);
    const auto J = tfel::math::st2tost2<3u,real>::J();
    const auto K = tfel::math::st2tost2<3u,real>::K();
    
    const auto KG0 = tfel::material::KGModuli<stress>(k0,mu0);
    const auto KGi = tfel::material::KGModuli<stress>(ki,mui);

    Spheroid<length> spheroid1(a, b);
    const tfel::math::stensor<3u,real> A2 = tfel::math::TransverseIsotropicWalpoleBasis<real>::p(n_a);
    const tfel::math::st2tost2<3u,real> A4 = A2^A2;
    UserDefinedDistributionOfSpheroids<stress> distrib1(spheroid1, real(0.5), KGi, A2, A4);
    const auto A_orient = distrib1.computeMeanLocalisator(KG0);
    const auto Aref_orient= computeAxisymmetricalEllipsoidLocalisationTensor<stress>(KG0,KGi,n_a,a/b);

    TFEL_TESTS_ASSERT(tfel::material::relative_error(A_orient,Aref_orient)/mu0<eps/stress(1));
    
    IsotropicDistribution<stress> distrib2(spheroid1, real(0.5), KGi);
    const tfel::math::stensor<3u,real> A2iso = 1./3*tfel::math::stensor<3u,real>::Id();
    const tfel::math::st2tost2<3u,real> A4iso = 1./3*J+2./15*K;
    UserDefinedDistributionOfSpheroids<stress> distrib3(spheroid1, real(0.5), KGi, A2iso, A4iso);
    
    const auto Aref_iso = distrib2.computeMeanLocalisator(KG0);
    const auto A_iso = distrib3.computeMeanLocalisator(KG0);
    
    TFEL_TESTS_ASSERT(tfel::material::relative_error(A_iso,Aref_iso)/mu0<10*eps/stress(1));

    unsigned short int index = 1;
    TransverseIsotropicDistribution<stress> distrib4(spheroid1, real(0.5), KGi,
                                                     n_b, index);
    const tfel::math::stensor<3u,real> A2TI = 1./2*tfel::math::TransverseIsotropicWalpoleBasis<real>::q(n_b);
    const auto E2d=tfel::math::TransverseIsotropicWalpoleBasis<real>::E2(n_b);
    const auto Fd=tfel::math::TransverseIsotropicWalpoleBasis<real>::F(n_b);
    const tfel::math::st2tost2<3u,real> A4TI = 1./2*E2d+1./4*Fd;
    UserDefinedDistributionOfSpheroids<stress> distrib5(spheroid1, real(0.5), KGi, A2TI, A4TI);

    const auto A_TI_ref = distrib4.computeMeanLocalisator(KG0);
    const auto A_TI = distrib5.computeMeanLocalisator(KG0);

    TFEL_TESTS_ASSERT(tfel::material::relative_error(A_TI,A_TI_ref)/mu0<10*eps/stress(1));

  }

};  // end of struct MicrostructureDescriptionTest

TFEL_TESTS_GENERATE_PROXY(MicrostructureDescriptionTest,
                          "MicrostructureDescription");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MicrostructureDescription.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
