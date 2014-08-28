/*! 
 * \file  src/Math/CubicSpline.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2011
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
