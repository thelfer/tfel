/*! 
 * \file  KrigingDefaultModel.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL_H_
#define _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL_H_ 

#include<cmath>

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/Kriging/KrigingVariable.hxx"
#include"TFEL/Math/Kriging/KrigingDefaultNuggetModel.hxx"

namespace tfel
{

  namespace math
  {

    template<unsigned short N,
	     typename T,
	     typename NM = KrigingDefaultNuggetModel<N,T> >
    struct KrigingDefaultModel;

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Kriging/KrigingDefaultModel1D.hxx"
#include"TFEL/Math/Kriging/KrigingDefaultModel2D.hxx"
#include"TFEL/Math/Kriging/KrigingDefaultModel3D.hxx"

#endif /* _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL_H */

