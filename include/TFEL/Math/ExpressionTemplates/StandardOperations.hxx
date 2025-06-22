/*!
 * \file  StandardOperations.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03 févr. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_EXPRESSIONTEMPLATES_STANDARDOPERATIONS_HXX
#define LIB_TFEL_MATH_EXPRESSIONTEMPLATES_STANDARDOPERATIONS_HXX

#include <type_traits>

#include "TFEL/TypeTraits/IsScalar.hxx"

#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"
#include "TFEL/Math/General/ComputeBinaryResult.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief negate a mathematical object
     * \tparam T1: type of the left  hand side argument
     * \param  a : argument
     * \return an handle which allow delayed evaluation of the
     * operation.
     *
     * This operator is defined if the
     * ComputeUnaryResult<T1,T2,OpPlus> metafunctions returns valid
     * Result and Handle.
     * \see ComputeBinaryResult
     * \see IsInvalidType
     */
    template <typename T1>
    TFEL_MATH_INLINE auto operator-(T1&& a) -> typename std::enable_if<
        ((!tfel::typetraits::IsScalar<typename std::decay<T1>::type>::cond) &&
         (isUnaryOperationResultTypeValid<decltype(a), OpNeg>::value) &&
         (!tfel::typetraits::IsInvalid<
             UnaryOperationHandler<decltype(a), OpNeg>>::cond)),
        UnaryOperationHandler<decltype(a), OpNeg>>::type {
      return UnaryOperationHandler<decltype(a), OpNeg>(std::forward<T1>(a));
    }

    /*!
     * \brief add to mathematical object
     * \tparam T1: type of the left  hand side argument
     * \tparam T2: type of the right hand side argument
     * \param  a : left  hand side argument
     * \param  b : right hand side argument
     * \return an handle which allow delayed evaluation of the
     * operation
     *
     * This operator is defined if the
     * ComputeBinaryResult<T1,T2,OpPlus> metafunctions returns valid
     * Result and Handle.
     * \see ComputeBinaryResult
     * \see IsInvalidType
     */
    template <typename T1, typename T2>
    TFEL_MATH_INLINE auto operator+(T1&& a, T2&& b) -> typename std::enable_if<
        isBinaryOperationResultTypeValid<decltype(a), decltype(b), OpPlus>::
                value &&
            (!((tfel::typetraits::IsScalar<
                   typename std::decay<T1>::type>::cond) &&
               (tfel::typetraits::IsScalar<
                   typename std::decay<T2>::type>::cond))),
        BinaryOperationHandler<decltype(a), decltype(b), OpPlus>>::type {
      return BinaryOperationHandler<decltype(a), decltype(b), OpPlus>(
          std::forward<T1>(a), std::forward<T2>(b));
    }

    /*!
     * \brief substract to mathematical object
     * \tparam T1: type of the left  hand side argument
     * \tparam T2: type of the right hand side argument
     * \param  a : left  hand side argument
     * \param  b : right hand side argument
     * \return an handle which allow delayed evaluation of the
     * operation
     *
     * This operator is defined if the
     * ComputeBinaryResult<T1,T2,OpMinus> metafunctions returns valid
     * Result and Handle.
     * \see ComputeBinaryResult
     * \see IsInvalidType
     */
    template <typename T1, typename T2>
    TFEL_MATH_INLINE auto operator-(T1&& a, T2&& b) -> typename std::enable_if<
        isBinaryOperationResultTypeValid<decltype(a), decltype(b), OpMinus>::
                value &&
            (!((tfel::typetraits::IsScalar<
                   typename std::decay<T1>::type>::cond) &&
               (tfel::typetraits::IsScalar<
                   typename std::decay<T2>::type>::cond))),
        BinaryOperationHandler<decltype(a), decltype(b), OpMinus>>::type {
      return BinaryOperationHandler<decltype(a), decltype(b), OpMinus>(
          std::forward<T1>(a), std::forward<T2>(b));
    }

    /*!
     * \brief multiply to mathematical object
     * \tparam T1: type of the left  hand side argument
     * \tparam T2: type of the right hand side argument
     * \param  a : left  hand side argument
     * \param  b : right hand side argument
     * \return an handle which allow delayed evaluation of the
     * operation
     *
     * This operator is defined if the
     * ComputeBinaryResult<T1,T2,OpMult> metafunctions returns valid
     * Result and Handle.
     * \see ComputeBinaryResult
     * \see IsInvalidType
     */
    template <typename T1, typename T2>
    TFEL_MATH_INLINE auto operator*(T1&& a, T2&& b) -> typename std::enable_if<
        isBinaryOperationResultTypeValid<decltype(a), decltype(b), OpMult>::
                value &&
            (!((tfel::typetraits::IsScalar<
                   typename std::decay<T1>::type>::cond) &&
               (tfel::typetraits::IsScalar<
                   typename std::decay<T2>::type>::cond))),
        BinaryOperationHandler<decltype(a), decltype(b), OpMult>>::type {
      return BinaryOperationHandler<decltype(a), decltype(b), OpMult>(
          std::forward<T1>(a), std::forward<T2>(b));
    }

    /*!
     * \brief divide a mathematical object by another
     * \tparam T1: type of the left  hand side argument
     * \tparam T2: type of the right hand side argument
     * \param  a : left  hand side argument
     * \param  b : right hand side argument
     * \return an handle which allow delayed evaluation of the
     * operation
     *
     * This operator is defined if the
     * ComputeBinaryResult<T1,T2,OpDiv> metafunctions returns valid
     * Result and Handle.
     * \see ComputeBinaryResult
     * \see IsInvalidType
     */
    template <typename T1, typename T2>
    TFEL_MATH_INLINE auto operator/(T1&& a, T2&& b) -> typename std::enable_if<
        isBinaryOperationResultTypeValid<decltype(a), decltype(b), OpDiv>::
                value &&
            (!((tfel::typetraits::IsScalar<
                   typename std::decay<T1>::type>::cond) &&
               (tfel::typetraits::IsScalar<
                   typename std::decay<T2>::type>::cond))),
        BinaryOperationHandler<decltype(a), decltype(b), OpDiv>>::type {
      return BinaryOperationHandler<decltype(a), decltype(b), OpDiv>(
          std::forward<T1>(a), std::forward<T2>(b));
    }

    /*!
     * \brief diadic product between two mathematical objects
     * \tparam T1: type of the left  hand side argument
     * \tparam T2: type of the right hand side argument
     * \param  a : left  hand side argument
     * \param  b : right hand side argument
     * \return an handle which allow delayed evaluation of the
     * operation
     *
     * This operator is defined if the
     * ComputeBinaryResult<T1,T2,OpDiadicProduct> metafunctions
     * returns valid Result and Handle.
     * \see ComputeBinaryResult
     * \see IsInvalidType
     */
    template <typename T1, typename T2>
    TFEL_MATH_INLINE auto operator^(T1&& a, T2&& b) -> typename std::enable_if<
        isBinaryOperationResultTypeValid<decltype(a),
                                         decltype(b),
                                         OpDiadicProduct>::value &&
            (!((tfel::typetraits::IsScalar<
                   typename std::decay<T1>::type>::cond) &&
               (tfel::typetraits::IsScalar<
                   typename std::decay<T2>::type>::cond))),
        BinaryOperationHandler<decltype(a), decltype(b), OpDiadicProduct>>::
        type {
      return BinaryOperationHandler<decltype(a), decltype(b), OpDiadicProduct>(
          std::forward<T1>(a), std::forward<T2>(b));
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_EXPRESSIONTEMPLATES_STANDARDOPERATIONS_HXX */
