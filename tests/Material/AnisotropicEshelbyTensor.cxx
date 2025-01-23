/*!
 * \file   tests/Material/AnisotropicEshelbyTensor.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22/06/2021
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
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Material/IsotropicEshelbyTensor.hxx"
#include "TFEL/Material/AnisotropicEshelbyTensor.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct AnisotropicEshelbyTensorTest final : public tfel::tests::TestCase {
  AnisotropicEshelbyTensorTest()
      : tfel::tests::TestCase("TFEL/Material", "AnisotropicEshelbyTensor") {
  }  // end of AnisotropicEshelbyTensorTest

  tfel::tests::TestResult execute() override {
    this->template errors<double, true>();
    this->template test_Eshelby<double, true>();
    this->template test_Eshelby2D<double, true>();
    this->template test_localisator<double, true>();
    return this->result;
  }

  // must return a warning
 private:
  template <typename NumericType, bool use_qt>
  void errors() {
    using namespace tfel::material::homogenization::elasticity;
    using lg = typename tfel::config::Types<1u, NumericType, use_qt>::length;
    using real = NumericType;
    using stress = typename tfel::config::Types<1u, real, use_qt>::stress;
    const auto nu = real{0.3};
    const auto young = stress{150e9};
    tfel::math::st2tost2<3u, stress> C_0;
    const tfel::math::tvector<3u, real> n_a = {0., 0., 1.};
    const tfel::math::tvector<3u, real> n_b = {1., 0., 0.};
    using namespace tfel::material;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, stress, real>(C_0, young, nu);
    // const auto S1 =
    // computeAnisotropicEshelbyTensor<real,stress,lg>(C_0,n_a,lg{0},n_b,lg{30},lg{3});
    // const auto S1 =
    // computeAnisotropicEshelbyTensor<real,stress,lg>(C_0,n_a,lg{-2},n_b,lg{30},lg{3});
  }

 private:
  template <typename NumericType, bool use_qt>
  void test_Eshelby() {
    using namespace tfel::material::homogenization::elasticity;
    using real = NumericType;
    using lg = typename tfel::config::Types<1u, real, use_qt>::length;
    using stress = typename tfel::config::Types<1u, real, use_qt>::stress;

    static constexpr auto eps = std::sqrt(std::numeric_limits<real>::epsilon());
    const auto nu = real{0.3};
    const auto young = stress{150e9};
    const tfel::math::tvector<3u, real> n_a = {1., 0., 0.};
    const tfel::math::tvector<3u, real> n_b = {0., 1., 0.};
    tfel::math::st2tost2<3u, stress> C_0;
    using namespace tfel::material;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, stress, real>(C_0, young, nu);
    const auto S1 = computeAnisotropicEshelbyTensor<real, stress, lg>(
        C_0, n_a, lg{3}, n_b, lg{2}, lg{1}, 14);
    const auto S2 = computeEshelbyTensor(nu, lg{1}, lg{2}, lg{3});
    const auto S3 = computeEshelbyTensor(nu, lg{2}, lg{3}, lg{1});
    for (int i : {0, 1, 2, 3, 4, 5}) {
      for (int j : {0, 1, 2, 3, 4, 5}) {
        TFEL_TESTS_ASSERT(std::abs(S1(i, j) - S2(i, j)) < eps);
        TFEL_TESTS_ASSERT(std::abs(S1(i, j) - S3(i, j)) < eps);
        // std::cout << S1(i,j)-S2(i,j)<< " "<< eps<<'\n';
        // std::cout << S1(i,j)-S3(i,j) << " "<< eps<<'\n';
      }
    }

    const auto SAxi1 = computeAnisotropicEshelbyTensor<real, stress, lg>(
        C_0, n_a, lg{30}, n_b, lg{3}, lg{3}, 14);
    const auto SAxi2 = computeEshelbyTensor(nu, lg{30}, lg{3}, lg{3});
    for (int i : {0, 1, 2, 3, 4, 5}) {
      for (int j : {0, 1, 2, 3, 4, 5}) {
        TFEL_TESTS_ASSERT(std::abs(SAxi1(i, j) - SAxi2(i, j)) < eps);
        // std::cout << SAxi1(i,j)-SAxi2(i,j) << " "<< eps <<'\n';
      }
    }

    const auto SSph1 = computeAnisotropicEshelbyTensor<real, stress, lg>(
        C_0, n_a, lg{3}, n_b, lg{3}, lg{3}, 14);
    const auto SSph2 = computeEshelbyTensor(nu, lg{30}, lg{30}, lg{30});
    for (int i : {0, 1, 2, 3, 4, 5}) {
      for (int j : {0, 1, 2, 3, 4, 5}) {
        TFEL_TESTS_ASSERT(std::abs(SSph1(i, j) - SSph2(i, j)) < eps);
        // std::cout << SSph1(i,j)-SSph2(i,j) << " "<< eps <<'\n';
      }
    }

  } // end of test_Eshelby

 private:
  template <typename NumericType, bool use_qt>
  void test_Eshelby2D() {
    using namespace tfel::material::homogenization::elasticity;
    using real = NumericType;
    using lg = typename tfel::config::Types<1u, real, use_qt>::length;
    using stress = typename tfel::config::Types<1u, real, use_qt>::stress;

    static constexpr auto eps = std::sqrt(std::numeric_limits<real>::epsilon());
    const auto nu = real{0.3};
    const auto young = stress{150e9};
    const tfel::math::tvector<2u, real> n_a = {1., 0.};
    const tfel::math::tvector<2u, real> n_b = {0., 1.};
    tfel::math::st2tost2<2u, stress> C_0;
    using namespace tfel::material;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<2u, value, stress, real>(C_0, young, nu);
    const auto S2D1 = compute2DAnisotropicEshelbyTensor<real, stress, lg>(
        C_0, n_a, lg{3}, lg{2}, 14);
    const auto S2D2 = computeEllipticCylinderEshelbyTensor(nu, real{1.5});
    for (int i : {0, 1, 2, 3}) {
      for (int j : {0, 1, 2, 3}) {
        TFEL_TESTS_ASSERT(std::abs(S2D1(i, j) - S2D2(i, j)) < eps);
        // std::cout << S2D1(i,j) <<" " <<S2D2(i,j)<< " "<< eps<<'\n';
      }
    }

    const auto S2DCir1 = compute2DAnisotropicEshelbyTensor<real, stress, lg>(
        C_0, n_a, lg{3}, lg{3}, 14);
    const auto S2DCir2 = computeCircularCylinderEshelbyTensor(nu);
    for (int i : {0, 1, 2, 3}) {
      for (int j : {0, 1, 2, 3}) {
        TFEL_TESTS_ASSERT(std::abs(S2DCir1(i, j) - S2DCir2(i, j)) < eps);
        // std::cout << i <<" " << j <<" " << S2DCir1(i,j) <<" " << S2DCir2(i,j)
        // << " "<< eps <<'\n';
      }
    }

  } // end of test_Eshelby2D

 private:
  template <typename NumericType, bool use_qt>
  void test_localisator() {
    using namespace tfel::material::homogenization::elasticity;
    using real = NumericType;
    using lg = typename tfel::config::Types<1u, real, use_qt>::length;
    using stress = typename tfel::config::Types<1u, real, use_qt>::stress;

    static constexpr auto eps = std::sqrt(std::numeric_limits<real>::epsilon());
    const auto nu = real{0.3};
    const auto young = stress{1e9};
    const auto young_i = stress{150e9};
    const auto nu_i = real{0.1};
    const tfel::math::tvector<3u, real> n_a = {0., 0., 1.};
    const tfel::math::tvector<3u, real> n_b = {1., 0., 0.};
    tfel::math::st2tost2<3u, stress> C_0;
    tfel::math::st2tost2<3u, stress> C_i;
    using namespace tfel::material;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, stress, real>(C_0, young, nu);
    computeIsotropicStiffnessTensorII<3u, value, stress, real>(C_i, young_i,
                                                               nu_i);

    const auto AAxis_0 = computeAnisotropicLocalisationTensor<real, stress, lg>(
        C_0, C_i, n_a, lg{20}, n_b, lg{2}, lg{1});
    const auto AAxis_1 = computeEllipsoidLocalisationTensor<real, stress, lg>(
        young, nu, young_i, nu_i, n_a, lg{20}, n_b, lg{2}, lg{1});
    for (int i : {0, 1, 2, 3, 4, 5}) {
      for (int j : {0, 1, 2, 3, 4, 5}) {
        TFEL_TESTS_ASSERT(std::abs(AAxis_0(i, j) - AAxis_1(i, j)) < eps);
        // std::cout << AAxis_0(i,j)-AAxis_1(i,j) << " "<< eps<<'\n';
      }
    }

  }  // end of test_localisator

};  // end of struct AnisotropicEshelbyTensorTest

TFEL_TESTS_GENERATE_PROXY(AnisotropicEshelbyTensorTest,
                          "AnisotropicEshelbyTensor");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("AnisotropicEshelbyTensor.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
