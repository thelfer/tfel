/*!
 * \file   ST2toST2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to st2tost2.
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#ifndef _LIB_TFEL_ST2TOST2_CONCEPT_OPERATIONS_HXX_
#define _LIB_TFEL_ST2TOST2_CONCEPT_OPERATIONS_HXX_ 

#include <cmath>

#include "Config/TFELConfig.hxx"

#include "Metaprogramming/Implements.hxx"
#include "Metaprogramming/HasRandomAccessConstIterator.hxx"

#include "TypeTraits/IsScalar.hxx"

#include "Math/General/ResultType.hxx"
#include "Math/General/BasicOperations.hxx"
#include "Math/General/ComputeObjectTag.hxx"
#include "Math/General/ComputeBinaryResult.hxx"
#include "Math/General/ComputeUnaryResult.hxx"

#include "Math/Stensor/StensorConcept.hxx"

#include "Math/ST2toST2/ST2toST2Expr.hxx"
#include "Math/ST2toST2/ScalarST2toST2Expr.hxx"
#include "Math/ST2toST2/ST2toST2ST2toST2Expr.hxx"
#include "Math/ST2toST2/ST2toST2NegExpr.hxx"
#include "Math/ST2toST2/ScalarST2toST2ExprWithoutConstIterator.hxx"
#include "Math/ST2toST2/ST2toST2ST2toST2ExprWithoutConstIterator.hxx"
#include "Math/ST2toST2/ST2toST2NegExprWithoutConstIterator.hxx"

namespace tfel
{

  namespace math
  {

    namespace internals
    {

      template<unsigned short N,
	       typename T1,
	       typename T2,
	       typename Result>
      struct ST2toST2byStensorMultiplication
      {}; // end of struct ST2toST2byStensorMultiplication

      template<typename T1,
	       typename T2,
	       typename Res>
      struct ST2toST2byStensorMultiplication<1u,T1,T2,Res>
      {
	static TFEL_MATH_INLINE void 
	exe(const T1& a,const T2& b,Res &r)
	{
	  r(0)=a(0,0)*b(0)+a(0,1)*b(1)+a(0,2)*b(2);
	  r(1)=a(1,0)*b(0)+a(1,1)*b(1)+a(1,2)*b(2);
	  r(2)=a(2,0)*b(0)+a(2,1)*b(1)+a(2,2)*b(2);
	}
      }; // end of struct ST2toST2byStensorMultiplication

      template<typename T1,
	       typename T2,
	       typename Res>
      struct ST2toST2byStensorMultiplication<2u,T1,T2,Res>
      {
	static TFEL_MATH_INLINE void 
	exe(const T1& a,const T2& b,Res &r)
	{
	  r(0)=a(0,0)*b(0)+a(0,1)*b(1)+a(0,2)*b(2)+a(0,3)*b(3);
	  r(1)=a(1,0)*b(0)+a(1,1)*b(1)+a(1,2)*b(2)+a(1,3)*b(3);
	  r(2)=a(2,0)*b(0)+a(2,1)*b(1)+a(2,2)*b(2)+a(2,3)*b(3);
	  r(3)=a(3,0)*b(0)+a(3,1)*b(1)+a(3,2)*b(2)+a(3,3)*b(3);
	}
      }; // end of struct ST2toST2byStensorMultiplication

      template<typename T1,
	       typename T2,
	       typename Res>
      struct ST2toST2byStensorMultiplication<3u,T1,T2,Res>
      {
	static TFEL_MATH_INLINE void 
	exe(const T1& a,const T2& b,Res &r)
	{
	  r(0)=a(0,0)*b(0)+a(0,1)*b(1)+a(0,2)*b(2)+a(0,3)*b(3)+a(0,4)*b(4)+a(0,5)*b(5);
	  r(1)=a(1,0)*b(0)+a(1,1)*b(1)+a(1,2)*b(2)+a(1,3)*b(3)+a(1,4)*b(4)+a(1,5)*b(5);
	  r(2)=a(2,0)*b(0)+a(2,1)*b(1)+a(2,2)*b(2)+a(2,3)*b(3)+a(2,4)*b(4)+a(2,5)*b(5);
	  r(3)=a(3,0)*b(0)+a(3,1)*b(1)+a(3,2)*b(2)+a(3,3)*b(3)+a(3,4)*b(4)+a(3,5)*b(5);
	  r(4)=a(4,0)*b(0)+a(4,1)*b(1)+a(4,2)*b(2)+a(4,3)*b(3)+a(4,4)*b(4)+a(4,5)*b(5);
	  r(5)=a(5,0)*b(0)+a(5,1)*b(1)+a(5,2)*b(2)+a(5,3)*b(3)+a(5,4)*b(4)+a(5,5)*b(5);
	}
      }; // end of struct ST2toST2byStensorMultiplication

    } // end of namespace internals

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tost2's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<St2toSt2Tag,St2toSt2Tag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename ST2toST2Type<A>::type StensA;
      typedef typename ST2toST2Type<B>::type StensB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond&&
                                      tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      ST2toST2ST2toST2Expr<A,B,Op>,
				      ST2toST2ST2toST2ExprWithoutConstIterator<A,B,Op>
				      >::type Expr;			    
  public:
    typedef typename ResultType<StensA,StensB,Op>::type Result;
    typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				    DummyHandle,
				    ST2toST2Expr<Result,Expr> >::type Handle;
};

/*
 * Partial Specialisation of ComputeBinaryResult_ for scalar-st2tost2 operations
 */
template<typename A, typename B,typename Op>
class ComputeBinaryResult_<ScalarTag,St2toSt2Tag,A,B,Op>
{
  struct DummyHandle{};
  typedef typename ST2toST2Type<B>::type StensB;
  typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<B>::cond,
				  ScalarST2toST2Expr<A,B,Op>,
				  ScalarST2toST2ExprWithoutConstIterator<A,B,Op>
				  >::type Expr;
public:
typedef typename ResultType<A,StensB,Op>::type Result;
typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				DummyHandle,
				ST2toST2Expr<Result,Expr> >::type Handle;
};

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tost2-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<St2toSt2Tag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename ST2toST2Type<A>::type      StensA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      ST2toST2ScalarExpr<A,B,Op>,
				      ST2toST2ScalarExprWithoutConstIterator<A,B,Op>
				      >::type Expr;
  public:
    typedef typename ResultType<StensA,B,Op>::type Result;
    typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				    DummyHandle,
				    ST2toST2Expr<Result,Expr> >::type Handle;
};

/*
 * Partial Specialisation of ComputeBinaryResult_ for st2tost2-stensor operations
 */
template<typename A, typename B>
class ComputeBinaryResult_<St2toSt2Tag,StensorTag,A,B,OpMult>
{
  struct DummyHandle{};
  typedef typename ST2toST2Type<A>::type StensA;
  typedef typename StensorType<B>::type  StensB;

public:
  typedef typename ResultType<StensA,StensB,OpMult>::type Result;
  typedef Result Handle;
};

/*
 * Partial Specialisation of ComputeUnaryResult_ for st2tost2s
 */
template<typename A>
struct ComputeUnaryResult_<St2toSt2Tag,UnaryOperatorTag,A,OpNeg>
{
  struct DummyHandle{};
  typedef typename ST2toST2Type<A>::type                    StensA;
  typedef typename ST2toST2Traits<A>::NumType               NumA;
  typedef typename ComputeUnaryResult<NumA,OpNeg>::Result  NumResult;
  typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				  ST2toST2NegExpr<A>,
				  ST2toST2NegExprWithoutConstIterator<A>
				  >::type Expr;
public:
typedef typename UnaryResultType<StensA,OpNeg>::type Result;
typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				DummyHandle,
				ST2toST2Expr<Result,Expr> >::type Handle;
};

template<typename T1,typename T2>
TFEL_MATH_INLINE 
typename tfel::meta::EnableIf<
  tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
  tfel::meta::Implements<T2,ST2toST2Concept>::cond&&
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
  tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
  tfel::meta::Implements<T2,ST2toST2Concept>::cond&&
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
  tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
  tfel::meta::Implements<T2,StensorConcept>::cond&&
  !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
  typename ComputeBinaryResult<T1,T2,OpMult>::Handle
  >::type
operator * (const T1& a,const T2& b)
{
  using namespace tfel::math::internals;
  typedef typename ComputeBinaryResult<T1,T2,OpMult>::Result Res;
  Res res;
  ST2toST2byStensorMultiplication<StensorTraits<Res>::dime,T1,T2,Res>::exe(a,b,res);
  return  res;
}

template<typename T1,typename T2>
TFEL_MATH_INLINE 
typename tfel::meta::EnableIf<
  tfel::typetraits::IsScalar<T1>::cond&&
  tfel::meta::Implements<T2,ST2toST2Concept>::cond&&
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
  tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
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
  tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
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
  tfel::meta::Implements<T1,ST2toST2Concept>::cond,
  typename ComputeUnaryResult<T1,OpNeg>::Handle
  >::type
operator - (const T1& a)
{
  typedef typename ComputeUnaryResult<T1,OpNeg>::Handle Handle;
  return Handle(a);
}
    
} // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_ST2TOST2_CONCEPT_OPERATIONS_HXX */

