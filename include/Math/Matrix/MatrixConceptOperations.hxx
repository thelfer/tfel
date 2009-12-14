/*!
 * \file   MatrixConceptOperations.hxx
 * \brief  This file implements operations that can be applied to matrices.
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#ifndef _LIB_TFEL_MATRIX_OPERATIONS_HXX_
#define _LIB_TFEL_MATRIX_OPERATIONS_HXX_ 

#include <cmath>

#include "Config/TFELConfig.hxx"

#include "TypeTraits/IsScalar.hxx"
#include "TypeTraits/IsInvalid.hxx"

#include "Metaprogramming/Implements.hxx"
#include "Metaprogramming/HasRandomAccessConstIterator.hxx"

#include "Math/General/ResultType.hxx"
#include "Math/General/BasicOperations.hxx"
#include "Math/General/ComputeObjectTag.hxx"
#include "Math/General/ComputeBinaryResult.hxx"
#include "Math/General/ComputeUnaryResult.hxx"
#include "Math/Function/StandardFunctions.hxx"
#include "Math/Vector/VectorConcept.hxx"

#include "Math/Matrix/MatrixExpr.hxx"
#include "Math/Matrix/FctMatrixExpr.hxx"
#include "Math/Matrix/ScalarMatrixExpr.hxx"
#include "Math/Matrix/MatrixMatrixExpr.hxx"
#include "Math/Matrix/MatrixNegExpr.hxx"
#include "Math/Matrix/FctMatrixExprWithoutConstIterator.hxx"
#include "Math/Matrix/ScalarMatrixExprWithoutConstIterator.hxx"
#include "Math/Matrix/MatrixMatrixExprWithoutConstIterator.hxx"
#include "Math/Matrix/MatrixNegExprWithoutConstIterator.hxx"

namespace tfel{

  namespace math{

    template<typename AType,
	     typename BType,
	     typename A,
	     typename B>
    class MatrixVectorHandle
    {
      struct DummyHandle
      {};
    public:
      typedef DummyHandle type;
    };

    template<typename AType,
	     typename BType,
	     typename A,
	     typename B>
    class VectorMatrixHandle
    {
      struct DummyHandle
      {};
    public:
      typedef DummyHandle type;
    };

    /*									                
     * Partial Specialisation of ComputeUnaryResult_ for matrix	                
     */									                
    template<typename A,typename Fct>
    class ComputeUnaryResult_<MatrixTag,FunctionTag,A,Fct>
    {
      struct DummyHandle{};
      typedef typename MatrixType<A>::type VectA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
                                      FctMatrixExpr<A,Fct>,
                                      FctMatrixExprWithoutConstIterator<A,Fct>
				      >::type Expr;
    public:
      typedef typename UnaryResultType<VectA,Fct>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      MatrixExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for matrix's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<MatrixTag,MatrixTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename MatrixType<A>::type MatA;
      typedef typename MatrixType<B>::type MatB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond&&
                                      tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      MatrixMatrixExpr<A,B,Op>,
				      MatrixMatrixExprWithoutConstIterator<A,B,Op>
				      >::type Expr;			    
    public:
      typedef typename ResultType<MatA,MatB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      MatrixExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-matrix operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ScalarTag,MatrixTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename MatrixType<B>::type MatB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      ScalarMatrixExpr<A,B,Op>,
				      ScalarMatrixExprWithoutConstIterator<A,B,Op>
                                     >::type Expr;
    public:
      typedef typename ResultType<A,MatB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      MatrixExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for matrix-scalar operations
     */
    template<typename A, typename B,typename Op>
    struct ComputeBinaryResult_<MatrixTag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename MatrixType<A>::type MatA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      MatrixScalarExpr<A,B,Op>,
				      MatrixScalarExprWithoutConstIterator<A,B,Op>
                                     >::type Expr;
    public:
      typedef typename ResultType<MatA,B,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      MatrixExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for matrix-vector multiplication
     */
    template<typename A, typename B>
    struct ComputeBinaryResult_<MatrixTag,VectorTag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename MatrixType<A>::type MatA;
      typedef typename VectorType<B>::type VecB;
    public:
      typedef typename ResultType<MatA,VecB,OpMult>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      typename MatrixVectorHandle<MatA,VecB,
								  A,B>::type >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for vector-matrix multiplication
     */
    template<typename A, typename B>
    struct ComputeBinaryResult_<VectorTag,MatrixTag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename VectorType<A>::type VecA;
      typedef typename MatrixType<B>::type MatB;
    public:
      typedef typename ResultType<VecA,MatB,OpMult>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      typename MatrixVectorHandle<VecA,MatB,
								  A,B>::type >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for matrices
     */
    template<typename A>
    class ComputeUnaryResult_<MatrixTag,UnaryOperatorTag,A,OpNeg>
    {
      struct DummyHandle{};
      typedef typename MatrixType<A>::type MatA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      MatrixNegExpr<A>,
				      MatrixNegExprWithoutConstIterator<A>
      >::type Expr;
    public:
      typedef typename UnaryResultType<MatA,OpNeg>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      MatrixExpr<Result,Expr> >::type Handle;
    };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,MatrixConcept>::cond&&
      tfel::meta::Implements<T2,MatrixConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,MatrixConcept>::cond&&
      tfel::meta::Implements<T2,MatrixConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,MatrixConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,MatrixConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1&,const T2);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,MatrixConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,MatrixConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,MatrixConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1&,const T2);

    template<typename T1>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,MatrixConcept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1&);
    
  } // end of namespace math

} // end of namespace tfel
  
#include"Math/Matrix/MatrixConceptOperations.ixx"

#endif /* _LIB_TFEL_MATRIX_OPERATIONS_HXX */

