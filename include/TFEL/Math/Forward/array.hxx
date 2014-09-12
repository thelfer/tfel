/*! 
 * \file  include/TFEL/Math/Forward/array.hxx
 * \brief This file introduces some forward declaration relative to
 * the array class.
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

