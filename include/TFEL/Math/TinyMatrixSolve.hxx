/*!
 * \file   TinyMatrixSolve.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Aug 2006
 */

#ifndef _LIB_TFEL_TINYMATRIXSOLVE_H_
#define _LIB_TFEL_TINYMATRIXSOLVE_H_ 

#include<cmath>
#include<limits>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/TypeTraits/IsReal.hxx"

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/LU/TinyPermutation.hxx"
#include"TFEL/Math/tmatrix.hxx"

#include"TFEL/Math/LU/LUException.hxx"

namespace tfel{

  namespace math{

    template<unsigned short N,typename T>
    struct TinyMatrixSolve
    {

      static void 
      exe(tmatrix<N,N,T>&, tvector<N,T>&,
	  const T = 100*std::numeric_limits<T>::min());

    };      

  } // end of namespace math

} // end of namespace tfel

#include "LU/TinyMatrixSolve.ixx"

#endif /* _LIB_TFEL_TINYMATRIXSOLVE_H */

