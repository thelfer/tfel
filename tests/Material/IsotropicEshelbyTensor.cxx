/*!
 * \file   tests/Material/IsotropicEshelbyTensor.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22/06/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
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
#include "TFEL/Config/TFELTypes.hxx"
#ifndef _LIBCPP_VERSION
#include "TFEL/Material/StiffnessTensor.hxx"
#include "TFEL/Material/IsotropicEshelbyTensor.hxx"
#include "TFEL/Material/LocalisationTensor.hxx"
#endif /* _LIBCPP_VERSION */

#include "TFEL/Math/qt.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#ifndef _LIBCPP_VERSION
template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}
#endif /* _LIBCPP_VERSION */

struct IsotropicEshelbyTensorTest final : public tfel::tests::TestCase {
  IsotropicEshelbyTensorTest()
      : tfel::tests::TestCase("TFEL/Material", "IsotropicEshelbyTensor") {
  }  // end of IsotropicEshelbyTensorTest

  tfel::tests::TestResult execute() override {
    this->template test_Eshelby_2d<double, true>();
    this->template test_Hill_2d<double, true>();
    this->template test_localisation_2d<double, true>();
    this->template test_Eshelby_tensors<double, true>();
    this->template test_Eshelby_limits<float, true>();
    this->template test_Eshelby_limits<double, true>();
    this->template test_Eshelby_limits<long double, true>();
    this->template test_localisation_tensors<double, true>();
    this->template test_hill_tensors<double, true>();
    this->template test_Eshelby_2d<double, false>();
    this->template test_Hill_2d<double, false>();
    this->template test_localisation_2d<double, false>();
    this->template test_Eshelby_tensors<double, false>();
    this->template test_Eshelby_limits<float, false>();
    this->template test_Eshelby_limits<double, false>();
    this->template test_Eshelby_limits<long double, false>();
    this->template test_localisation_tensors<double, false>();
    this->template test_hill_tensors<double, false>();
    return this->result;
  }

 private:
  template <typename NumericType, bool use_qt>
  void test_Eshelby_2d() {
#ifndef _LIBCPP_VERSION
    using real = NumericType;
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    const auto nu = static_cast<real>(0.3);

    // These functions must return the same thing
    using namespace tfel::material::homogenization::elasticity;
    {
      const auto S2d_1 = computeDiskPlaneStrainEshelbyTensor(nu);
      const auto S2d_2 = computePlaneStrainEshelbyTensor(nu, static_cast<real>(1));
      for (int i : {0, 1, 2, 3}) {
        for (int j : {0, 1, 2, 3}) {
          TFEL_TESTS_ASSERT(std::abs(S2d_1(i, j) - S2d_2(i, j)) < eps);
          // std::cout << S1(i,j) << " " << S2(i,j) << " " << value << '\n';
        }
      }
      
    }
#endif /* _LIBCPP_VERSION */
  }
  
  private:
  template <typename NumericType, bool use_qt>
  void test_Hill_2d() {
#ifndef _LIBCPP_VERSION
    using real = NumericType;
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using compliance =
        typename tfel::config::Types<1u, NumericType, use_qt>::compliance;
    using length = typename tfel::config::Types<1u, NumericType, use_qt>::length;
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    const auto nu = static_cast<real>(0.3);
    const auto young =stress(1e9);
    const auto ceps=compliance(eps);
    const tfel::math::tvector<2u, real> n_a = {0., 1.};
    const auto a= length(1);
    using namespace tfel::material;
    const auto Enu=YoungNuModuli<stress>(young,nu);
    using namespace tfel::material::homogenization::elasticity;
    {
      const auto P2d_1 = computeDiskPlaneStrainHillTensor<stress>(Enu);
      const auto P2d_2 = computePlaneStrainHillTensor<stress>(Enu,n_a,a,a);
      for (int i : {0, 1, 2, 3})
      for (int j : {0, 1, 2, 3}) {
          TFEL_TESTS_ASSERT(my_abs(P2d_1(i, j) - P2d_2(i, j)) < ceps);
      }
    }
#endif /* _LIBCPP_VERSION */
  }
  
   private:
  template <typename NumericType, bool use_qt>
  void test_localisation_2d() {
#ifndef _LIBCPP_VERSION
    using real = NumericType;
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using length = typename tfel::config::Types<1u, NumericType, use_qt>::length;
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    const auto nu = static_cast<real>(0.3);
    const auto young =stress(1e9);
    const tfel::math::tvector<2u, real> n_a = {0., 1.};
    const auto a= length(1);
    using namespace tfel::material;
    const auto Enu=YoungNuModuli<stress>(young,nu);
    const auto nu_i = static_cast<real>(0.1);
    const auto young_i =stress(5e9);
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    tfel::math::st2tost2<2u, stress> C_i;
    computeIsotropicStiffnessTensorII<2u, value, stress, real>(C_i, young_i,
                                                                   nu_i);
    using namespace tfel::material::homogenization::elasticity;
    {
      const auto A2d_1 = computeDiskPlaneStrainLocalisationTensor(Enu,C_i);
      const auto A2d_2 = computePlaneStrainLocalisationTensor(Enu,C_i,n_a,a,a);
      for (int i : {0, 1, 2, 3})
      for (int j : {0, 1, 2, 3}) {
          TFEL_TESTS_ASSERT(std::abs(A2d_1(i, j) - A2d_2(i, j)) < eps);
      }
    }
#endif /* _LIBCPP_VERSION */
  }

  template <typename NumericType, bool use_qt>
  void test_Eshelby_tensors() {
#ifndef _LIBCPP_VERSION
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using lg = typename tfel::config::Types<1u, NumericType, use_qt>::length;
    using real = NumericType;
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    const auto nu = static_cast<real>(0.3);

    using namespace tfel::material::homogenization::elasticity;
    // AxisymmetricalEshelbyTensor with e=1 is equal to SphereEshelbyTensor and
    // also equal to EshelbyTensor(2,2,2)
    {
      const auto SSphere_1 = computeSphereEshelbyTensor(nu);
      const auto SSphere_2 = computeAxisymmetricalEshelbyTensor(nu, static_cast<real>(1));
      const auto SSphere_3 =
          computeEshelbyTensor<stress>(nu, lg{2}, lg{2}, lg{2});
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(std::abs(SSphere_1(i, j) - SSphere_2(i, j)) < eps);
          // std::cout << S1(i,j) << " " << S2(i,j) << " " << value << '\n';
          TFEL_TESTS_ASSERT(std::abs(SSphere_1(i, j) - SSphere_3(i, j)) < eps);
          // std::cout << "13" << value << '\n';
        }
      }
    }
    // AxisymmetricalEshelbyTensor with e=10 is equal to EshelbyTensor(30,3,3)
    // or EshelbyTensor(3,3,30)
    {
      const auto SAxis_1 = computeAxisymmetricalEshelbyTensor(nu, static_cast<real>(10));
      const auto SAxis_2 =
          computeEshelbyTensor<stress>(nu, lg{30}, lg{3}, lg{3});
      const auto SAxis_3 =
          computeEshelbyTensor<stress>(nu, lg{3}, lg{3}, lg{30});
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(std::abs(SAxis_1(i, j) - SAxis_2(i, j)) < eps);
          TFEL_TESTS_ASSERT(std::abs(SAxis_1(i, j) - SAxis_3(i, j)) < eps);
        }
      }
    }
#endif /* _LIBCPP_VERSION */
  }

  template <typename NumericType, bool use_qt>
  void test_Eshelby_limits() {
#ifndef _LIBCPP_VERSION
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using lg = typename tfel::config::Types<1u, NumericType, use_qt>::length;
    using real = NumericType;
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    const auto nu = static_cast<real>(0.3);

    using namespace tfel::material::homogenization::elasticity;
    // AxisymmetricalEshelbyTensor when e is near 1 must be near
    // SphereEshelbyTensor (there is a numerical instability that we propose to
    // manage)
    {
      const auto SSphere_lim_1 = computeSphereEshelbyTensor(nu);
      tfel::math::st2tost2<3u, real> SSphere_lim_2;           
      if (tfel::math::ieee754::fpclassify(
              std::numeric_limits<float>::epsilon() - static_cast<float>(eps)) == FP_ZERO) {
        SSphere_lim_2 = computeAxisymmetricalEshelbyTensor(nu, static_cast<real>(1.001));
      } else if (tfel::math::ieee754::fpclassify(
                     std::numeric_limits<double>::epsilon() - static_cast<double>(eps)) == FP_ZERO) {
        SSphere_lim_2 = computeAxisymmetricalEshelbyTensor(nu, static_cast<real>(1.000001));
      } else if (tfel::math::ieee754::fpclassify(
                     std::numeric_limits<long double>::epsilon() - static_cast<long double>(eps)) ==
                 FP_ZERO) {
        SSphere_lim_2 = computeAxisymmetricalEshelbyTensor(nu, static_cast<real>(1.0000001));
      }
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(
              std::abs(SSphere_lim_1(i, j) - SSphere_lim_2(i, j)) < eps);
          // std::cout << S1(i,j) << " " << S2(i,j) << " " << value << '\n';
        }
      }
    }

    // EshelbyTensor when a is near b must be near AxisymmetricalEshelbyTensor
    // (there is a numerical instability that we propose to manage)
    {
      const auto SAxis_lim_1 = computeAxisymmetricalEshelbyTensor(nu, static_cast<real>(10));
      tfel::math::st2tost2<3u, real> SAxis_lim_2;
      if (tfel::math::ieee754::fpclassify(
              std::numeric_limits<float>::epsilon() - static_cast<float>(eps)) == FP_ZERO) {
        SAxis_lim_2 = computeEshelbyTensor<stress>(nu, lg{1.05}, lg{1}, lg{10});
      } else if (tfel::math::ieee754::fpclassify(
                     std::numeric_limits<double>::epsilon() - static_cast<double>(eps)) == FP_ZERO) {
        SAxis_lim_2 =
            computeEshelbyTensor<stress>(nu, lg{1.0005}, lg{1}, lg{10});
      } else if (tfel::math::ieee754::fpclassify(
                     std::numeric_limits<long double>::epsilon() - static_cast<long double>(eps)) ==
                 FP_ZERO) {
        SAxis_lim_2 =
            computeEshelbyTensor<stress>(nu, lg{1.00005}, lg{1}, lg{10});
      }

      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(std::abs(SAxis_lim_1(i, j) - SAxis_lim_2(i, j)) <
                            eps);
          // std::cout << S1(i,j) << " " << S2(i,j) << " " << value << '\n';
        }
      }
    }
#endif /* _LIBCPP_VERSION */
  }

  // These functions must return the same thing
  template <typename NumericType, bool use_qt>
  void test_localisation_tensors() {
#ifndef _LIBCPP_VERSION
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using lg = tfel::types::length<stress>;
    using real = tfel::types::real<stress>;
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    const stress young = stress{1e9};
    const real nu = static_cast<real>(0.3);
    const stress young_i = stress{150e9};
    const real nu_i = static_cast<real>(0.3);

    const tfel::math::tvector<3u, real> n_a = {0., 0., 1.};
    const tfel::math::tvector<3u, real> n_b = {1., 0., 0.};

    using namespace tfel::material::homogenization::elasticity;
    {
      const auto ASphere_1 =
          computeSphereLocalisationTensor<stress>(young, nu, young_i, nu_i);
      const auto ASphere_2 =
          computeAxisymmetricalEllipsoidLocalisationTensor<stress>(
              young, nu, young_i, nu_i, n_a, static_cast<real>(1));
      const auto ASphere_3 = computeEllipsoidLocalisationTensor<stress>(
          young, nu, young_i, nu_i, n_a, lg{2}, n_b, lg{2.00000001},
          lg{2.00001});
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(std::abs(ASphere_1(i, j) - ASphere_2(i, j)) < eps);
          TFEL_TESTS_ASSERT(std::abs(ASphere_1(i, j) - ASphere_3(i, j)) < eps);
        }
      }
    }
    {
      const auto AAxis_1 =
          computeAxisymmetricalEllipsoidLocalisationTensor<stress>(
              young, nu, young_i, nu_i, n_a, lg{20} / lg{2.0001});
      const auto AAxis_2 = computeEllipsoidLocalisationTensor<stress>(
          young, nu, young_i, nu_i, n_a, lg{20}, n_b, lg{2}, lg{2.0001});
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(std::abs(AAxis_1(i, j) - AAxis_2(i, j)) < eps);
          // std::cout << A1(i,j) <<" " << i<< " " << j<< " " << A2(i,j) <<
          // value << '\n';
        }
      }
    }

    // this test checks that the rotation of the principal axis of the
    // axisymmetrical ellipsoid in the direction n_aa is ok :
    //  A_nnnn when n=n_aa is much bigger than A_tttt where t=n_bb
    //
    {
      const tfel::math::tvector<3u, real> n_aa = {std::sqrt(2) / 2, 0.,
                                                  std::sqrt(2) / 2};
      // const tfel::math::tvector<3u, real> n_bb = {-std::sqrt(2) / 2, 0.,
      //                                             std::sqrt(2) / 2};
      const auto dem = static_cast<real>(0.5);
      const tfel::math::stensor<3u, real> n_aa_n_aa = {dem, 0.,  dem,
                                                       0.,  dem, 0.};
      const tfel::math::stensor<3u, real> n_bb_n_bb = {dem, 0.,   dem,
                                                       0.,  -dem, 0.};

      const auto Arot1 =
          computeAxisymmetricalEllipsoidLocalisationTensor<stress>(
              young, nu, young_i, nu_i, n_aa, static_cast<real>(20));
      const auto Annnn = n_aa_n_aa * (Arot1 * n_aa_n_aa);
      const auto Atttt = n_bb_n_bb * (Arot1 * n_bb_n_bb);
      // const auto Attnn = n_bb_n_bb * (Arot1 * n_aa_n_aa);
      //   std::cout << Atttt(0) << Annnn(0) << Attnn(0) << std::endl;
      TFEL_TESTS_ASSERT(Annnn(0) - 5 * Atttt(0) > 0);
    }
#endif /* _LIBCPP_VERSION */
  }

  // These functions must return the same thing
  template <typename NumericType, bool use_qt>
  void test_hill_tensors() {
#ifndef _LIBCPP_VERSION
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using lg = typename tfel::config::Types<1u, NumericType, use_qt>::length;
    using compliance = typename tfel::math::invert_type<stress>;
    using real = NumericType;
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    const auto young = stress{1e9};
    const auto nu = static_cast<real>(0.3);

    const tfel::math::tvector<3u, real> n_a = {0., 0., 1.};
    const tfel::math::tvector<3u, real> n_b = {1., 0., 0.};

    using namespace tfel::material::homogenization::elasticity;
    {
      const auto PSphere_1 =
          computeSphereHillPolarisationTensor<stress>(young, nu);
      const auto PSphere_2 =
          computeAxisymmetricalHillPolarisationTensor<stress>(young, nu, n_a,
                                                              static_cast<real>(1));
      const auto PSphere_3 = computeHillPolarisationTensor<stress>(
          young, nu, n_a, lg{2}, n_b, lg{2.00000001}, lg{2.00001});
      const auto kappa = young / 3. / (1 - 2 * nu);
      const auto mu = young / 2. / (1 + nu);
      const tfel::material::KGModuli<stress> KG(kappa, mu);
      const auto PSphere_4 = computeSphereHillPolarisationTensor<stress>(KG);
      const auto PSphere_5 = computeHillPolarisationTensor<stress>(
          KG, n_a, lg{2}, n_b, lg{2.00000001}, lg{2.00001});
      const auto PSphere_6 =
          computeAxisymmetricalHillPolarisationTensor<stress>(KG, n_a, real(1));
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(my_abs(PSphere_1(i, j) - PSphere_2(i, j)) <
                            compliance(eps));
          TFEL_TESTS_ASSERT(my_abs(PSphere_1(i, j) - PSphere_3(i, j)) <
                            compliance(eps));
          TFEL_TESTS_ASSERT(my_abs(PSphere_1(i, j) - PSphere_4(i, j)) <
                            compliance(eps));
          TFEL_TESTS_ASSERT(my_abs(PSphere_1(i, j) - PSphere_5(i, j)) <
                            compliance(eps));
          TFEL_TESTS_ASSERT(my_abs(PSphere_1(i, j) - PSphere_6(i, j)) <
                            compliance(eps));
        }
      }
    }
    {
      const auto PAxis_1 = computeAxisymmetricalHillPolarisationTensor<stress>(
          young, nu, n_a, lg{20} / lg{2.0001});
      const auto PAxis_2 = computeHillPolarisationTensor<stress>(
          young, nu, n_a, lg{20}, n_b, lg{2}, lg{2.0001});
      const auto kappa = young / 3. / (1 - 2 * nu);
      const auto mu = young / 2. / (1 + nu);
      const tfel::material::KGModuli<stress> KG(kappa, mu);
      const auto PAxis_3 = computeAxisymmetricalHillPolarisationTensor<stress>(
          KG, n_a, lg{20} / lg{2.0001});
      for (int i : {0, 1, 2, 3, 4, 5}) {
        for (int j : {0, 1, 2, 3, 4, 5}) {
          TFEL_TESTS_ASSERT(my_abs(PAxis_1(i, j) - PAxis_2(i, j)) <
                            compliance(eps));
          TFEL_TESTS_ASSERT(my_abs(PAxis_1(i, j) - PAxis_3(i, j)) <
                            compliance(eps));
        }
      }
    }

    // this test checks that the rotation of the principal axis of the
    // axisymmetrical ellipsoid in the direction n_aa is ok :
    //  P_nnnn when n=n_aa is much smaller than P_tttt where t=n_bb
    //
    {
      const tfel::math::tvector<3u, real> n_aa = {std::sqrt(2) / 2, 0.,
                                                  std::sqrt(2) / 2};
      // const tfel::math::tvector<3u, real> n_bb = {-std::sqrt(2) / 2, 0.,
      //                                             std::sqrt(2) / 2};
      const auto dem = static_cast<real>(0.5);
      const tfel::math::stensor<3u, real> n_aa_n_aa = {dem, 0.,  dem,
                                                       0.,  dem, 0.};
      const tfel::math::stensor<3u, real> n_bb_n_bb = {dem, 0.,   dem,
                                                       0.,  -dem, 0.};

      const auto Prot1 = computeAxisymmetricalHillPolarisationTensor<stress>(
          young, nu, n_aa, static_cast<real>(20));
      const auto Pnnnn = n_aa_n_aa * (Prot1 * n_aa_n_aa);
      const auto Ptttt = n_bb_n_bb * (Prot1 * n_bb_n_bb);
      //const auto Pttnn = n_bb_n_bb * (Prot1 * n_aa_n_aa);
      //std::cout << Ptttt(0).getValue() << Pnnnn(0).getValue()
      //          << Pttnn(0).getValue() << std::endl;
      TFEL_TESTS_ASSERT(5 * Pnnnn(0) - Ptttt(0) < compliance(0));
    }
#endif /* _LIBCPP_VERSION */
  }

};  // end of struct IsotropicEshelbyTensorTest

TFEL_TESTS_GENERATE_PROXY(IsotropicEshelbyTensorTest, "IsotropicEshelbyTensor");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("IsotropicEshelbyTensor.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
