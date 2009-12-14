/*! 
 * \file  KrigingDefaultModel.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL_H_
#define _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL_H_ 

#include<cmath>

#include"Math/tvector.hxx"
#include"Math/Kriging/KrigingVariable.hxx"

namespace tfel
{

  namespace math
  {

    template<unsigned short N,
	     typename T>
    struct KrigingDefaultModel;

  } // end of namespace math

} // end of namespace tfel

#include"Math/Kriging/KrigingDefaultModel1D.hxx"
#include"Math/Kriging/KrigingDefaultModel2D.hxx"
#include"Math/Kriging/KrigingDefaultModel3D.hxx"

#endif /* _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL_H */

