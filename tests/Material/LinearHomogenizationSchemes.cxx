/*!
 * \file   tests/Material/LinearHomogenizationSchemes.cxx
 * \brief
 * \author Antoine Martin
 * \date   25/10/2024
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
#include "TFEL/Material/StiffnessTensor.hxx"
#ifndef _LIBCPP_VERSION
#include "TFEL/Material/LinearHomogenizationSchemes.hxx"
#endif /* _LIBCPP_VERSION */
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#ifndef _LIBCPP_VERSION
template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}
#endif /* _LIBCPP_VERSION */

struct LinearHomogenizationSchemesTest final : public tfel::tests::TestCase {
  LinearHomogenizationSchemesTest()
      : tfel::tests::TestCase("TFEL/Material", "LinearHomogenizationSchemes") {
  }  // end of LinearHomogenizationSchemesTest
  tfel::tests::TestResult execute() override {
    using real = double;
    constexpr bool qt = true;
    using stress = typename tfel::config::Types<1u, real, qt>::stress;
    using length = typename tfel::config::Types<1u, real, qt>::length;

    this->template test1<real, stress, length>();
    this->template test3<real, stress, length>();
    this->template test4<real, stress, length>();
    this->template test5<real, stress, length>();
    this->template test6<real, stress, length>();
    this->template test7<real, stress, length>();
    this->template test8<real, stress, length>();
    return this->result;
  }

 private:
  template <typename real, typename stress, typename length>
  void test1() {
#ifndef _LIBCPP_VERSION
    // tests computeDiluteScheme and computeMoriTanakaScheme for spheres
    // comparing to computeSphereDiluteScheme and computeSphereMoriTanakaScheme
    using namespace tfel::material::homogenization::elasticity;
    constexpr auto eps = 100 * tfel::math::constexpr_fct::sqrt(
                                   std::numeric_limits<real>::epsilon());
    const auto young = stress{1e9};
    const auto seps = young * eps;
    const auto nu = real{0.3};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.2};
    const auto f = real{0.5};
    const auto A =
        computeSphereLocalisationTensor<real, stress>(young, nu, young_i, nu_i);
    const auto Chom0 =
        computeDiluteScheme<real, stress>(young, nu, f, young_i, nu_i, A);
    const auto muhom0 = (Chom0(0, 0) - Chom0(0, 1)) / 2;
    const auto khom0 = (Chom0(0, 0) + 2 * Chom0(0, 1)) / 3;
    const auto nuSphere_DS_0 =
        (3 * khom0 - 2 * muhom0) / (2 * muhom0 + 6 * khom0);
    const auto ESphere_DS_0 = 2 * muhom0 * (1 + nuSphere_DS_0);
    const auto Chom1 =
        computeMoriTanakaScheme<real, stress>(young, nu, f, young_i, nu_i, A);
    const auto muhom1 = (Chom1(0, 0) - Chom1(0, 1)) / 2;
    const auto khom1 = (Chom1(0, 0) + 2 * Chom1(0, 1)) / 3;
    const auto nuSphere_MT_1 =
        (3 * khom1 - 2 * muhom1) / (2 * muhom1 + 6 * khom1);
    const auto ESphere_MT_1 = 2 * muhom1 * (1 + nuSphere_MT_1);
    const auto pair2 =
        computeSphereDiluteScheme<real, stress>(young, nu, f, young_i, nu_i);
    const auto ESphere_DS_2 = std::get<0>(pair2);
    const auto nuSphere_DS_2 = std::get<1>(pair2);
    const auto pair3 = computeSphereMoriTanakaScheme<real, stress>(
        young, nu, f, young_i, nu_i);
    const auto ESphere_MT_3 = std::get<0>(pair3);
    const auto nuSphere_MT_3 = std::get<1>(pair3);

    // std::cout << (ESphere_DS_2-ESphere_DS_0).getValue() << " " << eps <<
    // '\n';
    TFEL_TESTS_ASSERT(my_abs(ESphere_DS_0 - ESphere_DS_2) < stress{seps});
    TFEL_TESTS_ASSERT(my_abs(nuSphere_DS_0 - nuSphere_DS_2) < eps);
    TFEL_TESTS_ASSERT(my_abs(ESphere_MT_1 - ESphere_MT_3) < stress{seps});
    TFEL_TESTS_ASSERT(my_abs(nuSphere_MT_1 - nuSphere_MT_3) < eps);
#endif /* _LIBCPP_VERSION */
  }

  template <typename real, typename stress, typename length>
  void test3() {
#ifndef _LIBCPP_VERSION
    {
      constexpr auto eps = 100 * tfel::math::constexpr_fct::sqrt(
                                     std::numeric_limits<real>::epsilon());
      using namespace tfel::material::homogenization::elasticity;
      const auto young = stress{1e9};
      const auto nu = real{0.3};
      const auto young_i = stress{150e9};
      const auto nu_i = real{0.2};
      const auto a = length{0.4};
      //    const auto b = length{0.3};
      //    const auto c = length{0.2};
      const auto f = real{0.5};
      const tfel::math::tvector<3u, real> n_a = {0., 0., 1.};
      const tfel::math::tvector<3u, real> n_b = {1., 0., 0.};
      // computeSphereDiluteScheme must be equal to IsotropicDiluteScheme,
      // TransverseIsotropicDiluteScheme and OrientedDiluteScheme when a=b=c
      const auto pair0 =
          computeSphereDiluteScheme<real, stress>(young, nu, f, young_i, nu_i);
      const auto ESphere_0 = std::get<0>(pair0);
      const auto nuSphere_0 = std::get<1>(pair0);
      const auto pair1 = computeIsotropicDiluteScheme<real, stress>(
          young, nu, f, young_i, nu_i, a, a, a);
      const auto ESphere_1 = std::get<0>(pair1);
      const auto nuSphere_1 = std::get<1>(pair1);
      const auto Chom2 =
          computeTransverseIsotropicDiluteScheme<real, stress, length>(
              young, nu, f, young_i, nu_i, n_a, a, a, a);
      const auto mu2 = (Chom2(0, 0) - Chom2(0, 1)) / 2;
      const auto ka2 = (Chom2(0, 0) + 2 * Chom2(0, 1)) / 3;
      const auto nuSphere_2 = (3 * ka2 - 2 * mu2) / (2 * mu2 + 6 * ka2);
      const auto ESphere_2 = 2 * mu2 * (1 + nuSphere_2);
      const auto Chom3 = computeOrientedDiluteScheme<real, stress, length>(
          young, nu, f, young_i, nu_i, n_a, a, n_b, a, a);
      const auto mu3 = (Chom3(0, 0) - Chom3(0, 1)) / 2;
      const auto ka3 = (Chom3(0, 0) + 2 * Chom3(0, 1)) / 3;
      const auto nuSphere_3 = (3 * ka3 - 2 * mu3) / (2 * mu3 + 6 * ka3);
      const auto ESphere_3 = 2 * mu3 * (1 + nuSphere_3);

      TFEL_TESTS_ASSERT(my_abs(ESphere_0 - ESphere_1) < stress{eps});
      // std::cout << (ESphere_0-ESphere_1).getValue() << " "<< eps << '\n';
      TFEL_TESTS_ASSERT(my_abs(nuSphere_0 - nuSphere_1) < eps);
      TFEL_TESTS_ASSERT(my_abs(ESphere_0 - ESphere_2) < stress{eps});
      // std::cout << (ESphere_2-ESphere_0).getValue() << " "<< eps << '\n';
      TFEL_TESTS_ASSERT(my_abs(nuSphere_0 - nuSphere_2) < eps);
      TFEL_TESTS_ASSERT(my_abs(ESphere_0 - ESphere_3) < stress{eps});
      // std::cout << (ESphere_0-ESphere_3).getValue() << " "<< eps << '\n';
      TFEL_TESTS_ASSERT(my_abs(nuSphere_0 - nuSphere_3) < eps);
    }
#endif /* _LIBCPP_VERSION */
  }

  template <typename real, typename stress, typename length>
  void test4() {
#ifndef _LIBCPP_VERSION
    {
      constexpr auto eps = 100 * tfel::math::constexpr_fct::sqrt(
                                     std::numeric_limits<real>::epsilon());
      using namespace tfel::material::homogenization::elasticity;
      const auto young = stress{1e9};
      const auto nu = real{0.3};
      const auto young_i = stress{150e9};
      const auto nu_i = real{0.2};
      const auto a = length{0.4};
      const auto b = length{0.3};
      const auto f = real{0.5};
      const tfel::math::tvector<3u, real> n_a = {0., 0., 1.};
      const tfel::math::tvector<3u, real> n_b = {1., 0., 0.};
      // TransverseIsotropicDiluteScheme must be equal to OrientedDiluteScheme
      // when b=c
      const auto Chom2 =
          computeTransverseIsotropicDiluteScheme<real, stress, length>(
              young, nu, f, young_i, nu_i, n_a, a, b, b);
      const auto mu2 = (Chom2(0, 0) - Chom2(0, 1)) / 2;
      const auto ka2 = (Chom2(0, 0) + 2 * Chom2(0, 1)) / 3;
      const auto nuTI_DS_2 = (3 * ka2 - 2 * mu2) / (2 * mu2 + 6 * ka2);
      const auto ETI_DS_2 = 2 * mu2 * (1 + nuTI_DS_2);
      const auto Chom3 = computeOrientedDiluteScheme<real, stress, length>(
          young, nu, f, young_i, nu_i, n_a, a, n_b, b, b);
      const auto mu3 = (Chom3(0, 0) - Chom3(0, 1)) / 2;
      const auto ka3 = (Chom3(0, 0) + 2 * Chom3(0, 1)) / 3;
      const auto nuTI_DS_3 = (3 * ka3 - 2 * mu3) / (2 * mu3 + 6 * ka3);
      const auto ETI_DS_3 = 2 * mu3 * (1 + nuTI_DS_3);
      TFEL_TESTS_ASSERT(my_abs(ETI_DS_2 - ETI_DS_3) < stress{eps});
      TFEL_TESTS_ASSERT(my_abs(nuTI_DS_2 - nuTI_DS_3) < eps);
      // std::cout << (E2-E3).getValue() << " "<< eps << '\n';
    }
#endif /* _LIBCPP_VERSION */
  }

  template <typename real, typename stress, typename length>
  void test5() {
#ifndef _LIBCPP_VERSION
    {
      using namespace tfel::material::homogenization::elasticity;
      constexpr auto eps = 10 * std::numeric_limits<real>::epsilon();
      const auto young = stress{1e9};
      const auto seps = young * eps;
      const auto nu = real{0.3};
      const auto young_i = stress{150e9};
      const auto nu_i = real{0.2};
      const auto a = length{0.4};
      const auto f = real{0.5};
      const tfel::math::tvector<3u, real> n_a = {0., 0., 1.};
      const tfel::math::tvector<3u, real> n_b = {1., 0., 0.};
      // SphereMoriTanakaScheme must be equal to IsotropicMoriTanakaScheme,
      // TransverseIsotropicMoriTanakaScheme and OrientedMoriTanakaScheme when
      // a=b=c
      const auto pair0 = computeSphereMoriTanakaScheme<real, stress>(
          young, nu, f, young_i, nu_i);
      const auto ESphere_MT_0 = std::get<0>(pair0);
      const auto nuSphere_MT_0 = std::get<1>(pair0);
      const auto pair1 = computeIsotropicMoriTanakaScheme<real, stress>(
          young, nu, f, young_i, nu_i, a, a, a);
      const auto ESphere_MT_1 = std::get<0>(pair1);
      const auto nuSphere_MT_1 = std::get<1>(pair1);
      const auto Chom2 =
          computeTransverseIsotropicMoriTanakaScheme<real, stress, length>(
              young, nu, f, young_i, nu_i, n_a, a, a, a);
      const auto mu2 = (Chom2(0, 0) - Chom2(0, 1)) / 2;
      const auto ka2 = (Chom2(0, 0) + 2 * Chom2(0, 1)) / 3;
      const auto nuSphere_MT_2 = (3 * ka2 - 2 * mu2) / (2 * mu2 + 6 * ka2);
      const auto ESphere_MT_2 = 2 * mu2 * (1 + nuSphere_MT_2);
      const auto Chom3 = computeOrientedMoriTanakaScheme<real, stress, length>(
          young, nu, f, young_i, nu_i, n_a, a, n_b, a, a);
      const auto mu3 = (Chom3(0, 0) - Chom3(0, 1)) / 2;
      const auto ka3 = (Chom3(0, 0) + 2 * Chom3(0, 1)) / 3;
      const auto nuSphere_MT_3 = (3 * ka3 - 2 * mu3) / (2 * mu3 + 6 * ka3);
      const auto ESphere_MT_3 = 2 * mu3 * (1 + nuSphere_MT_3);

      TFEL_TESTS_ASSERT(my_abs(ESphere_MT_0 - ESphere_MT_1) < stress{seps});
      // std::cout << (ESphere_MT_0-ESphere_MT_1).getValue() << " "<< eps <<
      // '\n';
      TFEL_TESTS_ASSERT(my_abs(nuSphere_MT_0 - nuSphere_MT_1) < eps);
      TFEL_TESTS_ASSERT(my_abs(ESphere_MT_0 - ESphere_MT_2) < stress{seps});
      // std::cout << (ESphere_MT_2-ESphere_MT_0).getValue() << " "<< eps <<
      // '\n';
      TFEL_TESTS_ASSERT(my_abs(nuSphere_MT_0 - nuSphere_MT_2) < eps);
      TFEL_TESTS_ASSERT(my_abs(ESphere_MT_0 - ESphere_MT_3) < stress{seps});
      // std::cout << (ESphere_MT_0-ESphere_MT_3).getValue() << " "<< eps <<
      // '\n';
      TFEL_TESTS_ASSERT(my_abs(nuSphere_MT_0 - nuSphere_MT_3) < eps);
    }
#endif /* _LIBCPP_VERSION */
  }

  template <typename real, typename stress, typename length>
  void test6() {
#ifndef _LIBCPP_VERSION
    {
      constexpr auto eps = 100 * tfel::math::constexpr_fct::sqrt(
                                     std::numeric_limits<real>::epsilon());
      using namespace tfel::material::homogenization::elasticity;
      const auto young = stress{1e9};
      const auto nu = real{0.3};
      const auto young_i = stress{150e9};
      const auto nu_i = real{0.2};
      const auto a = length{0.4};
      // const auto b = length{0.3};
      const auto c = length{0.2};
      const auto f = real{0.5};
      //    const tfel::math::tvector<3u,real> n_a = {0.,0.,1.};
      //    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
      // IsotropicDiluteScheme when a is near b must be near
      // IsotropicDiluteScheme when a=b
      const auto pair0 = computeIsotropicDiluteScheme<real, stress, length>(
          young, nu, f, young_i, nu_i, a, a + length{0.0000001}, c);
      const auto EI_DS_0 = std::get<0>(pair0);
      const auto nuI_DS_0 = std::get<1>(pair0);

      const auto pair1 = computeIsotropicDiluteScheme<real, stress, length>(
          young, nu, f, young_i, nu_i, a, c, a + length{0.0000001});
      const auto EI_DS_1 = std::get<0>(pair1);
      const auto nuI_DS_1 = std::get<1>(pair1);

      const auto pair2 = computeIsotropicDiluteScheme<real, stress, length>(
          young, nu, f, young_i, nu_i, c, a, a + length{0.0000001});
      const auto EI_DS_2 = std::get<0>(pair2);
      const auto nuI_DS_2 = std::get<1>(pair2);

      const auto pair3 = computeIsotropicDiluteScheme<real, stress, length>(
          young, nu, f, young_i, nu_i, a, c, a);
      const auto EI_DS_3 = std::get<0>(pair3);
      const auto nuI_DS_3 = std::get<1>(pair3);

      const auto pair4 = computeIsotropicDiluteScheme<real, stress, length>(
          young, nu, f, young_i, nu_i, c, a, a);
      const auto EI_DS_4 = std::get<0>(pair4);
      const auto nuI_DS_4 = std::get<1>(pair4);

      TFEL_TESTS_ASSERT(my_abs(EI_DS_1 - EI_DS_0) < stress{10 * eps});
      // std::cout << (E0-E1).getValue() << " "<< E1.getValue() << '\n';
      TFEL_TESTS_ASSERT(my_abs(nuI_DS_1 - nuI_DS_0) < eps);
      TFEL_TESTS_ASSERT(my_abs(EI_DS_1 - EI_DS_2) < stress{10 * eps});
      // std::cout << E0.getValue() << " "<< E2.getValue() << '\n';
      TFEL_TESTS_ASSERT(my_abs(nuI_DS_1 - nuI_DS_2) < eps);
      TFEL_TESTS_ASSERT(my_abs(EI_DS_1 - EI_DS_3) < stress{10 * eps});
      // std::cout << E0.getValue() << " "<< E3.getValue() << '\n';
      TFEL_TESTS_ASSERT(my_abs(nuI_DS_1 - nuI_DS_3) < eps);
      TFEL_TESTS_ASSERT(my_abs(EI_DS_1 - EI_DS_4) < stress{10 * eps});
      // std::cout << E3.getValue() << " "<< E4.getValue() << '\n';
      TFEL_TESTS_ASSERT(my_abs(nuI_DS_1 - nuI_DS_4) < eps);
    }
#endif /* _LIBCPP_VERSION */
  }

  template <typename real, typename stress, typename length>
  void test7() {
#ifndef _LIBCPP_VERSION
    {
      constexpr auto eps = 100 * tfel::math::constexpr_fct::sqrt(
                                     std::numeric_limits<real>::epsilon());
      using namespace tfel::material::homogenization::elasticity;
      const auto young = stress{1e9};
      const auto nu = real{0.3};
      const auto young_i = stress{150e9};
      const auto nu_i = real{0.2};
      const auto a = length{0.4};
      const auto b = length{0.3};
      const auto c = length{0.2};
      const auto f = real{0.5};
      const tfel::math::tvector<3u, real> n_a = {0., 0., 1.};
      //    const tfel::math::tvector<3u,real> n_b = {1.,0.,0.};
      // TransverseIsotropicDiluteScheme when b is near c must be near
      // TransverseIsotropicDiluteScheme when b=c
      const auto Chom0 =
          computeTransverseIsotropicDiluteScheme<real, stress, length>(
              young, nu, f, young_i, nu_i, n_a, a, b, b);
      const auto mu0 = (Chom0(0, 0) - Chom0(0, 1)) / 2;
      const auto ka0 = (Chom0(0, 0) + 2 * Chom0(0, 1)) / 3;
      const auto nuTI_DS_0 = (3 * ka0 - 2 * mu0) / (2 * mu0 + 6 * ka0);
      const auto ETI_DS_0 = 2 * mu0 * (1 + nuTI_DS_0);

      const auto Chom1 =
          computeTransverseIsotropicDiluteScheme<real, stress, length>(
              young, nu, f, young_i, nu_i, n_a, a, b, b - length{0.0000001});
      const auto mu1 = (Chom1(0, 0) - Chom1(0, 1)) / 2;
      const auto ka1 = (Chom1(0, 0) + 2 * Chom1(0, 1)) / 3;
      const auto nuTI_DS_1 = (3 * ka1 - 2 * mu1) / (2 * mu1 + 6 * ka1);
      const auto ETI_DS_1 = 2 * mu1 * (1 + nuTI_DS_1);

      TFEL_TESTS_ASSERT(my_abs(ETI_DS_1 - ETI_DS_0) < stress{10 * eps});
      // std::cout << (E0-E1).getValue() << " "<< eps << '\n';
      TFEL_TESTS_ASSERT(my_abs(nuTI_DS_1 - nuTI_DS_0) < eps);

      // TransverseIsotropicDiluteScheme when a is near b must be near
      // TransverseIsotropicDiluteScheme when a=b
      const auto Chom2 =
          computeTransverseIsotropicDiluteScheme<real, stress, length>(
              young, nu, f, young_i, nu_i, n_a, a, a, c);
      const auto mu2 = (Chom2(0, 0) - Chom2(0, 1)) / 2;
      const auto ka2 = (Chom2(0, 0) + 2 * Chom2(0, 1)) / 3;
      const auto nuTI_DS_2 = (3 * ka2 - 2 * mu2) / (2 * mu2 + 6 * ka2);
      const auto ETI_DS_2 = 2 * mu2 * (1 + nuTI_DS_2);

      const auto Chom3 =
          computeTransverseIsotropicDiluteScheme<real, stress, length>(
              young, nu, f, young_i, nu_i, n_a, a, a + length{0.0000001}, c);
      const auto mu3 = (Chom3(0, 0) - Chom3(0, 1)) / 2;
      const auto ka3 = (Chom3(0, 0) + 2 * Chom3(0, 1)) / 3;
      const auto nuTI_DS_3 = (3 * ka3 - 2 * mu3) / (2 * mu3 + 6 * ka3);
      const auto ETI_DS_3 = 2 * mu3 * (1 + nuTI_DS_3);

      TFEL_TESTS_ASSERT(my_abs(ETI_DS_2 - ETI_DS_3) < stress{10 * eps});
      // std::cout << (E2-E3).getValue() << " "<< value << '\n';
      TFEL_TESTS_ASSERT(my_abs(nuTI_DS_2 - nuTI_DS_3) < eps);
    }
#endif /* _LIBCPP_VERSION */
  }

  template <typename real, typename stress, typename length>
  void test8() {
#ifndef _LIBCPP_VERSION
    {
      using namespace tfel::material::homogenization::elasticity;
      constexpr auto eps = 10 * std::numeric_limits<real>::epsilon();
      const auto young = stress{1e9};
      const auto seps = young * eps;
      const auto nu = real{0.3};
      const auto young_i = stress{150e9};
      const auto nu_i = real{0.2};
      const auto a = length{20.};
      const auto b = length{1.};
      const auto c = length{3.};
      const auto f = real{0.2};
      const tfel::math::tvector<3u, real> n_a = {0., 0., 1.};
      const tfel::math::tvector<3u, real> n_b = {1., 0., 0.};
      // OrientedPCWScheme must be equal to OrientedMoriTanakaScheme,
      // when the tensor P_d is oriented in the same way as the ellipsoids,
      // with the same lengths
      const auto ChomMT1 =
          computeOrientedMoriTanakaScheme<real, stress, length>(
              young, nu, f, young_i, nu_i, n_a, a, n_b, b, c);
      const auto ChomPCW1 = computeOrientedPCWScheme<real, stress, length>(
          young, nu, f, young_i, nu_i, n_a, a, n_b, b, c, n_a, a, n_b, b, c);
      // Test the compilation
      const auto ChomPCW3 =
          computeTransverseIsotropicPCWScheme<real, stress, length>(
              young, nu, f, young_i, nu_i, n_a, a, b, c, n_a, a, n_b, b, c);

      const auto ChomPCW2 = computeIsotropicPCWScheme<real, stress, length>(
          young, nu, f, young_i, nu_i, a, b, c, n_a, b, n_b, b, c);

      TFEL_TESTS_ASSERT(my_abs(ChomMT1(2, 2) - ChomPCW1(2, 2)) < seps);
      // std::cout << (ChomMT1(2, 2)).getValue()<<" "<<ChomPCW1(2, 2).getValue()
      // << " " << seps.getValue()
      //           << '\n';
      TFEL_TESTS_ASSERT(my_abs(ChomMT1(0, 2) - ChomPCW1(0, 2)) < seps);
      // std::cout << (ChomMT1(0, 2) - ChomPCW1(0, 2)).getValue() << " " <<
      // seps.getValue()
      //           << '\n';
    }
#endif /* _LIBCPP_VERSION */
  }

};  // end of struct LinearHomogenizationSchemesTest

TFEL_TESTS_GENERATE_PROXY(LinearHomogenizationSchemesTest,
                          "LinearHomogenizationSchemes");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("LinearHomogenizationSchemes.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
