/*!
 * \file   include/TFEL/Math/Function/FunctionScalarExpr.hxx
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
#include"TFEL/Math/Function/FunctionNegExpr.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/General/ComputeBinaryResult.hxx"

#ifndef _LIB_TFEL_FUNCTIONSCALAREXPR_HXX_
#define _LIB_TFEL_FUNCTIONSCALAREXPR_HXX_ 

namespace tfel{

  namespace math{

    template<typename Func,typename Scal, typename Op>
    class FunctionScalarExpr;

    template<typename Func,typename Scal>
    struct FunctionTraits<FunctionExpr<FunctionScalarExpr<Func,Scal,OpPlus> > >
    {
    private:
      typedef typename FunctionTraits<Func>::DerivateFunc DF;
    public:
      typedef DF DerivateFunc;
    };
    
    template<typename Func,typename Scal>
    struct FunctionTraits<FunctionExpr<FunctionScalarExpr<Func,Scal,OpMinus> > >
    {
    private:
      typedef typename FunctionTraits<Func>::DerivateFunc DF;
    public:
      typedef DF DerivateFunc;
    };

    template<typename Func,typename Scal>
    struct FunctionTraits<FunctionExpr<FunctionScalarExpr<Func,Scal,OpMult> > >
    {
    private:
      typedef typename FunctionTraits<Func>::DerivateFunc DF;
      typedef FunctionExpr<FunctionScalarExpr<DF,Scal,OpMult> > Res;
    public:
      typedef Res DerivateFunc;
    };

    template<typename Func,typename Scal>
    struct FunctionTraits<FunctionExpr<FunctionScalarExpr<Func,Scal,OpDiv> > >
    {
    private:
      typedef typename FunctionTraits<Func>::DerivateFunc DF;
      typedef typename ComputeBinaryResult<DF,Scal,OpMult>::Handle S_DF;
      typedef typename ComputeUnaryResult<S_DF,OpNeg>::Handle N_;
      typedef typename ComputeBinaryResult<Func,Func,OpMult>::Handle D_;
      typedef typename ComputeBinaryResult<N_,D_,OpDiv>::Handle Res;
    public:
      typedef Res DerivateFunc;
    };

    namespace internals{
    
      template<typename Func,typename Scal, typename Op>
      struct DerivateFunctionScalarExpr;

      template<typename Func,typename Scal>
      struct DerivateFunctionScalarExpr<Func,Scal,OpPlus>
      {
	static const typename 
	FunctionTraits<FunctionExpr<FunctionScalarExpr<Func,Scal,OpPlus> > >::DerivateFunc
	exe(const Func f,const Scal)
	{
	  return derivate(f);
	}
      };

      template<typename Func,typename Scal>
      struct DerivateFunctionScalarExpr<Func,Scal,OpMinus>
      {
	static const typename 
	FunctionTraits<FunctionExpr<FunctionScalarExpr<Func,Scal,OpMinus> > >::DerivateFunc
	exe(const Func f,const Scal)
	{
	  return derivate(f);
	}
      };

      template<typename Func,typename Scal>
      struct DerivateFunctionScalarExpr<Func,Scal,OpMult>
      {
	static const typename 
	FunctionTraits<FunctionExpr<FunctionScalarExpr<Func,Scal,OpMult> > >::DerivateFunc
	exe(const Func f,const Scal s)
	{
	  return derivate(f)*s;
	}
      };

      template<typename Func,typename Scal>
      struct DerivateFunctionScalarExpr<Func,Scal,OpDiv>
      {
	static const typename 
	FunctionTraits<FunctionExpr<FunctionScalarExpr<Func,Scal,OpDiv> > >::DerivateFunc
	exe(const Func f,const Scal s)
	{
	  return derivate(f)/s;
	}
      };

    } // end of namespace internals

    template<typename F,typename S, typename Op>
    class FunctionScalarExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<F,FunctionConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<S>::cond));

      const F a;
      const S b;

      // Assignement operator (disabled)
      FunctionScalarExpr&
      operator = (const FunctionScalarExpr&);

      // Default Constructor (disabled)
      FunctionScalarExpr();

    protected:

      typedef const F first_arg;
      typedef const S second_arg;

    public:

      FunctionScalarExpr(const F a_, const S b_)
	: a(a_), b(b_)
      {}

      // Copy Construtor
      TFEL_MATH_INLINE
      FunctionScalarExpr(const FunctionScalarExpr& src)
	: a(src.a), b(src.b)
      {}

      template<typename T>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	!tfel::typetraits::IsInvalid<typename ComputeBinaryResult<
	typename ComputeUnaryResult<T,F>::Result,S,Op>::Result>::cond,
	typename ComputeBinaryResult<
	typename ComputeUnaryResult<T,F>::Handle,S,Op>::Handle
      >::type
      operator()(const T& x) const
      {
	return Op::apply(a(x),b);
      } // end of operator ()

      const F
      getFunc(void) const
      {
	return this->a;
      }

      const S
      getScalar(void) const
      {
	return this->b;
      }

    }; // end of FunctionScalarExpr

    template<typename Func,typename Scal,typename Op>
    const typename 
    FunctionTraits<FunctionExpr<FunctionScalarExpr<Func,Scal,Op> > >::DerivateFunc
    derivate(const FunctionExpr<FunctionScalarExpr<Func,Scal,Op> >&);

  } // end of namespace math

} // end of namespace tfel
  
#include"TFEL/Math/Function/FunctionScalarExpr.ixx"

#endif /* _LIB_TFEL_FUNCTIONSCALAREXPR_HXX */

