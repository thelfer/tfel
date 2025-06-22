/*!
 * \file   include/TFEL/Math/Matrix/tmatrix.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   28 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINY_MATRIX_IXX
#define LIB_TFEL_MATH_TINY_MATRIX_IXX

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/AbsCompare.hxx"

namespace tfel::math::internals {

  /*!
   * \brief an helper structure to build a view on derivative from a tiny
   * matrix.
   * \tparam is_function_type_scalar: boolean stating if `FunctionType` is a
   * scalar.
   * \tparam is_variable_type_scalar: boolean stating if `VariableType` is a
   * scalar.
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   */
  template <bool is_function_type_scalar,
            bool is_variable_type_scalar,
            typename FunctionType,
            typename VariableType>
  struct DerivativeViewFromTinyMatrixImplementation {
    static_assert(
        hasArrayPolicyFixedSizes<typename FunctionType::array_policy>(),
        "invalid function type");
    static_assert(
        hasArrayPolicyFixedSizes<typename VariableType::array_policy>(),
        "invalid variable type");
    //! \brief a simple alias
    using derivative_base_type = tfel::math::base_type<tfel::math::numeric_type<
        tfel::math::derivative_type<FunctionType, VariableType>>>;
    //! \brief a simple alias
    using size_type = unsigned short;
    /*!
     * \tparam I: row index.
     * \tparam J: column index.
     * \tparam N: number of rows of the matrix.
     * \tparam M: number of columns of the matrix.
     */
    template <size_type I, size_type J, size_type N, size_type M>
    static constexpr tfel::math::
        derivative_view_from_tiny_matrix<M, FunctionType, VariableType>
        exe(tfel::math::tmatrix<N, M, derivative_base_type>& m) {
      constexpr auto fsize =
          getArrayPolicySize<typename FunctionType::array_policy>();
      constexpr auto vsize =
          getArrayPolicySize<typename VariableType::array_policy>();
      static_assert(N >= I + fsize, "invalid row index");
      static_assert(M >= J + vsize, "invalid column index");
      return tfel::math::derivative_view_from_tiny_matrix<M, FunctionType,
                                                          VariableType>{
          &m(I, J)};
    }  // end of exe
    /*!
     * \tparam N: number of rows of the matrix.
     * \tparam M: number of columns of the matrix.
     * \param[in] i: row index.
     * \param[in] j: column index.
     */
    template <size_type N, size_type M>
    static constexpr tfel::math::
        derivative_view_from_tiny_matrix<M, FunctionType, VariableType>
        exe(tfel::math::tmatrix<N, M, derivative_base_type>& m,
            const size_type i,
            const size_type j) {
      return tfel::math::derivative_view_from_tiny_matrix<M, FunctionType,
                                                          VariableType>{
          &m(i, j)};
    }  // end of exe
  };   // end of struct DerivativeViewFromTinyMatrixImplementation

  /*!
   * \brief partial specialization if the function type is a scalar.
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeViewFromTinyMatrixImplementation<true,
                                                    false,
                                                    FunctionType,
                                                    VariableType> {
    static_assert(
        hasArrayPolicyFixedSizes<typename VariableType::array_policy>(),
        "invalid variable type");
    //! \brief a simple alias
    using derivative_base_type = tfel::math::base_type<tfel::math::numeric_type<
        tfel::math::derivative_type<FunctionType, VariableType>>>;
    //! \brief a simple alias
    using size_type = unsigned short;
    /*!
     * \tparam I: row index.
     * \tparam J: column index.
     * \tparam N: number of rows of the matrix.
     * \tparam M: number of columns of the matrix.
     */
    template <size_type I, size_type J, size_type N, size_type M>
    static constexpr tfel::math::
        derivative_view_from_tiny_matrix<M, FunctionType, VariableType>
        exe(tfel::math::tmatrix<N, M, derivative_base_type>& m) {
      constexpr auto vsize =
          getArrayPolicySize<typename VariableType::array_policy>();
      static_assert(N > I, "invalid row index");
      static_assert(M >= J + vsize, "invalid column index");
      return tfel::math::derivative_view_from_tiny_matrix<M, FunctionType,
                                                          VariableType>{
          &m(I, J)};
    }  // end of exe
    /*!
     * \tparam N: number of rows of the matrix.
     * \tparam M: number of columns of the matrix.
     * \param[in] i: row index.
     * \param[in] j: column index.
     */
    template <size_type N, size_type M>
    static constexpr tfel::math::
        derivative_view_from_tiny_matrix<M, FunctionType, VariableType>
        exe(tfel::math::tmatrix<N, M, derivative_base_type>& m,
            const size_type i,
            const size_type j) {
      return tfel::math::derivative_view_from_tiny_matrix<M, FunctionType,
                                                          VariableType>{
          &m(i, j)};
    }  // end of exe
  };   // end of struct DerivativeViewFromTinyMatrixImplementation

  /*!
   * \brief partial specialization if the variable type is a scalar.
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeViewFromTinyMatrixImplementation<false,
                                                    true,
                                                    FunctionType,
                                                    VariableType> {
    static_assert(
        hasArrayPolicyFixedSizes<typename FunctionType::array_policy>(),
        "invalid function type");
    //! \brief a simple alias
    using derivative_base_type = tfel::math::base_type<tfel::math::numeric_type<
        tfel::math::derivative_type<FunctionType, VariableType>>>;
    //! \brief a simple alias
    using size_type = unsigned short;
    /*!
     * \tparam I: row index.
     * \tparam J: column index.
     * \tparam N: number of rows of the matrix.
     * \tparam M: number of columns of the matrix.
     */
    template <size_type I, size_type J, size_type N, size_type M>
    static constexpr tfel::math::
        derivative_view_from_tiny_matrix<M, FunctionType, VariableType>
        exe(tfel::math::tmatrix<N, M, derivative_base_type>& m) {
      constexpr auto fsize =
          getArrayPolicySize<typename FunctionType::array_policy>();
      static_assert(N >= I + fsize, "invalid row index");
      static_assert(M > J, "invalid column index");
      return tfel::math::derivative_view_from_tiny_matrix<M, FunctionType,
                                                          VariableType>{
          &m(I, J)};
    }  // end of exe
    /*!
     * \tparam N: number of rows of the matrix.
     * \tparam M: number of columns of the matrix.
     * \param[in] i: row index.
     * \param[in] j: column index.
     */
    template <size_type N, size_type M>
    static constexpr tfel::math::
        derivative_view_from_tiny_matrix<M, FunctionType, VariableType>
        exe(tfel::math::tmatrix<N, M, derivative_base_type>& m,
            const size_type i,
            const size_type j) {
      return tfel::math::derivative_view_from_tiny_matrix<M, FunctionType,
                                                          VariableType>{
          &m(i, j)};
    }  // end of exe
  };   // end of struct DerivativeViewFromTinyMatrixImplementation

  /*!
   * \brief partial specialization if the function type and the variable type
   * are scalars.
   * \tparam FunctionType: function type.
   * \tparam VariableType: variable type.
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeViewFromTinyMatrixImplementation<true,
                                                    true,
                                                    FunctionType,
                                                    VariableType> {
    //! \brief a simple alias
    using derivative_base_type = tfel::math::base_type<tfel::math::numeric_type<
        tfel::math::derivative_type<FunctionType, VariableType>>>;
    //! \brief a simple alias
    using size_type = unsigned short;
    /*!
     * \tparam I: row index.
     * \tparam J: column index.
     * \tparam N: number of rows of the matrix.
     * \tparam M: number of columns of the matrix.
     */
    template <size_type I, size_type J, size_type N, size_type M>
    static constexpr tfel::math::
        derivative_view_from_tiny_matrix<M, FunctionType, VariableType>
        exe(tfel::math::tmatrix<N, M, derivative_base_type>& m) {
      static_assert(I < N, "invalid row index");
      static_assert(J < M, "invalid column index");
      return tfel::math::derivative_view_from_tiny_matrix<M, FunctionType,
                                                          VariableType>(
          m(I, J));
    }  // end of exe
    /*!
     * \tparam N: number of rows of the matrix.
     * \tparam M: number of columns of the matrix.
     * \param[in] i: row index.
     * \param[in] j: column index.
     */
    template <size_type N, size_type M>
    static constexpr tfel::math::
        derivative_view_from_tiny_matrix<M, FunctionType, VariableType>
        exe(tfel::math::tmatrix<N, M, derivative_base_type>& m,
            const size_type i,
            const size_type j) {
      return tfel::math::derivative_view_from_tiny_matrix<M, FunctionType,
                                                          VariableType>(
          m(i, j));
    }  // end of exe
  };   // end of struct DerivativeViewFromTinyMatrixImplementation

  template <typename Matrix>
  constexpr std::enable_if_t<
      implementsMatrixConcept<Matrix>(),
      typename ComputeUnaryResult<numeric_type<Matrix>, Power<3>>::Result>
  det2(const Matrix& m) {
    return m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1);
  }

  template <typename Matrix>
  constexpr std::enable_if_t<
      implementsMatrixConcept<Matrix>(),
      typename ComputeUnaryResult<numeric_type<Matrix>, Power<3>>::Result>
  det3(const Matrix& m) {
    const auto a = m(0, 0);
    const auto b = m(0, 1);
    const auto c = m(0, 2);
    const auto d = m(1, 0);
    const auto e = m(1, 1);
    const auto f = m(1, 2);
    const auto g = m(2, 0);
    const auto h = m(2, 1);
    const auto i = m(2, 2);
    return a * (e * i - f * h) + b * (f * g - d * i) + c * (d * h - e * g);
  }

}  // end of namespace tfel::math::internals

namespace tfel::math {

  template <unsigned short N, unsigned short M, typename T>
  TFEL_HOST_DEVICE constexpr unsigned short tmatrix<N, M, T>::getNbCols()
      const {
    return M;
  }

  template <unsigned short N, unsigned short M, typename T>
  TFEL_HOST_DEVICE constexpr unsigned short tmatrix<N, M, T>::getNbRows()
      const {
    return N;
  }

  template <unsigned short N, unsigned short M, typename T>
  TFEL_HOST_DEVICE constexpr auto tmatrix<N, M, T>::max() const {
    return *tfel::fsalgo::max_element<this->size()>::exe(this->v);
  }

  template <unsigned short N, unsigned short M, typename T>
  TFEL_HOST_DEVICE constexpr auto tmatrix<N, M, T>::abs_max() const {
    return tfel::math::abs(*tfel::fsalgo::max_element<this->size()>::exe(
        this->v, absCompare<T>()));
  }

  template <unsigned short N, unsigned short M, typename T>
  TFEL_HOST_DEVICE constexpr void tmatrix<N, M, T>::swap_rows(
      const unsigned short i, const unsigned short j) {
    tfel::fsalgo::swap_ranges<M>::exe(this->v + M * i, this->v + M * j);
  }

  template <unsigned short N, unsigned short M, typename T>
  TFEL_HOST_DEVICE constexpr auto tmatrix<N, M, T>::Id() {
    static_assert(N == M);
    tmatrix<N, N, T> id(static_cast<T>(0.));
    for (typename tmatrix<N, N, T>::size_type i = 0; i < N; ++i) {
      id(i, i) = static_cast<T>(1.);
    }
    return id;
  }

  template <unsigned short N, unsigned short M, typename T>
  template <typename InputIterator>
  TFEL_HOST_DEVICE constexpr void tmatrix<N, M, T>::copy(
      const InputIterator src) {
    tfel::fsalgo::copy<this->size()>::exe(src, this->v);
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I>
  constexpr auto tmatrix<N, M, T>::row_view() {
    static_assert(I < N, "invalid row index");
    return map<tvector<M, T>>(&(this->operator()(I, 0)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I>
  constexpr auto tmatrix<N, M, T>::row_view() const {
    static_assert(I < N, "invalid row index");
    return map<const tvector<M, T>>(&(this->operator()(I, 0)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I, unsigned short J, unsigned short K>
  constexpr auto tmatrix<N, M, T>::row_view() {
    static_assert(I < N, "invalid row index");
    static_assert(J < M, "invalid column index");
    static_assert(M >= J + K, "invalid view size");
    return map<tvector<K, T>>(&(this->operator()(I, J)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I, unsigned short J, unsigned short K>
  constexpr auto tmatrix<N, M, T>::row_view() const {
    static_assert(I < N, "invalid row index");
    static_assert(J < M, "invalid column index");
    static_assert(M >= J + K, "invalid view size");
    return map<const tvector<K, T>>(&(this->operator()(I, J)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I>
  constexpr auto tmatrix<N, M, T>::column_view() {
    static_assert(I < M, "invalid column index");
    using ViewIndexingPolicy =
        FixedSizeVectorIndexingPolicy<unsigned short, N, M>;
    return map<tvector<N, T>, ViewIndexingPolicy>(&(this->operator()(0, I)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I>
  constexpr auto tmatrix<N, M, T>::column_view() const {
    static_assert(I < M, "invalid column index");
    using ViewIndexingPolicy =
        FixedSizeVectorIndexingPolicy<unsigned short, N, M>;
    return map<const tvector<N, T>, ViewIndexingPolicy>(
        &(this->operator()(0, I)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I, unsigned short J, unsigned short K>
  constexpr auto tmatrix<N, M, T>::column_view() {
    static_assert(I < M, "invalid column index");
    static_assert(J < N, "invalid row index");
    static_assert(N >= J + K, "invalid view size");
    using ViewIndexingPolicy =
        FixedSizeVectorIndexingPolicy<unsigned short, K, M>;
    return map<tvector<K, T>, ViewIndexingPolicy>(&(this->operator()(J, I)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I, unsigned short J, unsigned short K>
  constexpr auto tmatrix<N, M, T>::column_view() const {
    static_assert(I < M, "invalid column index");
    static_assert(J < N, "invalid row index");
    static_assert(N >= J + K, "invalid view size");
    using ViewIndexingPolicy =
        FixedSizeVectorIndexingPolicy<unsigned short, K, M>;
    return map<const tvector<K, T>, ViewIndexingPolicy>(
        &(this->operator()(J, I)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I,
            unsigned short J,
            unsigned short R,
            unsigned short C>
  constexpr auto tmatrix<N, M, T>::submatrix_view() {
    static_assert(I < N, "invalid row index");
    static_assert(J < M, "invalid column index");
    static_assert(N >= I + R, "invalid number of rows for the submatrix");
    static_assert(M >= J + C, "invalid number of columns for the submatrix");
    using ViewIndexingPolicy =
        FixedSizeRowMajorMatrixIndexingPolicy<unsigned short, R, C, M>;
    return map<tmatrix<R, C, T>, ViewIndexingPolicy>(&(this->operator()(I, J)));
  }  // end of submatrix_view

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I,
            unsigned short J,
            unsigned short R,
            unsigned short C>
  constexpr auto tmatrix<N, M, T>::submatrix_view() const {
    static_assert(I < N, "invalid row index");
    static_assert(J < M, "invalid column index");
    static_assert(N >= I + R, "invalid number of rows for the submatrix");
    static_assert(M >= J + C, "invalid number of columns for the submatrix");
    using ViewIndexingPolicy =
        FixedSizeRowMajorMatrixIndexingPolicy<unsigned short, R, C, M>;
    return map<const tmatrix<R, C, T>, ViewIndexingPolicy>(
        &(this->operator()(I, J)));
  }  // end of submatrix_view

  // template<unsigned short N,unsigned short M,typename T>
  template <unsigned short N, unsigned short M, typename T>
  constexpr tmatrix<M, N, T> transpose(const tmatrix<N, M, T>& m) {
    tmatrix<M, N, T> tm;
    for (typename tmatrix<N, M, T>::size_type i = 0; i < N; ++i) {
      for (typename tmatrix<N, M, T>::size_type j = 0; j < M; ++j) {
        tm(j, i) = m(i, j);
      }
    }
    return tm;
  }

  template <typename T, typename Operation>
  constexpr auto det(const Expr<tmatrix<2, 2, T>, Operation>& m) {
    return tfel::math::internals::det2(m);
  }

  template <typename T>
  constexpr auto det(const tmatrix<2, 2, T>& m) {
    return tfel::math::internals::det2(m);
  }

  template <typename T, typename Operation>
  constexpr auto det(const Expr<tmatrix<3, 3, T>, Operation>& m) {
    return tfel::math::internals::det3(m);
  }

  template <typename T>
  constexpr auto det(const tmatrix<3, 3, T>& m) {
    return tfel::math::internals::det3(m);
  }

  template <unsigned short I,
            unsigned short J,
            typename FunctionType,
            typename VariableType,
            unsigned short N,
            unsigned short M>
  constexpr derivative_view_from_tiny_matrix<M, FunctionType, VariableType>
  map_derivative(
      tmatrix<
          N,
          M,
          base_type<numeric_type<derivative_type<FunctionType, VariableType>>>>&
          m) {
    using Implementation =
        tfel::math::internals::DerivativeViewFromTinyMatrixImplementation<
            isScalar<FunctionType>(), isScalar<VariableType>(), FunctionType,
            VariableType>;
    return Implementation::template exe<I, J>(m);
  }  // end of map_derivative

  template <typename FunctionType,
            typename VariableType,
            unsigned short N,
            unsigned short M>
  constexpr derivative_view_from_tiny_matrix<M, FunctionType, VariableType>
  map_derivative(
      tmatrix<
          N,
          M,
          base_type<numeric_type<derivative_type<FunctionType, VariableType>>>>&
          m,
      const unsigned short i,
      const unsigned short j) {
    using Implementation =
        tfel::math::internals::DerivativeViewFromTinyMatrixImplementation<
            isScalar<FunctionType>(), isScalar<VariableType>(), FunctionType,
            VariableType>;
    return Implementation::exe(m, i, j);
  }  // end of map_derivative

}  // namespace tfel::math

#endif /* LIB_TFEL_MATH_TINY_MATRIX_IXX */
