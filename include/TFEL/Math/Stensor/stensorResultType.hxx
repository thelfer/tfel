/*!
 * \file   include/TFEL/Math/Stensor/stensorResultType.hxx
 * \brief  this file implements partial specialisations of the
 * ResultType class for stensor.
 * \author Helfer Thomas
 * \date   03 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_STENSOR_RESULT_TYPE_HXX_
#define LIB_TFEL_STENSOR_RESULT_TYPE_HXX_

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/DotProduct.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Forward/st2tost2.hxx"

namespace tfel {

  namespace math {

    /*
     * Partial specialisation for unary operations
     */
    template <unsigned short N, typename T>
    class UnaryResultType<stensor<N, T>, OpNeg> {
      typedef typename UnaryResultType<T, OpNeg>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for stensor and
     * scalars operations
     * \see   ResultType_
     */
    template <unsigned short N, typename T, typename Scal>
    class ResultType_<StensorTag, ScalarTag, stensor<N, T>, Scal, OpMult> {
      typedef typename ResultType<T, Scal, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for stensor and
     * scalars operations
     * \see   ResultType_
     */
    template <unsigned short N, typename T, typename Scal>
    class ResultType_<StensorTag, ScalarTag, stensor<N, T>, Scal, OpDiv> {
      typedef typename ResultType<T, Scal, OpDiv>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for stensor and
     * scalars operations
     * \see   ResultType_
     */
    template <typename Scal, unsigned short N, typename T>
    class ResultType_<ScalarTag, StensorTag, Scal, stensor<N, T>, OpMult> {
      typedef typename ResultType<Scal, T, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<stensor<N, T>, stensor<N, T2>, OpPlus> {
      typedef typename ResultType<T, T2, OpPlus>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<stensor<N, T>, stensor<N, T2>, OpMinus> {
      typedef typename ResultType<T, T2, OpMinus>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<stensor<N, T>, stensor<N, T2>, OpMult> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<stensor<N, T>, stensor<N, T2>, OpDiadicProduct> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    st2tost2<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<stensor<N, T>, stensor<N, T2>, OpDotProduct> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;
      static constexpr bool isValid =
          tfel::typetraits::IsScalar<T>::cond &&
          tfel::typetraits::IsScalar<T2>::cond &&
          tfel::typetraits::IsInvalid<ResBase_>::cond;

     public:
      typedef typename std::
          conditional<isValid, tfel::meta::InvalidType, ResBase_>::type type;
    };  // end of class ResultType<stensor<N,T>,stensor<M,T2>,OpDotProduct>

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_STENSOR_RESULT_TYPE_HXX_ */
