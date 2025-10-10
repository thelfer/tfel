/*!
 * \file   include/TFEL/Math/Tensor/tensorResultType.hxx
 * \brief  this file implements partial specialisations of the
 * ResultType class for tensor.
 * \author Thomas Helfer
 * \date   03 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSOR_RESULT_TYPE_HXX
#define LIB_TFEL_MATH_TENSOR_RESULT_TYPE_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/Forward/tensor.hxx"
#include "TFEL/Math/Forward/t2tot2.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/t2tot2.hxx"

namespace tfel::math {

  /*
   * Partial specialisation for unary operations
   */
  template <unsigned short N, typename T>
  class UnaryResultType<tensor<N, T>, OpNeg> {
    typedef typename UnaryResultType<T, OpNeg>::type ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<TensorTag,
                                     ScalarTag,
                                     tensor<N, T>,
                                     Scal,
                                     OpMult> {
    typedef result_type<T, Scal, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, typename T, typename Scal>
  class ComputeBinaryOperationResult<TensorTag,
                                     ScalarTag,
                                     tensor<N, T>,
                                     Scal,
                                     OpDiv> {
    typedef result_type<T, Scal, OpDiv> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename Scal, unsigned short N, typename T>
  class ComputeBinaryOperationResult<ScalarTag,
                                     TensorTag,
                                     Scal,
                                     tensor<N, T>,
                                     OpMult> {
    typedef result_type<Scal, T, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tensor<N, T>, tensor<N, T2>, OpPlus> {
    typedef result_type<T, T2, OpPlus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tensor<N, T>, tensor<N, T2>, OpMinus> {
    typedef result_type<T, T2, OpMinus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tensor<N, T>, tensor<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T>, tensor<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tensor<N, T>, stensor<N, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tensor<N, T>, tensor<N, T2>, OpDiadicProduct> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    t2tot2<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tensor<N, T>, stensor<N, T2>, OpPlus> {
    typedef result_type<T, T2, OpPlus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T2>, tensor<N, T>, OpPlus> {
    typedef result_type<T, T2, OpPlus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tensor<N, T>, stensor<N, T2>, OpMinus> {
    typedef result_type<T, T2, OpMinus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<stensor<N, T2>, tensor<N, T>, OpMinus> {
    typedef result_type<T, T2, OpMinus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tensor<N, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tensor
   * \see   ResultType
   */
  template <unsigned short N, typename T, typename T2>
  class ResultType<tensor<N, T>, tensor<N, T2>, OpDotProduct> {
    typedef result_type<T, T2, OpMult> ResBase_;
    static const bool isValid =
        isScalar<T>() && isScalar<T2>() && isInvalid<ResBase_>();

   public:
    using type = std::conditional_t<isValid, tfel::meta::InvalidType, ResBase_>;
  };  // end of class ResultType<tensor<N,T>,tensor<M,T2>,OpDotProduct>

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TENSOR_RESULT_TYPE_HXX */
