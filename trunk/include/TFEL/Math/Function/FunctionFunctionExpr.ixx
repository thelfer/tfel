/*!
 * \file   FunctionFunctionExpr.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 fév 2008
 */

#ifndef _LIB_TFEL_FUNCTIONFUNCTIONEXPR_IXX_
#define _LIB_TFEL_FUNCTIONFUNCTIONEXPR_IXX_ 

namespace tfel{

  namespace math{

    template<typename Func1,typename Func2,typename Op>
    const typename 
    FunctionTraits<FunctionExpr<FunctionFunctionExpr<Func1,Func2,Op> > >::DerivateFunc
    derivate(const FunctionExpr<FunctionFunctionExpr<Func1,Func2,Op> > f)
    {
      using namespace tfel::math::internals;
      return DerivateFunctionFunctionExpr<Func1,Func2,Op>::exe(f.getFunc1(),f.getFunc2());
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_FUNCTIONFUNCTIONEXPR_IXX */

