/*!
 * \file   tests/Material/FiniteStrainHeatTransfer.cxx
 * \brief  This file tests the functions computing the thermal conductivity
 * matrix in the reference configuration.
 * \author Thomas Helfer
 * \date   30/01/2026
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

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <type_traits>
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/Tensor/PiolaTransformation.hxx"
#include "TFEL/Material/FiniteStrainHeatTransfer.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct FiniteStrainHeatTransferTest final : public tfel::tests::TestCase {
  FiniteStrainHeatTransferTest()
      : tfel::tests::TestCase("TFEL/Material", "FiniteStrainHeatTransfer") {
  }  // end of FiniteStrainHeatTransferTest
  tfel::tests::TestResult execute() override {
    this->test<true>();
    this->test<false>();
    this->test2<1u, true>();
    this->test2<1u, false>();
    this->test2<2u, true>();
    this->test2<2u, false>();
    this->test2<3u, true>();
    this->test2<3u, false>();
    this->test3<1u, true>();
    this->test3<1u, false>();
    this->test3<2u, true>();
    this->test3<2u, false>();
    this->test3<3u, true>();
    this->test3<3u, false>();
    return this->result;
  }  // end of execute

 private:
  //! \brief a simple alias
  using Matrix = std::array<std::array<double, 3>, 3>;
  /*!
   * \return the matrix \(J\,F^{-1}\,k\,F^{-T}\), where \(k\) is a diagonal
   * matrix.
   * \param[in] F: deformation gradient
   * \param[in] k: diagonal of the thermal conductivity matrix
   */
  static Matrix computeReferenceSolution(const Matrix& F,
                                         const std::array<double, 3>& k) {
    // adjugate of the deformation gradient
    const auto a = Matrix{{{F[1][1] * F[2][2] - F[1][2] * F[2][1],
                            F[0][2] * F[2][1] - F[0][1] * F[2][2],
                            F[0][1] * F[1][2] - F[0][2] * F[1][1]},
                           {F[1][2] * F[2][0] - F[1][0] * F[2][2],
                            F[0][0] * F[2][2] - F[0][2] * F[2][0],
                            F[0][2] * F[1][0] - F[0][0] * F[1][2]},
                           {F[1][0] * F[2][1] - F[1][1] * F[2][0],
                            F[0][1] * F[2][0] - F[0][0] * F[2][1],
                            F[0][0] * F[1][1] - F[0][1] * F[1][0]}}};
    const auto J = F[0][0] * a[0][0] + F[0][1] * a[1][0] + F[0][2] * a[2][0];
    auto K = Matrix{};
    for (unsigned short i = 0; i != 3; ++i) {
      for (unsigned short j = 0; j != 3; ++j) {
        K[i][j] = 0;
        for (unsigned short l = 0; l != 3; ++l) {
          K[i][j] += a[i][l] * k[l] * a[j][l] / J;
        }
      }
    }
    return K;
  }  // end of computeReferenceSolution
  /*!
   * \brief compare a matrix to the reference solution
   * \param[in] K: matrix
   * \param[in] Kr: reference solution
   */
  template <unsigned short N, typename MatrixType>
  void compare(const MatrixType& K, const Matrix& Kr) {
    constexpr auto eps = 1e-13;
    for (unsigned short i = 0; i != N; ++i) {
      for (unsigned short j = 0; j != N; ++j) {
        TFEL_TESTS_ASSERT(
            std::abs(tfel::math::base_type_cast(K(i, j)) - Kr[i][j]) < eps);
      }
    }
  }  // end of compare
  /*!
   * \brief compare the results of the functions to an independent
   * computation for a deformation gradient different from the identity.
   */
  template <unsigned short N, bool use_qt>
  void test2() {
    using namespace tfel::math;
    using namespace tfel::material;
    using real = typename tfel::config::ScalarTypes<double, use_qt>::real;
    using thermalconductivity = tfel::types::thermalconductivity<real>;
    using thermalconductivity_derivative =
        derivative_type<thermalconductivity, tfel::types::temperature<real>>;
    const auto k = std::array<double, 3>{3, 5, 2};
    // In space dimensions one and two, the out of plane stretch, given by the
    // third component of the deformation gradient, is taken into account: the
    // reference solution is the in plane block of the three dimensional
    // solution. In space dimension one, only the diagonal components of the
    // deformation gradient are described.
    const auto [F, Fm] = [] {
      if constexpr (N == 1u) {
        return std::pair{tensor<1u, real>{real(1.1), real(0.9), real(1.2)},
                         Matrix{{{1.1, 0, 0}, {0, 0.9, 0}, {0, 0, 1.2}}}};
      } else if constexpr (N == 2u) {
        return std::pair{
            tensor<2u, real>{real(1.1), real(0.9), real(1.2), real(0.15),
                             real(-0.08)},
            Matrix{{{1.1, 0.15, 0}, {-0.08, 0.9, 0}, {0, 0, 1.2}}}};
      } else {
        return std::pair{
            tensor<3u, real>{real(1.1), real(0.9), real(1.2), real(0.15),
                             real(-0.08), real(0.07), real(-0.12), real(0.05),
                             real(0.11)},
            Matrix{
                {{1.1, 0.15, 0.07}, {-0.08, 0.9, 0.05}, {-0.12, 0.11, 1.2}}}};
      }
    }();
    // isotropic case
    const auto Kr = computeReferenceSolution(Fm, {k[0], k[0], k[0]});
    this->template compare<N>(
        computeThermalConductivityMatrixInReferenceConfiguration(
            thermalconductivity{k[0]}, F),
        Kr);
    this->template compare<N>(
        computeThermalConductivityMatrixInReferenceConfiguration(
            thermalconductivity_derivative{k[0]}, F),
        Kr);
    // Piola transformation of the identity
    this->template compare<N>(
        computePiolaTransformOfIdentityMatrixInReferenceConfiguration(F),
        computeReferenceSolution(Fm, {1, 1, 1}));
    // orthotropic case
    const auto Kr2 = computeReferenceSolution(Fm, k);
    if constexpr (N == 2u) {
      this->template compare<N>(
          computeThermalConductivityMatrixInReferenceConfiguration(
              thermalconductivity{k[0]}, thermalconductivity{k[1]}, F),
          Kr2);
      this->template compare<N>(
          computeThermalConductivityMatrixInReferenceConfiguration(
              thermalconductivity_derivative{k[0]},
              thermalconductivity_derivative{k[1]}, F),
          Kr2);
    } else if constexpr (N == 3u) {
      this->template compare<N>(
          computeThermalConductivityMatrixInReferenceConfiguration(
              thermalconductivity{k[0]}, thermalconductivity{k[1]},
              thermalconductivity{k[2]}, F),
          Kr2);
      this->template compare<N>(
          computeThermalConductivityMatrixInReferenceConfiguration(
              thermalconductivity_derivative{k[0]},
              thermalconductivity_derivative{k[1]},
              thermalconductivity_derivative{k[2]}, F),
          Kr2);
    }
  }  // end of test2
  template <bool use_qt>
  void test() noexcept {
    using namespace tfel::math;
    using namespace tfel::material;
    using real = typename tfel::config::ScalarTypes<double, use_qt>::real;
    constexpr auto eps = tfel::types::thermalconductivity<real>{1e-14};
    constexpr auto eps2 =
        derivative_type<tfel::types::thermalconductivity<real>,
                        tfel::types::temperature<real>>{1e-14};
    constexpr auto k = tfel::types::thermalconductivity<real>{3};
    constexpr auto dk_dT =
        derivative_type<tfel::types::thermalconductivity<real>,
                        tfel::types::temperature<real>>{3};
    constexpr auto id = tensor<3u, double>::Id();
    constexpr auto K =
        computeThermalConductivityMatrixInReferenceConfiguration(k, id);
    constexpr auto dK_dT =
        computeThermalConductivityMatrixInReferenceConfiguration(dk_dT, id);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(0, 0) - k) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(0, 1)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(0, 2)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(1, 0)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(1, 1) - k) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(1, 2)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(2, 0)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(2, 1)) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(K(2, 2) - k) < eps);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(0, 0) - dk_dT) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(0, 1)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(0, 2)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(1, 0)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(1, 1) - dk_dT) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(1, 2)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(2, 0)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(2, 1)) < eps2);
    TFEL_TESTS_STATIC_ASSERT(my_abs(dK_dT(2, 2) - dk_dT) < eps2);
  }  // end of test
  /*!
   * \brief check the heat flux in the reference configuration and its
   * derivatives against finite differences, for the thermal conductivity
   * matrices returned by the
   * `computeThermalConductivityMatrixInReferenceConfiguration` functions.
   */
  template <unsigned short N, bool use_qt>
  void test3() {
    using namespace tfel::math;
    using namespace tfel::material;
    using real = typename tfel::config::ScalarTypes<double, use_qt>::real;
    using thermalconductivity = tfel::types::thermalconductivity<real>;
    using temperaturegradient =
        std::conditional_t<use_qt, qt<unit::TemperatureGradient, double>,
                           double>;
    constexpr auto h = 1e-6;
    constexpr auto eps = 1e-8;
    const auto F = [] {
      if constexpr (N == 1u) {
        return tensor<1u, real>{real(1.1), real(0.9), real(1.2)};
      } else if constexpr (N == 2u) {
        return tensor<2u, real>{real(1.1), real(0.9), real(1.2), real(0.15),
                                real(-0.08)};
      } else {
        return tensor<3u, real>{real(1.1),   real(0.9),   real(1.2),
                                real(0.15),  real(-0.08), real(0.07),
                                real(-0.12), real(0.05),  real(0.11)};
      }
    }();
    auto gT = tvector<N, temperaturegradient>{};
    for (unsigned short i = 0; i != N; ++i) {
      gT[i] = temperaturegradient(2.5 - 2 * i);
    }
    // conductivity matrix in the reference configuration, orthotropic in 2D
    // and 3D
    const auto K = [](const tensor<N, real>& F2) {
      constexpr auto k1 = thermalconductivity(3);
      constexpr auto k2 = thermalconductivity(5);
      constexpr auto k3 = thermalconductivity(2);
      if constexpr (N == 1u) {
        return computeThermalConductivityMatrixInReferenceConfiguration(k1, F2);
      } else if constexpr (N == 2u) {
        return computeThermalConductivityMatrixInReferenceConfiguration(k1, k2,
                                                                        F2);
      } else {
        return computeThermalConductivityMatrixInReferenceConfiguration(k1, k2,
                                                                        k3, F2);
      }
    };
    const auto [Q, dQ_dgT, dQ_dF] =
        computeHeatFluxInReferenceConfigurationAndDerivatives(K(F), gT, F);
    const auto Q2 = eval(-(K(F) * gT));
    for (unsigned short i = 0; i != N; ++i) {
      TFEL_TESTS_ASSERT(std::abs(base_type_cast(Q[i] - Q2[i])) < eps);
      for (unsigned short j = 0; j != N; ++j) {
        TFEL_TESTS_ASSERT(std::abs(base_type_cast(dQ_dgT(i, j) + K(F)(i, j))) <
                          eps);
      }
    }
    // derivative with respect to F, compared to centered finite differences
    for (unsigned short c = 0; c != TensorDimeToSize<N>::value; ++c) {
      auto Fp = F;
      auto Fm = F;
      Fp[c] += real(h);
      Fm[c] -= real(h);
      const auto Qp = eval(-(K(Fp) * gT));
      const auto Qm = eval(-(K(Fm) * gT));
      for (unsigned short i = 0; i != N; ++i) {
        const auto nd =
            (base_type_cast(Qp[i]) - base_type_cast(Qm[i])) / (2 * h);
        TFEL_TESTS_ASSERT(std::abs(nd - base_type_cast(dQ_dF(i, c))) < eps);
      }
    }
  }  // end of test3
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainHeatTransferTest,
                          "FiniteStrainHeatTransfer");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainHeatTransfer.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
