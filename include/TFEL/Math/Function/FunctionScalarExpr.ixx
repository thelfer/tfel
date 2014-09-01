/*!
 * \file   include/TFEL/Math/Function/FunctionScalarExpr.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 fév 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

