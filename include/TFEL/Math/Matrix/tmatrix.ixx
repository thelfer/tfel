/*!
 * \file   include/TFEL/Math/Matrix/tmatrix.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   28 Jun 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINY_MATRIX_IXX
#define LIB_TFEL_MATH_TINY_MATRIX_IXX

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/General/AbsCompare.hxx"
#include "TFEL/Math/Matrix/MatrixUtilities.hxx"

namespace tfel::math {

#ifndef DOXYGENSPECIFIC

  template <typename Child, unsigned short N, unsigned short M, typename T>
  template <typename T2, typename Operation>
  std::enable_if_t<isAssignableTo<T2, T>(), Child&>
  tmatrix_base<Child, N, M, T>::operator=(
      const Expr<tmatrix<N, M, T2>, Operation>& src) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<N, M, M>::copy(src, child);
    return child;
  }

  template <typename Child, unsigned short N, unsigned short M, typename T>
  template <typename T2>
  std::enable_if_t<isAssignableTo<T2, T>(), Child&>
  tmatrix_base<Child, N, M, T>::operator=(const tmatrix<N, M, T2>& src) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<N, M, M>::copy(src, child);
    return child;
  }

  template <typename Child, unsigned short N, unsigned short M, typename T>
  template <typename T2, typename Operation>
  std::enable_if_t<isAssignableTo<T2, T>(), Child&>
  tmatrix_base<Child, N, M, T>::operator+=(
      const Expr<tmatrix<N, M, T2>, Operation>& src) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<N, M, M>::plusEqual(child, src);
    return child;
  }

  template <typename Child, unsigned short N, unsigned short M, typename T>
  template <typename T2>
  std::enable_if_t<isAssignableTo<T2, T>(), Child&>
  tmatrix_base<Child, N, M, T>::operator+=(const tmatrix<N, M, T2>& src) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<N, M, M>::plusEqual(child, src);
    return child;
  }

  template <typename Child, unsigned short N, unsigned short M, typename T>
  template <typename T2, typename Operation>
  std::enable_if_t<isAssignableTo<T2, T>(), Child&>
  tmatrix_base<Child, N, M, T>::operator-=(
      const Expr<tmatrix<N, M, T2>, Operation>& src) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<N, M, M>::minusEqual(child, src);
    return child;
  }

  template <typename Child, unsigned short N, unsigned short M, typename T>
  template <typename T2>
  std::enable_if_t<isAssignableTo<T2, T>(), Child&>
  tmatrix_base<Child, N, M, T>::operator-=(const tmatrix<N, M, T2>& src) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<N, M, M>::minusEqual(child, src);
    return child;
  }

  // *= operator
  template <typename Child, unsigned short N, unsigned short M, typename T>
  template <typename T2>
  std::enable_if_t<isScalar<T2>() &&
                       std::is_same<result_type<T, T2, OpMult>, T>::value,
                   Child&>
  tmatrix_base<Child, N, M, T>::operator*=(const T2 s) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<N, M, M>::multByScalar(child, s);
    return child;
  }

  // /= operator
  template <typename Child, unsigned short N, unsigned short M, typename T>
  template <typename T2>
  std::enable_if_t<isScalar<T2>() &&
                       std::is_same<result_type<T, T2, OpMult>, T>::value,
                   Child&>
  tmatrix_base<Child, N, M, T>::operator/=(const T2 s) {
    auto& child = static_cast<Child&>(*this);
    matrix_utilities<N, M, M>::multByScalar(
        child, (static_cast<base_type<T2>>(1u)) / s);
    return child;
  }

  template <unsigned short N, unsigned short M, typename T>
  constexpr unsigned short tmatrix<N, M, T>::getNbCols() const {
    return M;
  }

  template <unsigned short N, unsigned short M, typename T>
  constexpr unsigned short tmatrix<N, M, T>::getNbRows() const {
    return N;
  }

  template <unsigned short N, unsigned short M, typename T>
  T tmatrix<N, M, T>::max() const {
    return *tfel::fsalgo::max_element<this->size()>::exe(this->v);
  }

  template <unsigned short N, unsigned short M, typename T>
  T tmatrix<N, M, T>::abs_max() const {
    return std::abs(*tfel::fsalgo::max_element<this->size()>::exe(
        this->v, absCompare<T>()));
  }

  template <unsigned short N, unsigned short M, typename T>
  void tmatrix<N, M, T>::swap_rows(const unsigned short i,
                                   const unsigned short j) {
    tfel::fsalgo::swap_ranges<M>::exe(this->v + M * i, this->v + M * j);
  }

  template <unsigned short N, unsigned short M, typename T>
  tmatrix<N, N, T> tmatrix<N, M, T>::Id() {
    static_assert(N == M);
    tmatrix<N, N, T> m2(static_cast<T>(0.));
    unsigned short i;
    for (i = 0; i < N; ++i) {
      m2(i, i) = static_cast<T>(1.);
    }
    return m2;
  }

  template <unsigned short N, unsigned short M, typename T>
  template <typename InputIterator>
  void tmatrix<N, M, T>::copy(const InputIterator src) {
    tfel::fsalgo::copy<this->size()>::exe(src, this->v);
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I>
  constexpr auto tmatrix<N, M, T>::row_view() {
    static_assert(I < N, "invalid row index");
    return map_array<tvector<M, T>>(&(this->operator()(I, 0)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I>
  constexpr auto tmatrix<N, M, T>::row_view() const {
    static_assert(I < N, "invalid row index");
    return map_array<const tvector<M, T>>(&(this->operator()(I, 0)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I, unsigned short J, unsigned short K>
  constexpr auto tmatrix<N, M, T>::row_view() {
    static_assert(I < N, "invalid row index");
    static_assert(J < M, "invalid column index");
    static_assert(M >= J + K, "invalid view size");
    return map_array<tvector<K, T>>(&(this->operator()(I, J)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I, unsigned short J, unsigned short K>
  constexpr auto tmatrix<N, M, T>::row_view() const {
    static_assert(I < N, "invalid row index");
    static_assert(J < M, "invalid column index");
    static_assert(M >= J + K, "invalid view size");
    return map_array<const tvector<K, T>>(&(this->operator()(I, J)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I>
  constexpr auto tmatrix<N, M, T>::column_view() {
    static_assert(I < M, "invalid column index");
    return map_array<tvector<N, T>, 0, M>(&(this->operator()(0, I)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I>
  constexpr auto tmatrix<N, M, T>::column_view() const {
    static_assert(I < M, "invalid column index");
    return map_array<const tvector<N, T>, 0, M>(&(this->operator()(0, I)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I, unsigned short J, unsigned short K>
  constexpr auto tmatrix<N, M, T>::column_view() {
    static_assert(I < M, "invalid column index");
    static_assert(J < N, "invalid row index");
    static_assert(N >= J + K, "invalid view size");
    return map_array<tvector<K, T>, 0, M>(&(this->operator()(J, I)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I, unsigned short J, unsigned short K>
  constexpr auto tmatrix<N, M, T>::column_view() const {
    static_assert(I < M, "invalid column index");
    static_assert(J < N, "invalid row index");
    static_assert(N >= J + K, "invalid view size");
    return map_array<const tvector<K, T>, 0, M>(&(this->operator()(J, I)));
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I,
            unsigned short J,
            unsigned short R,
            unsigned short C>
  tmatrix_submatrix_view<N, M, I, J, R, C, T>
  tmatrix<N, M, T>::submatrix_view() {
    return tmatrix_submatrix_view<N, M, I, J, R, C, T>(*this);
  }

  template <unsigned short N, unsigned short M, typename T>
  template <unsigned short I,
            unsigned short J,
            unsigned short R,
            unsigned short C>
  tmatrix_const_submatrix_view<N, M, I, J, R, C, T>
  tmatrix<N, M, T>::submatrix_view() const {
    return tmatrix_const_submatrix_view<N, M, I, J, R, C, T>(*this);
  }

  // template<unsigned short N,unsigned short M,typename T>
  template <unsigned short N, unsigned short M, typename T>
  TFEL_MATH_INLINE2 tmatrix<M, N, T> transpose(const tmatrix<N, M, T>& m) {
    tmatrix<M, N, T> tm;
    unsigned short i, j;
    for (i = 0; i < N; ++i) {
      for (j = 0; j < M; ++j) {
        tm(j, i) = m(i, j);
      }
    }
    return tm;
  }

  namespace internals {

    template <typename Matrix>
    TFEL_MATH_INLINE std::enable_if_t<
        implementsMatrixConcept<Matrix>(),
        typename ComputeUnaryResult<numeric_type<Matrix>, Power<3>>::Result>
    det2(const Matrix& m) {
      return m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1);
    }

    template <typename Matrix>
    TFEL_MATH_INLINE std::enable_if_t<
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

  }  // namespace internals

  template <typename T, typename Operation>
  typename ComputeUnaryResult<T, Power<3>>::Result det(
      const Expr<tmatrix<2, 2, T>, Operation>& m) {
    return tfel::math::internals::det2(m);
  }

  template <typename T>
  typename ComputeUnaryResult<T, Power<3>>::Result det(
      const tmatrix<2, 2, T>& m) {
    return tfel::math::internals::det2(m);
  }

  template <typename T, typename Operation>
  typename ComputeUnaryResult<T, Power<3>>::Result det(
      const Expr<tmatrix<3, 3, T>, Operation>& m) {
    return tfel::math::internals::det3(m);
  }

  template <typename T>
  typename ComputeUnaryResult<T, Power<3>>::Result det(
      const tmatrix<3, 3, T>& m) {
    return tfel::math::internals::det3(m);
  }

  template <unsigned short N, unsigned short M, typename T>
  tvector<M, T> getRow(const tmatrix<N, M, T>& m, const unsigned short i) {
    using size_type = typename tfel::fsalgo::loop<M>::size_type;
    tvector<M, T> r;
    tfel::fsalgo::loop<M>::exe(
        [&r, &m, i](const size_type j) { r(j) = m(i, j); });
    return r;
  }

  template <unsigned short N, unsigned short M, typename T>
  tvector<N, T> getColumn(const tmatrix<N, M, T>& m, const unsigned short);

#endif /* DOXYGENSPECIFIC */

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINY_MATRIX_IXX */
