/*!
 * \file   ScalarFunctionExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   26 déc 2006
 */

#include<string>

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/General/ComputeBinaryResult.hxx"
#include"TFEL/Math/Function/FunctionNegExpr.hxx"

#ifndef _LIB_TFEL_SCALARFUNCTIONEXPR_HXX_
#define _LIB_TFEL_SCALARFUNCTIONEXPR_HXX_ 

namespace tfel{

  namespace math{

    template<typename Scal,typename Func, typename Op>
    class ScalarFunctionExpr;

    template<typename Scal,typename Func>
    struct FunctionTraits<FunctionExpr<ScalarFunctionExpr<Scal,Func,OpPlus> > >
    {
    private:
      typedef typename FunctionTraits<Func>::DerivateFunc DF;
    public:
      typedef DF DerivateFunc;
    };
    
    template<typename Scal,typename Func>
    struct FunctionTraits<FunctionExpr<ScalarFunctionExpr<Scal,Func,OpMinus> > >
    {
    private:
      typedef typename FunctionTraits<Func>::DerivateFunc DF;
      typedef FunctionExpr<FunctionNegExpr<DF> > Res;
    public:
      typedef Res DerivateFunc;
    };

    template<typename Scal,typename Func>
    struct FunctionTraits<FunctionExpr<ScalarFunctionExpr<Scal,Func,OpMult> > >
    {
    private:
      typedef typename FunctionTraits<Func>::DerivateFunc DF;
      typedef FunctionExpr<ScalarFunctionExpr<Scal,DF,OpMult> > Res;
    public:
      typedef Res DerivateFunc;
    };

    template<typename Scal,typename Func>
    struct FunctionTraits<FunctionExpr<ScalarFunctionExpr<Scal,Func,OpDiv> > >
    {
    private:
      typedef typename FunctionTraits<Func>::DerivateFunc DF;
      typedef typename ComputeBinaryResult<Scal,DF,OpMult>::Handle S_DF;
      typedef typename ComputeUnaryResult<S_DF,OpNeg>::Handle N_;
      typedef typename ComputeBinaryResult<Func,Func,OpMult>::Handle D_;
      typedef typename ComputeBinaryResult<N_,D_,OpDiv>::Handle Res;
    public:
      typedef Res DerivateFunc;
    };

    namespace internals{
    
      template<typename Scal,typename Func, typename Op>
      struct DerivateScalarFunctionExpr;

      template<typename Scal,typename Func>
      struct DerivateScalarFunctionExpr<Scal,Func,OpPlus>
      {
	static const typename 
	FunctionTraits<FunctionExpr<ScalarFunctionExpr<Scal,Func,OpPlus> > >::DerivateFunc
	exe(const Scal,const Func f)
	{
	  return derivate(f);
	}
      };

      template<typename Scal,typename Func>
      struct DerivateScalarFunctionExpr<Scal,Func,OpMinus>
      {
	static const typename 
	FunctionTraits<FunctionExpr<ScalarFunctionExpr<Scal,Func,OpMinus> > >::DerivateFunc
	exe(const Scal,const Func f)
	{
	  return -derivate(f);
	}
      };

      template<typename Scal,typename Func>
      struct DerivateScalarFunctionExpr<Scal,Func,OpMult>
      {
	static const typename 
	FunctionTraits<FunctionExpr<ScalarFunctionExpr<Scal,Func,OpMult> > >::DerivateFunc
	exe(const Scal s,const Func f)
	{
	  return s*derivate(f);
	}
      };

      template<typename Scal,typename Func>
      struct DerivateScalarFunctionExpr<Scal,Func,OpDiv>
      {
	static const typename 
	FunctionTraits<FunctionExpr<ScalarFunctionExpr<Scal,Func,OpDiv> > >::DerivateFunc
	exe(const Scal s,const Func f)
	{
	  return (-s*derivate(f))/(f*f);
	}
      };

    } // end of namespace internals

    template<typename S,typename F, typename Op>
    class ScalarFunctionExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<S>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<F,FunctionConcept>::cond));

      const S a;
      const F b;

      // Assignement operator (disabled)
      ScalarFunctionExpr&
      operator = (const ScalarFunctionExpr&);

      // Default Constructor (disabled)
      ScalarFunctionExpr();

    protected:

      typedef const S first_arg;
      typedef const F second_arg;

    public:

      ScalarFunctionExpr(const S a_, const F b_)
	: a(a_), b(b_)
      {}

      // Copy Construtor
      TFEL_MATH_INLINE
      ScalarFunctionExpr(const ScalarFunctionExpr& src)
	: a(src.a), b(src.b)
      {}

      template<typename T>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	!tfel::typetraits::IsInvalid<typename ComputeBinaryResult<
	S,typename ComputeUnaryResult<T,F>::Result,Op>::Result>::cond,
	typename ComputeBinaryResult<
	S, typename ComputeUnaryResult<T,F>::Handle,Op>::Handle
      >::type
      operator()(const T& x) const
      {
	return Op::apply(a,b(x));
      } // end of operator ()

      const S
      getScalar(void) const
      {
	return this->a;
      }

      const F
      getFunc(void) const
      {
	return this->b;
      }

    }; // end of ScalarFunctionExpr

    template<typename Scal,typename Func,typename Op>
    const typename 
    FunctionTraits<FunctionExpr<ScalarFunctionExpr<Scal,Func,Op> > >::DerivateFunc
    derivate(const FunctionExpr<ScalarFunctionExpr<Scal,Func,Op> >);

  } // end of namespace math

} // end of namespace tfel
  
#include"TFEL/Math/Function/ScalarFunctionExpr.ixx"

#endif /* _LIB_TFEL_SCALARFUNCTIONEXPR_HXX */

