/*!
 * \file   FunctionNegExpr.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 fév 2008
 */

#ifndef _LIB_TFEL_FUNCTIONNEGEXPR_IXX_
#define _LIB_TFEL_FUNCTIONNEGEXPR_IXX_ 

namespace tfel{

  namespace math{

    template<typename F>
    const typename FunctionTraits<FunctionExpr<FunctionNegExpr<F> > >::DerivateFunc
    derivate(const FunctionExpr<FunctionNegExpr<F> > f)
    {
      return -derivate(f.getFunc());
    }

  } // end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_FUNCTIONNEGEXPR_IXX */

