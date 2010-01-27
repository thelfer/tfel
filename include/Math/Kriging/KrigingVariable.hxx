/*! 
 * \file  KrigingVariables.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGINGVARIABLES_H_
#define _LIB_TFEL_MATH_KRIGINGVARIABLES_H_ 

#include"Math/tvector.hxx"

namespace tfel
{

  namespace math
  {
  
    template<unsigned short N,
	     typename T>
    struct KrigingVariable
    {
      typedef tvector<N,T> type;
    }; // end of KrigingVariable<N,T>

    template<typename T>
    struct KrigingVariable<1u,T>
    {
      typedef T type;
    }; // end of KrigingVariable<1u,T>
    
  }// end of namespace math
  
}// end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGINGVARIABLES_H */

