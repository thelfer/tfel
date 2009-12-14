/*!
 * \file   TinyNewtonRaphson.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   02 Aug 2006
 */

#ifndef _LIB_TFEL_TINYNEWTONRAPHSON_H_
#define _LIB_TFEL_TINYNEWTONRAPHSON_H_ 

#include<string>

#include"Config/TFELConfig.hxx"

#include"Utilities/Name.hxx"
#include"Utilities/ToString.hxx"
#include"FSAlgorithm/FSAlgorithm.hxx"

#include"Math/MathException.hxx"
#include"Math/TinyMatrixSolve.hxx"
#include"Math/tvector.hxx"
#include"Math/tmatrix.hxx"

namespace tfel{

  namespace math{

    template<unsigned short N,
	     typename T,
	     typename Func>
    class TinyNewtonRaphson;

  } // end of namespace math

} // end of namespace tfel

#include"Math/Newton-Raphson/TinyNewtonRaphson.ixx"

#endif /* _LIB_TFEL_TINYNEWTONRAPHSON_H */

