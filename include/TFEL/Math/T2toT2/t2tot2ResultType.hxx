/*!
 * \file   include/TFEL/Math/T2toT2/t2tot2ResultType.hxx
 * \brief  this file implements partial specialisations of the
 * ResultType class for t2tot2.
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOT2_RESULT_TYPE_HXX
#define LIB_TFEL_MATH_T2TOT2_RESULT_TYPE_HXX

#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Math/Forward/tensor.hxx"

namespace tfel::math {

  /*
   * Partial specialisation for negation
   */
  template <unsigned short N, typename T>
  class UnaryResultType<t2tot2<N, T>, OpNeg> {
    typedef typename UnaryResultType<T, OpNeg>::type ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tot2 and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<T2toT2Tag,
                                     ScalarTag,
                                     t2tot2<N, T>,
                                     Scal,
                                     OpMult> {
    typedef result_type<T, Scal, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tot2 and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<T2toT2Tag,
                                     ScalarTag,
                                     t2tot2<N, T>,
                                     Scal,
                                     OpDiv> {
    typedef result_type<T, Scal, OpDiv> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tot2 and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename Scal, unsigned short N, typename T>
  class ComputeBinaryOperationResult<ScalarTag,
                                     T2toT2Tag,
                                     Scal,
                                     t2tot2<N, T>,
                                     OpMult> {
    typedef result_type<Scal, T, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tot2
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<t2tot2<N, T>, t2tot2<N, T2>, OpPlus> {
    typedef result_type<T, T2, OpPlus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tot2
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<t2tot2<N, T>, t2tot2<N, T2>, OpMinus> {
    typedef result_type<T, T2, OpMinus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tot2 and tensor multiplication
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<t2tot2<N, T>, tensor<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor and t2tot2 multiplication
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tensor<N, T2>, t2tot2<N, T>, OpMult> {
    typedef result_type<T2, T, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tot2 and t2tot2 multiplication
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<t2tot2<N, T>, t2tot2<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOT2_RESULT_TYPE_HXX */
