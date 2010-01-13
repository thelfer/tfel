/*!
 * \file   Broyden.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 avr 2008
 */

#ifndef _LIB_TFEL_MATH_BROYDEN_HXX_
#define _LIB_TFEL_MATH_BROYDEN_HXX_ 

#include<utility>

#include"Math/tvector.hxx"

namespace tfel
{
  
  namespace math
  {

    template<unsigned short N,
	     typename T,
	     const tvector<N,T> (*f)(const tvector<N,T>&)>
    const std::pair<bool,tvector<N,T> >
    broyden(const tvector<N,T>&,
	    tmatrix<N,N,T>&,
	    const T,
	    const unsigned short);
    
    template<unsigned short N,
	     typename T,
	     const tvector<N,T> (*f)(const tvector<N,T>&)>
    const std::pair<bool,tvector<N,T> >
    broyden(const tvector<N,T>&,
	    const T,
	    const unsigned short);
    
  } // end of namespace math

} // end of namespace tfel

#include"Math/RootFinding/Broyden.ixx"

#endif /* _LIB_TFEL_MATH_BROYDEN_HXX */

