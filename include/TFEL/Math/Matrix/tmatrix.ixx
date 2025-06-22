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

#ifndef LIB_TFEL_TINY_MATRIXIXX
#define LIB_TFEL_TINY_MATRIXIXX

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"

#include "TFEL/Math/General/AbsCompare.hxx"
#include "TFEL/Math/Matrix/MatrixUtilities.hxx"

namespace tfel {

  namespace math {

#ifndef DOXYGENSPECIFIC

    template <unsigned short N, unsigned short M, typename T>
    constexpr tmatrix<N, M, T>::tmatrix() {}

    template <unsigned short N, unsigned short M, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr tmatrix<N, M, T>::tmatrix(const T2& init)
        : fsarray<N * M, T>(init) {}

    template <unsigned short N, unsigned short M, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr tmatrix<N, M, T>::tmatrix(const std::initializer_list<T2>& init)
        : fsarray<N * M, T>(init) {}

    template <unsigned short N, unsigned short M, typename T>
    template <
        typename T2,
        typename Operation,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    tmatrix<N, M, T>::tmatrix(const Expr<tmatrix<N, M, T2>, Operation>& src) {
      matrix_utilities<N, M, M>::copy(src, *this);
    }

    template <unsigned short N, unsigned short M, typename T>
    constexpr const T& tmatrix<N, M, T>::operator()(
        const unsigned short i, const unsigned short j) const {
      return this->v[i * M + j];
    }

    template <unsigned short N, unsigned short M, typename T>
    T& tmatrix<N, M, T>::operator()(const unsigned short i,
                                    const unsigned short j) {
      return this->v[i * M + j];
    }

    template <typename Child, unsigned short N, unsigned short M, typename T>
    template <typename T2, typename Operation>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tmatrix_base<Child, N, M, T>::operator=(
        const Expr<tmatrix<N, M, T2>, Operation>& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<N, M, M>::copy(src, child);
      return child;
    }

    template <typename Child, unsigned short N, unsigned short M, typename T>
    template <typename T2>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tmatrix_base<Child, N, M, T>::operator=(const tmatrix<N, M, T2>& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<N, M, M>::copy(src, child);
      return child;
    }

    template <typename Child, unsigned short N, unsigned short M, typename T>
    template <typename T2, typename Operation>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tmatrix_base<Child, N, M, T>::operator+=(
        const Expr<tmatrix<N, M, T2>, Operation>& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<N, M, M>::plusEqual(child, src);
      return child;
    }

    template <typename Child, unsigned short N, unsigned short M, typename T>
    template <typename T2>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tmatrix_base<Child, N, M, T>::operator+=(const tmatrix<N, M, T2>& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<N, M, M>::plusEqual(child, src);
      return child;
    }

    template <typename Child, unsigned short N, unsigned short M, typename T>
    template <typename T2, typename Operation>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tmatrix_base<Child, N, M, T>::operator-=(
        const Expr<tmatrix<N, M, T2>, Operation>& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<N, M, M>::minusEqual(child, src);
      return child;
    }

    template <typename Child, unsigned short N, unsigned short M, typename T>
    template <typename T2>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tmatrix_base<Child, N, M, T>::operator-=(const tmatrix<N, M, T2>& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<N, M, M>::minusEqual(child, src);
      return child;
    }

    // *= operator
    template <typename Child, unsigned short N, unsigned short M, typename T>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<typename ResultType<T, T2, OpMult>::type, T>::value,
        Child&>::type
    tmatrix_base<Child, N, M, T>::operator*=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<N, M, M>::multByScalar(child, s);
      return child;
    }

    // /= operator
    template <typename Child, unsigned short N, unsigned short M, typename T>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<typename ResultType<T, T2, OpMult>::type, T>::value,
        Child&>::type
    tmatrix_base<Child, N, M, T>::operator/=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<N, M, M>::multByScalar(
          child,
          (static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u)) / s);
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
      return *tfel::fsalgo::max_element<N * M>::exe(this->v);
    }

    template <unsigned short N, unsigned short M, typename T>
    T tmatrix<N, M, T>::abs_max() const {
      return std::abs(
          *tfel::fsalgo::max_element<N * M>::exe(this->v, absCompare<T>()));
    }

    template <unsigned short N, unsigned short M, typename T>
    void tmatrix<N, M, T>::swap_rows(const unsigned short i,
                                     const unsigned short j) {
      tfel::fsalgo::swap_ranges<M>::exe(this->v + M * i, this->v + M * j);
    }

    template <unsigned short N, unsigned short M, typename T>
    tmatrix<N, N, T> tmatrix<N, M, T>::Id() {
      TFEL_STATIC_ASSERT(N == M);
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
      tfel::fsalgo::copy<N * M>::exe(src, this->v);
    }

    template <unsigned short N, unsigned short M, typename T>
    template <unsigned short I>
    tmatrix_row_view<N, M, I, 0, M, T> tmatrix<N, M, T>::row_view() {
      return tmatrix_row_view<N, M, I, 0, M, T>(*this);
    }

    template <unsigned short N, unsigned short M, typename T>
    template <unsigned short I, unsigned short J, unsigned short K>
    tmatrix_row_view<N, M, I, J, K, T> tmatrix<N, M, T>::row_view() {
      return tmatrix_row_view<N, M, I, J, K, T>(*this);
    }

    template <unsigned short N, unsigned short M, typename T>
    template <unsigned short I>
    tmatrix_const_row_view<N, M, I, 0, M, T> tmatrix<N, M, T>::row_view()
        const {
      return tmatrix_const_row_view<N, M, I, 0, M, T>(*this);
    }

    template <unsigned short N, unsigned short M, typename T>
    template <unsigned short I, unsigned short J, unsigned short K>
    tmatrix_const_row_view<N, M, I, J, K, T> tmatrix<N, M, T>::row_view()
        const {
      return tmatrix_const_row_view<N, M, I, J, K, T>(*this);
    }

    template <unsigned short N, unsigned short M, typename T>
    template <unsigned short I>
    tmatrix_column_view<N, M, I, 0, N, T> tmatrix<N, M, T>::column_view() {
      return tmatrix_column_view<N, M, I, 0, N, T>(*this);
    }

    template <unsigned short N, unsigned short M, typename T>
    template <unsigned short I, unsigned short J, unsigned short K>
    tmatrix_column_view<N, M, I, J, K, T> tmatrix<N, M, T>::column_view() {
      return tmatrix_column_view<N, M, I, J, K, T>(*this);
    }

    template <unsigned short N, unsigned short M, typename T>
    template <unsigned short I>
    tmatrix_const_column_view<N, M, I, 0, N, T> tmatrix<N, M, T>::column_view()
        const {
      return tmatrix_const_column_view<N, M, I, 0, N, T>(*this);
    }

    template <unsigned short N, unsigned short M, typename T>
    template <unsigned short I, unsigned short J, unsigned short K>
    tmatrix_const_column_view<N, M, I, J, K, T> tmatrix<N, M, T>::column_view()
        const {
      return tmatrix_const_column_view<N, M, I, J, K, T>(*this);
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
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<Matrix, MatrixConcept>::cond,
          typename ComputeUnaryResult<typename MatrixTraits<Matrix>::NumType,
                                      Power<3>>::Result>::type
      det2(const Matrix& m) {
        return m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1);
      }

      template <typename Matrix>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<Matrix, MatrixConcept>::cond,
          typename ComputeUnaryResult<typename MatrixTraits<Matrix>::NumType,
                                      Power<3>>::Result>::type
      det3(const Matrix& m) {
        typedef typename MatrixTraits<Matrix>::NumType T;
        const T a = m(0, 0);
        const T b = m(0, 1);
        const T c = m(0, 2);
        const T d = m(1, 0);
        const T e = m(1, 1);
        const T f = m(1, 2);
        const T g = m(2, 0);
        const T h = m(2, 1);
        const T i = m(2, 2);
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

#endif /* LIB_TFEL_TINY_MATRIXIXX */

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_TINY_MATRIXIXX */
