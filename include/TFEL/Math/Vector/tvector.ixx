/*!
 * \file   include/TFEL/Math/Vector/tvector.ixx
 * \brief
 *
 * \author Helfer Thomas
 * \date   28 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TINY_VECTOR_IXX_
#define LIB_TFEL_TINY_VECTOR_IXX_

#include <iterator>

#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/DotProduct.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"

namespace tfel {

  namespace math {

#ifndef DOXYGENSPECIFIC

    template <typename Child, unsigned short N, typename T>
    template <typename T2, typename Operation>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tvector_base<Child, N, T>::operator=(
        const Expr<tvector<N, T2>, Operation>& src) {
      vectorToTab<N>::exe(src, static_cast<Child&>(*this));
      return static_cast<Child&>(*this);
    }

    template <typename Child, unsigned short N, typename T>
    template <typename T2>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tvector_base<Child, N, T>::operator=(const tvector<N, T2>& src) {
      vectorToTab<N>::exe(src, static_cast<Child&>(*this));
      return static_cast<Child&>(*this);
    }

    template <typename Child, unsigned short N, typename T>
    template <typename T2, typename Operation>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tvector_base<Child, N, T>::operator+=(
        const Expr<tvector<N, T2>, Operation>& src) {
      VectorUtilities<N>::PlusEqual(static_cast<Child&>(*this), src);
      return static_cast<Child&>(*this);
    }

    template <typename Child, unsigned short N, typename T>
    template <typename T2, typename Operation>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tvector_base<Child, N, T>::operator-=(
        const Expr<tvector<N, T2>, Operation>& src) {
      VectorUtilities<N>::MinusEqual(static_cast<Child&>(*this), src);
      return static_cast<Child&>(*this);
    }

    template <typename Child, unsigned short N, typename T>
    template <typename T2>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tvector_base<Child, N, T>::operator+=(const tvector<N, T2>& src) {
      VectorUtilities<N>::PlusEqual(static_cast<Child&>(*this), src);
      return static_cast<Child&>(*this);
    }

    template <typename Child, unsigned short N, typename T>
    template <typename T2>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            Child&>::type
    tvector_base<Child, N, T>::operator-=(const tvector<N, T2>& src) {
      VectorUtilities<N>::MinusEqual(static_cast<Child&>(*this), src);
      return static_cast<Child&>(*this);
    }

    // *= operator
    template <typename Child, unsigned short N, typename T>
    template <typename T2>
    typename std::enable_if<IsTVectorScalarOperationValid<T, T2, OpMult>::cond,
                            Child&>::type
    tvector_base<Child, N, T>::operator*=(const T2 s) {
      VectorUtilities<N>::scale(static_cast<Child&>(*this), s);
      return static_cast<Child&>(*this);
    }

    // /= operator
    template <typename Child, unsigned short N, typename T>
    template <typename T2>
    typename std::enable_if<IsTVectorScalarOperationValid<T, T2, OpDiv>::cond,
                            Child&>::type
    tvector_base<Child, N, T>::operator/=(const T2 s) {
      VectorUtilities<N>::scale(
          static_cast<Child&>(*this),
          (static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u)) / s);
      return static_cast<Child&>(*this);
    }

    template <unsigned short N, typename T>
    constexpr tvector<N, T>::tvector() {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr tvector<N, T>::tvector(const T2& init) : fsarray<N, T>(init) {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr tvector<N, T>::tvector(const std::initializer_list<T2>& init)
        : fsarray<N, T>(init) {}

    template <unsigned short N, typename T>
    tvector<N, T>::tvector(const T* const init) {
      tfel::fsalgo::copy<N>::exe(init, this->v);
    }

    template <unsigned short N, typename T>
    template <typename T2, typename Operation>
    tvector<N, T>::tvector(const Expr<tvector<N, T2>, Operation>& src) {
      TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<T2, T>::cond));
      vectorToTab<N>::exe(src, this->v);
    }  // end of tvector<N,T>::tvector(const Expr<tvector<N,T2>,Operation>&)

    template <unsigned short N, typename T>
    constexpr const T& tvector<N, T>::operator()(
        const unsigned short i) const noexcept {
      return this->v[i];
    }

    template <unsigned short N, typename T>
    T& tvector<N, T>::operator()(const unsigned short i) noexcept {
      return this->v[i];
    }

    template <unsigned short N, typename T>
    constexpr typename tvector<N, T>::RunTimeProperties
    tvector<N, T>::getRunTimeProperties(void) const noexcept {
      return RunTimeProperties();
    }

    template <unsigned short N, typename T>
    template <typename InputIterator>
    void tvector<N, T>::copy(const InputIterator src) {
      tfel::fsalgo::copy<N>::exe(src, this->v);
    }

    template <unsigned short N, typename T>
    template <unsigned short I>
    Expr<tvector<N - I, T>,
         TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, false>>
    tvector<N, T>::slice(void) {
      return Expr<tvector<N - I, T>,
                  TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, false>>(
          *this);
    }  // end of tvector<N,T>::slice(void)

    template <unsigned short N, typename T>
    template <unsigned short I, unsigned short J>
    Expr<tvector<J - I, T>,
         TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, false>>
    tvector<N, T>::slice(void) {
      return Expr<tvector<J - I, T>,
                  TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, false>>(
          *this);
    }

    template <unsigned short N, typename T>
    template <unsigned short I>
    Expr<tvector<N - I, T>,
         TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, true>>
    tvector<N, T>::slice(void) const {
      return Expr<tvector<N - I, T>,
                  TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, true>>(
          *this);
    }  // end of tvector<N,T>::slice

    template <unsigned short N, typename T>
    template <unsigned short I, unsigned short J>
    Expr<tvector<J - I, T>,
         TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, true>>
    tvector<N, T>::slice(void) const {
      return Expr<tvector<J - I, T>,
                  TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, true>>(
          *this);
    }  // end of tvector<N,T>::slice

    template <unsigned short N, typename T, typename OutputIterator>
    typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, void>::type
    exportToBaseTypeArray(const tvector<N, T>& v, OutputIterator p) {
      typedef tfel::fsalgo::copy<N> Copy;
      typedef typename tfel::typetraits::BaseType<T>::type base;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      Copy::exe(reinterpret_cast<const base*>(&v[0]), p);
    }  // end of exportToBaseTypePointer

    // Norm2
    template <unsigned short N, typename T>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T>::cond,
        typename tfel::typetraits::RealPartType<T>::type>::type
    norm(const tvector<N, T>& vec) {
      typedef typename ResultType<T, T, OpMult>::type squareT;
      return std::sqrt(
          real(dotProduct<N>::exe(vec.begin(), vec.begin(), squareT(0u))));
    }

    template <unsigned short N, typename T>
    typename tfel::typetraits::AbsType<T>::type abs(const tvector<N, T>& v) {
      using namespace tfel::fsalgo;
      AbsSum<T> a;
      for_each<N>::exe(v.begin(), a);
      return a.result;
    }

#endif /* LIB_TFEL_TINY_VECTOR_IXX_ */

    template <typename T>
    tvector<1u, T> makeTVector1D(const T v) {
      return tvector<1u, T>(v);
    }  // end of makeTVector1D

    template <typename T>
    tvector<2u, T> makeTVector2D(const T v1, const T v2) {
      tvector<2u, T> r;
      r[0] = v1;
      r[1] = v2;
      return r;
    }  // end of makeTVector2D

    template <typename T>
    tvector<3u, T> makeTVector3D(const T v1, const T v2, const T v3) {
      tvector<3u, T> r;
      r[0] = v1;
      r[1] = v2;
      r[2] = v3;
      return r;
    }  // end of makeTVector3D

    template <typename T>
    tvector<3u, T> cross_product(const tvector<2u, T>& v1,
                                 const tvector<2u, T>& v2) {
      tvector<3u, T> v3;
      v3[0] = T(0);
      v3[1] = T(0);
      v3[2] = v1[0] * v2[1] - v1[1] * v2[0];
      return v3;
    }  // end of cross_product

    template <typename T>
    tvector<3u, T> cross_product(const tvector<3u, T>& v1,
                                 const tvector<3u, T>& v2) {
      tvector<3u, T> v3;
      v3[0] = v1[1] * v2[2] - v1[2] * v2[1];
      v3[1] = v1[2] * v2[0] - v1[0] * v2[2];
      v3[2] = v1[0] * v2[1] - v1[1] * v2[0];
      return v3;
    }  // end of cross_product

    template <typename T>
    void find_perpendicular_vector(tvector<3u, T>& y, const tvector<3u, T>& x) {
      using namespace std;
      using namespace tfel::math;
      typedef typename tfel::typetraits::BaseType<T>::type real;
      typedef typename ComputeBinaryResult<real, T, OpDiv>::Result inv_T;
      T norm2_x = (x | x);
      if (norm2_x < 100 * std::numeric_limits<T>::min()) {
        // x is null
        y(0) = static_cast<T>(1.);
        y(1) = static_cast<T>(0.);
        y(2) = static_cast<T>(0.);
        return;
      }
      inv_T inv_norm2_x = real(1) / norm2_x;
      if (abs(x(0)) < abs(x(1))) {
        if (abs(x(0)) < abs(x(2))) {
          //|x0| is min, (1 0 0) is a good choice
          y(0) = real(1.) - x(0) * x(0) * inv_norm2_x;
          y(1) = -x(0) * x(1) * inv_norm2_x;
          y(2) = -x(0) * x(2) * inv_norm2_x;
        } else {
          //|x2| is min, (0 0 1) is a good choice
          y(0) = -x(2) * x(0) * inv_norm2_x;
          y(1) = -x(2) * x(1) * inv_norm2_x;
          y(2) = real(1) - x(2) * x(2) * inv_norm2_x;
        }
      } else if (abs(x(1)) < abs(x(2))) {
        // |x1| is min, (0 0 1) is a good choice
        y(0) = -x(1) * x(0) * inv_norm2_x;
        y(1) = real(1) - x(1) * x(1) * inv_norm2_x;
        y(2) = -x(1) * x(2) * inv_norm2_x;
      } else {
        // |x2| is min, (0 0 1) is a good choice
        y(0) = -x(2) * x(0) * inv_norm2_x;
        y(1) = -x(2) * x(1) * inv_norm2_x;
        y(2) = real(1) - x(2) * x(2) * inv_norm2_x;
      }
      const T tmp = std::sqrt(y | y);
      y(0) /= tmp;
      y(1) /= tmp;
      y(2) /= tmp;
    }

    template <unsigned short I, unsigned short N, typename T>
    Expr<tvector<N - I, T>,
         TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, false>>
    slice(tvector<N, T>& v) {
      return v.template slice<I>();
    }  // end of slice

    template <unsigned short I, unsigned short J, unsigned short N, typename T>
    Expr<tvector<J - I, T>,
         TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, false>>
    slice(tvector<N, T>& v) {
      return v.template slice<I, J>();
    }  // end of slice

    template <unsigned short I, unsigned short N, typename T>
    Expr<tvector<N - I, T>,
         TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, true>>
    slice(const tvector<N, T>& v) {
      return v.template slice<I>();
    }  // end of slice

    template <unsigned short I, unsigned short J, unsigned short N, typename T>
    Expr<tvector<J - I, T>,
         TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, true>>
    slice(const tvector<N, T>& v) {
      return v.template slice<I, J>();
    }  // end of slice

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_TINY_VECTOR_IXX_ */
