/*! 
 * \file  st2tost.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_ST2TOST2_H_
#define _LIB_TFEL_MATH_FORWARD_ST2TOST2_H_ 

namespace tfel{
  
  namespace math {

    /*
     * \class st2tost2
     * \brief finite size linear function on symmetric tensor.
     * \param N, the spatial dimension, see StensorDimeToSize for details. 
     * \param T, numerical type used, by default, double
     * \pre   This class is only defined for N=1u,2u and 3u.
     * \see   StensorDimeToSize and StensorSizeToDime.
     */
    template<unsigned short N,
	     typename T = double>
    class st2tost2;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_ST2TOST2_H */

