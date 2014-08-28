/*! 
 * \file  include/TFEL/Math/Forward/tvector.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 sept. 2011
 */

#ifndef _LIB_TFEL_MATH_FORWARD_TVECTOR_H_
#define _LIB_TFEL_MATH_FORWARD_TVECTOR_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace math
  {

    /*
     * a base for tvector or classes acting like tvector.
     */
    template<typename Child,
	     unsigned short N,
	     typename T>
    struct tvector_base;

    /*
     * a class representing tiny vectors of fixed size.
     */
    template<unsigned short N, typename T>
    struct tvector;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_TVECTOR_H */

