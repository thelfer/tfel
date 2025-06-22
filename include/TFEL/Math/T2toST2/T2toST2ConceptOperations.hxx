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

#ifndef LIB_TFEL_T2TOST2_CONCEPT_OPERATIONS_HXX
#define LIB_TFEL_T2TOST2_CONCEPT_OPERATIONS_HXX

#include <cmath>

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/HasRandomAccessConstIterator.hxx"

#include "TFEL/TypeTraits/IsScalar.hxx"

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

namespace tfel {

  namespace math {

    template <typename T_type, typename Operation>
    struct T2toST2Type<Expr<T_type, Operation>> {
      typedef T_type type;
    };

    template <typename T_type, typename Operation>
    struct T2toST2Traits<Expr<T_type, Operation>> {
      typedef typename T2toST2Traits<T_type>::NumType NumType;
      static constexpr unsigned short dime = T2toST2Traits<T_type>::dime;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for t2tost2's operation
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<T2toST2Tag, T2toST2Tag, A, B, Op> {
      struct DummyHandle {};
      typedef typename T2toST2Type<typename std::decay<A>::type>::type StensA;
      typedef typename T2toST2Type<typename std::decay<B>::type>::type StensB;

     public:
      typedef typename ResultType<StensA, StensB, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, BinaryOperation<A, B, Op>>>::type Handle;
    };

    /*!
     * Partial Specialisation of ComputeBinaryResult_ for the
     * multiplication of object of type st2tost2 by object of type
     * t2tost2.
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<ST2toST2Tag, T2toST2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef
          typename ST2toST2Type<typename std::decay<A>::type>::type ST2toST2A;
      typedef typename T2toST2Type<typename std::decay<B>::type>::type T2toST2B;

     public:
      typedef typename ResultType<ST2toST2A, T2toST2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result,
               ST2toST2T2toST2ProductExpr<T2toST2Traits<Result>::dime>>>::type
          Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for t2tost2-t2tot2
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<T2toST2Tag, T2toT2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename T2toST2Type<typename std::decay<A>::type>::type T2toST2A;
      typedef typename T2toT2Type<typename std::decay<B>::type>::type T2toT2B;

     public:
      typedef typename ResultType<T2toST2A, T2toT2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, T2toST2T2toT2ProductExpr<T2toST2Traits<Result>::dime>>>::
          type Handle;
    };

    /*!
     * Partial Specialisation of ComputeBinaryResult_ for stensor's operation
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<StensorTag, TensorTag, A, B, OpDiadicProduct> {
      struct DummyHandle {};
      typedef typename StensorType<typename std::decay<A>::type>::type StensA;
      typedef typename TensorType<typename std::decay<B>::type>::type TensB;

     public:
      typedef typename ResultType<StensA, TensB, OpDiadicProduct>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, DiadicProductOperation<A, B>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for T2toST2-Tensor
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<T2toST2Tag, TensorTag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename T2toST2Type<typename std::decay<A>::type>::type T2toST2A;
      typedef typename TensorType<typename std::decay<B>::type>::type TensB;

     public:
      typedef typename ResultType<T2toST2A, TensB, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, T2toST2TensorProductExpr<StensorTraits<Result>::dime>>>::
          type Handle;
    };

    /*!
     * \brief partial specialisation of ComputeBinaryResult_ for Stensor-T2toST2
     * operations
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<StensorTag, T2toST2Tag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename StensorType<typename std::decay<A>::type>::type StensA;
      typedef typename T2toST2Type<typename std::decay<B>::type>::type T2toST2B;

     public:
      typedef typename ResultType<StensA, T2toST2B, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, StensorT2toST2ProductExpr<TensorTraits<Result>::dime>>>::
          type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-t2tost2
     * operations
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<ScalarTag, T2toST2Tag, A, B, Op> {
      struct DummyHandle {};
      typedef typename std::decay<A>::type ScalarA;
      typedef typename T2toST2Type<typename std::decay<B>::type>::type T2toST2B;

     public:
      typedef typename ResultType<ScalarA, T2toST2B, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ScalarObjectOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for t2tost2-scalar
     * operations
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<T2toST2Tag, ScalarTag, A, B, Op> {
      struct DummyHandle {};
      typedef typename T2toST2Type<typename std::decay<A>::type>::type StensA;

     public:
      typedef typename ResultType<StensA, B, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ObjectScalarOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for t2tost2s
     */
    template <typename A>
    struct ComputeUnaryResult_<T2toST2Tag, UnaryOperatorTag, A, OpNeg> {
      struct DummyHandle {};
      typedef typename T2toST2Type<typename std::decay<A>::type>::type T2toST2A;

     public:
      typedef typename UnaryResultType<T2toST2A, OpNeg>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, UnaryOperation<A, OpNeg>>>::type Handle;
    };

    template <typename T1, typename T2>
    TFEL_MATH_INLINE typename std::enable_if<
        tfel::meta::Implements<T1, StensorConcept>::cond &&
            tfel::meta::Implements<T2, T2toST2Concept>::cond &&
            !tfel::typetraits::IsInvalid<
                typename ComputeBinaryResult<T1, T2, OpMult>::Result>::cond,
        typename ComputeBinaryResult<T1, T2, OpMult>::Handle>::type
    operator|(const T1& a, const T2& b) {
      typedef typename ComputeBinaryResult<T1, T2, OpMult>::Handle Handle;
      return Handle(a, b);
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_T2TOST2_CONCEPT_OPERATIONS_HXX */
