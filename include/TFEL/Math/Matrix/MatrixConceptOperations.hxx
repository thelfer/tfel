/*!
 * \file   include/TFEL/Math/Matrix/MatrixConceptOperations.hxx
 * \brief  This file implements operations that can be applied to matrices.
 * \author Thomas Helfer
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATRIX_OPERATIONS_HXX
#define LIB_TFEL_MATRIX_OPERATIONS_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"

namespace tfel::math {

  template <typename AType, typename BType, typename A, typename B>
  class MatrixVectorHandle {
    struct DummyHandle {};

   public:
    using type = DummyHandle;
  };

  template <typename AType, typename BType, typename A, typename B>
  class VectorMatrixHandle {
    struct DummyHandle {};

   public:
    using type = DummyHandle;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for matrix's operation
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<MatrixTag, MatrixTag, A, B, Op> {
    struct DummyHandle {};
    using MatrixTypeA = EvaluationResult<A>;
    using MatrixTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<MatrixTypeA, MatrixTypeB, Op>;
    using Handle = std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                                      DummyHandle,
                                      Expr<Result, BinaryOperation<A, B, Op>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for scalar-matrix operations
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<ScalarTag, MatrixTag, A, B, Op> {
    struct DummyHandle {};
    using MatrixTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<A, MatrixTypeB, Op>;
    using Handle =
        std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                           DummyHandle,
                           Expr<Result, ScalarObjectOperation<A, B, Op>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for matrix-scalar operations
   */
  template <typename A, typename B, typename Op>
  struct ComputeBinaryResult_<MatrixTag, ScalarTag, A, B, Op> {
    struct DummyHandle {};
    using MatrixTypeA = EvaluationResult<A>;

   public:
    using Result = result_type<MatrixTypeA, B, Op>;
    using Handle =
        std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                           DummyHandle,
                           Expr<Result, ObjectScalarOperation<A, B, Op>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for matrix-vector
   * multiplication
   */
  template <typename A, typename B>
  struct ComputeBinaryResult_<MatrixTag, VectorTag, A, B, OpMult> {
    struct DummyHandle {};
    using MatrixTypeA = EvaluationResult<A>;
    using VectorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<MatrixTypeA, VectorTypeB, OpMult>;
    using Handle = std::conditional_t<
        tfel::typetraits::IsInvalid<Result>::cond,
        DummyHandle,
        typename MatrixVectorHandle<MatrixTypeA, VectorTypeB, A, B>::type>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for vector-matrix
   * multiplication
   */
  template <typename A, typename B>
  struct ComputeBinaryResult_<VectorTag, MatrixTag, A, B, OpMult> {
    struct DummyHandle {};
    using VectorTypeA = EvaluationResult<A>;
    using MatrixTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<VectorTypeA, MatrixTypeB, OpMult>;
    using Handle = std::conditional_t<
        tfel::typetraits::IsInvalid<Result>::cond,
        DummyHandle,
        typename MatrixVectorHandle<VectorTypeA, MatrixTypeB, A, B>::type>;
  };

  /*
   * Partial Specialisation of ComputeUnaryResult_ for matrices
   */
  template <typename A>
  class ComputeUnaryResult_<MatrixTag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    using MatrixTypeA = EvaluationResult<A>;

   public:
    using Result = typename UnaryResultType<MatrixTypeA, OpNeg>::type;
    using Handle = std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATRIX_OPERATIONS_HXX */
