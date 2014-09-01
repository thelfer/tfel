/*!
 * \file   include/TFEL/Math/Function/FunctionsCompositionExpr.ixx
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

