/*!
 * \file   T2toST2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to t2tost2.
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _LIB_TFEL_T2TOST2_CONCEPT_OPERATIONS_HXX_
#define _LIB_TFEL_T2TOST2_CONCEPT_OPERATIONS_HXX_ 

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

#include"TFEL/Math/Stensor/StensorConcept.hxx"

#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExprWithoutConstIterator.hxx"

#include"TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include"TFEL/Math/T2toST2/T2toST2Expr.hxx"
#include"TFEL/Math/T2toST2/StensorTensorDiadicProductExpr.hxx"

namespace tfel
{

  namespace math
  {

    /*
     * Partial Specialisation of ComputeBinaryResult_ for t2tost2's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<T2toST2Tag,T2toST2Tag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename T2toST2Type<A>::type StensA;
      typedef typename T2toST2Type<B>::type StensB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond&&
      tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      MathObjectMathObjectExpr<T2toST2Concept,T2toST2Traits,
							       A,B,Op>,
				      MathObjectMathObjectExprWithoutConstIterator<T2toST2Concept,T2toST2Traits,
										   A,B,Op>
      >::type Expr;			    
    public:
      typedef typename ResultType<StensA,StensB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      T2toST2Expr<Result,Expr> >::type Handle;
    };

    /*!
     * Partial Specialisation of ComputeBinaryResult_ for stensor's operation
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<StensorTag,TensorTag,A,B,OpDiadicProduct>
    {
      struct DummyHandle{};
      typedef typename StensorType<A>::type StensA;
      typedef typename TensorType<B>::type  TensB;
      typedef StensorTensorDiadicProductExpr<A,B> Expr;
    public:
      typedef typename ResultType<StensA,TensB,OpDiadicProduct>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      T2toST2Expr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-t2tost2 operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ScalarTag,T2toST2Tag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename T2toST2Type<B>::type StensB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      ScalarMathObjectExpr<T2toST2Concept,T2toST2Traits,
							   A,B,Op>,
				      ScalarMathObjectExprWithoutConstIterator<T2toST2Concept,T2toST2Traits,
									       A,B,Op>
      >::type Expr;
    public:
      typedef typename ResultType<A,StensB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      T2toST2Expr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for t2tost2-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<T2toST2Tag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename T2toST2Type<A>::type      StensA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      MathObjectScalarExpr<T2toST2Concept,T2toST2Traits,
							   A,B,Op>,
				      MathObjectScalarExprWithoutConstIterator<T2toST2Concept,T2toST2Traits,
									       A,B,Op>
      >::type Expr;
    public:
      typedef typename ResultType<StensA,B,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      T2toST2Expr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for t2tost2s
     */
    template<typename A>
    struct ComputeUnaryResult_<T2toST2Tag,UnaryOperatorTag,A,OpNeg>
    {
      struct DummyHandle{};
      typedef typename T2toST2Type<A>::type                    StensA;
      typedef typename T2toST2Traits<A>::NumType               NumA;
      typedef typename ComputeUnaryResult<NumA,OpNeg>::Result  NumResult;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      MathObjectNegExpr<T2toST2Concept,T2toST2Traits,A>,
				      MathObjectNegExprWithoutConstIterator<T2toST2Concept,T2toST2Traits,A>
      >::type Expr;
    public:
      typedef typename UnaryResultType<StensA,OpNeg>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      T2toST2Expr<Result,Expr> >::type Handle;
    };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,T2toST2Concept>::cond&&
      tfel::meta::Implements<T2,T2toST2Concept>::cond&&
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
      tfel::meta::Implements<T1,T2toST2Concept>::cond&&
    tfel::meta::Implements<T2,T2toST2Concept>::cond&&
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
      tfel::typetraits::IsScalar<T1>::cond&&
    tfel::meta::Implements<T2,T2toST2Concept>::cond&&
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
      tfel::meta::Implements<T1,T2toST2Concept>::cond&&
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
      tfel::meta::Implements<T1,T2toST2Concept>::cond&&
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
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,T2toST2Concept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1& a)
    {
      typedef typename ComputeUnaryResult<T1,OpNeg>::Handle Handle;
      return Handle(a);
    }
    
    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,TensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle
    >::type
    operator ^ (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle Handle;
      return  Handle(a,b);
    } // end of operator ^

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_T2TOST2_CONCEPT_OPERATIONS_HXX */

