/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to t2tot2.
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_T2TOT2_CONCEPT_OPERATIONS_HXX_
#define LIB_TFEL_T2TOT2_CONCEPT_OPERATIONS_HXX_

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/T2toT2/T2toT2TensorProductExpr.hxx"
#include "TFEL/Math/T2toT2/TensorT2toT2ProductExpr.hxx"
#include "TFEL/Math/T2toT2/T2toT2T2toT2ProductExpr.hxx"

namespace tfel {

  namespace math {

    template <typename T_type, typename Operation>
    struct T2toT2Type<Expr<T_type, Operation>> {
      typedef T_type type;
    };

    template <typename T_type, typename Operation>
    struct T2toT2Traits<Expr<T_type, Operation>> {
      typedef typename T2toT2Traits<T_type>::NumType NumType;
      static constexpr unsigned short dime = T2toT2Traits<T_type>::dime;
    };

    /*!
     * \brief partial specialisation of ComputeBinaryResult_ for t2tot2's
     * operation \tparam A:  left hand side \tparam B:  right hand side \tparam
     * Op: operation
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<T2toT2Tag, T2toT2Tag, A, B, Op> {
      struct DummyHandle {};
      typedef
          typename T2toT2Type<typename std::decay<A>::type>::type T2toT2TypeA;
      typedef
          typename T2toT2Type<typename std::decay<B>::type>::type T2toT2TypeB;

     public:
      typedef typename ResultType<T2toT2TypeA, T2toT2TypeB, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, BinaryOperation<A, B, Op>>>::type Handle;
    };

    /*!
     * \brief partial specialisation of ComputeBinaryResult_ for scalar-t2tot2
     * operations \tparam A:  left hand side \tparam B:  right hand side \tparam
     * Op: operation
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<ScalarTag, T2toT2Tag, A, B, Op> {
      struct DummyHandle {};
      typedef
          typename T2toT2Type<typename std::decay<B>::type>::type T2toT2TypeB;

     public:
      typedef typename ResultType<A, T2toT2TypeB, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ScalarObjectOperation<A, B, Op>>>::type Handle;
    };

    /*!
     * \brief partial specialisation of ComputeBinaryResult_ for t2tot2-scalar
     * operations \tparam A:  left hand side \tparam B:  right hand side \tparam
     * Op: operation
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<T2toT2Tag, ScalarTag, A, B, Op> {
      struct DummyHandle {};
      typedef
          typename T2toT2Type<typename std::decay<A>::type>::type T2toT2TypeA;

     public:
      typedef typename ResultType<T2toT2TypeA, B, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ObjectScalarOperation<A, B, Op>>>::type Handle;
    };

    /*!
     * \brief partial specialisation of ComputeBinaryResult_ for T2toT2-T2toT2
     * operations \tparam A:  left hand side \tparam B:  right hand side \tparam
     * Op: operation
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<T2toT2Tag, T2toT2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename T2toT2Type<typename std::decay<A>::type>::type T2toT2A;
      typedef typename T2toT2Type<typename std::decay<B>::type>::type T2toT2B;

     public:
      typedef typename ResultType<T2toT2A, T2toT2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, T2toT2T2toT2ProductExpr<T2toT2Traits<Result>::dime>>>::
          type Handle;
    };
    /*!
     * \brief partial specialisation of ComputeBinaryResult_ for T2toT2-Tensor
     * operations \tparam A:  left hand side \tparam B:  right hand side \tparam
     * Op: operation
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<T2toT2Tag, TensorTag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename T2toT2Type<typename std::decay<A>::type>::type T2toT2A;
      typedef typename TensorType<typename std::decay<B>::type>::type TensB;

     public:
      typedef typename ResultType<T2toT2A, TensB, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, T2toT2TensorProductExpr<TensorTraits<Result>::dime>>>::
          type Handle;
    };

    /*
     * \brief partial specialisation of ComputeBinaryResult_ for Tensor-T2toT2
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<TensorTag, T2toT2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename TensorType<typename std::decay<A>::type>::type TensA;
      typedef typename T2toT2Type<typename std::decay<B>::type>::type T2toT2B;

     public:
      typedef typename ResultType<TensA, T2toT2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, TensorT2toT2ProductExpr<TensorTraits<Result>::dime>>>::
          type Handle;
    };
    /*!
     * \brief partial specialisation of `ComputeUnaryResult_` for
     * `T2toT2`'s object
     * \tparam A: object
     */
    template <typename A>
    struct ComputeUnaryResult_<T2toT2Tag, UnaryOperatorTag, A, OpNeg> {
      struct DummyHandle {};
      typedef
          typename T2toT2Type<typename std::decay<A>::type>::type T2toT2TypeA;

     public:
      typedef typename UnaryResultType<T2toT2TypeA, OpNeg>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, UnaryOperation<A, OpNeg>>>::type Handle;
    };

    template <typename T1, typename T2>
    TFEL_MATH_INLINE typename std::enable_if<
        tfel::meta::Implements<T1, TensorConcept>::cond &&
            tfel::meta::Implements<T2, T2toT2Concept>::cond &&
            !tfel::typetraits::IsInvalid<
                typename ComputeBinaryResult<T1, T2, OpMult>::Result>::cond,
        typename ComputeBinaryResult<T1, T2, OpMult>::Handle>::type
    operator|(const T1& a, const T2& b) {
      typedef typename ComputeBinaryResult<T1, T2, OpMult>::Handle Handle;
      return Handle(a, b);
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_T2TOT2_CONCEPT_OPERATIONS_HXX_ */
