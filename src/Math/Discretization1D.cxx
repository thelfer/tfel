/*! 
 * \file  src/Math/Discretization1D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 déc. 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
