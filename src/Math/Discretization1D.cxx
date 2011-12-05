/*! 
 * \file  Discretization1D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 déc. 2011
 */

#include"TFEL/Math/Discretization1D.hxx"

namespace tfel
{

  namespace math
  {
    
    GeometricDiscretizationInvalidDensity::GeometricDiscretizationInvalidDensity()
      : MathRunTimeException("geometricDiscretization : invalid density")
    {} // end of GeometricDiscretizationInvalidDensity::GeometricDiscretizationInvalidDensity

    GeometricDiscretizationInvalidLength::GeometricDiscretizationInvalidLength()
      : MathRunTimeException("geometricDiscretization : invalid length")
    {} // end of GeometricDiscretizationInvalidLength::GeometricDiscretizationInvalidLength

    GeometricDiscretizationInvalidNumberOfElements::GeometricDiscretizationInvalidNumberOfElements()
      : MathRunTimeException("geometricDiscretization : invalid number of elements")
    {} // end of GeometricDiscretizationInvalidNumberOfElements::GeometricDiscretizationInvalidNumberOfElements

  } // end of namespace math

} // end of namespace tfel
