/*!
 * \file   TFEL/Math/Tensor/PiolaTransformation.ixx
 * \brief  This file implements functions related to the Piola
 * transformation.
 * \author Thomas Helfer
 * \date   04/02/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PIOLATRANSFORMATION_IXX
#define LIB_TFEL_MATH_PIOLATRANSFORMATION_IXX

#include <type_traits>

namespace tfel::math::internals {

  /*!
   * \return the product of a tensor and a vector
   * \param[in] t: tensor
   * \param[in] v: vector
   */
  template <unsigned short N, typename ValueType, TensorConcept TensorType>
  requires(getSpaceDimension<TensorType>() == N)  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto applyTensor(
          const TensorType& t, const tvector<N, ValueType>& v) noexcept {
    using ResultType = tvector<N, std::decay_t<decltype(t[0] * v[0])>>;
    if constexpr (N == 1) {
      return ResultType{t[0] * v[0]};
    } else if constexpr (N == 2) {
      return ResultType{t[0] * v[0] + t[3] * v[1],  //
                        t[4] * v[0] + t[1] * v[1]};
    } else {
      return ResultType{t[0] * v[0] + t[3] * v[1] + t[5] * v[2],
                        t[4] * v[0] + t[1] * v[1] + t[7] * v[2],
                        t[6] * v[0] + t[8] * v[1] + t[2] * v[2]};
    }
  }  // end of applyTensor

  /*!
   * \return the product of the transpose of a tensor and a vector
   * \param[in] t: tensor
   * \param[in] v: vector
   */
  template <unsigned short N, typename ValueType, TensorConcept TensorType>
  requires(getSpaceDimension<TensorType>() == N)  //
      TFEL_HOST_DEVICE [[nodiscard]] constexpr auto applyTransposedTensor(
          const TensorType& t, const tvector<N, ValueType>& v) noexcept {
    using ResultType = tvector<N, std::decay_t<decltype(t[0] * v[0])>>;
    if constexpr (N == 1) {
      return ResultType{t[0] * v[0]};
    } else if constexpr (N == 2) {
      return ResultType{t[0] * v[0] + t[4] * v[1],  //
                        t[3] * v[0] + t[1] * v[1]};
    } else {
      return ResultType{t[0] * v[0] + t[4] * v[1] + t[6] * v[2],
                        t[3] * v[0] + t[1] * v[1] + t[8] * v[2],
                        t[5] * v[0] + t[7] * v[1] + t[2] * v[2]};
    }
  }  // end of applyTransposedTensor

  /*!
   * \return the row index of a component of a tensor
   * \param[in] c: index of the component
   * \note the components are stored in the following order: 00, 11, 22,
   * 01, 10, 02, 20, 12, 21. Only the first 3 (1D) or 5 (2D) are stored in
   * 1D and 2D. The indices are computed, not read from a table.
   */
  TFEL_HOST_DEVICE constexpr unsigned short getTensorComponentRowIndex(
      const unsigned short c) noexcept {
    if (c < 3) {
      return c;
    }
    // off-diagonal components: pairs (0, 1), (0, 2) and (1, 2)
    const auto p = static_cast<unsigned short>((c - 3) / 2);
    const auto a = static_cast<unsigned short>(p / 2);
    const auto b = static_cast<unsigned short>((p + 3) / 2);
    return ((c - 3) % 2 == 0) ? a : b;
  }  // end of getTensorComponentRowIndex

  /*!
   * \return the column index of a component of a tensor
   * \param[in] c: index of the component
   * \see getTensorComponentRowIndex
   */
  TFEL_HOST_DEVICE constexpr unsigned short getTensorComponentColumnIndex(
      const unsigned short c) noexcept {
    if (c < 3) {
      return c;
    }
    const auto p = static_cast<unsigned short>((c - 3) / 2);
    const auto a = static_cast<unsigned short>(p / 2);
    const auto b = static_cast<unsigned short>((p + 3) / 2);
    return ((c - 3) % 2 == 0) ? b : a;
  }  // end of getTensorComponentColumnIndex

  /*!
   * \return the component (i, j) of a tensor, the components which are not
   * stored in 1D and 2D being null
   * \param[in] t: tensor
   * \param[in] i: row index, lower than 3
   * \param[in] j: column index, lower than 3
   */
  template <TensorConcept TensorType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr numeric_type<TensorType>
  getTensorComponent(const TensorType& t,
                     const unsigned short i,
                     const unsigned short j) noexcept {
    constexpr auto N = getSpaceDimension<TensorType>();
    if (i == j) {
      return t[i];
    }
    const auto c =
        static_cast<unsigned short>(3 + 2 * (i + j - 1) + ((i > j) ? 1 : 0));
    if (c >= TensorDimeToSize<N>::value) {
      return numeric_type<TensorType>{0};
    }
    return t[c];
  }  // end of getTensorComponent

  /*!
   * \return the component i of a vector, the components which are not
   * stored in 1D and 2D being null
   * \param[in] v: vector
   * \param[in] i: index, lower than 3
   */
  template <unsigned short N, typename ValueType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr ValueType getVectorComponent(
      const tvector<N, ValueType>& v, const unsigned short i) noexcept {
    return (i < N) ? v[i] : ValueType{0};
  }  // end of getVectorComponent

  /*!
   * \return a matrix whose components are given by a function
   * \tparam MatrixType: type of the matrix
   * \param[in] f: function of the indices (i, j)
   */
  template <unsigned short N, typename MatrixType, typename FunctionType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr MatrixType buildMatrix(
      const FunctionType& f) noexcept {
    auto m = MatrixType{};
    for (unsigned short i = 0; i != N; ++i) {
      for (unsigned short j = 0; j != N; ++j) {
        m(i, j) = f(i, j);
      }
    }
    return m;
  }  // end of buildMatrix

  /*!
   * \return the derivative of a vector with respect to a tensor
   * \tparam DerivativeType: type of the derivative
   * \param[in] f: function returning the derivative of the component i of
   * the vector with respect to the component (k, l) of the tensor
   */
  template <unsigned short N, typename DerivativeType, typename FunctionType>
  TFEL_HOST_DEVICE [[nodiscard]] constexpr DerivativeType
  buildDerivativeWithRespectToTensor(const FunctionType& f) noexcept {
    auto d = DerivativeType{};
    for (unsigned short i = 0; i != N; ++i) {
      for (unsigned short c = 0; c != TensorDimeToSize<N>::value; ++c) {
        d(i, c) = f(i, getTensorComponentRowIndex(c),
                    getTensorComponentColumnIndex(c));
      }
    }
    return d;
  }  // end of buildDerivativeWithRespectToTensor

  //! \brief type of the derivative of a vector with respect to a tensor
  template <typename VectorType, TensorConcept TensorType>
  using VectorDerivativeWithRespectToTensor = derivative_type<
      VectorType,
      tensor<getSpaceDimension<TensorType>(), numeric_type<TensorType>>>;

}  // end of namespace tfel::math::internals

namespace tfel::math {

  template <tfel::math::NoUnitTensorConcept DeformationGradientType>
  constexpr auto computePiolaTransformOfIdentityMatrixInReferenceConfiguration(
      const DeformationGradientType& F) noexcept {
    using real = base_type<numeric_type<DeformationGradientType>>;
    constexpr auto icste = Cste<real>::isqrt2;
    constexpr auto N = getSpaceDimension<DeformationGradientType>();
    static_assert((N == 1) || (N == 2) || (N == 3));
    using ResultType = tmatrix<N, N, numeric_type<DeformationGradientType>>;
    const auto C = computeRightCauchyGreenTensor(F);
    const auto iC = invert(C);
    const auto J = det(F);
    if constexpr (N == 1) {
      return ResultType{J * iC[0]};
    } else if constexpr (N == 2) {
      return ResultType{J * iC[0], J * iC[3] * icste,  //
                        J * iC[3] * icste, J * iC[1]};
    } else {
      return ResultType{
          J * iC[0],         J * iC[3] * icste, J * iC[4] * icste,  //
          J * iC[3] * icste, J * iC[1],         J * iC[5] * icste,
          J * iC[4] * icste, J * iC[5] * icste, J * iC[2]};
    }
  }  // end of computePiolaTransformOfIdentityMatrixInReferenceConfiguration

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computePiolaTransform(
          const tvector<N, ValueType>& q,
          const DeformationGradientType& F) noexcept {
    static_assert((N == 1) || (N == 2) || (N == 3));
    return internals::applyTensor(computeAdjugateDeterminantDerivative(F), q);
  }  // end of computePiolaTransform

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computeInversePiolaTransform(
          const tvector<N, ValueType>& Q,
          const DeformationGradientType& F) noexcept {
    static_assert((N == 1) || (N == 2) || (N == 3));
    return internals::applyTensor(F, Q) / det(F);
  }  // end of computeInversePiolaTransform

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computeNansonTransform(
          const tvector<N, ValueType>& n,
          const DeformationGradientType& F) noexcept {
    static_assert((N == 1) || (N == 2) || (N == 3));
    return internals::applyTransposedTensor(
        computeAdjugateDeterminantDerivative(F), n);
  }  // end of computeNansonTransform

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computeInverseNansonTransform(
          const tvector<N, ValueType>& n,
          const DeformationGradientType& F) noexcept {
    static_assert((N == 1) || (N == 2) || (N == 3));
    return internals::applyTransposedTensor(F, n) / det(F);
  }  // end of computeInverseNansonTransform

  /*
   * In the following functions, F^{-1} is computed as adj/J where adj is
   * the adjugate of F, i.e. J.F^{-1}. The derivatives with respect to F are
   * written using the indices (k, l) of the component of F:
   *
   * - d(J.F^{-1}.q)_i/dF_kl = Q_i.F^{-1}_lk - F^{-1}_ik.Q_l
   * - d(F.Q/J)_i/dF_kl = delta_ik.Q_l/J - q_i.F^{-1}_lk
   * - d(J.F^{-T}.N)_i/dF_kl = n_i.F^{-1}_lk - n_k.F^{-1}_li
   * - d(F^{T}.n/J)_i/dF_kl = delta_il.n_k/J - N_i.F^{-1}_lk
   */

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computePiolaTransformAndDerivatives(
          const tvector<N, ValueType>& q,
          const DeformationGradientType& F) noexcept {
    using namespace internals;
    static_assert((N == 1) || (N == 2) || (N == 3));
    const auto adj = computeAdjugateDeterminantDerivative(F);
    const auto J = det(F);
    const auto Q = applyTensor(adj, q);
    using VectorType = std::decay_t<decltype(Q)>;
    using DerivativeType1 = derivative_type<VectorType, tvector<N, ValueType>>;
    using DerivativeType2 =
        VectorDerivativeWithRespectToTensor<VectorType,
                                            DeformationGradientType>;
    const auto dQ_dq = buildMatrix<N, DerivativeType1>(
        [&adj](const unsigned short i, const unsigned short j) {
          return getTensorComponent(adj, i, j);
        });
    const auto dQ_dF = buildDerivativeWithRespectToTensor<N, DerivativeType2>(
        [&adj, &Q, J](const unsigned short i, const unsigned short k,
                      const unsigned short l) {
          return (Q[i] * getTensorComponent(adj, l, k) -
                  getTensorComponent(adj, i, k) * getVectorComponent(Q, l)) /
                 J;
        });
    return std::tuple{Q, dQ_dq, dQ_dF};
  }  // end of computePiolaTransformAndDerivatives

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computeInversePiolaTransformAndDerivatives(
          const tvector<N, ValueType>& Q,
          const DeformationGradientType& F) noexcept {
    using namespace internals;
    static_assert((N == 1) || (N == 2) || (N == 3));
    const auto adj = computeAdjugateDeterminantDerivative(F);
    const auto J = det(F);
    const auto q = eval(applyTensor(F, Q) / J);
    using VectorType = std::decay_t<decltype(q)>;
    using DerivativeType1 = derivative_type<VectorType, tvector<N, ValueType>>;
    using DerivativeType2 =
        VectorDerivativeWithRespectToTensor<VectorType,
                                            DeformationGradientType>;
    const auto dq_dQ = buildMatrix<N, DerivativeType1>(
        [&F, J](const unsigned short i, const unsigned short j) {
          return getTensorComponent(F, i, j) / J;
        });
    const auto dq_dF = buildDerivativeWithRespectToTensor<N, DerivativeType2>(
        [&adj, &q, &Q, J](const unsigned short i, const unsigned short k,
                          const unsigned short l) {
          const auto d = (i == k) ? getVectorComponent(Q, l) : ValueType{0};
          return (d - q[i] * getTensorComponent(adj, l, k)) / J;
        });
    return std::tuple{q, dq_dQ, dq_dF};
  }  // end of computeInversePiolaTransformAndDerivatives

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computeNansonTransformAndDerivatives(
          const tvector<N, ValueType>& Nv,
          const DeformationGradientType& F) noexcept {
    using namespace internals;
    static_assert((N == 1) || (N == 2) || (N == 3));
    const auto adj = computeAdjugateDeterminantDerivative(F);
    const auto J = det(F);
    const auto n = applyTransposedTensor(adj, Nv);
    using VectorType = std::decay_t<decltype(n)>;
    using DerivativeType1 = derivative_type<VectorType, tvector<N, ValueType>>;
    using DerivativeType2 =
        VectorDerivativeWithRespectToTensor<VectorType,
                                            DeformationGradientType>;
    const auto dn_dN = buildMatrix<N, DerivativeType1>(
        [&adj](const unsigned short i, const unsigned short j) {
          return getTensorComponent(adj, j, i);
        });
    const auto dn_dF = buildDerivativeWithRespectToTensor<N, DerivativeType2>(
        [&adj, &n, J](const unsigned short i, const unsigned short k,
                      const unsigned short l) {
          return (n[i] * getTensorComponent(adj, l, k) -
                  getVectorComponent(n, k) * getTensorComponent(adj, l, i)) /
                 J;
        });
    return std::tuple{n, dn_dN, dn_dF};
  }  // end of computeNansonTransformAndDerivatives

  template <unsigned short N,
            typename ValueType,
            tfel::math::NoUnitTensorConcept DeformationGradientType>
  requires(getSpaceDimension<DeformationGradientType>() == N)  //
      constexpr auto computeInverseNansonTransformAndDerivatives(
          const tvector<N, ValueType>& n,
          const DeformationGradientType& F) noexcept {
    using namespace internals;
    static_assert((N == 1) || (N == 2) || (N == 3));
    const auto adj = computeAdjugateDeterminantDerivative(F);
    const auto J = det(F);
    const auto Nv = eval(applyTransposedTensor(F, n) / J);
    using VectorType = std::decay_t<decltype(Nv)>;
    using DerivativeType1 = derivative_type<VectorType, tvector<N, ValueType>>;
    using DerivativeType2 =
        VectorDerivativeWithRespectToTensor<VectorType,
                                            DeformationGradientType>;
    const auto dN_dn = buildMatrix<N, DerivativeType1>(
        [&F, J](const unsigned short i, const unsigned short j) {
          return getTensorComponent(F, j, i) / J;
        });
    const auto dN_dF = buildDerivativeWithRespectToTensor<N, DerivativeType2>(
        [&adj, &n, &Nv, J](const unsigned short i, const unsigned short k,
                           const unsigned short l) {
          const auto d = (i == l) ? getVectorComponent(n, k) : ValueType{0};
          return (d - Nv[i] * getTensorComponent(adj, l, k)) / J;
        });
    return std::tuple{Nv, dN_dn, dN_dF};
  }  // end of computeInverseNansonTransformAndDerivatives

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_PIOLATRANSFORMATION_IXX */
