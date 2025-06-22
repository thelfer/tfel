/*!
 * \file   include/TFEL/Math/ST2toST2/ST2toST2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to st2tost2.
 * \author Thomas Helfer
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
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

namespace tfel {

  namespace math {

    template <typename T_type, typename Operation>
    struct ST2toST2Type<Expr<T_type, Operation>> {
      typedef T_type type;
    };

    template <typename T_type, typename Operation>
    struct ST2toST2Traits<Expr<T_type, Operation>> {
      typedef typename ST2toST2Traits<T_type>::NumType NumType;
      static constexpr unsigned short dime = ST2toST2Traits<T_type>::dime;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tost2's operation
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<ST2toST2Tag, ST2toST2Tag, A, B, Op> {
      struct DummyHandle {};
      typedef
          typename ST2toST2Type<typename std::decay<A>::type>::type ST2toST2A;
      typedef
          typename ST2toST2Type<typename std::decay<B>::type>::type ST2toST2B;

     public:
      typedef typename ResultType<ST2toST2A, ST2toST2B, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, BinaryOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-st2tost2
     * operations
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<ScalarTag, ST2toST2Tag, A, B, Op> {
      struct DummyHandle {};
      typedef
          typename ST2toST2Type<typename std::decay<B>::type>::type ST2toST2B;

     public:
      typedef typename ResultType<A, ST2toST2B, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ScalarObjectOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tost2-scalar
     * operations
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<ST2toST2Tag, ScalarTag, A, B, Op> {
      struct DummyHandle {};
      typedef
          typename ST2toST2Type<typename std::decay<A>::type>::type ST2toST2A;

     public:
      typedef typename ResultType<ST2toST2A, B, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ObjectScalarOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tost2-st2tost2
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<ST2toST2Tag, ST2toST2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef
          typename ST2toST2Type<typename std::decay<A>::type>::type ST2toST2A;
      typedef
          typename ST2toST2Type<typename std::decay<B>::type>::type ST2toST2B;

     public:
      typedef typename ResultType<ST2toST2A, ST2toST2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result,
               ST2toST2ST2toST2ProductExpr<ST2toST2Traits<Result>::dime>>>::type
          Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tost2-stensor
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<ST2toST2Tag, StensorTag, A, B, OpMult> {
      struct DummyHandle {};
      typedef
          typename ST2toST2Type<typename std::decay<A>::type>::type ST2toST2A;
      typedef typename StensorType<typename std::decay<B>::type>::type StensB;

     public:
      typedef typename ResultType<ST2toST2A, StensB, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result,
               ST2toST2StensorProductExpr<StensorTraits<Result>::dime>>>::type
          Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for stensor-st2tost2
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<StensorTag, ST2toST2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename StensorType<typename std::decay<A>::type>::type StensA;
      typedef
          typename ST2toST2Type<typename std::decay<B>::type>::type ST2toST2B;

     public:
      typedef typename ResultType<StensA, ST2toST2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result,
               StensorST2toST2ProductExpr<StensorTraits<Result>::dime>>>::type
          Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for st2tost2s
     */
    template <typename A>
    struct ComputeUnaryResult_<ST2toST2Tag, UnaryOperatorTag, A, OpNeg> {
      struct DummyHandle {};
      typedef
          typename ST2toST2Type<typename std::decay<A>::type>::type ST2toST2A;

     public:
      typedef typename UnaryResultType<ST2toST2A, OpNeg>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, UnaryOperation<A, OpNeg>>>::type Handle;
    };

    template <typename T1, typename T2>
    TFEL_MATH_INLINE typename std::enable_if<
        tfel::meta::Implements<T1, StensorConcept>::cond &&
            tfel::meta::Implements<T2, ST2toST2Concept>::cond &&
            !tfel::typetraits::IsInvalid<
                typename ComputeBinaryResult<T1, T2, OpMult>::Result>::cond,
        typename ComputeBinaryResult<T1, T2, OpMult>::Handle>::type
    operator|(const T1& a, const T2& b) {
      typedef typename ComputeBinaryResult<T1, T2, OpMult>::Handle Handle;
      return Handle(a, b);
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_ST2TOST2_CONCEPT_OPERATIONS_HXX */
