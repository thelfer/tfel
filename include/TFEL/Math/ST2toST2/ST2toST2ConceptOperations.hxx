/*!
 * \file   include/TFEL/Math/ST2toST2/ST2toST2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to st2tost2.
 * \author Thomas Helfer
 * \date   01/07/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2_CONCEPT_OPERATIONS_HXX
#define LIB_TFEL_MATH_ST2TOST2_CONCEPT_OPERATIONS_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2StensorProductExpr.hxx"
#include "TFEL/Math/ST2toST2/StensorST2toST2ProductExpr.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ST2toST2ProductExpr.hxx"

namespace tfel::math {

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for
   * st2tost2-st2tost2 operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<ST2toST2Tag, ST2toST2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using ST2toST2TypeA = EvaluationResult<A>;
    using ST2toST2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<ST2toST2TypeA, ST2toST2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, ST2toST2ST2toST2ProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for
   * st2tost2-stensor operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<ST2toST2Tag, StensorTag, A, B, OpMult> {
    struct DummyHandle {};
    using ST2toST2TypeA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<ST2toST2TypeA, StensorTypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, ST2toST2StensorProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for
   * stensor-st2tost2 operations
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<StensorTag, ST2toST2Tag, A, B, OpMult> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensorTypeA = EvaluationResult<A>;
    //! \brief a simple alias
    using ST2toST2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<StensorTypeA, ST2toST2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, StensorST2toST2ProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * Partial Specialisation of ComputeUnaryResult_ for st2tost2s
   */
  template <typename A>
  struct ComputeUnaryResult_<ST2toST2Tag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    using ST2toST2TypeA = EvaluationResult<A>;

   public:
    using Result = typename UnaryResultType<ST2toST2TypeA, OpNeg>::type;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

  template <typename T1, typename T2>
  TFEL_MATH_INLINE typename std::enable_if<
      implementsStensorConcept<T1>() && implementsST2toST2Concept<T2>() &&
          !isInvalid<BinaryOperationResult<T1, T2, OpMult>>(),
      BinaryOperationHandler<T1, T2, OpMult>>::type
  operator|(const T1& a, const T2& b) {
    using Handle = BinaryOperationHandler<T1, T2, OpMult>;
    return Handle(a, b);
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOST2_CONCEPT_OPERATIONS_HXX */
