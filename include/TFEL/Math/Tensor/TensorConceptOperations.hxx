/*!
 * \file   include/TFEL/Math/Tensor/TensorConceptOperations.hxx
 * \brief  This file implements operations that can be applied to tensor.
 * \author Thomas Helfer
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSOR_CONCEPT_OPERATIONS_HXX
#define LIB_TFEL_MATH_TENSOR_CONCEPT_OPERATIONS_HXX

#include <cmath>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/Tensor/TensorProduct.hxx"
#include "TFEL/Math/Tensor/TensorViewFromStensor.hxx"

namespace tfel::math {

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for tensor's
   * multiplication
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<TensorTag, TensorTag, A, B, OpMult> {
    struct DummyHandle {};
    using TensorTypeA = EvaluationResult<A>;
    using TensorTypeB = EvaluationResult<B>;
    using Handler = std::conditional_t<
        getSpaceDimension<TensorTypeA>() == 1u,
        TensorProductExpr1D<A, B>,
        typename std::conditional<getSpaceDimension<TensorTypeA>() == 2u,
                                  TensorProductExpr2D<A, B>,
                                  TensorProductExpr3D<A, B>>::type>;

   public:
    using Result = result_type<TensorTypeA, TensorTypeB, OpMult>;
    using Handle = std::
        conditional_t<isInvalid<Result>(), DummyHandle, Expr<Result, Handler>>;
  };

  template <typename A, typename B>
  class ComputeBinaryOperationHandler<TensorTag,
                                      TensorTag,
                                      A,
                                      B,
                                      OpDotProduct> {
    struct DummyHandle {};
    //! \brief a simple alias
    using TensorA = EvaluationResult<A>;
    //! \brief a simple alias
    using TensorB = EvaluationResult<B>;

   public:
    typedef result_type<TensorA, TensorB, OpDotProduct> Result;
    typedef result_type<TensorA, TensorB, OpDotProduct> Handle;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for tensor vs
   * stensor operations
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryOperationHandler<TensorTag, StensorTag, A, B, Op> {
    struct DummyHandle {};
    //! \brief a simple alias
    using TensorTypeA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensorTypeB = EvaluationResult<B>;
    //! \brief a simple alias
    using TensorTypeB = typename TensorViewFromStensor<B>::type;

   public:
    using Result = result_type<TensorTypeA, StensorTypeB, Op>;
    using Handle =
        std::conditional_t<isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, BinaryOperation<A, TensorTypeB&&, Op>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for stensor vs
   * tensor operations
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryOperationHandler<StensorTag, TensorTag, A, B, Op> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensorTypeA = EvaluationResult<A>;
    //! \brief a simple alias
    typedef typename TensorViewFromStensor<A>::type TensorTypeA;
    //! \brief a simple alias
    using TensorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<StensorTypeA, TensorTypeB, Op>;
    using Handle =
        std::conditional_t<isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, BinaryOperation<TensorTypeA&&, B, Op>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for tensor's
   * multiplication
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<TensorTag, StensorTag, A, B, OpMult> {
    struct DummyHandle {};
    //! \brief a simple alias
    using TensorTypeA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensorTypeB = EvaluationResult<B>;
    //! \brief a simple alias
    typedef typename TensorViewFromStensor<B>::type TensorTypeB;
    //! \brief a simple alias
    using TensorProductOperation = std::conditional_t<
        getSpaceDimension<TensorTypeA>() == 1u,
        TensorProductExpr1D<A, TensorTypeB&&>,
        typename std::conditional<getSpaceDimension<TensorTypeA>() == 2u,
                                  TensorProductExpr2D<A, TensorTypeB&&>,
                                  TensorProductExpr3D<A, TensorTypeB&&>>::type>;

   public:
    using Result = result_type<TensorTypeA, StensorTypeB, OpMult>;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, TensorProductOperation>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for tensor's
   * multiplication
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<StensorTag, TensorTag, A, B, OpMult> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensorTypeA = EvaluationResult<A>;
    //! \brief a simple alias
    typedef typename TensorViewFromStensor<A>::type TensorTypeA;
    using TensorTypeB = EvaluationResult<B>;
    using TensorProductOperation = std::conditional_t<
        getSpaceDimension<TensorTypeB>() == 1u,
        TensorProductExpr1D<TensorTypeA&&, B>,
        typename std::conditional<getSpaceDimension<TensorTypeB>() == 2u,
                                  TensorProductExpr2D<TensorTypeA&&, B>,
                                  TensorProductExpr3D<TensorTypeA&&, B>>::type>;

   public:
    using Result = result_type<StensorTypeA, TensorTypeB, OpMult>;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, TensorProductOperation>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for tensor's
   * operation
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<TensorTag,
                                      TensorTag,
                                      A,
                                      B,
                                      OpDiadicProduct> {
    struct DummyHandle {};
    using TensorTypeA = EvaluationResult<A>;
    using TensorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<TensorTypeA, TensorTypeB, OpDiadicProduct>;
    using Handle =
        std::conditional_t<isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, DiadicProductOperation<A, B>>>;
  };

  /*
   * Partial Specialisation of ComputeUnaryResult_ for tensors
   */
  template <typename A>
  struct ComputeUnaryResult_<TensorTag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    using TensorTypeA = EvaluationResult<A>;
    typedef numeric_type<A> NumA;
    typedef typename ComputeUnaryResult<NumA, OpNeg>::Result NumResult;

   public:
    using Result = typename UnaryResultType<TensorTypeA, OpNeg>::type;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

  /*!
   * \return the inner product of a tensor
   * \tparam TensorType1, type of the left  tensor.
   * \tparam TensorType2, type of the right tensor.
   * \param[in] a: the left  tensor.
   * \param[in] b: the right tensor.
   * \return const result_type<T,T2,OpMult>, the
   * result.
   * \warning the operator| has not the priority expected for such
   * an operation : use of parenthesis is required.
   */
  template <TensorConcept TensorType1, TensorConcept TensorType2>
  TFEL_HOST_DEVICE constexpr auto operator|(const TensorType1&,
                                            const TensorType2&) noexcept
      requires(
          !isInvalid<
              BinaryOperationResult<TensorType1, TensorType2, OpDotProduct>>());

}  // end of namespace tfel::math

#include "TFEL/Math/Tensor/TensorConceptOperations.ixx"

#endif /* LIB_TFEL_MATH_TENSOR_CONCEPT_OPERATIONS_HXX */
