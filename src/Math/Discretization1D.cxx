/*!
 * \file  src/Math/Discretization1D.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 déc. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Math/Discretization1D.hxx"

namespace tfel::math {

  const char* GeometricDiscretizationInvalidDensity::what() const noexcept {
    return "geometricDiscretization : invalid density";
  }  // end of GeometricDiscretizationInvalidDensity::what

  GeometricDiscretizationInvalidDensity::
      ~GeometricDiscretizationInvalidDensity() noexcept = default;

  const char* GeometricDiscretizationInvalidLength::what() const noexcept {
    return "geometricDiscretization : invalid length";
  }  // end of GeometricDiscretizationInvalidLength::what

  GeometricDiscretizationInvalidLength::
      ~GeometricDiscretizationInvalidLength() noexcept = default;

  const char* GeometricDiscretizationInvalidNumberOfElements::what()
      const noexcept {
    return "geometricDiscretization : invalid number of elements";
  }  // end of GeometricDiscretizationInvalidNumberOfElements::what

  GeometricDiscretizationInvalidNumberOfElements::
      ~GeometricDiscretizationInvalidNumberOfElements() noexcept = default;

}  // end of namespace tfel::math
