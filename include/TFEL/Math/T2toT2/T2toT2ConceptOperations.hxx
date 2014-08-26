/*!
 * \file   T2toT2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to t2tot2.
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _LIB_TFEL_T2TOT2_CONCEPT_OPERATIONS_HXX_
#define _LIB_TFEL_T2TOT2_CONCEPT_OPERATIONS_HXX_ 

#include <cmath>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/HasRandomAccessConstIterator.hxx"

#include"TFEL/TypeTraits/IsScalar.hxx"

#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/ComputeObjectTag.hxx"
#include"TFEL/Math/General/ComputeBinaryResult.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"

#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExprWithoutConstIterator.hxx"

#include"TFEL/Math/T2toT2/T2toT2Expr.hxx"
#include"TFEL/Math/T2toT2/T2toT2TensorProductExpr.hxx"
#include"TFEL/Math/T2toT2/TensorT2toT2ProductExpr.hxx"
#include"TFEL/Math/T2toT2/T2toT2T2toT2ProductExpr.hxx"

namespace tfel
{

  namespace math
  {

    /*
     * Partial Specialisation of ComputeBinaryResult_ for t2tot2's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<T2toT2Tag,T2toT2Tag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename T2toT2Type<A>::type T2toT2TypeA;
      typedef typename T2toT2Type<B>::type T2toT2TypeB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond&&
      tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      MathObjectMathObjectExpr<T2toT2Concept,T2toT2Traits,
							       A,B,Op>,
				      MathObjectMathObjectExprWithoutConstIterator<T2toT2Concept,T2toT2Traits,
										   A,B,Op>
      >::type Expr;			    
    public:
      typedef typename ResultType<T2toT2TypeA,T2toT2TypeB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      T2toT2Expr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-t2tot2 operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ScalarTag,T2toT2Tag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename T2toT2Type<B>::type T2toT2TypeB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      ScalarMathObjectExpr<T2toT2Concept,T2toT2Traits,
							   A,B,Op>,
				      ScalarMathObjectExprWithoutConstIterator<T2toT2Concept,T2toT2Traits,
									       A,B,Op>
      >::type Expr;
    public:
      typedef typename ResultType<A,T2toT2TypeB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      T2toT2Expr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for t2tot2-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<T2toT2Tag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename T2toT2Type<A>::type T2toT2TypeA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      MathObjectScalarExpr<T2toT2Concept,T2toT2Traits,
							   A,B,Op>,
				      MathObjectScalarExprWithoutConstIterator<T2toT2Concept,T2toT2Traits,
									       A,B,Op>
      >::type Expr;
    public:
      typedef typename ResultType<T2toT2TypeA,B,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      T2toT2Expr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for T2toT2-T2toT2 operations
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<T2toT2Tag,T2toT2Tag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename T2toT2Type<A>::type T2toT2A;
      typedef typename T2toT2Type<B>::type T2toT2B;
    public:
      typedef typename ResultType<T2toT2A,T2toT2B,OpMult>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      T2toT2Expr<Result,T2toT2T2toT2ProductExpr<T2toT2Traits<Result>::dime> > >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for T2toT2-Tensor operations
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<T2toT2Tag,TensorTag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename T2toT2Type<A>::type T2toT2A;
      typedef typename TensorType<B>::type  TensB;
    public:
      typedef typename ResultType<T2toT2A,TensB,OpMult>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,T2toT2TensorProductExpr<TensorTraits<Result>::dime> > >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for Tensor-T2toT2 operations
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<TensorTag,T2toT2Tag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename TensorType<A>::type  TensA;
      typedef typename T2toT2Type<B>::type T2toT2B;
    public:
      typedef typename ResultType<TensA,T2toT2B,OpMult>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,TensorT2toT2ProductExpr<TensorTraits<Result>::dime> > >:: type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for t2tot2s
     */
    template<typename A>
    struct ComputeUnaryResult_<T2toT2Tag,UnaryOperatorTag,A,OpNeg>
    {
      struct DummyHandle{};
      typedef typename T2toT2Type<A>::type                    T2toT2TypeA;
      typedef typename T2toT2Traits<A>::NumType               NumA;
      typedef typename ComputeUnaryResult<NumA,OpNeg>::Result  NumResult;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      MathObjectNegExpr<T2toT2Concept,T2toT2Traits,A>,
				      MathObjectNegExprWithoutConstIterator<T2toT2Concept,T2toT2Traits,A>
      >::type Expr;
    public:
      typedef typename UnaryResultType<T2toT2TypeA,OpNeg>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      T2toT2Expr<Result,Expr> >::type Handle;
    };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,T2toT2Concept>::cond&&
    tfel::meta::Implements<T2,T2toT2Concept>::cond&&
    !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpPlus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,T2toT2Concept>::cond&&
    tfel::meta::Implements<T2,T2toT2Concept>::cond&&
    !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMinus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,T2toT2Concept>::cond&&
      tfel::meta::Implements<T2,T2toT2Concept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,T2toT2Concept>::cond&&
      tfel::meta::Implements<T2,TensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
      >::type
    operator * (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,TensorConcept>::cond&&
      tfel::meta::Implements<T2,T2toT2Concept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator | (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,T2toT2Concept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1 a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,T2toT2Concept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1& a,const T2 b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,T2toT2Concept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
    !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1& a,const T2 b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiv>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,T2toT2Concept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1& a)
    {
      typedef typename ComputeUnaryResult<T1,OpNeg>::Handle Handle;
      return Handle(a);
    }
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_T2TOT2_CONCEPT_OPERATIONS_HXX */

