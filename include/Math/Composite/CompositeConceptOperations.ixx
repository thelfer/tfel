/*!
 * \file   CompositeConceptOperations.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 mar 2007
 */

#ifndef _LIB_TFEL_COMPOSITECONCEPTOPERATIONS_IXX_
#define _LIB_TFEL_COMPOSITECONCEPTOPERATIONS_IXX_ 

#include"Metaprogramming/IF.hxx"
#include"TypeTraits/IsInvalid.hxx"
#include"Math/General/ResultType.hxx"
#include"Math/General/ComputeBinaryResult.hxx"
#include"Math/General/ComputeUnaryResult.hxx"
#include"Math/General/BasicOperations.hxx"

#include"Math/Composite/CompositeExpr.hxx"

namespace tfel{

  namespace math{

    /*									                
     * Partial Specialisation of ComputeUnaryResult_ for composites	                
     */									                
    template<typename A,typename Fct>
    class ComputeUnaryResult_<CompositeTag,FunctionTag,A,Fct>
    {
      struct DummyHandle{};
      typedef typename CompositeType<A>::type VectA;
      typedef FctCompositeExpr<A,Fct> Expr;
    public:
      typedef typename UnaryResultType<VectA,Fct>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      CompositeExpr<Result,Expr> >::type Handle;
    };
    
    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-vector operations
     */
    template<typename A, typename B,typename Op>
    struct ComputeBinaryResult_<ScalarTag,CompositeTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename CompositeType<B>::type      VectB;
      typedef ScalarCompositeExpr<A,B,Op> Expr;
    public:
      typedef typename ResultType<A,VectB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      CompositeExpr<Result,Expr> >::type Handle;
    };
    
    /*
     * Partial Specialisation of ComputeBinaryResult_ for vector-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<CompositeTag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename CompositeType<A>::type      VectA;
      typedef CompositeScalarExpr<A,B,Op> Expr;
    public:
      typedef typename ResultType<VectA,B,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      CompositeExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for composites
     */
    template<typename A>
    class ComputeUnaryResult_<CompositeTag,UnaryOperatorTag,A,OpNeg>
    {
      struct DummyHandle{};
      typedef typename CompositeType<A>::type VectA;
      typedef CompositeNegExpr<A> Expr;
    public:
      typedef typename UnaryResultType<VectA,OpNeg>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      CompositeExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for composite's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<CompositeTag,CompositeTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename CompositeType<A>::type CompA;
      typedef typename CompositeType<B>::type CompB;
      typedef CompositeCompositeExpr<A,B,Op> Expr;
    public:
      typedef typename ResultType<CompA,CompB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      CompositeExpr<Result,Expr> >::type Handle;
    };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,CompositeConcept>::cond&&
      tfel::meta::Implements<T2,CompositeConcept>::cond&&
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
      tfel::meta::Implements<T1,CompositeConcept>::cond&&
      tfel::meta::Implements<T2,CompositeConcept>::cond&&
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
      tfel::meta::Implements<T1,CompositeConcept>::cond&&
      tfel::meta::Implements<T2,CompositeConcept>::cond&&
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
      tfel::meta::Implements<T1,CompositeConcept>::cond&&
      tfel::meta::Implements<T2,CompositeConcept>::cond&&
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
      tfel::meta::Implements<T2,CompositeConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1 a, const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpPlus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,CompositeConcept>::cond&&
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
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,CompositeConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1 a, const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMinus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,CompositeConcept>::cond&&
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
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,CompositeConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1 a, const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,CompositeConcept>::cond&&
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
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,CompositeConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1 a, const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiv>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,CompositeConcept>::cond&&
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
      tfel::meta::Implements<T1,CompositeConcept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1& a)
    {
      typedef typename ComputeUnaryResult<T1,OpNeg>::Handle Handle;
      return Handle(a);
    }

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_COMPOSITECONCEPTOPERATIONS_IXX */

