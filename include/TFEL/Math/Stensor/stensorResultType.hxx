/*!
 * \file   include/TFEL/Math/Stensor/stensorResultType.hxx
 * \brief  this file implements partial specialisations of the
 * ResultType class for stensor.
 * \author Thomas Helfer
 * \date 03/07/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSOR_RESULT_TYPE_HXX
#define LIB_TFEL_MATH_STENSOR_RESULT_TYPE_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/DotProduct.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Forward/st2tost2.hxx"

namespace tfel::math {

  /*
   * Partial specialisation for unary operations
   */
  template <unsigned short N, typename T>
  class UnaryResultType<stensor<N, T>, OpNeg> {
    using ResBase_ = typename UnaryResultType<T, OpNeg>::type;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for stensor and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<StensorTag,
                                     ScalarTag,
                                     stensor<N, T>,
                                     Scal,
                                     OpMult> {
    using ResBase_ = result_type<T, Scal, OpMult>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for stensor and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<StensorTag,
                                     ScalarTag,
                                     stensor<N, T>,
                                     Scal,
                                     OpDiv> {
    using ResBase_ = result_type<T, Scal, OpDiv>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for stensor and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename Scal, unsigned short N, typename T>
  class ComputeBinaryOperationResult<ScalarTag,
                                     StensorTag,
                                     Scal,
                                     stensor<N, T>,
                                     OpMult> {
    using ResBase_ = result_type<Scal, T, OpMult>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for stensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T>, stensor<N, T2>, OpPlus> {
    using ResBase_ = result_type<T, T2, OpPlus>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for stensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T>, stensor<N, T2>, OpMinus> {
    using ResBase_ = result_type<T, T2, OpMinus>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    stensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for stensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T>, stensor<N, T2>, OpMult> {
    using ResBase_ = result_type<T, T2, OpMult>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for stensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T>, stensor<N, T2>, OpDiadicProduct> {
    using ResBase_ = result_type<T, T2, OpMult>;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    st2tost2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for stensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T>, stensor<N, T2>, OpDotProduct> {
    using ResBase_ = result_type<T, T2, OpMult>;
    static constexpr bool isValid =
        isScalar<T>() && isScalar<T2>() && isInvalid<ResBase_>();

   public:
    using type = std::conditional_t<isValid, tfel::meta::InvalidType, ResBase_>;
  };  // end of class ResultType<stensor<N,T>,stensor<M,T2>,OpDotProduct>

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSOR_RESULT_TYPE_HXX */
