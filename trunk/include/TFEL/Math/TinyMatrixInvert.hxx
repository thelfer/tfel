/*! 
 * \file  TinyMatrixInvert.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 08 mars 2013
 */

#ifndef _LIB_TFEL_MATH_TINYMATRIXINVERT_H_
#define _LIB_TFEL_MATH_TINYMATRIXINVERT_H_ 

#include<limits>

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/TinyMatrixSolve.hxx"

namespace tfel{
  
  namespace math{

    template<unsigned short N,
	     typename T>
    struct TinyMatrixInvert
    {
      
      static void
      exe(tfel::math::tmatrix<N,N,T>&,
	  const T = 100*std::numeric_limits<T>::min());
    }; // end of struct TinyMatrixInvert

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Matrix/TinyMatrixInvert.ixx"

#endif /* _LIB_TFEL_MATH_TINYMATRIXINVERT_H */

