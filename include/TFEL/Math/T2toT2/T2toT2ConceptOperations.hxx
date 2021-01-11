/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to t2tot2.
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_T2TOT2_CONCEPT_OPERATIONS_HXX
#define LIB_TFEL_T2TOT2_CONCEPT_OPERATIONS_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/T2toT2/T2toT2TensorProductExpr.hxx"
#include "TFEL/Math/T2toT2/TensorT2toT2ProductExpr.hxx"
#include "TFEL/Math/T2toT2/T2toT2T2toT2ProductExpr.hxx"

namespace tfel::math {

  /*!
   * \brief partial specialisation of ComputeBinaryResult_ for t2tot2's
   * operation \tparam A:  left hand side \tparam B:  right hand side \tparam
   * Op: operation
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<T2toT2Tag, T2toT2Tag, A, B, Op> {
    struct DummyHandle {};
    using T2toT2TypeA = EvaluationResult<A>;
    using T2toT2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<T2toT2TypeA, T2toT2TypeB, Op>;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, BinaryOperation<A, B, Op>>>;
  };

  /*!
   * \brief partial specialisation of ComputeBinaryResult_ for scalar-t2tot2
   * operations \tparam A:  left hand side \tparam B:  right hand side \tparam
   * Op: operation
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<ScalarTag, T2toT2Tag, A, B, Op> {
    struct DummyHandle {};
    using T2toT2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<A, T2toT2TypeB, Op>;
    using Handle =
        std::conditional_t<isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, ScalarObjectOperation<A, B, Op>>>;
  };

  /*!
   * \brief partial specialisation of ComputeBinaryResult_ for t2tot2-scalar
   * operations \tparam A:  left hand side \tparam B:  right hand side \tparam
   * Op: operation
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<T2toT2Tag, ScalarTag, A, B, Op> {
    struct DummyHandle {};
    using T2toT2TypeA = EvaluationResult<A>;

   public:
    using Result = result_type<T2toT2TypeA, B, Op>;
    using Handle =
        std::conditional_t<isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, ObjectScalarOperation<A, B, Op>>>;
  };

  /*!
   * \brief partial specialisation of ComputeBinaryResult_ for T2toT2-T2toT2
   * operations \tparam A:  left hand side \tparam B:  right hand side \tparam
   * Op: operation
   */
  template <typename A, typename B>
  class ComputeBinaryResult_<T2toT2Tag, T2toT2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using T2toT2TypeA = EvaluationResult<A>;
    using T2toT2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<T2toT2TypeA, T2toT2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, T2toT2T2toT2ProductExpr<getSpaceDimension<Result>()>>>;
  };
  /*!
   * \brief partial specialisation of ComputeBinaryResult_ for T2toT2-Tensor
   * operations \tparam A:  left hand side \tparam B:  right hand side \tparam
   * Op: operation
   */
  template <typename A, typename B>
  class ComputeBinaryResult_<T2toT2Tag, TensorTag, A, B, OpMult> {
    struct DummyHandle {};
    using T2toT2TypeA = EvaluationResult<A>;
    using TensorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<T2toT2TypeA, TensorTypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, T2toT2TensorProductExpr<getSpaceDimension<Result>()>>>;
  };

  /*
   * \brief partial specialisation of ComputeBinaryResult_ for Tensor-T2toT2
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryResult_<TensorTag, T2toT2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using TensorTypeA = EvaluationResult<A>;
    using T2toT2TypeB = EvaluationResult<B>;

   public:
    using Result = result_type<TensorTypeA, T2toT2TypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        Expr<Result, TensorT2toT2ProductExpr<getSpaceDimension<Result>()>>>;
  };
  /*!
   * \brief partial specialisation of `ComputeUnaryResult_` for
   * `T2toT2`'s object
   * \tparam A: object
   */
  template <typename A>
  struct ComputeUnaryResult_<T2toT2Tag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    using T2toT2TypeA = EvaluationResult<A>;

   public:
    using Result = typename UnaryResultType<T2toT2TypeA, OpNeg>::type;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

  template <typename T1, typename T2>
  TFEL_MATH_INLINE std::enable_if_t<
      implementsTensorConcept<T1>() && implementsT2toT2Concept<T2>() &&
          !isInvalid<BinaryOperationResult<T1, T2, OpMult>>(),
      BinaryOperationHandler<T1, T2, OpMult>>
  operator|(const T1& a, const T2& b) {
    typedef BinaryOperationHandler<T1, T2, OpMult> Handle;
    return Handle(a, b);
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_T2TOT2_CONCEPT_OPERATIONS_HXX */
