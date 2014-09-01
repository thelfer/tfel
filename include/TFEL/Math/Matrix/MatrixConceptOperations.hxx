/*!
 * \file   include/TFEL/Math/Matrix/MatrixConceptOperations.hxx
 * \brief  This file implements operations that can be applied to matrices.
 * \author Helfer Thomas
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATRIX_OPERATIONS_HXX_
#define _LIB_TFEL_MATRIX_OPERATIONS_HXX_ 

#include <cmath>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/HasRandomAccessConstIterator.hxx"

#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/ComputeObjectTag.hxx"
#include"TFEL/Math/General/ComputeBinaryResult.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/Function/StandardFunctions.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"

#include"TFEL/Math/ExpressionTemplates/FctMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/FctMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExprWithoutConstIterator.hxx"

#include"TFEL/Math/Matrix/MatrixExpr.hxx"

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
                                      FctMathObjectExpr<MatrixConcept,MatrixTraits,A,Fct>,
                                      FctMathObjectExprWithoutConstIterator<MatrixConcept,MatrixTraits,A,Fct>
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
				      MathObjectMathObjectExpr<MatrixConcept,MatrixTraits,A,B,Op>,
				      MathObjectMathObjectExprWithoutConstIterator<MatrixConcept,MatrixTraits,A,B,Op>
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
				      ScalarMathObjectExpr<MatrixConcept,MatrixTraits,A,B,Op>,
				      ScalarMathObjectExprWithoutConstIterator<MatrixConcept,MatrixTraits,A,B,Op>
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
				      MathObjectScalarExpr<MatrixConcept,MatrixTraits,A,B,Op>,
				      MathObjectScalarExprWithoutConstIterator<MatrixConcept,MatrixTraits,A,B,Op>
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
				      MathObjectNegExpr<MatrixConcept,MatrixTraits,A>,
				      MathObjectNegExprWithoutConstIterator<MatrixConcept,MatrixTraits,A>
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
  
#include"TFEL/Math/Matrix/MatrixConceptOperations.ixx"

#endif /* _LIB_TFEL_MATRIX_OPERATIONS_HXX */

