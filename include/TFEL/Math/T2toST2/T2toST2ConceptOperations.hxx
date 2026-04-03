/*!
 * \file   include/TFEL/Math/T2toST2/T2toST2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to t2tost2.
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOST2_CONCEPT_OPERATIONS_HXX
#define LIB_TFEL_MATH_T2TOST2_CONCEPT_OPERATIONS_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/HasRandomAccessConstIterator.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"
#include "TFEL/Math/General/ComputeBinaryResult.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include "TFEL/Math/T2toST2/StensorT2toST2ProductExpr.hxx"
#include "TFEL/Math/T2toST2/T2toST2TensorProductExpr.hxx"
#include "TFEL/Math/T2toST2/ST2toST2T2toST2ProductExpr.hxx"
#include "TFEL/Math/T2toST2/T2toST2T2toT2ProductExpr.hxx"

namespace tfel::math {

  /*!
   * Partial Specialisation of ComputeBinaryOperationHandler for the
   * multiplication of object of type st2tost2 by object of type
   * t2tost2.
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<ST2toST2Tag, T2toST2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using ST2toST2TypeA = EvaluationResult<A>;
    using T2toST2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<ST2toST2TypeA, T2toST2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, ST2toST2T2toST2ProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for t2tost2-t2tot2
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<T2toST2Tag, T2toT2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using T2toST2TypeA = EvaluationResult<A>;
    using T2toT2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<T2toST2TypeA, T2toT2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, T2toST2T2toT2ProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*!
   * Partial Specialisation of ComputeBinaryOperationHandler for stensor's
   * operation
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<StensorTag,
                                      TensorTag,
                                      A,
                                      B,
                                      OpDiadicProduct> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensorTypeA = EvaluationResult<A>;
    using TensorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<StensorTypeA, TensorTypeB, OpDiadicProduct>;
    using Handle =
        std::conditional_t<isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, DiadicProductOperation<A, B>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for T2toST2-Tensor
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<T2toST2Tag, TensorTag, A, B, OpMult> {
    struct DummyHandle {};
    using T2toST2TypeA = EvaluationResult<A>;
    using TensorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<T2toST2TypeA, TensorTypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, T2toST2TensorProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for Stensor-T2toST2
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<StensorTag, T2toST2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using StensorTypeA = EvaluationResult<A>;
    using T2toST2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<StensorTypeA, T2toST2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, StensorT2toST2ProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeUnaryResult_ for t2tost2s
   */
  template <typename A>
  struct ComputeUnaryResult_<T2toST2Tag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    using T2toST2TypeA = EvaluationResult<A>;

   public:
    using Result = typename UnaryResultType<T2toST2TypeA, OpNeg>::type;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

  TFEL_HOST_DEVICE constexpr auto operator|(
      const StensorConcept auto& a,
      const T2toST2Concept auto& b) noexcept  //
      requires(!isInvalid<
               BinaryOperationResult<decltype(a), decltype(b), OpMult>>()) {
    typedef BinaryOperationHandler<decltype(a), decltype(b), OpMult> Handle;
    return Handle(a, b);
  }  // end of operator |

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOST2_CONCEPT_OPERATIONS_HXX */
