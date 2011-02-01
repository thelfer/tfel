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

    /*!
     * solve a linear problem using LU decomposition
     * \param N : dimension of the system
     * \param T : numerical type
     */
    template<unsigned short N,typename T>
    struct TinyMatrixSolve
    {

      /*!
       * solve the linear system m.x = b
       * \param m   : matrix to be inverted
       * \param b   : right member
       * \param eps : numerical paramater to detect null pivot
       * \note the matrix m is overwritten during computations
       * \note the right member is overwritten by the solution
       */
      static void 
      exe(tmatrix<N,N,T>& m, tvector<N,T>&,
	  const T = 100*std::numeric_limits<T>::min());

    }; 

    /*!
     * solve a linear problem by direct invertion
     * \param T : numerical type
     */
    template<typename T>
    struct TinyMatrixSolve<2u,T>
    {

      /*!
       * solve the linear system m.x = b
       * \param m   : matrix to be inverted
       * \param b   : right member
       * \param eps : numerical paramater to detect null pivot
       * \note the right member is overwritten by the solution
       */
      static void 
      exe(const tmatrix<2u,2u,T>& m, tvector<2u,T>&,
	  const T = 100*std::numeric_limits<T>::min());

    }; 

    /*!
     * solve a linear problem by direct invertion
     * \param T : numerical type
     */
    template<typename T>
    struct TinyMatrixSolve<3u,T>
    {

      /*!
       * solve the linear system m.x = b
       * \param m   : matrix to be inverted
       * \param b   : right member
       * \param eps : numerical paramater to detect null pivot
       * \note the right member is overwritten by the solution
       */
      static void 
      exe(const tmatrix<3u,3u,T>& m, tvector<3u,T>&,
	  const T = 100*std::numeric_limits<T>::min());

    }; 

  } // end of namespace math

} // end of namespace tfel

#include "LU/TinyMatrixSolve.ixx"

#endif /* _LIB_TFEL_TINYMATRIXSOLVE_H */

