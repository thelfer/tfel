/*!
 * \file   include/TFEL/Math/Vector/VectorConceptOperations.hxx
 * \brief  This file implements operations that can be applied to matrices.
 * \author Helfer Thomas
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_VECTOR_OPERATIONS_HXX_
#define LIB_TFEL_VECTOR_OPERATIONS_HXX_

#include <cmath>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"

#include "TFEL/TypeTraits/RealPartType.hxx"

#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/Vector/VectorVectorDotProduct.hxx"

namespace tfel {

  namespace math {

    template <typename T>
    struct VectorType {
      typedef T type;
    };

    template <typename Result, typename Operation>
    struct VectorType<Expr<Result, Operation>> {
      typedef Result type;
    };

    template <typename T_type, typename Operation>
    struct VectorTraits<Expr<T_type, Operation>> {
      typedef typename VectorTraits<T_type>::NumType NumType;
      typedef typename VectorTraits<T_type>::IndexType IndexType;
      typedef
          typename VectorTraits<T_type>::RunTimeProperties RunTimeProperties;
    };

    template <typename T1, typename T2>
    struct VectorDotProductHandle {
      typedef VectorVectorDotProduct type;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for vector's operation
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<VectorTag, VectorTag, A, B, Op> {
      struct DummyHandle {};
      typedef typename VectorType<typename std::decay<A>::type>::type VecA;
      typedef typename VectorType<typename std::decay<B>::type>::type VecB;

     public:
      typedef typename ResultType<VecA, VecB, Op>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, BinaryOperation<A, B, Op>>>::type Handle;
    };

    template <typename A, typename B>
    class ComputeBinaryResult_<VectorTag, VectorTag, A, B, OpDotProduct> {
      struct DummyHandle {};
      typedef typename VectorType<typename std::decay<A>::type>::type VecA;
      typedef typename VectorType<typename std::decay<B>::type>::type VecB;

     public:
      typedef typename ResultType<VecA, VecB, OpDotProduct>::type Result;
      typedef typename VectorDotProductHandle<VecA, VecB>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for vector's operation
     */
    template <typename A, typename B>
    class ComputeBinaryResult_<VectorTag, VectorTag, A, B, OpDiadicProduct> {
      struct DummyHandle {};
      typedef typename VectorType<typename std::decay<A>::type>::type VecA;
      typedef typename VectorType<typename std::decay<A>::type>::type VecB;

     public:
      typedef typename ResultType<VecA, VecB, OpDiadicProduct>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, DiadicProductOperation<A, B>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-vector
     * operations
     */
    template <typename A, typename B, typename Op>
    struct ComputeBinaryResult_<ScalarTag, VectorTag, A, B, Op> {
      struct DummyHandle {};
      typedef typename VectorType<typename std::decay<B>::type>::type VectB;

     public:
      typedef typename ResultType<typename std::decay<A>::type, VectB, Op>::type
          Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ScalarObjectOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for vector-scalar
     * operations
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<VectorTag, ScalarTag, A, B, Op> {
      struct DummyHandle {};
      typedef typename VectorType<typename std::decay<A>::type>::type VectA;

     public:
      typedef typename ResultType<VectA, typename std::decay<B>::type, Op>::type
          Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, ObjectScalarOperation<A, B, Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for vectors
     */
    template <typename A>
    class ComputeUnaryResult_<VectorTag, UnaryOperatorTag, A, OpNeg> {
      struct DummyHandle {};
      typedef typename VectorType<typename std::decay<A>::type>::type VectA;

     public:
      typedef typename UnaryResultType<VectA, OpNeg>::type Result;
      typedef typename std::conditional<
          tfel::typetraits::IsInvalid<Result>::cond,
          DummyHandle,
          Expr<Result, UnaryOperation<A, OpNeg>>>::type Handle;
    };

    template <typename T1, typename T2, typename Op>
    struct IsVectorVectorOperationValid {
      static constexpr bool cond =
          (tfel::meta::Implements<T1, VectorConcept>::cond &&
           tfel::meta::Implements<T2, VectorConcept>::cond &&
           (!tfel::typetraits::IsInvalid<
               typename ComputeBinaryResult<T1, T2, Op>::Result>::cond));
    };

    template <typename T1, typename T2, typename Op>
    struct IsScalarVectorOperationValid {
      static constexpr bool cond =
          (tfel::typetraits::IsScalar<T1>::cond &&
           tfel::meta::Implements<T2, VectorConcept>::cond &&
           (!tfel::typetraits::IsInvalid<
               typename ComputeBinaryResult<T1, T2, Op>::Result>::cond));
    };

    template <typename T1>
    struct IsEuclidianNormValid {
      static constexpr bool cond =
          tfel::meta::Implements<T1, VectorConcept>::cond &&
          !tfel::typetraits::IsInvalid<typename tfel::typetraits::RealPartType<
              typename ComputeBinaryResult<T1, T1, OpDotProduct>::Result>::
                                           type>::cond;
    };

    /*!
     * \return the inner product of a vector
     * \param const T1&, the left  vector.
     * \param const T2&, the right vector.
     * \return const typename ResultType<T,T2,OpMult>::type, the
     * result.
     * \warning the operator| has not the priority expected for such
     * an operation : use of parenthesis is required.
     */
    template <typename T1, typename T2>
    typename std::enable_if<
        IsVectorVectorOperationValid<T1, T2, OpDotProduct>::cond,
        typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::type
    operator|(const T1&, const T2&);

    /*!
     * \brief  return the euclidian norm of a tvector
     * \param  v : the vector.
     * \return const typename tfel::typetraits::RealPartType<T>::type, the
     * result
     */
    template <typename T1>
    typename std::enable_if<
        IsEuclidianNormValid<T1>::cond,
        typename tfel::typetraits::RealPartType<
            typename ComputeBinaryResult<T1, T1, OpDotProduct>::Result>::type>::
        type
        norm(const T1&);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Vector/VectorConceptOperations.ixx"

#endif /* LIB_TFEL_VECTOR_OPERATIONS_HXX_ */
