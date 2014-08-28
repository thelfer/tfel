/*!
 * \file   include/TFEL/Math/Function/FunctionConceptOperations.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   26 déc 2006
 */

#ifndef _LIB_TFEL_FUNCTIONCONCEPTOPERATIONS_HXX_
#define _LIB_TFEL_FUNCTIONCONCEPTOPERATIONS_HXX_ 

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"
#include"TFEL/Math/Function/FunctionExpr.hxx"
#include"TFEL/Math/Function/FunctionFunctionExpr.hxx"
#include"TFEL/Math/Function/ScalarFunctionExpr.hxx"
#include"TFEL/Math/Function/FunctionScalarExpr.hxx"
#include"TFEL/Math/Function/FunctionNegExpr.hxx"

namespace tfel{

  namespace math{

    /*
     * Partial Specialisation of ComputeBinaryResult_ for function's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<FunctionTag,FunctionTag,A,B,Op>
    {
      typedef FunctionFunctionExpr<A,B,Op> Expr;
    public:
      typedef FunctionExpr<Expr> Result;
      typedef FunctionExpr<Expr> Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-function operations
     */
    template<typename A, typename B,typename Op>
    struct ComputeBinaryResult_<ScalarTag,FunctionTag,A,B,Op>
    {
      typedef ScalarFunctionExpr<A,B,Op> Expr;
    public:
      typedef FunctionExpr<Expr> Result;
      typedef FunctionExpr<Expr> Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for function-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<FunctionTag,ScalarTag,A,B,Op>
    {
      typedef FunctionScalarExpr<A,B,Op> Expr;
    public:
      typedef FunctionExpr<Expr> Result;
      typedef FunctionExpr<Expr> Handle;
    };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,FunctionConcept>::cond&&
      tfel::meta::Implements<T2,FunctionConcept>::cond,
      FunctionExpr<FunctionFunctionExpr<T1,T2,OpPlus> >
    >::type
    operator + (const T1& a,const T2& b)
    {
      typedef FunctionFunctionExpr<T1,T2,OpPlus> Expr;
      return FunctionExpr<Expr>(a,b);
    } // end of operator +

    template<typename T1,typename T2>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,FunctionConcept>::cond&&
      tfel::meta::Implements<T2,FunctionConcept>::cond,
      FunctionExpr<FunctionFunctionExpr<T1,T2,OpMinus> >
    >::type
    operator - (const T1& a,const T2& b)
    {
      typedef FunctionFunctionExpr<T1,T2,OpMinus> Expr;
      return FunctionExpr<Expr>(a,b);
    } // end of operator -

    template<typename T1,typename T2>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,FunctionConcept>::cond&&
      tfel::meta::Implements<T2,FunctionConcept>::cond,
      FunctionExpr<FunctionFunctionExpr<T1,T2,OpMult> >
    >::type
    operator * (const T1& a,const T2& b)
    {
      typedef FunctionFunctionExpr<T1,T2,OpMult> Expr;
      return FunctionExpr<Expr>(a,b);
    } // end of operator *

    template<typename T1,typename T2>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,FunctionConcept>::cond&&
      tfel::meta::Implements<T2,FunctionConcept>::cond,
      FunctionExpr<FunctionFunctionExpr<T1,T2,OpDiv> >
    >::type
    operator / (const T1& a,const T2& b)
    {
      typedef FunctionFunctionExpr<T1,T2,OpDiv> Expr;
      return FunctionExpr<Expr>(a,b);
    } // end of operator /

    template<typename S,typename F>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<S>::cond&&
      tfel::meta::Implements<F,FunctionConcept>::cond,
      FunctionExpr<ScalarFunctionExpr<S,F,OpMult> >
    >::type
    operator * (const S a, const F b)
    {
      typedef FunctionExpr<ScalarFunctionExpr<S,F,OpMult> > Res;
      return Res(a,b);
    } // end of operator *

    template<typename S,typename F>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<S>::cond&&
      tfel::meta::Implements<F,FunctionConcept>::cond,
      FunctionExpr<ScalarFunctionExpr<S,F,OpDiv> >
    >::type
    operator / (const S a, const F b)
    {
      typedef FunctionExpr<ScalarFunctionExpr<S,F,OpDiv> > Res;
      return Res(a,b);
    } // end of operator /

    template<typename F,typename S>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<S>::cond&&
      tfel::meta::Implements<F,FunctionConcept>::cond,
      FunctionExpr<FunctionScalarExpr<F,S,OpMult> >
    >::type
    operator * (const F a, const S b)
    {
      typedef FunctionExpr<FunctionScalarExpr<F,S,OpMult> > Res;
      return Res(a,b);
    } // end of operator *

    template<typename F,typename S>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<S>::cond&&
      tfel::meta::Implements<F,FunctionConcept>::cond,
      FunctionExpr<FunctionScalarExpr<F,S,OpDiv> >
    >::type
    operator / (const F a, const S b)
    {
      typedef FunctionExpr<FunctionScalarExpr<F,S,OpDiv> > Res;
      return Res(a,b);
    } // end of operator /

    template<typename F>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<F,FunctionConcept>::cond,
      FunctionExpr<FunctionNegExpr<F> >
    >::type
    operator - (const F a)
    {
      typedef FunctionExpr<FunctionNegExpr<F> > Res;
      return Res(a);
    } // end of operator -

    /*
     * Partial specialisation for the addition of two functions
     */
    template<typename X,typename F1>
    struct UnaryResultType<X,FunctionExpr<FunctionNegExpr<F1> > >
    {
      typedef typename UnaryResultType<typename UnaryResultType<X,F1>::type,
				       OpNeg>::type type;
    }; 

    /*
     * Partial specialisation for the addition of two functions
     */
    template<typename X,typename F1,typename F2>
    struct UnaryResultType<X,FunctionExpr<FunctionFunctionExpr<F1,F2,OpPlus> > >
    {
      typedef typename ResultType<typename UnaryResultType<X,F1>::type,
				  typename UnaryResultType<X,F2>::type,OpPlus>::type type;
    }; 

    /*
     * Partial specialisation for the substraction of two functions
     */
    template<typename X,typename F1,typename F2>
    struct UnaryResultType<X,FunctionExpr<FunctionFunctionExpr<F1,F2,OpMinus> > >{
      typedef typename ResultType<typename UnaryResultType<X,F1>::type,
				  typename UnaryResultType<X,F2>::type,OpMinus>::type type;
    }; 

    /*
     * Partial specialisation for the multiplication of two functions
     */
    template<typename X,typename F1,typename F2>
    struct UnaryResultType<X,FunctionExpr<FunctionFunctionExpr<F1,F2,OpMult> > >{
      typedef typename ResultType<typename UnaryResultType<X,F1>::type,
				  typename UnaryResultType<X,F2>::type,OpMult>::type type;
    }; 

    /*
     * Partial specialisation for the divistion of two functions
     */
    template<typename X,typename F1,typename F2>
    struct UnaryResultType<X,FunctionExpr<FunctionFunctionExpr<F1,F2,OpDiv> > >{
      typedef typename ResultType<typename UnaryResultType<X,F1>::type,
				  typename UnaryResultType<X,F2>::type,OpDiv>::type type;
    }; 

    /*
     * Partial specialisation for the composition of two functions
     */
    template<typename X,typename F1,typename F2>
    struct UnaryResultType<X,FunctionExpr<FunctionsCompositionExpr<F1,F2> > >
    {
      typedef typename 
      UnaryResultType<typename UnaryResultType<X,F2>::type,F1>::type type;
    }; 
    
    /*
     * Partial specialisation for the left product by a scalar of a function
     */
    template<typename X,typename S,typename F>
    struct UnaryResultType<X,FunctionExpr<ScalarFunctionExpr<S,F,OpMult> > >
    {
      typedef typename 
      ResultType<S,typename UnaryResultType<X,F>::type,OpMult>::type type;
    }; 

    /*
     * Partial specialisation for the division of a scalar by a function
     */
    template<typename X,typename S,typename F>
    struct UnaryResultType<X,FunctionExpr<ScalarFunctionExpr<S,F,OpDiv> > >
    {
      typedef typename 
      ResultType<S,typename UnaryResultType<X,F>::type,OpDiv>::type type;
    }; 

    /*
     * Partial specialisation for the right product by a scalar of a function
     */
    template<typename X,typename F,typename S>
    struct UnaryResultType<X,FunctionExpr<FunctionScalarExpr<F,S,OpMult> > >
    {
      typedef typename 
      ResultType<typename UnaryResultType<X,F>::type,S,OpMult>::type type;
    }; 

    /*
     * Partial specialisation for the division of a function by a scalar
     */
    template<typename X,typename S,typename F>
    struct UnaryResultType<X,FunctionExpr<FunctionScalarExpr<F,S,OpDiv> > >{
      typedef typename 
      ResultType<typename UnaryResultType<X,F>::type,S,OpDiv>::type type;
    }; 

        /*
     * Partial specialisation for addition of two functions.
     */
    template<typename F1,typename F2>
    class ComputeUnaryResult_<FunctionTag,UnaryOperatorTag,
			      FunctionExpr<FunctionFunctionExpr<F1,F2,OpPlus> >,OpDiff>
    {
      typedef typename ComputeUnaryResult<F1,OpDiff>::Handle DF1;
      typedef typename ComputeUnaryResult<F2,OpDiff>::Handle DF2;
      typedef FunctionExpr<FunctionFunctionExpr<DF1,DF2,OpPlus> > DF_;
    public:
      typedef DF_ Result;
      typedef DF_ Handle;
    }; // end of ComputeUnaryResult_

    /*
     * Partial specialisation for substraction of two functions.
     */
    template<typename F1,typename F2>
    class ComputeUnaryResult_<FunctionTag,UnaryOperatorTag,
			      FunctionExpr<FunctionFunctionExpr<F1,F2,OpMinus> >,OpDiff>
    {
      typedef typename ComputeUnaryResult<F1,OpDiff>::Handle DF1;
      typedef typename ComputeUnaryResult<F2,OpDiff>::Handle DF2;
      typedef FunctionExpr<FunctionFunctionExpr<DF1,DF2,OpMinus> > DF_;
    public:
      typedef DF_ Result;
      typedef DF_ Handle;
    }; // end of ComputeUnaryResult_

    /*
     * Partial specialisation for multiplication of two functions.
     */
    template<typename F1,typename F2>
    class ComputeUnaryResult_<FunctionTag,UnaryOperatorTag,
			      FunctionExpr<FunctionFunctionExpr<F1,F2,OpMult> >,OpDiff>
    {
      typedef typename ComputeUnaryResult<F1,OpDiff>::Handle DF1;
      typedef typename ComputeUnaryResult<F2,OpDiff>::Handle DF2;
      typedef FunctionExpr<FunctionFunctionExpr<DF1,F2,OpMult> > DF1F2;
      typedef FunctionExpr<FunctionFunctionExpr<F1,DF2,OpMult> > F1DF2;
      typedef FunctionExpr<FunctionFunctionExpr<DF1F2,F1DF2,OpPlus> > DF_;
    public:
      typedef DF_ Result;
      typedef DF_ Handle;
    }; // end of ComputeUnaryResult_

    /*
     * Partial specialisation for division of two functions.
     */
    template<typename F1,typename F2>
    class ComputeUnaryResult_<FunctionTag,UnaryOperatorTag,
			      FunctionExpr<FunctionFunctionExpr<F1,F2,OpDiv> >,OpDiff>
    {
      typedef typename ComputeUnaryResult<F1,OpDiff>::Handle DF1;
      typedef typename ComputeUnaryResult<F2,OpDiff>::Handle DF2;
      typedef FunctionExpr<FunctionFunctionExpr<DF1,F2,OpMult> > DF1F2;
      typedef FunctionExpr<FunctionFunctionExpr<F1,DF2,OpMult> > F1DF2;
      typedef FunctionExpr<FunctionFunctionExpr<DF1F2,F1DF2,OpMinus> > DFN_;
      typedef FunctionExpr<FunctionFunctionExpr<F2,F2,OpMult> > DFD_;
      typedef FunctionExpr<FunctionFunctionExpr<DFN_,DFD_,OpDiv> > DF_;
    public:
      typedef DF_ Result;
      typedef DF_ Handle;
    }; // end of ComputeUnaryResult_
    
    /*
     * Partial specialisation for the addition of a scalar and a function.
     */
    template<typename S,typename F>
    class ComputeUnaryResult_<FunctionTag,UnaryOperatorTag,
			      FunctionExpr<ScalarFunctionExpr<S,F,OpPlus> >,OpDiff>
    {
      typedef typename ComputeUnaryResult<F,OpDiff>::Handle DF_;
    public:
      typedef DF_ Result;
      typedef DF_ Handle;
    }; // end of ComputeUnaryResult_

    /*
     * Partial specialisation for the substraction of a scalar and a function.
     */
    template<typename S,typename F>
    class ComputeUnaryResult_<FunctionTag,UnaryOperatorTag,
			      FunctionExpr<ScalarFunctionExpr<S,F,OpMinus> >,OpDiff>
    {
      typedef typename ComputeUnaryResult<F,OpDiff>::Handle DF_;
    public:
      typedef FunctionExpr<FunctionNegExpr<DF_> > Result;
      typedef FunctionExpr<FunctionNegExpr<DF_> > Handle;
    }; // end of ComputeUnaryResult_
    
    /*
     * Partial specialisation for the multiplication of a scalar by a function.
     */
    template<typename S,typename F>
    class ComputeUnaryResult_<FunctionTag,UnaryOperatorTag,
			      FunctionExpr<ScalarFunctionExpr<S,F,OpMult> >,OpDiff>
    {
      typedef typename ComputeUnaryResult<F,OpDiff>::Handle DF_;
    public:
      typedef FunctionExpr<ScalarFunctionExpr<S,DF_,OpMult> > Result;
      typedef FunctionExpr<ScalarFunctionExpr<S,DF_,OpMult> > Handle;
    }; // end of ComputeUnaryResult_
    
    /*
     * Partial specialisation for the division of a scalar by a function.
     */
    template<typename S,typename F>
    class ComputeUnaryResult_<FunctionTag,UnaryOperatorTag,
			      FunctionExpr<ScalarFunctionExpr<S,F,OpDiv> >,OpDiff>
    {
      typedef typename ComputeUnaryResult<F,OpDiff>::Handle DF_;
      typedef FunctionExpr<ScalarFunctionExpr<S,DF_,OpMult> > N_;
      typedef FunctionExpr<FunctionFunctionExpr<F,F,OpMult> > D_;
    public:
      typedef FunctionExpr<FunctionFunctionExpr<N_,D_,OpMult> > Result;
      typedef FunctionExpr<FunctionFunctionExpr<N_,D_,OpMult> > Handle;
    }; // end of ComputeUnaryResult_
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_FUNCTIONCONCEPTOPERATIONS_HXX */

