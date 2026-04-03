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

#ifndef LIB_TFEL_MATH_MATRIX_OPERATIONS_HXX
#define LIB_TFEL_MATH_MATRIX_OPERATIONS_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
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

  template <typename AType, typename BType, typename A, typename B>
  class MatrixMatrixHandle {
    struct DummyHandle {};

   public:
    using type = DummyHandle;
  };

  /*!
   * \brief partial specialisation of ComputeBinaryOperationHandler for
   * matrix-vector multiplication
   */
  template <typename A, typename B>
  struct ComputeBinaryOperationHandler<MatrixTag, VectorTag, A, B, OpMult> {
    struct DummyHandle {};
    using MatrixTypeA = EvaluationResult<A>;
    using VectorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<MatrixTypeA, VectorTypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        typename MatrixVectorHandle<MatrixTypeA, VectorTypeB, A, B>::type>;
  };

  /*!
   * \brief partial specialisation of ComputeBinaryOperationHandler for
   * vector-matrix multiplication
   */
  template <typename A, typename B>
  struct ComputeBinaryOperationHandler<VectorTag, MatrixTag, A, B, OpMult> {
    struct DummyHandle {};
    using VectorTypeA = EvaluationResult<A>;
    using MatrixTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<VectorTypeA, MatrixTypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        typename MatrixVectorHandle<VectorTypeA, MatrixTypeB, A, B>::type>;
  };

  /*!
   * \brief partial specialisation of ComputeBinaryOperationHandler for
   * matrix-matrix multiplication
   */
  template <typename A, typename B>
  struct ComputeBinaryOperationHandler<MatrixTag, MatrixTag, A, B, OpMult> {
    struct DummyHandle {};
    using MatrixTypeA = EvaluationResult<A>;
    using MatrixTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<MatrixTypeA, MatrixTypeB, OpMult>;
    using Handle = std::conditional_t<
        isInvalid<Result>(),
        DummyHandle,
        typename MatrixMatrixHandle<MatrixTypeA, MatrixTypeB, A, B>::type>;
  };

  /*!
   * \brief partial specialisation of ComputeUnaryResult_ for matrices
   */
  template <typename A>
  class ComputeUnaryResult_<MatrixTag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    using MatrixTypeA = EvaluationResult<A>;

   public:
    using Result = typename UnaryResultType<MatrixTypeA, OpNeg>::type;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_MATRIX_OPERATIONS_HXX */
