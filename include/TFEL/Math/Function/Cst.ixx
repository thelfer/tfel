/*!
 * \file   include/TFEL/Math/Function/Cst.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 fév 2008
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

