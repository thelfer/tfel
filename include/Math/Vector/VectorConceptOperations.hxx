/*!
 * \file   VectorConceptOperations.hxx
 * \brief  This file implements operations that can be applied to matrices.
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#ifndef _LIB_TFEL_VECTOR_OPERATIONS_HXX_
#define _LIB_TFEL_VECTOR_OPERATIONS_HXX_ 

#include <cmath>

#include "Config/TFELConfig.hxx"

#include "Metaprogramming/Implements.hxx"
#include "Metaprogramming/IF.hxx"
#include "Metaprogramming/HasRandomAccessConstIterator.hxx"

#include "TypeTraits/IsScalar.hxx"

#include "Math/General/ResultType.hxx"
#include "Math/General/BasicOperations.hxx"

#include"Math/General/ComputeBinaryResult.hxx"
#include"Math/General/ComputeUnaryResult.hxx"
#include"Math/Function/StandardFunctions.hxx"

#include "Math/Vector/VectorExpr.hxx"
#include "Math/Vector/FctVectorExpr.hxx"
#include "Math/Vector/ScalarVectorExpr.hxx"
#include "Math/Vector/VectorVectorExpr.hxx"
#include "Math/Vector/VectorVectorDiadicProductExpr.hxx"
#include "Math/Vector/VectorNegExpr.hxx"
#include "Math/Vector/VectorExpr.hxx"
#include "Math/Vector/FctVectorExprWithoutConstIterator.hxx"
#include "Math/Vector/ScalarVectorExprWithoutConstIterator.hxx"
#include "Math/Vector/VectorVectorExprWithoutConstIterator.hxx"
#include "Math/Vector/VectorNegExprWithoutConstIterator.hxx"
#include "Math/Matrix/MatrixExpr.hxx"

namespace tfel{

  namespace math{

    template<typename T1,typename T2>
    class VectorDotProductHandle
    {
      struct DummyHandle
      {};
    public:
      typedef DummyHandle type;
    };

    /*									                
     * Partial Specialisation of ComputeUnaryResult_ for vectors	                
     */									                
    template<typename A,typename Fct>
    class ComputeUnaryResult_<VectorTag,FunctionTag,A,Fct>
    {
      struct DummyHandle{};
      typedef typename VectorType<A>::type VectA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
                                      FctVectorExpr<A,Fct>,
                                      FctVectorExprWithoutConstIterator<A,Fct>
				      >::type Expr;
    public:
      typedef typename UnaryResultType<VectA,Fct>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      VectorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for vector's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<VectorTag,VectorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename VectorType<A>::type VecA;
      typedef typename VectorType<B>::type VecB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond&&
                                      tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      VectorVectorExpr<A,B,Op>,
				      VectorVectorExprWithoutConstIterator<A,B,Op>
				      >::type Expr;
    public:
      typedef typename ResultType<VecA,VecB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      VectorExpr<Result,Expr> >::type Handle;
    };

    template<typename A, typename B>
    class ComputeBinaryResult_<VectorTag,VectorTag,A,B,OpDotProduct>
    {
      struct DummyHandle{};
      typedef typename VectorType<A>::type VecA;
      typedef typename VectorType<B>::type VecB;
    public:
      typedef typename ResultType<VecA,VecB,OpDotProduct>::type Result;
      typedef typename VectorDotProductHandle<VecA,VecB>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for vector's operation
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<VectorTag,VectorTag,A,B,OpDiadicProduct>
    {
      struct DummyHandle{};
      typedef typename VectorType<A>::type VecA;
      typedef typename VectorType<B>::type VecB;
      typedef VectorVectorDiadicProductExpr<A,B> Expr;
    public:
      typedef typename ResultType<VecA,VecB,OpDiadicProduct>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      MatrixExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-vector operations
     */
    template<typename A, typename B,typename Op>
    struct ComputeBinaryResult_<ScalarTag,VectorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename VectorType<B>::type      VectB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      ScalarVectorExpr<A,B,Op>,
				      ScalarVectorExprWithoutConstIterator<A,B,Op>
                                     >::type Expr;
    public:
      typedef typename ResultType<A,VectB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      VectorExpr<Result,Expr> >::type Handle;
    };
    
    /*
     * Partial Specialisation of ComputeBinaryResult_ for vector-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<VectorTag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename VectorType<A>::type      VectA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      VectorScalarExpr<A,B,Op>,
				      VectorScalarExprWithoutConstIterator<A,B,Op>
                                     >::type Expr;
    public:
      typedef typename ResultType<VectA,B,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      VectorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for vectors
     */
    template<typename A>
    class ComputeUnaryResult_<VectorTag,UnaryOperatorTag,A,OpNeg>
    {
      struct DummyHandle{};
      typedef typename VectorType<A>::type VectA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      VectorNegExpr<A>,
				      VectorNegExprWithoutConstIterator<A>
				      >::type Expr;
    public:
      typedef typename UnaryResultType<VectA,OpNeg>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      VectorExpr<Result,Expr> >::type Handle;
    };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle
    >::type
    operator ^ (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1&,const T2&);
    
    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1, const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1&,const T2);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1&,const T2);

    template<typename T1>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1&);

    /*!
     * \return the inner product of a tvector
     * \param const T1&, the left  vector.
     * \param const T2&, the right vector.
     * \return const typename ResultType<T,T2,OpMult>::type, the
     * result.
     * \warning the operator| has not the priority expected for such
     * an operation : use of parenthesis is required.
     */
    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result
    >::type
    operator | (const T1&, const T2&);

  } // end of namespace math

} // end of namespace tfel

#include"Math/Vector/VectorConceptOperations.ixx"
  
#endif /* _LIB_TFEL_VECTOR_OPERATIONS_HXX */

