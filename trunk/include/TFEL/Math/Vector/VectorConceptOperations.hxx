/*!
 * \file   VectorConceptOperations.hxx
 * \brief  This file implements operations that can be applied to matrices.
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#ifndef _LIB_TFEL_VECTOR_OPERATIONS_HXX_
#define _LIB_TFEL_VECTOR_OPERATIONS_HXX_ 

#include <cmath>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/HasRandomAccessConstIterator.hxx"

#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/RealPartType.hxx"

#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"

#include"TFEL/Math/General/ComputeBinaryResult.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/Function/StandardFunctions.hxx"

#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/FctMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/FctMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectDiadicProductExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExprWithoutConstIterator.hxx"

#include"TFEL/Math/Vector/VectorExpr.hxx"
#include"TFEL/Math/Vector/VectorVectorDotProduct.hxx"
#include"TFEL/Math/Matrix/MatrixExpr.hxx"

namespace tfel{

  namespace math{

    template<typename T1,typename T2>
    struct VectorDotProductHandle
    {
      typedef VectorVectorDotProduct type;
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
                                      FctMathObjectExpr<VectorConcept,VectorTraits,A,Fct>,
                                      FctMathObjectExprWithoutConstIterator<VectorConcept,
									    VectorTraits,A,Fct>
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
				      MathObjectMathObjectExpr<VectorConcept,
							       VectorTraits,
							       A,B,Op>,
				      MathObjectMathObjectExprWithoutConstIterator<VectorConcept,
										   VectorTraits,
										   A,B,Op>
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
      typedef MathObjectMathObjectDiadicProductExpr<VectorConcept,VectorTraits,A,B> Expr;
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
				      ScalarMathObjectExpr<VectorConcept,VectorTraits,A,B,Op>,
				      ScalarMathObjectExprWithoutConstIterator<VectorConcept,VectorTraits,A,B,Op>
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
				      MathObjectScalarExpr<VectorConcept,VectorTraits,A,B,Op>,
				      MathObjectScalarExprWithoutConstIterator<VectorConcept,VectorTraits,A,B,Op>
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
				      MathObjectNegExpr<VectorConcept,VectorTraits,A>,
				      MathObjectNegExprWithoutConstIterator<VectorConcept,VectorTraits,A>
				      >::type Expr;
    public:
      typedef typename UnaryResultType<VectA,OpNeg>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      VectorExpr<Result,Expr> >::type Handle;
    };

    template<typename T1,typename T2,typename Op>
    struct IsVectorVectorOperationValid
    {
      enum{
	cond = (tfel::meta::Implements<T1,VectorConcept>::cond&&
		tfel::meta::Implements<T2,VectorConcept>::cond&&
		(!tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,Op>::Result>::cond))
      };
    };

    template<typename T1,typename T2,typename Op>
    struct IsScalarVectorOperationValid
    {
      enum{
	cond = (tfel::typetraits::IsScalar<T1>::cond&&
		tfel::meta::Implements<T2,VectorConcept>::cond&&
		(!tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,Op>::Result>::cond))
      };
    };

    template<typename T1,typename T2,typename Op>
    struct IsVectorScalarOperationValid
    {
      static const bool cond =	tfel::meta::Implements<T1,VectorConcept>::cond&&
	tfel::typetraits::IsScalar<T2>::cond&&
	!tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,Op>::Result>::cond;
    };

    template<typename T1>
    struct IsEuclidianNormValid
    {
      static const bool cond =tfel::meta::Implements<T1,VectorConcept>::cond&&
	!tfel::typetraits::IsInvalid<typename tfel::typetraits::RealPartType<typename ComputeBinaryResult<T1,T1,OpDotProduct>::Result>::type>::cond;
    };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      IsVectorVectorOperationValid<T1,T2,OpPlus>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      IsVectorVectorOperationValid<T1,T2,OpMinus>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      IsVectorVectorOperationValid<T1,T2,OpMult>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      IsVectorVectorOperationValid<T1,T2,OpDiadicProduct>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle
    >::type
    operator ^ (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      IsVectorVectorOperationValid<T1,T2,OpDiv>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1&,const T2&);
    
    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      IsScalarVectorOperationValid<T1,T2,OpMult>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1, const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      IsVectorScalarOperationValid<T1,T2,OpMult>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1&,const T2);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      IsVectorScalarOperationValid<T1,T2,OpDiv>::cond,
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
     * \return the inner product of a vector
     * \param const T1&, the left  vector.
     * \param const T2&, the right vector.
     * \return const typename ResultType<T,T2,OpMult>::type, the
     * result.
     * \warning the operator| has not the priority expected for such
     * an operation : use of parenthesis is required.
     */
    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      IsVectorVectorOperationValid<T1,T2,OpDotProduct>::cond,
      typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result
    >::type
    operator | (const T1&, const T2&);

    /*!
     * \brief  return the euclidian norm of a tvector
     * \param  v : the vector.
     * \return const typename tfel::typetraits::RealPartType<T>::type, the result
     */
    template<typename T1>
    typename tfel::meta::EnableIf<
      IsEuclidianNormValid<T1>::cond,
      typename tfel::typetraits::RealPartType<typename ComputeBinaryResult<T1,T1,OpDotProduct>::Result>::type
    >::type
    norm(const T1&);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Vector/VectorConceptOperations.ixx"
  
#endif /* _LIB_TFEL_VECTOR_OPERATIONS_HXX */

