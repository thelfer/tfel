/*!
 * \file   include/TFEL/Math/Vector/tvector.ixx
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

#ifndef LIB_TFEL_TINY_VECTORIXX
#define LIB_TFEL_TINY_VECTORIXX

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
      constexpr const auto one = tfel::typetraits::base_type<T2>(1);
      VectorUtilities<N>::scale(static_cast<Child&>(*this), one / s);
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
    tvector<N, T>::getRunTimeProperties() const noexcept {
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
    tvector<N, T>::slice() {
      return Expr<tvector<N - I, T>,
                  TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, false>>(
          *this);
    }  // end of tvector<N,T>::slice()

    template <unsigned short N, typename T>
    template <unsigned short I, unsigned short J>
    Expr<tvector<J - I, T>,
         TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, false>>
    tvector<N, T>::slice() {
      return Expr<tvector<J - I, T>,
                  TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, false>>(
          *this);
    }

    template <unsigned short N, typename T>
    template <unsigned short I>
    Expr<tvector<N - I, T>,
         TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, true>>
    tvector<N, T>::slice() const {
      return Expr<tvector<N - I, T>,
                  TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, true>>(
          *this);
    }  // end of tvector<N,T>::slice

    template <unsigned short N, typename T>
    template <unsigned short I, unsigned short J>
    Expr<tvector<J - I, T>,
         TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, true>>
    tvector<N, T>::slice() const {
      return Expr<tvector<J - I, T>,
                  TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, true>>(
          *this);
    }  // end of tvector<N,T>::slice

    template <unsigned short N, typename T, typename OutputIterator>
    typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, void>::type
    exportToBaseTypeArray(const tvector<N, T>& v, OutputIterator p) {
      typedef tfel::fsalgo::copy<N> Copy;
      typedef tfel::typetraits::base_type<T> base;
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
      AbsSum<T> a;
      tfel::fsalgo::for_each<N>::exe(v.begin(), a);
      return a.result;
    }

#endif /* LIB_TFEL_TINY_VECTORIXX */

    template <typename T>
    tvector<1u, T> makeTVector1D(const T v) {
      return {v};
    }  // end of makeTVector1D

    template <typename T>
    tvector<2u, T> makeTVector2D(const T v1, const T v2) {
      return {v1, v2};
    }  // end of makeTVector2D

    template <typename T>
    tvector<3u, T> makeTVector3D(const T v1, const T v2, const T v3) {
      return {v1, v2, v3};
    }  // end of makeTVector3D

    /*!
     * \brief create a new tvector by applying a functor
     * \param[in] f: functor
     * \param[in] x: inital value
     */
    template <typename F, typename T, unsigned short N>
    tvector<N, typename std::result_of<F(T)>::type> map(
        F f, const tvector<N, T>& x) {
      tvector<N, typename std::result_of<F(T)>::type> r;
      tfel::fsalgo::transform<N>::exe(x.begin(), r.begin(), f);
      return r;
    }  // end of map

    template <typename T>
    tvector<3u, T> cross_product(const tvector<2u, T>& v1,
                                 const tvector<2u, T>& v2) {
      constexpr const auto zero = T(0);
      return {zero, zero, v1[0] * v2[1] - v1[1] * v2[0]};
    }  // end of cross_product

    template <typename T>
    tvector<3u, T> cross_product(const tvector<3u, T>& v1,
                                 const tvector<3u, T>& v2) {
      return {v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2],
              v1[0] * v2[1] - v1[1] * v2[0]};
    }  // end of cross_product

    template <typename T>
    void find_perpendicular_vector(tvector<3u, T>& y, const tvector<3u, T>& x) {
      using real = tfel::typetraits::base_type<T>;
      constexpr const auto zero = T(0);
      constexpr const auto one = T(1);
      const auto nx = (x | x);
      if (nx < 100 * std::numeric_limits<decltype(nx)>::min()) {
        // x is null
        y = {one, zero, zero};
        return;
      }
      const auto inx = real(1) / nx;
      if (std::abs(x(0)) < std::abs(x(1))) {
        if (std::abs(x(0)) < std::abs(x(2))) {
          //|x0| is min, (1 0 0) is a good choice
          y(0) = one - x(0) * x(0) * inx;
          y(1) = -x(0) * x(1) * inx;
          y(2) = -x(0) * x(2) * inx;
        } else {
          //|x2| is min, (0 0 1) is a good choice
          y(0) = -x(2) * x(0) * inx;
          y(1) = -x(2) * x(1) * inx;
          y(2) = one - x(2) * x(2) * inx;
        }
      } else if (std::abs(x(1)) < std::abs(x(2))) {
        // |x1| is min, (0 0 1) is a good choice
        y(0) = -x(1) * x(0) * inx;
        y(1) = one - x(1) * x(1) * inx;
        y(2) = -x(1) * x(2) * inx;
      } else {
        // |x2| is min, (0 0 1) is a good choice
        y(0) = -x(2) * x(0) * inx;
        y(1) = -x(2) * x(1) * inx;
        y(2) = one - x(2) * x(2) * inx;
      }
      y /= std::sqrt(y | y);
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

#endif /* LIB_TFEL_TINY_VECTORIXX */
