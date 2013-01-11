/*!
 * \file   FunctionsCompositionExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   26 déc 2006
 */

#ifndef _LIB_TFEL_FUNCTIONSCOMPOSITIONEXPR_HXX_
#define _LIB_TFEL_FUNCTIONSCOMPOSITIONEXPR_HXX_ 

#include<string>

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/General/ComputeBinaryResult.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"

namespace tfel{

  namespace math{

    template<typename Func1,typename Func2>
    class FunctionsCompositionExpr;

    template<typename Func1,typename Func2>
    struct FunctionTraits<FunctionExpr<FunctionsCompositionExpr<Func1,Func2> > >
    {
    private:
      typedef typename FunctionTraits<Func1>::DerivateFunc DF1;
      typedef typename FunctionTraits<Func2>::DerivateFunc DF2;
      typedef FunctionExpr<FunctionsCompositionExpr<DF1,Func2> > DF1_F2;
      typedef typename ComputeBinaryResult<DF1_F2,DF2,OpMult>::Handle Res;
    public:
      typedef Res DerivateFunc;
    };

    template<typename Func1,typename Func2>
    class FunctionsCompositionExpr
    {
  
      TFEL_STATIC_ASSERT((tfel::meta::Implements<Func1,FunctionConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<Func2,FunctionConcept>::cond));

      const Func1 a;
      const Func2 b;

      // Assignement operator (disabled)
      FunctionsCompositionExpr&
      operator = (const FunctionsCompositionExpr&);

      // Default Constructor (disabled)
      FunctionsCompositionExpr();

    protected:

      typedef Func1 first_arg;
      typedef Func2 second_arg;

    public:

      TFEL_MATH_INLINE
      FunctionsCompositionExpr(const Func1 a_, const Func2 b_)
	: a(a_), b(b_)
      {}

      // Copy Construtor
      TFEL_MATH_INLINE
      FunctionsCompositionExpr(const FunctionsCompositionExpr& src)
	: a(src.a), b(src.b)
      {}

      template<typename T>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	!tfel::typetraits::IsInvalid<
	typename ComputeUnaryResult<
	typename ComputeUnaryResult<T,Func2>::Result,Func1>::Result>::cond,
	typename ComputeUnaryResult<
	typename ComputeUnaryResult<T,Func2>::Handle,Func1>::Handle
      >::type
      operator()(const T& x) const
      {
	return a(b(x));
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
      
    }; // end of FunctionsCompositionExpr

    template<typename Func1,typename Func2>
    const typename 
    FunctionTraits<FunctionExpr<FunctionsCompositionExpr<Func1,Func2> > >::DerivateFunc
    derivate(const FunctionExpr<FunctionsCompositionExpr<Func1,Func2> >);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Function/FunctionsCompositionExpr.ixx"

#endif /* _LIB_TFEL_FUNCTIONSCOMPOSITIONEXPR_HXX */

