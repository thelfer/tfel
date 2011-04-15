/*! 
 * \file  CubicSpline.cxx
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
      : tfel::exception::TFELException("undefined collocation points")
    {} // end of CubicSplineUninitialised::CubicSplineUninitialised

    CubicSplineInvalidAbscissaVectorSize::CubicSplineInvalidAbscissaVectorSize()
      : tfel::exception::TFELException("abscissa size is smaller than 3")
    {} // end of CubicSplineInvalidAbscissaVectorSize::CubicSplineInvalidAbscissaVectorSize
    
    CubicSplineInvalidOrdinateVectorSize::CubicSplineInvalidOrdinateVectorSize()
      : tfel::exception::TFELException("ordinate size is smaller than 3")
    {} // end of CubicSplineInvalidOrdinateVectorSize::CubicSplineInvalidOrdinateVectorSize
    
    CubicSplineInvalidInputs::CubicSplineInvalidInputs()
      : tfel::exception::TFELException("abscissa and ordinate vectors don't have the same size")
    {} // end of CubicSplineInvalidInputs::CubicSplineInvalidInputs

    CubicSplineUnorderedAbscissaVector::CubicSplineUnorderedAbscissaVector()
      : tfel::exception::TFELException("abscissa vector is not ordered")
    {} // end of CubicSplineUnorderedAbscissaVector::CubicSplineUnorderedAbscissaVector

  } // end of namespace math

} // end of namespace tfel
