/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2/T2toST2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to t2tost2.
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

#include"TFEL/Math/T2toST2/T2toST2Expr.hxx"

namespace tfel
{

  namespace math
  {

    // namespace internals
    // {

    //   template<unsigned short N,
    // 	       typename T1,
    // 	       typename T2,
    // 	       typename Result>
    //   struct T2toST2byT2toST2Multiplication
    //   {
    // 	static TFEL_MATH_INLINE void 
    // 	exe(const T1& a,const T2& b,Result &r)
    // 	{
    // 	  typename T1::size_type i;
    // 	  typename T1::size_type j;
    // 	  typename T1::size_type k;
    // 	  for(i=0;i!=StensorDimeToSize<N>::value;++i){
    // 	    for(j=0;j!=StensorDimeToSize<N>::value;++j){
    // 	      r(i,j)=0.;
    // 	      for(k=0;k!=StensorDimeToSize<N>::value;++k){
    // 		r(i,j)+=a(i,k)*b(k,j);
    // 	      }
    // 	    }
    // 	  }
    // 	} // end of exe
    //   }; // end of struct T2toST2byT2toST2Multiplication

    //   template<unsigned short N,
    // 	       typename T1,
    // 	       typename T2,
    // 	       typename Result>
    //   struct T2toST2byStensorMultiplication
    //   {}; // end of struct T2toST2byStensorMultiplication

    //   template<typename T1,
    // 	       typename T2,
    // 	       typename Res>
    //   struct T2toST2byStensorMultiplication<1u,T1,T2,Res>
    //   {
    // 	static TFEL_MATH_INLINE void 
    // 	exe(const T1& a,const T2& b,Res &r)
    // 	{
    // 	  r(0)=a(0,0)*b(0)+a(0,1)*b(1)+a(0,2)*b(2);
    // 	  r(1)=a(1,0)*b(0)+a(1,1)*b(1)+a(1,2)*b(2);
    // 	  r(2)=a(2,0)*b(0)+a(2,1)*b(1)+a(2,2)*b(2);
    // 	}
    //   }; // end of struct T2toST2byStensorMultiplication

    //   template<typename T1,
    // 	       typename T2,
    // 	       typename Res>
    //   struct T2toST2byStensorMultiplication<2u,T1,T2,Res>
    //   {
    // 	static TFEL_MATH_INLINE void 
    // 	exe(const T1& a,const T2& b,Res &r)
    // 	{
    // 	  r(0)=a(0,0)*b(0)+a(0,1)*b(1)+a(0,2)*b(2)+a(0,3)*b(3);
    // 	  r(1)=a(1,0)*b(0)+a(1,1)*b(1)+a(1,2)*b(2)+a(1,3)*b(3);
    // 	  r(2)=a(2,0)*b(0)+a(2,1)*b(1)+a(2,2)*b(2)+a(2,3)*b(3);
    // 	  r(3)=a(3,0)*b(0)+a(3,1)*b(1)+a(3,2)*b(2)+a(3,3)*b(3);
    // 	}
    //   }; // end of struct T2toST2byStensorMultiplication

    //   template<typename T1,
    // 	       typename T2,
    // 	       typename Res>
    //   struct T2toST2byStensorMultiplication<3u,T1,T2,Res>
    //   {
    // 	static TFEL_MATH_INLINE void 
    // 	exe(const T1& a,const T2& b,Res &r)
    // 	{
    // 	  r(0)=a(0,0)*b(0)+a(0,1)*b(1)+a(0,2)*b(2)+a(0,3)*b(3)+a(0,4)*b(4)+a(0,5)*b(5);
    // 	  r(1)=a(1,0)*b(0)+a(1,1)*b(1)+a(1,2)*b(2)+a(1,3)*b(3)+a(1,4)*b(4)+a(1,5)*b(5);
    // 	  r(2)=a(2,0)*b(0)+a(2,1)*b(1)+a(2,2)*b(2)+a(2,3)*b(3)+a(2,4)*b(4)+a(2,5)*b(5);
    // 	  r(3)=a(3,0)*b(0)+a(3,1)*b(1)+a(3,2)*b(2)+a(3,3)*b(3)+a(3,4)*b(4)+a(3,5)*b(5);
    // 	  r(4)=a(4,0)*b(0)+a(4,1)*b(1)+a(4,2)*b(2)+a(4,3)*b(3)+a(4,4)*b(4)+a(4,5)*b(5);
    // 	  r(5)=a(5,0)*b(0)+a(5,1)*b(1)+a(5,2)*b(2)+a(5,3)*b(3)+a(5,4)*b(4)+a(5,5)*b(5);
    // 	}
    //   }; // end of struct T2toST2byStensorMultiplication

    //   template<unsigned short N,
    // 	       typename T1,
    // 	       typename T2,
    // 	       typename Result>
    //   struct StensorbyT2toST2Multiplication
    //   {}; // end of struct StensorbyT2toST2Multiplication

    //   template<typename T1,
    // 	       typename T2,
    // 	       typename Res>
    //   struct StensorbyT2toST2Multiplication<1u,T1,T2,Res>
    //   {
    // 	static TFEL_MATH_INLINE void 
    // 	exe(const T1& b,const T2& a,Res &r)
    // 	{
    // 	  r(0)=a(0,0)*b(0)+a(1,0)*b(1)+a(2,0)*b(2);
    // 	  r(1)=a(0,1)*b(0)+a(1,1)*b(1)+a(2,1)*b(2);
    // 	  r(2)=a(0,2)*b(0)+a(1,2)*b(1)+a(2,2)*b(2);
    // 	}
    //   }; // end of struct StensorbyT2toST2Multiplication

    //   template<typename T1,
    // 	       typename T2,
    // 	       typename Res>
    //   struct StensorbyT2toST2Multiplication<2u,T1,T2,Res>
    //   {
    // 	static TFEL_MATH_INLINE void 
    // 	exe(const T1& b,const T2& a,Res &r)
    // 	{
    // 	  r(0)=a(0,0)*b(0)+a(1,0)*b(1)+a(2,0)*b(2)+a(3,0)*b(3);
    // 	  r(1)=a(0,1)*b(0)+a(1,1)*b(1)+a(2,1)*b(2)+a(3,1)*b(3);
    // 	  r(2)=a(0,2)*b(0)+a(1,2)*b(1)+a(2,2)*b(2)+a(3,2)*b(3);
    // 	  r(3)=a(0,3)*b(0)+a(1,3)*b(1)+a(2,3)*b(2)+a(3,3)*b(3);
    // 	}
    //   }; // end of struct StensorbyT2toST2Multiplication

    //   template<typename T1,
    // 	       typename T2,
    // 	       typename Res>
    //   struct StensorbyT2toST2Multiplication<3u,T1,T2,Res>
    //   {
    // 	static TFEL_MATH_INLINE void 
    // 	exe(const T1& b,const T2& a,Res &r)
    // 	{
    // 	  r(0)=a(0,0)*b(0)+a(1,0)*b(1)+a(2,0)*b(2)+a(3,0)*b(3)+a(4,0)*b(4)+a(5,0)*b(5);
    // 	  r(1)=a(0,1)*b(0)+a(1,1)*b(1)+a(2,1)*b(2)+a(3,1)*b(3)+a(4,1)*b(4)+a(5,1)*b(5);
    // 	  r(2)=a(0,2)*b(0)+a(1,2)*b(1)+a(2,2)*b(2)+a(3,2)*b(3)+a(4,2)*b(4)+a(5,2)*b(5);
    // 	  r(3)=a(0,3)*b(0)+a(1,3)*b(1)+a(2,3)*b(2)+a(3,3)*b(3)+a(4,3)*b(4)+a(5,3)*b(5);
    // 	  r(4)=a(0,4)*b(0)+a(1,4)*b(1)+a(2,4)*b(2)+a(3,4)*b(3)+a(4,4)*b(4)+a(5,4)*b(5);
    // 	  r(5)=a(0,5)*b(0)+a(1,5)*b(1)+a(2,5)*b(2)+a(3,5)*b(3)+a(4,5)*b(4)+a(5,5)*b(5);
    // 	}
    //   }; // end of struct StensorbyT2toST2Multiplication

    // } // end of namespace internals

    /*
     * Partial Specialisation of ComputeBinaryResult_ for t2tost2's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<T2tost2Tag,T2tost2Tag,A,B,Op>
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

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-t2tost2 operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ScalarTag,T2tost2Tag,A,B,Op>
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
    class ComputeBinaryResult_<T2tost2Tag,ScalarTag,A,B,Op>
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

    // /*
    //  * Partial Specialisation of ComputeBinaryResult_ for t2tost2-t2tost2 operations
    //  */
    // template<typename A, typename B>
    // class ComputeBinaryResult_<T2tost2Tag,T2tost2Tag,A,B,OpMult>
    // {
    //   struct DummyHandle{};
    //   typedef typename T2toST2Type<A>::type StensA;
    //   typedef typename T2toST2Type<B>::type StensB;

    // public:
    //   typedef typename ResultType<StensA,StensB,OpMult>::type Result;
    //   typedef Result Handle;
    // };

    // /*
    //  * Partial Specialisation of ComputeBinaryResult_ for t2tost2-stensor operations
    //  */
    // template<typename A, typename B>
    // class ComputeBinaryResult_<T2tost2Tag,StensorTag,A,B,OpMult>
    // {
    //   struct DummyHandle{};
    //   typedef typename T2toST2Type<A>::type StensA;
    //   typedef typename StensorType<B>::type  StensB;

    // public:
    //   typedef typename ResultType<StensA,StensB,OpMult>::type Result;
    //   typedef Result Handle;
    // };

    // /*
    //  * Partial Specialisation of ComputeBinaryResult_ for stensor-t2tost2 operations
    //  */
    // template<typename A, typename B>
    // class ComputeBinaryResult_<StensorTag,T2tost2Tag,A,B,OpMult>
    // {
    //   struct DummyHandle{};
    //   typedef typename StensorType<A>::type  StensA;
    //   typedef typename T2toST2Type<B>::type StensB;

    // public:
    //   typedef typename ResultType<StensB,StensA,OpMult>::type Result;
    //   typedef Result Handle;
    // };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for t2tost2s
     */
    template<typename A>
    struct ComputeUnaryResult_<T2tost2Tag,UnaryOperatorTag,A,OpNeg>
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

    // template<typename T1,typename T2>
    // TFEL_MATH_INLINE 
    // typename tfel::meta::EnableIf<
    //   tfel::meta::Implements<T1,T2toST2Concept>::cond&&
    // tfel::meta::Implements<T2,T2toST2Concept>::cond&&
    // !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
    //   typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    // >::type
    // operator * (const T1& a,const T2& b)
    // {
    //   using namespace tfel::math::internals;
    //   typedef typename ComputeBinaryResult<T1,T2,OpMult>::Result Res;
    //   Res res;
    //   T2toST2byT2toST2Multiplication<T2toST2Traits<Res>::dime,T1,T2,Res>::exe(a,b,res);
    //   return  res;
    // }

    // template<typename T1,typename T2>
    // TFEL_MATH_INLINE 
    // typename tfel::meta::EnableIf<
    //   tfel::meta::Implements<T1,T2toST2Concept>::cond&&
    // tfel::meta::Implements<T2,StensorConcept>::cond&&
    // !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
    //   typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    // >::type
    // operator * (const T1& a,const T2& b)
    // {
    //   using namespace tfel::math::internals;
    //   typedef typename ComputeBinaryResult<T1,T2,OpMult>::Result Res;
    //   Res res;
    //   T2toST2byStensorMultiplication<StensorTraits<Res>::dime,T1,T2,Res>::exe(a,b,res);
    //   return  res;
    // }

    // template<typename T1,typename T2>
    // TFEL_MATH_INLINE 
    // typename tfel::meta::EnableIf<
    //   tfel::meta::Implements<T1,StensorConcept>::cond&&
    //   tfel::meta::Implements<T2,T2toST2Concept>::cond&&
    // !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
    //   typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    // >::type
    // operator | (const T1& a,const T2& b)
    // {
    //   using namespace tfel::math::internals;
    //   typedef typename ComputeBinaryResult<T1,T2,OpMult>::Result Res;
    //   Res res;
    //   StensorbyT2toST2Multiplication<StensorTraits<Res>::dime,T1,T2,Res>::exe(a,b,res);
    //   return  res;
    // }

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
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,T2toST2Concept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1& a)
    {
      typedef typename ComputeUnaryResult<T1,OpNeg>::Handle Handle;
      return Handle(a);
    }
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_T2TOST2_CONCEPT_OPERATIONS_HXX */

