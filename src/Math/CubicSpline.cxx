/*! 
 * \file  src/Math/CubicSpline.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Math/CubicSpline.hxx"

namespace tfel
{

  namespace math
  {

    CubicSplineUninitialised::CubicSplineUninitialised()
      : tfel::exception::TFELException("CubicSpline : undefined collocation points")
    {} // end of CubicSplineUninitialised::CubicSplineUninitialised

    CubicSplineNullPivot::CubicSplineNullPivot()
      : tfel::exception::TFELException("CubicSpline : null pivot")
    {} // end of CubicSplineNullPivot::CubicSplineNullPivot

    CubicSplineInvalidAbscissaVectorSize::CubicSplineInvalidAbscissaVectorSize()
      : tfel::exception::TFELException("CubicSpline : abscissa size is smaller than 1")
    {} // end of CubicSplineInvalidAbscissaVectorSize::CubicSplineInvalidAbscissaVectorSize
    
    CubicSplineInvalidOrdinateVectorSize::CubicSplineInvalidOrdinateVectorSize()
      : tfel::exception::TFELException("CubicSpline : ordinate size is smaller than 1")
    {} // end of CubicSplineInvalidOrdinateVectorSize::CubicSplineInvalidOrdinateVectorSize
    
    CubicSplineInvalidInputs::CubicSplineInvalidInputs()
      : tfel::exception::TFELException("CubicSpline : abscissa and ordinate "
				       "vectors don't have the same size")
    {} // end of CubicSplineInvalidInputs::CubicSplineInvalidInputs

    CubicSplineUnorderedAbscissaVector::CubicSplineUnorderedAbscissaVector()
      : tfel::exception::TFELException("CubicSpline : abscissa vector "
				       "is not ordered")
    {} // end of CubicSplineUnorderedAbscissaVector::CubicSplineUnorderedAbscissaVector

  } // end of namespace math

} // end of namespace tfel
