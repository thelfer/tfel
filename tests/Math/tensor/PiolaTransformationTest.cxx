/*!
 * \file   tests/Math/tensor/PiolaTransformationTest.cxx
 * \brief  This file tests the Piola and Nanson transformations of vectors.
 * \author Tristan Chenaille
 * \date   15/11/2026
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
#include <type_traits>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/Tensor/PiolaTransformation.hxx"

/*!
 * \return true if the derivatives of the Piola transform are the expected
 * ones for the identity. This function is evaluated at compile time.
 */
static constexpr bool checkAtIdentity() {
  using namespace tfel::math;
  const auto F = tensor<3u, double>::Id();
  const auto [Q, dQ_dq, dQ_dF] =
      computePiolaTransformAndDerivatives(tvector<3u, double>{1, 2, 3}, F);
  // dQ_i/dF_kl = Q_i.delta_kl - delta_ik.Q_l, F_01 being the component 3
  const auto dQ_dF2 = tmatrix<3u, 3u, double>::Id() * dQ_dF;
  constexpr auto near = [](const double a, const double b) {
    return tfel::math::abs(a - b) < 1e-14;
  };
  return near(Q[2], 3) && near(dQ_dq(1, 1), 1) && near(dQ_dq(0, 1), 0) &&
         near(dQ_dF(0, 1), 1) && near(dQ_dF(0, 3), -2) &&
         near(dQ_dF2(0, 3), -2);
}  // end of checkAtIdentity

struct PiolaTransformationTest final : public tfel::tests::TestCase {
  PiolaTransformationTest()
      : tfel::tests::TestCase("TFEL/Math", "PiolaTransformationTest") {
  }  // end of PiolaTransformationTest
  tfel::tests::TestResult execute() override {
    TFEL_TESTS_STATIC_ASSERT(checkAtIdentity());
    this->test<1u, true>();
    this->test<1u, false>();
    this->test<2u, true>();
    this->test<2u, false>();
    this->test<3u, true>();
    this->test<3u, false>();
    this->testOutOfPlaneStretchConvention();
    return this->result;
  }  // end of execute

 private:
  //! \brief a simple alias
  using Matrix = std::array<std::array<double, 3>, 3>;
  //! \brief a simple alias
  using Vector = std::array<double, 3>;
  //! \return the adjugate of a matrix
  static Matrix computeAdjugate(const Matrix& F) {
    return {{{F[1][1] * F[2][2] - F[1][2] * F[2][1],
              F[0][2] * F[2][1] - F[0][1] * F[2][2],
              F[0][1] * F[1][2] - F[0][2] * F[1][1]},
             {F[1][2] * F[2][0] - F[1][0] * F[2][2],
              F[0][0] * F[2][2] - F[0][2] * F[2][0],
              F[0][2] * F[1][0] - F[0][0] * F[1][2]},
             {F[1][0] * F[2][1] - F[1][1] * F[2][0],
              F[0][1] * F[2][0] - F[0][0] * F[2][1],
              F[0][0] * F[1][1] - F[0][1] * F[1][0]}}};
  }  // end of computeAdjugate
  /*!
   * \return the product of a matrix or of its transpose by a vector
   * \param[in] m: matrix
   * \param[in] v: vector
   * \param[in] b: if true, the transpose of the matrix is used
   */
  static Vector apply(const Matrix& m, const Vector& v, const bool b) {
    auto r = Vector{};
    for (unsigned short i = 0; i != 3; ++i) {
      r[i] = 0;
      for (unsigned short j = 0; j != 3; ++j) {
        r[i] += (b ? m[j][i] : m[i][j]) * v[j];
      }
    }
    return r;
  }  // end of apply
  /*!
   * \brief compare a vector to a reference solution
   * \param[in] v: vector
   * \param[in] vr: reference solution
   */
  template <unsigned short N, typename VectorType>
  void compare(const VectorType& v, const Vector& vr) {
    constexpr auto eps = 1e-13;
    for (unsigned short i = 0; i != N; ++i) {
      TFEL_TESTS_ASSERT(std::abs(tfel::math::base_type_cast(v[i]) - vr[i]) <
                        eps);
    }
  }  // end of compare

  /*!
   * \brief check a function returning a transformed vector and its
   * derivatives
   * \param[in] f: function
   * \param[in] v: vector
   * \param[in] F: deformation gradient
   * \param[in] vr: reference solution for the transformed vector
   */
  template <unsigned short N,
            typename FunctionType,
            typename VectorType,
            typename TensorType>
  void checkDerivatives(const FunctionType& f,
                        const VectorType& v,
                        const TensorType& F,
                        const Vector& vr) {
    using namespace tfel::math;
    using real = numeric_type<TensorType>;
    using value_type = numeric_type<VectorType>;
    constexpr auto h = 1e-6;
    constexpr auto eps = 1e-8;
    const auto [r, dr_dv, dr_dF] = f(v, F);
    this->template compare<N>(r, vr);
    // the transformations are linear in the vector
    for (unsigned short j = 0; j != N; ++j) {
      auto e = VectorType{value_type{0}};
      e[j] = value_type{1};
      const auto rj = std::get<0>(f(e, F));
      for (unsigned short i = 0; i != N; ++i) {
        TFEL_TESTS_ASSERT(std::abs(base_type_cast(rj[i]) -
                                   base_type_cast(dr_dv(i, j))) < eps);
      }
    }
    // derivative with respect to F, compared to centered finite differences
    for (unsigned short c = 0; c != TensorDimeToSize<N>::value; ++c) {
      auto Fp = F;
      auto Fm = F;
      Fp[c] += real(h);
      Fm[c] -= real(h);
      const auto rp = std::get<0>(f(v, Fp));
      const auto rm = std::get<0>(f(v, Fm));
      for (unsigned short i = 0; i != N; ++i) {
        const auto nd =
            (base_type_cast(rp[i]) - base_type_cast(rm[i])) / (2 * h);
        TFEL_TESTS_ASSERT(std::abs(nd - base_type_cast(dr_dF(i, c))) < eps);
      }
    }
  }  // end of checkDerivatives

  template <unsigned short N, bool use_qt>
  void test() {
    using namespace tfel::math;
    using real = typename tfel::config::ScalarTypes<double, use_qt>::real;
    using heatfluxdensity =
        std::conditional_t<use_qt, qt<unit::HeatFluxDensity, double>, double>;
    const auto [F, Fm] = [] {
      // In space dimensions one and two, the deformation gradient is a block
      // diagonal matrix: the out of plane stretch, given by the third component
      // of the deformation gradient, is taken into account and the third
      // component of the vectors is null. In space dimension one, only the
      // diagonal components of the deformation gradient are described.
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
    // the components of the vector not described in 1D and 2D are null
    const auto vr = [] {
      if constexpr (N == 1u) {
        return Vector{2.5, 0, 0};
      } else if constexpr (N == 2u) {
        return Vector{2.5, -1.5, 0};
      } else {
        return Vector{2.5, -1.5, 0.75};
      }
    }();
    auto v = tvector<N, heatfluxdensity>{};
    for (unsigned short i = 0; i != N; ++i) {
      v[i] = heatfluxdensity{vr[i]};
    }
    const auto a = computeAdjugate(Fm);
    const auto J = Fm[0][0] * a[0][0] + Fm[0][1] * a[1][0] + Fm[0][2] * a[2][0];
    const auto scale = [](const Vector& x, const double s) {
      return Vector{x[0] * s, x[1] * s, x[2] * s};
    };
    // J.F^{-1}.v
    const auto Q = computePiolaTransform(v, F);
    this->template compare<N>(Q, apply(a, vr, false));
    // F.v/J
    this->template compare<N>(computeInversePiolaTransform(v, F),
                              scale(apply(Fm, vr, false), 1 / J));
    // J.F^{-T}.v
    const auto n = computeNansonTransform(v, F);
    this->template compare<N>(n, apply(a, vr, true));
    // F^{T}.v/J
    this->template compare<N>(computeInverseNansonTransform(v, F),
                              scale(apply(Fm, vr, true), 1 / J));
    // the inverse transformations give back the initial vector
    this->template compare<N>(computeInversePiolaTransform(Q, F), vr);
    this->template compare<N>(computeInverseNansonTransform(n, F), vr);
    // derivatives
    this->template checkDerivatives<N>(
        [](const auto& x, const auto& y) {
          return computePiolaTransformAndDerivatives(x, y);
        },
        v, F, apply(a, vr, false));
    this->template checkDerivatives<N>(
        [](const auto& x, const auto& y) {
          return computeInversePiolaTransformAndDerivatives(x, y);
        },
        v, F, scale(apply(Fm, vr, false), 1 / J));
    this->template checkDerivatives<N>(
        [](const auto& x, const auto& y) {
          return computeNansonTransformAndDerivatives(x, y);
        },
        v, F, apply(a, vr, true));
    this->template checkDerivatives<N>(
        [](const auto& x, const auto& y) {
          return computeInverseNansonTransformAndDerivatives(x, y);
        },
        v, F, scale(apply(Fm, vr, true), 1 / J));
  }  // end of test
  /*!
   * \brief In space dimensions one and two, the out of plane stretch, given by
   * the third component of the deformation gradient, is taken into account: the
   * result is the in plane block of the result obtained in the three
   * dimensional case. This function checks that this convention is honoured.
   */
  void testOutOfPlaneStretchConvention() {
    using namespace tfel::math;
    constexpr auto eps = 1e-13;
    // block diagonal deformation gradient: the out of plane direction is not
    // coupled with the in plane one
    const auto F3 = tensor<3u, double>{1.1, 0.9, 1.2, 0.15, -0.08, 0, 0, 0, 0};
    const auto F2 = tensor<2u, double>{1.1, 0.9, 1.2, 0.15, -0.08};
    // in space dimension one, only the diagonal components of the deformation
    // gradient are described: the reference solution is the three dimensional
    // result obtained with a diagonal deformation gradient
    const auto F3d = tensor<3u, double>{1.1, 0.9, 1.2, 0, 0, 0, 0, 0, 0};
    const auto F1 = tensor<1u, double>{1.1, 0.9, 1.2};
    // the third component of the vectors of the current configuration is null
    const auto q3 = tvector<3u, double>{2.5, -1.5, 0};
    const auto q2 = tvector<2u, double>{2.5, -1.5};
    const auto q1 = tvector<1u, double>{2.5};
    const auto as_vector = [](const auto& v) {
      return Vector{tfel::math::base_type_cast(v[0]),
                    tfel::math::base_type_cast(v[1]),
                    tfel::math::base_type_cast(v[2])};
    };
    const auto Q3 = computePiolaTransform(q3, F3);
    this->compare<2u>(computePiolaTransform(q2, F2), as_vector(Q3));
    this->compare<1u>(computePiolaTransform(q1, F1),
                      as_vector(computePiolaTransform(q3, F3d)));
    const auto n3 = computeNansonTransform(q3, F3);
    this->compare<2u>(computeNansonTransform(q2, F2), as_vector(n3));
    this->compare<1u>(computeNansonTransform(q1, F1),
                      as_vector(computeNansonTransform(q3, F3d)));
    // the out of plane component of the three dimensional results is null, so
    // that the transformations are consistent with the convention
    TFEL_TESTS_ASSERT(std::abs(Q3[2]) < eps);
    TFEL_TESTS_ASSERT(std::abs(n3[2]) < eps);
    // Piola's transformation of the identity matrix
    const auto P3 =
        computePiolaTransformOfIdentityMatrixInReferenceConfiguration(F3);
    const auto P2 =
        computePiolaTransformOfIdentityMatrixInReferenceConfiguration(F2);
    const auto P1 =
        computePiolaTransformOfIdentityMatrixInReferenceConfiguration(F1);
    for (unsigned short i = 0; i != 2u; ++i) {
      for (unsigned short j = 0; j != 2u; ++j) {
        TFEL_TESTS_ASSERT(std::abs(P2(i, j) - P3(i, j)) < eps);
      }
    }
    TFEL_TESTS_ASSERT(
        std::abs(P1(0, 0) -
                 computePiolaTransformOfIdentityMatrixInReferenceConfiguration(
                     F3d)(0, 0)) < eps);
    // in space dimension one, the result is F22.F33/F11
    TFEL_TESTS_ASSERT(std::abs(P1(0, 0) - 0.9 * 1.2 / 1.1) < eps);
    // the missing out of plane component of the three dimensional result is
    // det(F)/F33^2 where det(F) is the in plane determinant
    TFEL_TESTS_ASSERT(std::abs(P3(2, 2) - (1.1 * 0.9 - 0.15 * -0.08) / 1.2) <
                      eps);
  }  // end of testOutOfPlaneStretchConvention
};

TFEL_TESTS_GENERATE_PROXY(PiolaTransformationTest, "PiolaTransformationTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("PiolaTransformation.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
