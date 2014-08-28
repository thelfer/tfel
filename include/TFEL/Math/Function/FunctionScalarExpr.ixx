/*!
 * \file   include/TFEL/Math/Function/FunctionScalarExpr.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 fév 2008
 */

#ifndef _LIB_TFEL_FUNCTIONSCALAREXPR_IXX_
#define _LIB_TFEL_FUNCTIONSCALAREXPR_IXX_ 

namespace tfel{

  namespace math{

    template<typename Func,typename Scal,typename Op>
    const typename 
    FunctionTraits<FunctionExpr<FunctionScalarExpr<Func,Scal,Op> > >::DerivateFunc
    derivate(const FunctionExpr<FunctionScalarExpr<Func,Scal,Op> >& f)
    {
      using namespace tfel::math::internals;
      return DerivateFunctionScalarExpr<Scal,Func,Op>::exe(f.getFunc(),f.getScalar());
    } // end of function derivate

  } // end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_FUNCTIONSCALAREXPR_IXX */

