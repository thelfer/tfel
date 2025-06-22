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

#ifndef LIB_TFEL_ST2TOT2_CONCEPT_OPERATIONS_HXX
#define LIB_TFEL_ST2TOT2_CONCEPT_OPERATIONS_HXX

#include <cmath>

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Metaprogramming/Implements.hxx"

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

namespace tfel {

  namespace math {

    template <typename T_type, typename Operation>
    struct ST2toT2Type<Expr<T_type, Operation>> {
      typedef T_type type;
    };

    template <typename T_type, typename Operation>
    struct ST2toT2Traits<Expr<T_type, Operation>> {
      typedef typename ST2toT2Traits<T_type>::NumType NumType;
      static constexpr unsigned short dime = ST2toT2Traits<T_type>::dime;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tot2's operation
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<ST2toT2Tag, ST2toT2Tag, A, B, Op> {
      struct DummyHandle {};
      typedef typename ST2toT2Type<typename std::decay<A>::type>::type StensA;
      typedef typename ST2toT2Type<typename std::decay<B>::type>::type StensB;

     public:
      typedef typename ResultType<StensA, StensB, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, BinaryOperation<A, B, Op>>>::type Handle;
    };

    /*!
     * Partial Specialisation of ComputeBinaryResult_ for stensor's operation
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<TensorTag, StensorTag, A, B, OpDiadicProduct> {
      struct DummyHandle {};
      typedef typename TensorType<typename std::decay<A>::type>::type TensA;
      typedef typename StensorType<typename std::decay<B>::type>::type StensB;

     public:
      typedef typename ResultType<TensA, StensB, OpDiadicProduct>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, DiadicProductOperation<A, B>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-st2tot2
     * operations
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<ScalarTag, ST2toT2Tag, A, B, Op> {
      struct DummyHandle {};
      typedef typename ST2toT2Type<typename std::decay<B>::type>::type StensB;

     public:
      typedef typename ResultType<A, StensB, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ScalarObjectOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tot2-scalar
     * operations
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<ST2toT2Tag, ScalarTag, A, B, Op> {
      struct DummyHandle {};
      typedef typename ST2toT2Type<typename std::decay<A>::type>::type StensA;

     public:
      typedef typename ResultType<StensA, B, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ObjectScalarOperation<A, B, Op>>>::type Handle;
    };

    /*!
     * Partial Specialisation of ComputeBinaryResult_ for T2toT2-ST2toT2
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<T2toT2Tag, ST2toT2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename T2toT2Type<typename std::decay<A>::type>::type T2toT2A;
      typedef typename ST2toT2Type<typename std::decay<B>::type>::type ST2toT2B;

     public:
      typedef typename ResultType<T2toT2A, ST2toT2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, T2toT2ST2toT2ProductExpr<ST2toT2Traits<Result>::dime>>>::
          type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tot2-t2tost2
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<ST2toT2Tag, T2toST2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename ST2toT2Type<typename std::decay<A>::type>::type ST2toT2A;
      typedef typename T2toST2Type<typename std::decay<B>::type>::type T2toST2B;

     public:
      typedef typename ResultType<ST2toT2A, T2toST2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ST2toT2T2toST2ProductExpr<T2toT2Traits<Result>::dime>>>::
          type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tot2-st2tost2
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<ST2toT2Tag, ST2toST2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename ST2toT2Type<typename std::decay<A>::type>::type ST2toT2A;
      typedef
          typename ST2toST2Type<typename std::decay<B>::type>::type ST2toST2B;

     public:
      typedef typename ResultType<ST2toT2A, ST2toST2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result,
               ST2toT2ST2toST2ProductExpr<ST2toT2Traits<Result>::dime>>>::type
          Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for t2tost2-st2tot2
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<T2toST2Tag, ST2toT2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename T2toST2Type<typename std::decay<A>::type>::type T2toST2A;
      typedef typename ST2toT2Type<typename std::decay<B>::type>::type ST2toT2B;

     public:
      typedef typename ResultType<T2toST2A, ST2toT2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result,
               T2toST2ST2toT2ProductExpr<ST2toST2Traits<Result>::dime>>>::type
          Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tot2-stensor
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<ST2toT2Tag, StensorTag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename ST2toT2Type<typename std::decay<A>::type>::type ST2toT2A;
      typedef typename StensorType<typename std::decay<B>::type>::type StensB;

     public:
      typedef typename ResultType<ST2toT2A, StensB, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ST2toT2StensorProductExpr<TensorTraits<Result>::dime>>>::
          type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor-st2tot2
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<TensorTag, ST2toT2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename TensorType<typename std::decay<A>::type>::type TensA;
      typedef typename ST2toT2Type<typename std::decay<B>::type>::type ST2toT2B;

     public:
      typedef typename ResultType<TensA, ST2toT2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, TensorST2toT2ProductExpr<StensorTraits<Result>::dime>>>::
          type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for st2tot2s
     */
    template <typename A>
    struct ComputeUnaryResult_<ST2toT2Tag, UnaryOperatorTag, A, OpNeg> {
      struct DummyHandle {};
      typedef typename ST2toT2Type<typename std::decay<A>::type>::type StensA;

     public:
      typedef typename UnaryResultType<StensA, OpNeg>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, UnaryOperation<A, OpNeg>>>::type Handle;
    };

    template <typename T1, typename T2>
    TFEL_MATH_INLINE typename std::enable_if<
        tfel::meta::Implements<T1, TensorConcept>::cond &&
            tfel::meta::Implements<T2, ST2toT2Concept>::cond &&
            !tfel::typetraits::IsInvalid<
                typename ComputeBinaryResult<T1, T2, OpMult>::Result>::cond,
        typename ComputeBinaryResult<T1, T2, OpMult>::Handle>::type
    operator|(const T1& a, const T2& b) {
      typedef typename ComputeBinaryResult<T1, T2, OpMult>::Handle Handle;
      return Handle(a, b);
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_ST2TOT2_CONCEPT_OPERATIONS_HXX */
