/*! 
 * \file  include/TFEL/Math/Forward/array.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_ARRAY_H_
#define _LIB_TFEL_MATH_FORWARD_ARRAY_H_ 

namespace tfel{
  
  namespace math {

    // Forward declaration
    template<unsigned short,typename>
    struct ArrayRowMajorStorage;

    // Forward declaration
    template<unsigned short N,
	     typename T=double,
	     template<unsigned short,
		      typename> class TStorage=ArrayRowMajorStorage>
    struct array;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_ARRAY_H */

