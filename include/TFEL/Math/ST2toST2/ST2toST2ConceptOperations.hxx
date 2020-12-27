/*!
 * \file   include/TFEL/Math/ST2toST2/ST2toST2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to st2tost2.
 * \author Thomas Helfer
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_ST2TOST2_CONCEPT_OPERATIONS_HXX
#define LIB_TFEL_ST2TOST2_CONCEPT_OPERATIONS_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2StensorProductExpr.hxx"
#include "TFEL/Math/ST2toST2/StensorST2toST2ProductExpr.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ST2toST2ProductExpr.hxx"

namespace tfel::math {

  template <typename T_type, typename Operation>
  struct ST2toST2Traits<Expr<T_type, Operation>> {
    using NumType = typename ST2toST2Traits<T_type>::NumType;
    static constexpr unsigned short dime = ST2toST2Traits<T_type>::dime;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for st2tost2's operation
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<ST2toST2Tag, ST2toST2Tag, A, B, Op> {
    struct DummyHandle {};
    using ST2toST2TypeA = EvaluationResult<A>;
    using ST2toST2TypeB = EvaluationResult<B>;

   public:
    using Result = typename ResultType<ST2toST2TypeA, ST2toST2TypeB, Op>::type;
    using Handle = std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                                      DummyHandle,
                                      Expr<Result, BinaryOperation<A, B, Op>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for scalar-st2tost2
   * operations
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<ScalarTag, ST2toST2Tag, A, B, Op> {
    struct DummyHandle {};
    using ST2toST2TypeB = EvaluationResult<B>;

   public:
    using Result = typename ResultType<A, ST2toST2TypeB, Op>::type;
    using Handle =
        std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                           DummyHandle,
                           Expr<Result, ScalarObjectOperation<A, B, Op>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for st2tost2-scalar
   * operations
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<ST2toST2Tag, ScalarTag, A, B, Op> {
    struct DummyHandle {};
    using ST2toST2TypeA = EvaluationResult<A>;

   public:
    using Result = typename ResultType<ST2toST2TypeA, B, Op>::type;
    using Handle =
        std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                           DummyHandle,
                           Expr<Result, ObjectScalarOperation<A, B, Op>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for st2tost2-st2tost2
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryResult_<ST2toST2Tag, ST2toST2Tag, A, B, OpMult> {
    struct DummyHandle {};
    using ST2toST2TypeA = EvaluationResult<A>;
    using ST2toST2TypeB = EvaluationResult<B>;

   public:
    using Result =
        typename ResultType<ST2toST2TypeA, ST2toST2TypeB, OpMult>::type;
    using Handle = std::conditional_t<
        tfel::typetraits::IsInvalid<Result>::cond,
        DummyHandle,
        Expr<Result,
             ST2toST2ST2toST2ProductExpr<ST2toST2Traits<Result>::dime>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for st2tost2-stensor
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryResult_<ST2toST2Tag, StensorTag, A, B, OpMult> {
    struct DummyHandle {};
    using ST2toST2TypeA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensB = EvaluationResult<B>;

   public:
    using Result = typename ResultType<ST2toST2TypeA, StensB, OpMult>::type;
    using Handle = std::conditional_t<
        tfel::typetraits::IsInvalid<Result>::cond,
        DummyHandle,
        Expr<Result, ST2toST2StensorProductExpr<StensorTraits<Result>::dime>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for stensor-st2tost2
   * operations
   */
  template <typename A, typename B>
  class ComputeBinaryResult_<StensorTag, ST2toST2Tag, A, B, OpMult> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensA = EvaluationResult<A>;
    //! \brief a simple alias
    using ST2toST2TypeB = EvaluationResult<B>;

   public:
    using Result = typename ResultType<StensA, ST2toST2TypeB, OpMult>::type;
    using Handle = std::conditional_t<
        tfel::typetraits::IsInvalid<Result>::cond,
        DummyHandle,
        Expr<Result, StensorST2toST2ProductExpr<StensorTraits<Result>::dime>>>;
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
    using Handle = std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

  template <typename T1, typename T2>
  TFEL_MATH_INLINE typename std::enable_if<
      tfel::meta::Implements<T1, StensorConcept>::cond &&
          tfel::meta::Implements<T2, ST2toST2Concept>::cond &&
          !tfel::typetraits::IsInvalid<
              typename ComputeBinaryResult<T1, T2, OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1, T2, OpMult>::Handle>::type
  operator|(const T1& a, const T2& b) {
    using Handle = typename ComputeBinaryResult<T1, T2, OpMult>::Handle;
    return Handle(a, b);
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_ST2TOST2_CONCEPT_OPERATIONS_HXX */
