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

#include"Config/TFELConfig.hxx"

#include"TypeTraits/IsReal.hxx"

#include"Math/tvector.hxx"
#include"Math/LU/TinyPermutation.hxx"
#include"Math/tmatrix.hxx"

#include"Math/LU/LUException.hxx"

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

