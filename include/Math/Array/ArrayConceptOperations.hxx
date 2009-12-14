/*!
 * \file   ArrayConceptOperations.hxx
 * \brief  This file implements the various operations that could be made on arrays.
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#ifndef _LIB_TFEL_ARRAY_OPERATIONS_HXX_
#define _LIB_TFEL_ARRAY_OPERATIONS_HXX_ 

#include <cmath>

#include "Config/TFELConfig.hxx"

#include "Metaprogramming/Implements.hxx"
#include "Metaprogramming/HasRandomAccessConstIterator.hxx"

#include "TypeTraits/IsScalar.hxx"
#include "TypeTraits/IsInvalid.hxx"

#include "Math/General/ResultType.hxx"
#include "Math/General/UnaryResultType.hxx"
#include "Math/General/BasicOperations.hxx"

#include"Math/General/ComputeBinaryResult.hxx"
#include"Math/General/ComputeUnaryResult.hxx"
#include"Math/Function/StandardFunctions.hxx"

#include "Math/Array/ArrayExpr.hxx"
#include "Math/Array/FctArrayExpr.hxx"
#include "Math/Array/ScalarArrayExpr.hxx"
#include "Math/Array/ArrayArrayExpr.hxx"
#include "Math/Array/ArrayNegExpr.hxx"
#include "Math/Array/FctArrayExprWithoutConstIterator.hxx"
#include "Math/Array/ScalarArrayExprWithoutConstIterator.hxx"
#include "Math/Array/ArrayArrayExprWithoutConstIterator.hxx"
#include "Math/Array/ArrayNegExprWithoutConstIterator.hxx"

namespace tfel{

  namespace math{

    /*									                
     * Partial Specialisation of ComputeUnaryResult_ for vectors	                
     */									                
    template<typename A,typename Fct>
    class ComputeUnaryResult_<ArrayTag,FunctionTag,A,Fct>
    {
      struct DummyHandle{};
      typedef typename ArrayType<A>::type VectA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
                                      FctArrayExpr<A,Fct>,
                                      FctArrayExprWithoutConstIterator<A,Fct>
				      >::type Expr;
    public:
      typedef typename UnaryResultType<VectA,Fct>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      ArrayExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for array's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ArrayTag,ArrayTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename ArrayType<A>::type ArrA;
      typedef typename ArrayType<B>::type ArrB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond&&
                                      tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      ArrayArrayExpr<A,B,Op>,
				      ArrayArrayExprWithoutConstIterator<A,B,Op>
				      >::type Expr;			    
    public:
      typedef typename ResultType<ArrA,ArrB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      ArrayExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-array operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ScalarTag,ArrayTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename ArrayType<B>::type      ArrB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      ScalarArrayExpr<A,B,Op>,
				      ScalarArrayExprWithoutConstIterator<A,B,Op>
                                     >::type Expr;
    public:
      typedef typename ResultType<A,ArrB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      ArrayExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for array-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ArrayTag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename ArrayType<A>::type      ArrA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      ArrayScalarExpr<A,B,Op>,
				      ArrayScalarExprWithoutConstIterator<A,B,Op>
                                     >::type Expr;
    public:
      typedef typename ResultType<ArrA,B,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      ArrayExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for arrays
     */
    template<typename A>
    class ComputeUnaryResult_<ArrayTag,UnaryOperatorTag,A,OpNeg>
    {
      struct DummyHandle{};
      typedef typename ArrayType<A>::type ArrA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      ArrayNegExpr<A>,
				      ArrayNegExprWithoutConstIterator<A>
				      >::type Expr;
    public:
      typedef typename UnaryResultType<ArrA,OpNeg>::type Result;
typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				DummyHandle,
				ArrayExpr<Result,Expr> >::type Handle;
    };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,ArrayConcept>::cond&&
      tfel::meta::Implements<T2,ArrayConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1& a, const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpPlus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,ArrayConcept>::cond&&
      tfel::meta::Implements<T2,ArrayConcept>::cond&&
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
      tfel::meta::Implements<T1,ArrayConcept>::cond&&
      tfel::meta::Implements<T2,ArrayConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,ArrayConcept>::cond&&
      tfel::meta::Implements<T2,ArrayConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiv>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,ArrayConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1 a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpPlus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,ArrayConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1 a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMinus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,ArrayConcept>::cond&&
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
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,ArrayConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1 a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiv>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,ArrayConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1& a,const T2 b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpPlus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,ArrayConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1& a,const T2 b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMinus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,ArrayConcept>::cond&&
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
      tfel::meta::Implements<T1,ArrayConcept>::cond&&
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
      tfel::meta::Implements<T1,ArrayConcept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1& a)
    {
      typedef typename ComputeUnaryResult<T1,OpNeg>::Handle Handle;
      return Handle(a);
    }
    
  } // end of namespace math

} // end of namespace tfel
  

#endif /* _LIB_TFEL_ARRAY_OPERATIONS_HXX */

