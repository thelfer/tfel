/*!
 * \file   include/TFEL/Math/Function/ScalarFunctionExpr.ixx
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

#ifndef _LIB_TFEL_SCALARFUNCTIONEXPR_IXX_
#define _LIB_TFEL_SCALARFUNCTIONEXPR_IXX_ 

namespace tfel{

  namespace math{

    template<typename Scal,typename Func,typename Op>
    const typename 
    FunctionTraits<FunctionExpr<ScalarFunctionExpr<Scal,Func,Op> > >::DerivateFunc
    derivate(const FunctionExpr<ScalarFunctionExpr<Scal,Func,Op> > f)
    {
      using namespace tfel::math::internals;
      return DerivateScalarFunctionExpr<Scal,Func,Op>::exe(f.getScalar(),f.getFunc());
    } 

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_SCALARFUNCTIONEXPR_IXX */

