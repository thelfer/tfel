/*!
 * \file   include/TFEL/Math/T2toT2/t2tot2ResultType.hxx
 * \brief  this file implements partial specialisations of the
 * ResultType class for t2tot2.
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOT2_RESULT_TYPE_HXX_
#define LIB_TFEL_MATH_T2TOT2_RESULT_TYPE_HXX_

#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Math/Forward/tensor.hxx"

namespace tfel {

  namespace math {

    /*
     * Partial specialisation for negation
     */
    template <unsigned short N, typename T>
    class UnaryResultType<t2tot2<N, T>, OpNeg> {
      typedef typename UnaryResultType<T, OpNeg>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for t2tot2 and
     * scalars operations
     * \see   ResultType_
     */
    template <unsigned short N, typename T, typename Scal>
    class ResultType_<T2toT2Tag, ScalarTag, t2tot2<N, T>, Scal, OpMult> {
      typedef typename ResultType<T, Scal, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for t2tot2 and
     * scalars operations
     * \see   ResultType_
     */
    template <unsigned short N, typename T, typename Scal>
    class ResultType_<T2toT2Tag, ScalarTag, t2tot2<N, T>, Scal, OpDiv> {
      typedef typename ResultType<T, Scal, OpDiv>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for t2tot2 and
     * scalars operations
     * \see   ResultType_
     */
    template <typename Scal, unsigned short N, typename T>
    class ResultType_<ScalarTag, T2toT2Tag, Scal, t2tot2<N, T>, OpMult> {
      typedef typename ResultType<Scal, T, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for t2tot2
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<t2tot2<N, T>, t2tot2<N, T2>, OpPlus> {
      typedef typename ResultType<T, T2, OpPlus>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for t2tot2
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<t2tot2<N, T>, t2tot2<N, T2>, OpMinus> {
      typedef typename ResultType<T, T2, OpMinus>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for t2tot2 and tensor multiplication
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<t2tot2<N, T>, tensor<N, T2>, OpMult> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tensor and t2tot2 multiplication
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<tensor<N, T2>, t2tot2<N, T>, OpMult> {
      typedef typename ResultType<T2, T, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for t2tot2 and t2tot2 multiplication
     * \see   ResultType
     */
    template <unsigned short N, typename T, typename T2>
    class ResultType<t2tot2<N, T>, t2tot2<N, T2>, OpMult> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>::type type;
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOT2_RESULT_TYPE_HXX_ */
