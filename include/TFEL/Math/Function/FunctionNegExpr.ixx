/*!
 * \file   include/TFEL/Math/Function/FunctionNegExpr.ixx
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

