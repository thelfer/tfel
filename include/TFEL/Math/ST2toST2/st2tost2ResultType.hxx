/*!
 * \file   include/TFEL/Math/ST2toST2/st2tost2ResultType.hxx
 * \brief  this file implements partial specialisations of the
 * ResultType class for st2tost2.
 * \author Thomas Helfer
 * \date   03 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_ST2TOST2_RESULT_TYPE_HXX
#define LIB_TFEL_ST2TOST2_RESULT_TYPE_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/Forward/stensor.hxx"

namespace tfel::math {

  /*
   * Partial specialisation for negation
   */
  template <unsigned short N, typename T>
  class UnaryResultType<st2tost2<N, T>, OpNeg> {
    using ResBase_ = typename UnaryResultType<T, OpNeg>::type;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    st2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for st2tost2 and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<ST2toST2Tag,
                                     ScalarTag,
                                     st2tost2<N, T>,
                                     Scal,
                                     OpMult> {
    typedef result_type<T, Scal, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    st2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for st2tost2 and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<ST2toST2Tag,
                                     ScalarTag,
                                     st2tost2<N, T>,
                                     Scal,
                                     OpDiv> {
    typedef result_type<T, Scal, OpDiv> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    st2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for st2tost2 and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename Scal, unsigned short N, typename T>
  class ComputeBinaryOperationResult<ScalarTag,
                                     ST2toST2Tag,
                                     Scal,
                                     st2tost2<N, T>,
                                     OpMult> {
    typedef result_type<Scal, T, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    st2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for st2tost2
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<st2tost2<N, T>, st2tost2<N, T2>, OpPlus> {
    typedef result_type<T, T2, OpPlus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    st2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for st2tost2
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<st2tost2<N, T>, st2tost2<N, T2>, OpMinus> {
    typedef result_type<T, T2, OpMinus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    st2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for st2tost2 and stensor multiplication
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<st2tost2<N, T>, stensor<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for stensor and st2tost2 multiplication
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T2>, st2tost2<N, T>, OpMult> {
    typedef result_type<T2, T, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for st2tost2 and st2tost2 multiplication
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<st2tost2<N, T>, st2tost2<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    st2tost2<N, ResBase_>>;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_ST2TOST2_RESULT_TYPE_HXX */
