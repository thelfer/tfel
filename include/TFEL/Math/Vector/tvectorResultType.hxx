/*!
 * \file   include/TFEL/Math/Vector/tvectorResultType.hxx
 * \brief  this file implements partial specialisations of the ResultType.
 * \brief
 * \author Helfer Thomas
 * \date   09 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TINY_VECTOR_RESULT_TYPE_H_
#define LIB_TFEL_TINY_VECTOR_RESULT_TYPE_H_

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/DotProduct.hxx"
#include "TFEL/Math/tvector.hxx"

namespace tfel {

  namespace math {

    /*
     * Partial specialisation for negation.
     */
    template <unsigned short N, typename T>
    struct UnaryResultType<tvector<N, T>, OpNeg> {
      typedef typename UnaryResultType<T, OpNeg>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tvector and
     * scalars operations
     * \see   ResultType_
     */
    template <unsigned short N, typename T, typename Scal>
    class ResultType_<VectorTag, ScalarTag, tvector<N, T>, Scal, OpMult> {
      typedef typename ResultType<T, Scal, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tvector and
     * scalars operations
     * \see   ResultType_
     */
    template <unsigned short N, typename T, typename Scal>
    class ResultType_<VectorTag, ScalarTag, tvector<N, T>, Scal, OpDiv> {
      typedef typename ResultType<T, Scal, OpDiv>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tvector and
     * scalars operations
     * \see   ResultType_
     */
    template <typename Scal, unsigned short N, typename T>
    class ResultType_<ScalarTag, VectorTag, Scal, tvector<N, T>, OpMult> {
      typedef typename ResultType<Scal, T, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tvector
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<tvector<N, T>, tvector<N, T2>, OpPlus> {
      typedef typename ResultType<T, T2, OpPlus>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tvector
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<tvector<N, T>, tvector<N, T2>, OpMinus> {
      typedef typename ResultType<T, T2, OpMinus>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tvector
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<tvector<N, T>, tvector<N, T2>, OpMult> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tvector
     * \see   ResultType
     */
    template <unsigned short N, typename T, unsigned short M, typename T2>
    class ResultType<tvector<N, T>, tvector<M, T2>, OpDiadicProduct> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tvector
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    struct ResultType<tvector<N, T>, tvector<N, T2>, OpDotProduct> {
     private:
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;
      static constexpr bool isValid =
          tfel::typetraits::IsInvalid<ResBase_>::cond;

     public:
      typedef typename std::
          conditional<isValid, tfel::meta::InvalidType, ResBase_>::type type;
    };  // end of class ResultType<tvector<N,T>,tvector<M,T2>,OpDotProduct>

    template <unsigned short N, typename T, typename T2>
    struct VectorDotProductHandle<tvector<N, T>, tvector<N, T2>> {
      typedef DotProduct<N, 0u> type;
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_TINY_VECTOR_RESULT_TYPE_H_ */
