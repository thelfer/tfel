/*!
 * \file   include/TFEL/Math/Matrix/MatrixConceptOperations.hxx
 * \brief  This file implements operations that can be applied to matrices.
 * \author Thomas Helfer
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATRIX_OPERATIONS_HXX
#define LIB_TFEL_MATRIX_OPERATIONS_HXX

#include <cmath>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"

namespace tfel {

  namespace math {

    template <typename T>
    struct MatrixType {
      typedef T type;
    };

    template <typename T_type, typename Operation>
    struct MatrixType<Expr<T_type, Operation>> {
      typedef T_type type;
    };

    template <typename T_type, typename Operation>
    struct MatrixTraits<Expr<T_type, Operation>> {
      typedef typename MatrixTraits<T_type>::NumType NumType;
      typedef typename MatrixTraits<T_type>::IndexType IndexType;
    };

    template <typename AType, typename BType, typename A, typename B>
    class MatrixVectorHandle {
      struct DummyHandle {};

     public:
      typedef DummyHandle type;
    };

    template <typename AType, typename BType, typename A, typename B>
    class VectorMatrixHandle {
      struct DummyHandle {};

     public:
      typedef DummyHandle type;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for matrix's operation
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<MatrixTag, MatrixTag, A, B, Op> {
      struct DummyHandle {};
      typedef typename MatrixType<typename std::decay<A>::type>::type MatA;
      typedef typename MatrixType<typename std::decay<B>::type>::type MatB;

     public:
      typedef typename ResultType<MatA, MatB, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, BinaryOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-matrix
     * operations
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<ScalarTag, MatrixTag, A, B, Op> {
      struct DummyHandle {};
      typedef typename MatrixType<typename std::decay<B>::type>::type MatB;

     public:
      typedef typename ResultType<A, MatB, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ScalarObjectOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for matrix-scalar
     * operations
     */
    template <typename A, typename B, typename Op>
    struct ComputeBinaryResult_<MatrixTag, ScalarTag, A, B, Op> {
      struct DummyHandle {};
      typedef typename MatrixType<typename std::decay<A>::type>::type MatA;

     public:
      typedef typename ResultType<MatA, B, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ObjectScalarOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for matrix-vector
     * multiplication
     */
    template <typename A, typename B>
    struct ComputeBinaryResult_<MatrixTag, VectorTag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename MatrixType<typename std::decay<A>::type>::type MatA;
      typedef typename VectorType<typename std::decay<B>::type>::type VecB;

     public:
      typedef typename ResultType<MatA, VecB, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          typename MatrixVectorHandle<MatA, VecB, A, B>::type>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for vector-matrix
     * multiplication
     */
    template <typename A, typename B>
    struct ComputeBinaryResult_<VectorTag, MatrixTag, A, B, OpMult> {
      struct DummyHandle {};
      typedef typename VectorType<typename std::decay<A>::type>::type VecA;
      typedef typename MatrixType<typename std::decay<B>::type>::type MatB;

     public:
      typedef typename ResultType<VecA, MatB, OpMult>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          typename MatrixVectorHandle<VecA, MatB, A, B>::type>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for matrices
     */
    template <typename A>
    class ComputeUnaryResult_<MatrixTag, UnaryOperatorTag, A, OpNeg> {
      struct DummyHandle {};
      typedef typename MatrixType<typename std::decay<A>::type>::type MatA;

     public:
      typedef typename UnaryResultType<MatA, OpNeg>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, UnaryOperation<A, OpNeg>>>::type Handle;
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATRIX_OPERATIONS_HXX */
