/*!
 * \file   include/TFEL/Math/Function/Cst.ixx
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

#ifndef _LIB_TFEL_CST_IXX_
#define _LIB_TFEL_CST_IXX_ 

namespace tfel{

  namespace math{

    template<short N,unsigned short D>
    Cst<0> derivate(const Cst<N,D>)
    {
      return Cst<0>();
    } // end of function derivate

  } // end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_CST_IXX */

