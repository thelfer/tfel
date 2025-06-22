/*!
 * \file   include/TFEL/Math/T2toT2/t2tot2.ixx
 * \brief  This file implements the methods of the class t2tot2.
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOT2IXX
#define LIB_TFEL_MATH_T2TOT2IXX

#include <cmath>
#include <iterator>
#include <algorithm>
#include <type_traits>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Matrix/MatrixUtilities.hxx"
#include "TFEL/Math/Matrix/MatrixUtilities.hxx"
#include "TFEL/Math/T2toT2/TensorProductLeftDerivativeExpr.hxx"
#include "TFEL/Math/T2toT2/TensorProductRightDerivativeExpr.hxx"
#include "TFEL/Math/T2toT2/BuildFromRotationMatrix.hxx"
#include "TFEL/Math/T2toT2/ChangeBasis.hxx"

#if defined(__GNUG__) && (not defined(__CLANG__)) && \
    (not defined(__INTEL_COMPILER))
#if ((__GNUC__ > 4) ||                            \
     ((__GNUC__ == 4) && ((__GNUC_MINOR__ > 9) || \
                          (__GNUC_MINOR__ == 9 && __GNUC_PATCHLEVEL__ > 0))))
#define TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
#else
// don't define TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
#endif
#elif defined(_MSC_VER)
// don't define TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
#else
#define TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
#endif

namespace tfel {

  namespace math {

    namespace internals {

      /*!
       * \brief an helper class used to compute special t2tot2 values
       * \tparam N: space dimension
       * \tparam T: numerical type
       */
      template <unsigned short N, typename T>
      struct ComputeSpecialT2toT2Values;
      /*!
       * \brief Partial specialisation of the
       * ComputeSpecialT2toT2Values class in 1D.
       * \tparam T: numerical type
       */
      template <typename T>
      struct TFEL_VISIBILITY_LOCAL ComputeSpecialT2toT2Values<1u, T> {
#ifdef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
        static TFEL_CONSTEXPR const T zero = T{0};
        static TFEL_CONSTEXPR const T one = T{1};
        static TFEL_CONSTEXPR const T one_third = T{1} / T{3};
        static TFEL_CONSTEXPR const T two_third = T{2} / T{3};
#endif
        /*!
         * \return the derivative of the transpose of a tensor with respect of
         * this tensor
         */
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<1u, T>
        transpose_derivative() {
          return ComputeSpecialT2toT2Values<1u, T>::Id();
        }
        //! \return the identity
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<1u, T> Id() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto zero = T{0};
          const auto one = T{1};
#endif
          return {one, zero, zero, zero, one, zero, zero, zero, one};
        }
        //! \return Id2^Id2, where Id2 is the identity tensor
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<1u, T> IxI() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto one = T{1};
#endif
          return {one, one, one, one, one, one, one, one, one};
        }
        /*!
         * \return Id4-Id2^Id2/3, where Id4 is the identity of t2tot2 and
         * Id2 is the identity tensor
         */
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<1u, T> K() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto one_third = T{1} / T{3};
          const auto two_third = T{2} / T{3};
#endif
          return {two_third,  -one_third, -one_third, -one_third, two_third,
                  -one_third, -one_third, -one_third, two_third};
        }
      };
      /*!
       * \brief Partial specialisation of the
       * ComputeSpecialT2toT2Values class in 2D.
       * \tparam T: numerical type
       */
      template <typename T>
      struct TFEL_VISIBILITY_LOCAL ComputeSpecialT2toT2Values<2u, T> {
#ifdef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
        static TFEL_CONSTEXPR const T zero = T{0};
        static TFEL_CONSTEXPR const T one = T{1};
        static TFEL_CONSTEXPR const T one_third = T{1} / T{3};
        static TFEL_CONSTEXPR const T two_third = T{2} / T{3};
#endif
        /*!
         * \return the derivative of the transpose of a tensor with respect of
         * this tensor
         */
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<2u, T>
        transpose_derivative() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto zero = T{0};
          const auto one = T{1};
#endif
          return {one,  zero, zero, zero, zero, zero, one,  zero, zero,
                  zero, zero, zero, one,  zero, zero, zero, zero, zero,
                  zero, one,  zero, zero, zero, one,  zero};
        }
        //! \return the identity
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<2u, T> Id() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto zero = T{0};
          const auto one = T{1};
#endif
          return {one,  zero, zero, zero, zero, zero, one,  zero, zero,
                  zero, zero, zero, one,  zero, zero, zero, zero, zero,
                  one,  zero, zero, zero, zero, zero, one};
        }
        //! \return Id2^Id2, where Id2 is the identity tensor
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<2u, T> IxI() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto zero = T{0};
          const auto one = T{1};
#endif
          return {one,  one,  one,  zero, zero, one,  one,  one,  zero,
                  zero, one,  one,  one,  zero, zero, zero, zero, zero,
                  zero, zero, zero, zero, zero, zero, zero};
        }
        /*!
         * \return Id4-Id2^Id2/3, where Id4 is the identity of t2tot2 and
         * Id2 is the identity tensor
         */
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<2u, T> K() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto zero = T{0};
          const auto one = T{1};
          const auto one_third = T{1} / T{3};
          const auto two_third = T{2} / T{3};
#endif
          return {two_third,  -one_third, -one_third, zero, zero,
                  -one_third, two_third,  -one_third, zero, zero,
                  -one_third, -one_third, two_third,  zero, zero,
                  zero,       zero,       zero,       one,  zero,
                  zero,       zero,       zero,       zero, one};
        }
      };
      /*!
       * \brief Partial specialisation of the
       * ComputeSpecialT2toT2Values class in 3D.
       * \tparam T: numerical type
       */
      template <typename T>
      struct TFEL_VISIBILITY_LOCAL ComputeSpecialT2toT2Values<3u, T> {
#ifdef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
        static TFEL_CONSTEXPR const T zero = T{0};
        static TFEL_CONSTEXPR const T one = T{1};
        static TFEL_CONSTEXPR const T one_third = T{1} / T{3};
        static TFEL_CONSTEXPR const T two_third = T{2} / T{3};
#endif
        /*!
         * \return the derivative of the transpose of a tensor with respect of
         * this tensor
         */
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<3u, T>
        transpose_derivative() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto zero = T{0};
          const auto one = T{1};
#endif
          return {one,  zero, zero, zero, zero, zero, zero, zero, zero,
                  zero, one,  zero, zero, zero, zero, zero, zero, zero,
                  zero, zero, one,  zero, zero, zero, zero, zero, zero,
                  zero, zero, zero, zero, one,  zero, zero, zero, zero,
                  zero, zero, zero, one,  zero, zero, zero, zero, zero,
                  zero, zero, zero, zero, zero, zero, one,  zero, zero,
                  zero, zero, zero, zero, zero, one,  zero, zero, zero,
                  zero, zero, zero, zero, zero, zero, zero, zero, one,
                  zero, zero, zero, zero, zero, zero, zero, one,  zero};
        }
        //! \return the identity
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<3u, T> Id() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto zero = T{0};
          const auto one = T{1};
#endif
          return {one,  zero, zero, zero, zero, zero, zero, zero, zero,
                  zero, one,  zero, zero, zero, zero, zero, zero, zero,
                  zero, zero, one,  zero, zero, zero, zero, zero, zero,
                  zero, zero, zero, one,  zero, zero, zero, zero, zero,
                  zero, zero, zero, zero, one,  zero, zero, zero, zero,
                  zero, zero, zero, zero, zero, one,  zero, zero, zero,
                  zero, zero, zero, zero, zero, zero, one,  zero, zero,
                  zero, zero, zero, zero, zero, zero, zero, one,  zero,
                  zero, zero, zero, zero, zero, zero, zero, zero, one};
        }
        //! \return Id2^Id2, where Id2 is the identity tensor
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<3u, T> IxI() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto zero = T{0};
          const auto one = T{1};
#endif
          return {one,  one,  one,  zero, zero, zero, zero, zero, zero,
                  one,  one,  one,  zero, zero, zero, zero, zero, zero,
                  one,  one,  one,  zero, zero, zero, zero, zero, zero,
                  zero, zero, zero, zero, zero, zero, zero, zero, zero,
                  zero, zero, zero, zero, zero, zero, zero, zero, zero,
                  zero, zero, zero, zero, zero, zero, zero, zero, zero,
                  zero, zero, zero, zero, zero, zero, zero, zero, zero,
                  zero, zero, zero, zero, zero, zero, zero, zero, zero,
                  zero, zero, zero, zero, zero, zero, zero, zero, zero};
        }
        /*!
         * \return Id4-Id2^Id2/3, where Id4 is the identity of t2tot2 and
         * Id2 is the identity tensor
         */
        static TFEL_MATH_INLINE TFEL_CONSTEXPR tfel::math::t2tot2<3u, T> K() {
#ifndef TFEL_MATH_T2TOT2_SPECIALVALUES_CONSTEXPR_SUPPORT
          const auto zero = T{0};
          const auto one = T{1};
          const auto one_third = T{1} / T{3};
          const auto two_third = T{2} / T{3};
#endif
          return {two_third, -one_third, -one_third, zero,       zero,
                  zero,      zero,       zero,       zero,       -one_third,
                  two_third, -one_third, zero,       zero,       zero,
                  zero,      zero,       zero,       -one_third, -one_third,
                  two_third, zero,       zero,       zero,       zero,
                  zero,      zero,       zero,       zero,       zero,
                  one,       zero,       zero,       zero,       zero,
                  zero,      zero,       zero,       zero,       zero,
                  one,       zero,       zero,       zero,       zero,
                  zero,      zero,       zero,       zero,       zero,
                  one,       zero,       zero,       zero,       zero,
                  zero,      zero,       zero,       zero,       zero,
                  one,       zero,       zero,       zero,       zero,
                  zero,      zero,       zero,       zero,       zero,
                  one,       zero,       zero,       zero,       zero,
                  zero,      zero,       zero,       zero,       zero,
                  one};
        }
      };
    }  // end of namespace internals

    // Assignement operator
    template <typename Child>
    template <typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond &&
            T2toT2Traits<Child>::dime == T2toT2Traits<T2toT2Type>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename T2toT2Traits<T2toT2Type>::NumType,
                typename T2toT2Traits<Child>::NumType>::cond,
        Child&>::type
    t2tot2_base<Child>::operator=(const T2toT2Type& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<
          TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
          TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
          TensorDimeToSize<T2toT2Traits<Child>::dime>::value>::copy(src, child);
      return child;
    }

    template <typename Child>
    template <typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond &&
            T2toT2Traits<Child>::dime == T2toT2Traits<T2toT2Type>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename T2toT2Traits<T2toT2Type>::NumType,
                typename T2toT2Traits<Child>::NumType>::cond,
        Child&>::type
    t2tot2_base<Child>::operator+=(const T2toT2Type& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<
          TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
          TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
          TensorDimeToSize<T2toT2Traits<Child>::dime>::value>::plusEqual(child,
                                                                         src);
      return child;
    }

    // Assignement operator
    template <typename Child>
    template <typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond &&
            T2toT2Traits<Child>::dime == T2toT2Traits<T2toT2Type>::dime &&
            tfel::typetraits::IsAssignableTo<
                typename T2toT2Traits<T2toT2Type>::NumType,
                typename T2toT2Traits<Child>::NumType>::cond,
        Child&>::type
    t2tot2_base<Child>::operator-=(const T2toT2Type& src) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<
          TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
          TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
          TensorDimeToSize<T2toT2Traits<Child>::dime>::value>::minusEqual(child,
                                                                          src);
      return child;
    }

    // *= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<typename T2toT2Traits<Child>::NumType,
                                    T2,
                                    OpMult>::type,
                typename T2toT2Traits<Child>::NumType>::value,
        Child&>::type
    t2tot2_base<Child>::operator*=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
                       TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
                       TensorDimeToSize<T2toT2Traits<Child>::dime>::value>::
          multByScalar(child, s);
      return child;
    }

    // /= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<typename T2toT2Traits<Child>::NumType,
                                    T2,
                                    OpDiv>::type,
                typename T2toT2Traits<Child>::NumType>::value,
        Child&>::type
    t2tot2_base<Child>::operator/=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
                       TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
                       TensorDimeToSize<T2toT2Traits<Child>::dime>::value>::
          divByScalar(child, s);
      return child;
    }

    template <unsigned short N, typename T>
    t2tot2<N, typename tfel::typetraits::BaseType<T>::type>
    t2tot2<N, T>::fromRotationMatrix(
        const tmatrix<3u, 3u, typename tfel::typetraits::BaseType<T>::type>&
            r) {
      return t2tot2_internals::BuildFromRotationMatrix<N, T>::exe(r);
    }  // end of t2tot2<N,T>::fromRotationMatrix

    template <unsigned short N, typename T>
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == N &&
            tfel::typetraits::IsAssignableTo<
                typename TensorTraits<TensorType>::NumType,
                T>::cond,
        Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>>::type
    t2tot2<N, T>::tpld(const TensorType& B) {
      return Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>(B);
    }  // end of t2tot2<N,T>

    template <unsigned short N, typename T>
    template <typename TensorType, typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond &&
            TensorTraits<TensorType>::dime == N &&
            T2toT2Traits<T2toT2Type>::dime == N &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename TensorTraits<TensorType>::NumType,
                    typename T2toT2Traits<T2toT2Type>::NumType,
                    OpMult>::Result,
                T>::cond,
        Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>>::type
    t2tot2<N, T>::tpld(const TensorType& B, const T2toT2Type& C) {
      return Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>(B, C);
    }

    template <unsigned short N, typename T>
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == N &&
            tfel::typetraits::IsAssignableTo<
                typename TensorTraits<TensorType>::NumType,
                T>::cond,
        Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>>::type
    t2tot2<N, T>::tprd(const TensorType& A) {
      return Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>(A);
    }

    template <unsigned short N, typename T>
    template <typename TensorType, typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond &&
            TensorTraits<TensorType>::dime == N &&
            T2toT2Traits<T2toT2Type>::dime == N &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename TensorTraits<TensorType>::NumType,
                    typename T2toT2Traits<T2toT2Type>::NumType,
                    OpMult>::Result,
                T>::cond,
        Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>>::type
    t2tot2<N, T>::tprd(const TensorType& A, const T2toT2Type& C) {
      return Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>(A, C);
    }

    template <unsigned short N, typename T>
    typename tfel::math::t2tot2<N, typename tfel::typetraits::BaseType<T>::type>
        TFEL_CONSTEXPR t2tot2<N, T>::transpose_derivative() {
      using base = typename tfel::typetraits::BaseType<T>::type;
      return internals::ComputeSpecialT2toT2Values<
          N, base>::transpose_derivative();
    }

    template <unsigned short N, typename T>
    TFEL_CONSTEXPR t2tot2<N, typename tfel::typetraits::BaseType<T>::type>
    t2tot2<N, T>::Id() {
      using base = typename tfel::typetraits::BaseType<T>::type;
      return internals::ComputeSpecialT2toT2Values<N, base>::Id();
    }

    template <unsigned short N, typename T>
    TFEL_CONSTEXPR t2tot2<N, typename tfel::typetraits::BaseType<T>::type>
    t2tot2<N, T>::IxI() {
      using base = typename tfel::typetraits::BaseType<T>::type;
      return internals::ComputeSpecialT2toT2Values<N, base>::IxI();
    }

    template <unsigned short N, typename T>
    TFEL_CONSTEXPR t2tot2<N, typename tfel::typetraits::BaseType<T>::type>
    t2tot2<N, T>::K() {
      using base = typename tfel::typetraits::BaseType<T>::type;
      return internals::ComputeSpecialT2toT2Values<N, base>::K();
    }

    template <unsigned short N, typename T>
    constexpr t2tot2<N, T>::t2tot2() {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr t2tot2<N, T>::t2tot2(const T2& init)
        : fsarray<TensorDimeToSize<N>::value * TensorDimeToSize<N>::value, T>(
              init) {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr t2tot2<N, T>::t2tot2(const std::initializer_list<T2>& values)
        : fsarray<TensorDimeToSize<N>::value * TensorDimeToSize<N>::value, T>(
              values) {}

    template <unsigned short N, typename T>
    template <typename T2, typename Op>
    t2tot2<N, T>::t2tot2(const Expr<t2tot2<N, T2>, Op>& src) {
      matrix_utilities<TensorDimeToSize<N>::value, TensorDimeToSize<N>::value,
                       TensorDimeToSize<N>::value>::copy(src, *this);
    }

    template <unsigned short N, typename T>
    constexpr t2tot2<N, T>::t2tot2(const t2tot2<N, T>& src)
        : T2toT2Concept<t2tot2<N, T>>(src),
          fsarray<TensorDimeToSize<N>::value * TensorDimeToSize<N>::value, T>(
              src) {}

    template <unsigned short N, typename T>
    t2tot2<N, T>& t2tot2<N, T>::operator=(const t2tot2<N, T>& src) {
      fsarray<TensorDimeToSize<N>::value * TensorDimeToSize<N>::value,
              T>::operator=(src);
      return *this;
    }

    template <unsigned short N, typename T>
    T& t2tot2<N, T>::operator()(const unsigned short i,
                                const unsigned short j) {
      return this->v[TensorDimeToSize<N>::value * i + j];
    }

    template <unsigned short N, typename T>
    constexpr const T& t2tot2<N, T>::operator()(const unsigned short i,
                                                const unsigned short j) const {
      return this->v[TensorDimeToSize<N>::value * i + j];
    }

    template <unsigned short N, typename T>
    template <typename InputIterator>
    TFEL_MATH_INLINE2 void t2tot2<N, T>::copy(const InputIterator src) {
      tfel::fsalgo::copy<TensorDimeToSize<N>::value *
                         TensorDimeToSize<N>::value>::exe(src, *this);
    }

    template <unsigned short N, typename T>
    typename t2tot2<N, T>::RunTimeProperties
    t2tot2<N, T>::getRunTimeProperties() const {
      return RunTimeProperties();
    }  // end of t2tot2<N,T>::getRunTimeProperties

    template <typename T2toT2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond,
        t2tot2<T2toT2Traits<T2toT2Type>::dime,
               typename T2toT2Traits<T2toT2Type>::NumType>>::type
    change_basis(
        const T2toT2Type& s,
        const tfel::math::tmatrix<
            3u,
            3u,
            typename tfel::typetraits::BaseType<
                typename T2toT2Traits<T2toT2Type>::NumType>::type>& r) {
      return t2tot2_internals::ChangeBasis<T2toT2Traits<T2toT2Type>::dime>::exe(
          s, r);
    }

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond,
        t2tot2<TensorTraits<TensorType>::dime,
               typename TensorTraits<TensorType>::NumType>>::type
    computeVelocityGradientDerivative(const TensorType& F) {
      using res = t2tot2<TensorTraits<TensorType>::dime,
                         typename TensorTraits<TensorType>::NumType>;
      const auto iF = invert(F);
      return res::tpld(iF);
    }

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond,
        t2tot2<TensorTraits<TensorType>::dime,
               typename TensorTraits<TensorType>::NumType>>::type
    computeSpinRateDerivative(const TensorType& F) {
      using value_type = typename TensorTraits<TensorType>::NumType;
      using base = typename tfel::typetraits::BaseType<value_type>::type;
      using res = t2tot2<TensorTraits<TensorType>::dime, value_type>;
      using resbase = t2tot2<TensorTraits<TensorType>::dime, base>;
      ;
      const auto iF = invert(F);
      const auto itF = invert(transpose(F));
      const auto dt = resbase::transpose_derivative();
      const res r = res::tpld(iF) - res::tprd(itF, dt);
      return r / 2;
    }

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 1u &&
            tfel::typetraits::IsScalar<
                typename TensorTraits<TensorType>::NumType>::cond,
        t2tot2<1u, typename TensorTraits<TensorType>::NumType>>::type
    computeDeterminantSecondDerivative(const TensorType& t) {
      using NumType = typename TensorTraits<TensorType>::NumType;
      constexpr const auto zero = NumType{0};
      return {zero, t[2], t[1], t[2], zero, t[0], t[1], t[0], zero};
    }  // end of computeDeterminantSecondDerivative

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 2u &&
            tfel::typetraits::IsScalar<
                typename TensorTraits<TensorType>::NumType>::cond,
        t2tot2<2u, typename TensorTraits<TensorType>::NumType>>::type
    computeDeterminantSecondDerivative(const TensorType& t) {
      using NumType = typename TensorTraits<TensorType>::NumType;
      constexpr const auto zero = NumType{0};
      return {zero,  t[2], t[1], zero, zero,  t[2],  zero, t[0], zero,
              zero,  t[1], t[0], zero, -t[4], -t[3], zero, zero, -t[3],
              -t[2], zero, zero, zero, -t[4], zero,  -t[2]};
    }  // end of computeDeterminantSecondDerivative

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 3u &&
            tfel::typetraits::IsScalar<
                typename TensorTraits<TensorType>::NumType>::cond,
        t2tot2<3u, typename TensorTraits<TensorType>::NumType>>::type
    computeDeterminantSecondDerivative(const TensorType& t) {
      using NumType = typename TensorTraits<TensorType>::NumType;
      constexpr const auto zero = NumType{0};
      return {zero,  t[2],  t[1],  zero,  zero,  zero,  zero,  -t[8], -t[7],
              t[2],  zero,  t[0],  zero,  zero,  -t[6], -t[5], zero,  zero,
              t[1],  t[0],  zero,  -t[4], -t[3], zero,  zero,  zero,  zero,
              zero,  zero,  -t[3], -t[2], zero,  t[8],  zero,  zero,  t[5],
              zero,  zero,  -t[4], zero,  -t[2], zero,  t[7],  t[6],  zero,
              zero,  -t[5], zero,  t[7],  zero,  -t[1], zero,  t[3],  zero,
              zero,  -t[6], zero,  zero,  t[8],  zero,  -t[1], zero,  t[4],
              -t[7], zero,  zero,  zero,  t[5],  t[4],  zero,  -t[0], zero,
              -t[8], zero,  zero,  t[6],  zero,  zero,  t[3],  zero,  -t[0]};
    }  // end of computeDeterminantSecondDerivative

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOT2IXX */
