/*!
 * \file   include/TFEL/Math/Stensor/StensorConceptOperations.hxx
 * \brief  This file implements operations that can be applied to stensor.
 * \author Thomas Helfer
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSOR_CONCEPT_OPERATIONS_HXX
#define LIB_TFEL_MATH_STENSOR_CONCEPT_OPERATIONS_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/Stensor/StensorProduct.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel::math {

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for stensor's
   * multiplication
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<StensorTag, StensorTag, A, B, OpMult> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensB = EvaluationResult<B>;
    //! \brief a simple alias
    using Operation =
        std::conditional_t<getSpaceDimension<StensA>() == 1u,
                           StensorProductExpr1D<A, B>,
                           std::conditional_t<getSpaceDimension<StensA>() == 2u,
                                              StensorProductExpr2D<A, B>,
                                              StensorProductExpr3D<A, B>>>;

   public:
    using Result = result_type<StensA, StensB, OpMult>;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, Operation>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for stensor's
   * operation
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<StensorTag,
                                      StensorTag,
                                      A,
                                      B,
                                      OpDiadicProduct> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensB = EvaluationResult<B>;

   public:
    using Result = result_type<StensA, StensB, OpDiadicProduct>;
    using Handle =
        std::conditional_t<isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, DiadicProductOperation<A, B>>>;
  };

  /*
   * Partial Specialisation of ComputeUnaryResult_ for stensors
   */
  template <typename A>
  struct ComputeUnaryResult_<StensorTag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensA = EvaluationResult<A>;

   public:
    using Result = typename UnaryResultType<StensA, OpNeg>::type;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

  template <typename A, typename B>
  class ComputeBinaryOperationHandler<StensorTag,
                                      StensorTag,
                                      A,
                                      B,
                                      OpDotProduct> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensorA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensorB = EvaluationResult<B>;

   public:
    typedef result_type<StensorA, StensorB, OpDotProduct> Result;
    typedef result_type<StensorA, StensorB, OpDotProduct> Handle;
  };

  /*!
   * \return the inner product of a stensor
   * \param const T1&, the left  stensor.
   * \param const T2&, the right stensor.
   * \return const result_type<T,T2,OpMult>, the
   * result.
   * \warning the operator| has not the priority expected for such
   * an operation : use of parenthesis is required.
   */
  template <StensorConcept T1, StensorConcept T2>
  TFEL_HOST_DEVICE constexpr BinaryOperationResult<T1, T2, OpDotProduct>
  operator|(const T1&, const T2&) noexcept
      requires((getSpaceDimension<T1>() == getSpaceDimension<T2>()) &&
               (!isInvalid<BinaryOperationResult<T1, T2, OpDotProduct>>()));

}  // end of namespace tfel::math

#include "TFEL/Math/Stensor/StensorConceptOperations.ixx"

#endif /* LIB_TFEL_MATH_STENSOR_CONCEPT_OPERATIONS_HXX */
