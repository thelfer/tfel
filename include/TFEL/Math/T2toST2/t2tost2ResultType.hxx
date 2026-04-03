/*!
 * \file   include/TFEL/Math/T2toST2/t2tost2ResultType.hxx
 * \brief  this file implements partial specialisations of the
 * ResultType class for t2tost2.
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOST2_RESULT_TYPE_HXX
#define LIB_TFEL_MATH_T2TOST2_RESULT_TYPE_HXX

#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/Forward/tensor.hxx"
#include "TFEL/Math/Forward/stensor.hxx"
#include "TFEL/Math/Forward/t2tot2.hxx"
#include "TFEL/Math/Forward/st2tost2.hxx"

namespace tfel::math {

  /*
   * Partial specialisation for negation
   */
  template <unsigned short N, typename T>
  class UnaryResultType<t2tost2<N, T>, OpNeg> {
    typedef typename UnaryResultType<T, OpNeg>::type ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tost2 and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<T2toST2Tag,
                                     ScalarTag,
                                     t2tost2<N, T>,
                                     Scal,
                                     OpMult> {
    typedef result_type<T, Scal, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tost2 and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<T2toST2Tag,
                                     ScalarTag,
                                     t2tost2<N, T>,
                                     Scal,
                                     OpDiv> {
    typedef result_type<T, Scal, OpDiv> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tost2 and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename Scal, unsigned short N, typename T>
  class ComputeBinaryOperationResult<ScalarTag,
                                     T2toST2Tag,
                                     Scal,
                                     t2tost2<N, T>,
                                     OpMult> {
    typedef result_type<Scal, T, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tost2
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<t2tost2<N, T>, t2tost2<N, T2>, OpPlus> {
    typedef result_type<T, T2, OpPlus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tost2
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<t2tost2<N, T>, t2tost2<N, T2>, OpMinus> {
    typedef result_type<T, T2, OpMinus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for stensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T>, tensor<N, T2>, OpDiadicProduct> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tost2 and t2tost2 multiplication
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<st2tost2<N, T>, t2tost2<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tost2 and t2tot2 multiplication
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<t2tost2<N, T>, t2tot2<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for t2tost2 and tensor multiplication
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<t2tost2<N, T>, tensor<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor and t2tost2 multiplication
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T2>, t2tost2<N, T>, OpMult> {
    typedef result_type<T2, T, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOST2_RESULT_TYPE_HXX */
