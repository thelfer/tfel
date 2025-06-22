/*!
 * \file   include/TFEL/Math/ST2toT2/ST2toT2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to st2tot2.
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOT2_CONCEPT_OPERATIONS_HXX
#define LIB_TFEL_MATH_ST2TOT2_CONCEPT_OPERATIONS_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/ST2toT2/TensorST2toT2ProductExpr.hxx"
#include "TFEL/Math/ST2toT2/ST2toT2StensorProductExpr.hxx"
#include "TFEL/Math/ST2toT2/T2toT2ST2toT2ProductExpr.hxx"
#include "TFEL/Math/ST2toT2/ST2toT2ST2toST2ProductExpr.hxx"
#include "TFEL/Math/ST2toT2/ST2toT2T2toST2ProductExpr.hxx"
#include "TFEL/Math/ST2toT2/T2toST2ST2toT2ProductExpr.hxx"

namespace tfel::math {

  /*!
   * Partial Specialisation of ComputeBinaryOperationHandler for stensor's
   * operation
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<TensorTag,
                                      StensorTag,
                                      A,
                                      B,
                                      OpDiadicProduct> {
    struct DummyHandle {};
    using TensorTypeA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<TensorTypeA, StensorTypeB, OpDiadicProduct>;
    using Handle =
        std::conditional_t<isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, DiadicProductOperation<A, B>>>;
  };

  /*!
   * Partial Specialisation of ComputeBinaryOperationHandler for T2toT2-ST2toT2
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<T2toT2Tag, ST2toT2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using T2toT2TypeA = EvaluationResult<A>;
    using ST2toT2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<T2toT2TypeA, ST2toT2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, T2toT2ST2toT2ProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for st2tot2-t2tost2
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<ST2toT2Tag, T2toST2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using ST2toT2TypeA = EvaluationResult<A>;
    using T2toST2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<ST2toT2TypeA, T2toST2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, ST2toT2T2toST2ProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for
   * st2tot2-st2tost2 operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<ST2toT2Tag, ST2toST2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using ST2toT2TypeA = EvaluationResult<A>;
    using ST2toST2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<ST2toT2TypeA, ST2toST2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, ST2toT2ST2toST2ProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for t2tost2-st2tot2
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<T2toST2Tag, ST2toT2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using T2toST2TypeA = EvaluationResult<A>;
    using ST2toT2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<T2toST2TypeA, ST2toT2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, T2toST2ST2toT2ProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for st2tot2-stensor
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<ST2toT2Tag, StensorTag, A, B, OpMult> {
    struct DummyHandle {};
    using ST2toT2TypeA = EvaluationResult<A>;
    using StensorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<ST2toT2TypeA, StensorTypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, ST2toT2StensorProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for tensor-st2tot2
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<TensorTag, ST2toT2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using TensorTypeA = EvaluationResult<A>;
    using ST2toT2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<TensorTypeA, ST2toT2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, TensorST2toT2ProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeUnaryResult_ for st2tot2s
   */
  template <typename A>
  struct ComputeUnaryResult_<ST2toT2Tag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    using ST2toT2TypeA = EvaluationResult<A>;

   public:
    using Result = typename UnaryResultType<ST2toT2TypeA, OpNeg>::type;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

  template <typename T1, typename T2>
  TFEL_MATH_INLINE typename std::enable_if<
      implementsTensorConcept<T1>() && implementsST2toT2Concept<T2>() &&
          !isInvalid<BinaryOperationResult<T1, T2, OpMult>>(),
      BinaryOperationHandler<T1, T2, OpMult>>::type
  operator|(const T1& a, const T2& b) {
    using Handle = BinaryOperationHandler<T1, T2, OpMult>;
    return Handle(a, b);
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOT2_CONCEPT_OPERATIONS_HXX */
