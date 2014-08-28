/*! 
 * \file  include/TFEL/Math/Forward/t2tot2.hxx
 * \brief
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _LIB_TFEL_MATH_FORWARD_T2TOT2_H_
#define _LIB_TFEL_MATH_FORWARD_T2TOT2_H_ 

namespace tfel{
  
  namespace math {

    /*
     * \class t2tot2
     * \brief finite size linear function which turns a (unsymmetric)
     * tensor to symmetric tensor.
     * \param N, the spatial dimension, see StensorDimeToSize for details. 
     * \param T, numerical type used, by default, double
     * \pre   This class is only defined for N=1u,2u and 3u.
     * \see   StensorDimeToSize and StensorSizeToDime.
     */
    template<unsigned short N,
	     typename T = double>
    struct t2tot2;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_T2TOT2_H */

