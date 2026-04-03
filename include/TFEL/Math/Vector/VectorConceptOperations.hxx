/*!
 * \file   include/TFEL/Math/Vector/VectorConceptOperations.hxx
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

#ifndef LIB_TFEL_MATH_VECTORCONCEPTOPERATIONS_HXX
#define LIB_TFEL_MATH_VECTORCONCEPTOPERATIONS_HXX

#include <cmath>
#include <type_traits>
#include "TFEL/Math/power.hxx"
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/RealPartType.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/Vector/VectorVectorDotProduct.hxx"

namespace tfel::math {

  template <typename T1, typename T2>
  struct VectorDotProductHandle {
    using type = VectorVectorDotProduct;
  };

  template <typename A, typename B>
  class ComputeBinaryOperationHandler<VectorTag,
                                      VectorTag,
                                      A,
                                      B,
                                      OpDotProduct> {
    struct DummyHandle {};
    using VectorTypeA = EvaluationResult<A>;
    using VectorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<VectorTypeA, VectorTypeB, OpDotProduct>;
    using Handle =
        typename VectorDotProductHandle<VectorTypeA, VectorTypeB>::type;
  };

  /*
   * Partial Specialisation of ComputeBinaryOperationHandler for vector's
   * operation
   */
  template <typename A, typename B>
  class ComputeBinaryOperationHandler<VectorTag,
                                      VectorTag,
                                      A,
                                      B,
                                      OpDiadicProduct> {
    struct DummyHandle {};
    using VectorTypeA = EvaluationResult<A>;
    using VectorTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<VectorTypeA, VectorTypeB, OpDiadicProduct>;
    using Handle =
        std::conditional_t<isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, DiadicProductOperation<A, B>>>;
  };

  /*
   * Partial Specialisation of ComputeUnaryResult_ for vectors
   */
  template <typename A>
  class ComputeUnaryResult_<VectorTag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    using VectorTypeA = EvaluationResult<A>;

   public:
    using Result = typename UnaryResultType<VectorTypeA, OpNeg>::type;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

  /*!
   * \return the inner product of a vector
   * \param const T1&, the left  vector.
   * \param const T2&, the right vector.
   * \return const result_type<T,T2,OpMult>, the
   * result.
   * \warning the operator| has not the priority expected for such
   * an operation : use of parenthesis is required.
   */
  template <VectorConcept T1, VectorConcept T2>
  TFEL_HOST_DEVICE constexpr auto operator|(const T1&, const T2&) requires(
      !isInvalid<BinaryOperationResult<T1, T2, OpDotProduct>>());

  /*!
   * \brief  return the euclidian norm of a tvector
   * \param  v : the vector.
   * \return const typename tfel::typetraits::RealPartType<T>::type, the result
   */
  template <VectorConcept T1>
  TFEL_HOST_DEVICE auto norm(const T1& v) requires(
      !isInvalid<BinaryOperationResult<T1, T1, OpDotProduct>>());

}  // end of namespace tfel::math

#include "TFEL/Math/Vector/VectorConceptOperations.ixx"

#endif /* LIB_TFEL_MATH_VECTORCONCEPTOPERATIONS_HXX */
