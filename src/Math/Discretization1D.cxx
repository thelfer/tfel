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
    
    const char*
    GeometricDiscretizationInvalidDensity::what() const noexcept
    {
      return "geometricDiscretization : invalid density";
    } // end of GeometricDiscretizationInvalidDensity::what

    GeometricDiscretizationInvalidDensity::~GeometricDiscretizationInvalidDensity() noexcept
    {} // end of GeometricDiscretizationInvalidDensity::~GeometricDiscretizationInvalidDensity

    const char*
    GeometricDiscretizationInvalidLength::what() const noexcept
    {
      return "geometricDiscretization : invalid length";
    } // end of GeometricDiscretizationInvalidLength::what

    GeometricDiscretizationInvalidLength::~GeometricDiscretizationInvalidLength() noexcept
    {} // end of GeometricDiscretizationInvalidLength::~GeometricDiscretizationInvalidLength

    const char*
    GeometricDiscretizationInvalidNumberOfElements::what() const noexcept
    {
      return "geometricDiscretization : invalid number of elements";
    } // end of GeometricDiscretizationInvalidNumberOfElements::what

    GeometricDiscretizationInvalidNumberOfElements::~GeometricDiscretizationInvalidNumberOfElements() noexcept
    {} // end of GeometricDiscretizationInvalidNumberOfElements::~GeometricDiscretizationInvalidNumberOfElements

  } // end of namespace math

} // end of namespace tfel
