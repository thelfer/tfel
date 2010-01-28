/*!
 * \file   FunctionsCompositionExpr.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 fév 2008
 */

#ifndef _LIB_TFEL_FUNCTIONSCOMPOSITIONEXPR_IXX_
#define _LIB_TFEL_FUNCTIONSCOMPOSITIONEXPR_IXX_ 

namespace tfel{

  namespace math{

    template<typename Func1,typename Func2>
    const typename 
    FunctionTraits<FunctionExpr<FunctionsCompositionExpr<Func1,Func2> > >::DerivateFunc
    derivate(const FunctionExpr<FunctionsCompositionExpr<Func1,Func2> > f)
    {
      return (derivate(f.getFunc1())[f.getFunc2()])*derivate(f.getFunc2());
    } // end of function derivate

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_FUNCTIONSCOMPOSITIONEXPR_IXX */

