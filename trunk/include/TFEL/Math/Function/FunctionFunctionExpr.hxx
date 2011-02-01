/*!
 * \file   FunctionFunctionExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   26 déc 2006
 */

#include<string>

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/General/ComputeBinaryResult.hxx"

#ifndef _LIB_TFEL_FUNCTIONFUNCTIONEXPR_HXX_
#define _LIB_TFEL_FUNCTIONFUNCTIONEXPR_HXX_ 


namespace tfel{

  namespace math{

    template<typename Func1,typename Func2, typename Op>
    class FunctionFunctionExpr;

    template<typename Func1,typename Func2>
    class FunctionTraits<FunctionExpr<FunctionFunctionExpr<Func1,Func2,OpPlus> > >
    {
      typedef typename FunctionTraits<Func1>::DerivateFunc DF1;
      typedef typename FunctionTraits<Func2>::DerivateFunc DF2;
      typedef typename ComputeBinaryResult<DF1,DF2,OpPlus>::Handle Res;
    public:
      typedef Res DerivateFunc;
    };
    
    template<typename Func1,typename Func2>
    class FunctionTraits<FunctionExpr<FunctionFunctionExpr<Func1,Func2,OpMinus> > >
    {
      typedef typename FunctionTraits<Func1>::DerivateFunc DF1;
      typedef typename FunctionTraits<Func2>::DerivateFunc DF2;
      typedef typename ComputeBinaryResult<DF1,DF2,OpMinus>::Handle Res;
    public:
      typedef Res DerivateFunc;
    };

    template<typename Func1,typename Func2>
    class FunctionTraits<FunctionExpr<FunctionFunctionExpr<Func1,Func2,OpMult> > >
    {
      typedef typename FunctionTraits<Func1>::DerivateFunc DF1;
      typedef typename FunctionTraits<Func2>::DerivateFunc DF2;
      typedef typename ComputeBinaryResult<DF1,Func2,OpMult>::Handle DF1_F2;
      typedef typename ComputeBinaryResult<Func1,DF2,OpMult>::Handle F1_DF2;
      typedef typename ComputeBinaryResult<DF1_F2,F1_DF2,OpPlus>::Handle Res;
    public:
      typedef Res DerivateFunc;
    };

    template<typename Func1,typename Func2>
    class FunctionTraits<FunctionExpr<FunctionFunctionExpr<Func1,Func2,OpDiv> > >
    {
      typedef typename FunctionTraits<Func1>::DerivateFunc DF1;
      typedef typename FunctionTraits<Func2>::DerivateFunc DF2;
      typedef typename ComputeBinaryResult<DF1,Func2,OpMult>::Handle DF1_F2;
      typedef typename ComputeBinaryResult<Func1,DF2,OpMult>::Handle F1_DF2;
      typedef typename ComputeBinaryResult<DF1_F2,F1_DF2,OpMinus>::Handle N_;
      typedef typename ComputeBinaryResult<Func2,Func2,OpMult>::Handle D_;
      typedef typename ComputeBinaryResult<N_,D_,OpDiv>::Handle Res;
    public:
      typedef Res DerivateFunc;
    };

    namespace internals{
    
      template<typename Func1,typename Func2, typename Op>
      struct DerivateFunctionFunctionExpr;

      template<typename Func1,typename Func2>
      struct DerivateFunctionFunctionExpr<Func1,Func2,OpPlus>
      {
	static const typename 
	FunctionTraits<FunctionExpr<FunctionFunctionExpr<Func1,Func2,OpPlus> > >::DerivateFunc
	exe(const Func1 f,const Func2 g)
	{
	  return derivate(f)+derivate(g);
	}
      };

      template<typename Func1,typename Func2>
      struct DerivateFunctionFunctionExpr<Func1,Func2,OpMinus>
      {
	static const typename 
	FunctionTraits<FunctionExpr<FunctionFunctionExpr<Func1,Func2,OpMinus> > >::DerivateFunc
	exe(const Func1 f,const Func2 g)
	{
	  return derivate(f)-derivate(g);
	}
      };

      template<typename Func1,typename Func2>
      struct DerivateFunctionFunctionExpr<Func1,Func2,OpMult>
      {
	static const typename 
	FunctionTraits<FunctionExpr<FunctionFunctionExpr<Func1,Func2,OpMult> > >::DerivateFunc
	exe(const Func1 f,const Func2 g)
	{
	  return derivate(f)*g+f*derivate(g);
	}
      };

      template<typename Func1,typename Func2>
      struct DerivateFunctionFunctionExpr<Func1,Func2,OpDiv>
      {
	static const typename 
	FunctionTraits<FunctionExpr<FunctionFunctionExpr<Func1,Func2,OpDiv> > >::DerivateFunc
	exe(const Func1 f,const Func2 g)
	{
	  return (derivate(f)*g-f*derivate(g))/(g*g);
	}
	
      };

    } // end of namespace internals

    template<typename Func1,typename Func2, typename Op>
    class FunctionFunctionExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<Func1,FunctionConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<Func2,FunctionConcept>::cond));

      const Func1 a;
      const Func2 b;

      // Assignement operator (disabled)
      FunctionFunctionExpr&
      operator = (const FunctionFunctionExpr&);

      // Default Constructor (disabled)
      FunctionFunctionExpr();

    protected:

      typedef Func1 first_arg;
      typedef Func2 second_arg;

    public:

      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("FunctionFunctionExpr<")
	  +Name<Func1>::getName()+string(",")
	  +Name<Func2>::getName()+string(",")
	  +Name<Op>::getName()+string(">");
      }

      FunctionFunctionExpr(const Func1 a_, const Func2 b_)
	: a(a_), b(b_)
      {}

      // Copy Construtor
      TFEL_MATH_INLINE
      FunctionFunctionExpr(const FunctionFunctionExpr& src)
	: a(src.a), b(src.b)
      {}

      template<typename T>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	!tfel::typetraits::IsInvalid<typename ComputeBinaryResult<
	typename ComputeUnaryResult<T,Func1>::Result, 
	typename ComputeUnaryResult<T,Func2>::Result,
	Op>::Result>::cond,
	typename ComputeBinaryResult<
	typename ComputeUnaryResult<T,Func1>::Handle, 
	typename ComputeUnaryResult<T,Func2>::Handle,
	Op>::Handle
      >::type
      operator()(const T& x) const
      {
	return Op::apply(a(x),b(x));
      } // end of operator ()

      const Func1
      getFunc1(void) const
      {
	return this->a;
      }

      const Func2
      getFunc2(void) const
      {
	return this->b;
      }
      
    }; // end of FunctionFunctionExpr

    template<typename Func1,typename Func2,typename Op>
    const typename 
    FunctionTraits<FunctionExpr<FunctionFunctionExpr<Func1,Func2,Op> > >::DerivateFunc
    derivate(const FunctionExpr<FunctionFunctionExpr<Func1,Func2,Op> >);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Function/FunctionFunctionExpr.ixx"

#endif /* _LIB_TFEL_FUNCTIONFUNCTIONEXPR_HXX */

