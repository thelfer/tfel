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

    const char*
    CubicSplineUninitialised::what() const noexcept
    {
      return "CubicSpline : undefined collocation points";
    } // end of CubicSplineUninitialised::what

    CubicSplineUninitialised::~CubicSplineUninitialised() noexcept
    {} // end of CubicSplineUninitialised::~CubicSplineUninitialised

    const char*
    CubicSplineNullPivot::what() const noexcept
    {
      return "CubicSpline : null pivot";
    } // end of CubicSplineNullPivot::what

    CubicSplineNullPivot::~CubicSplineNullPivot() noexcept
    {} // end of CubicSplineNullPivot::CubicSplineNullPivot

    const char*
    CubicSplineInvalidAbscissaVectorSize::what() const noexcept
    {
      return "CubicSpline : abscissa size is smaller than 1";
    } // end of CubicSplineInvalidAbscissaVectorSize::what
    
    CubicSplineInvalidAbscissaVectorSize::~CubicSplineInvalidAbscissaVectorSize() noexcept
    {} // end of CubicSplineInvalidAbscissaVectorSize::~CubicSplineInvalidAbscissaVectorSize

    const char*
    CubicSplineInvalidOrdinateVectorSize::what() const noexcept
    {
      return "CubicSpline : ordinate size is smaller than 1";
    } // end of CubicSplineInvalidOrdinateVectorSize::what

    CubicSplineInvalidOrdinateVectorSize::~CubicSplineInvalidOrdinateVectorSize() noexcept
    {} // end of CubicSplineInvalidOrdinateVectorSize::~CubicSplineInvalidOrdinateVectorSize
    
    const char*
    CubicSplineInvalidInputs::what() const noexcept
    {
      return "CubicSpline : abscissa and ordinate vectors don't have the same size";
    } // end of CubicSplineInvalidInputs::what

    CubicSplineInvalidInputs::~CubicSplineInvalidInputs() noexcept
    {} // end of CubicSplineInvalidInputs::~CubicSplineInvalidInputs

    const char*
    CubicSplineUnorderedAbscissaVector::what() const noexcept
    {
      return "CubicSpline : abscissa vector is not ordered";
    } // end of CubicSplineUnorderedAbscissaVector::what

    CubicSplineUnorderedAbscissaVector::~CubicSplineUnorderedAbscissaVector() noexcept
    {} // end of CubicSplineUnorderedAbscissaVector::~CubicSplineUnorderedAbscissaVector

  } // end of namespace math

} // end of namespace tfel
