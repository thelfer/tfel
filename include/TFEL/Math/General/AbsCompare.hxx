/*!
 * \file   AbsCompare.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   03 aoû 2006
 */

#ifndef _LIB_TFEL_ABS_COMPARE_HXX_
#define _LIB_TFEL_ABS_COMPARE_HXX_ 

#include<functional>

namespace tfel{
  
  namespace math{

    /*
     * \class absCompare
     * \brief a adaptable binary predicate which compare the absolute value
     * of two objects
     * \see http://www.sgi.com/tech/stl/AdaptableBinaryFunction.html for details.
     */
    template<typename T>
    struct absCompare
      : public std::binary_function<T,T,bool>
    {	

      bool 
      operator()(const T&a,const T&b)
      {
	if(std::abs(a)>std::abs(b)){
	  return true;
	} else {
	  return false;
	}
      } // end of operation()

    }; // end of absCompare
    
  } // end of namespace math

} // end of namespace tfel
  

#endif /* _LIB_TFEL_ABS_COMPARE_HXX */

