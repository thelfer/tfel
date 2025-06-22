/*!
 * \file   src/Math/MathException.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   14 fév 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Math/MathException.hxx"

namespace tfel {

  namespace math {

    MathException::~MathException() noexcept = default;

    MathRunTimeException::~MathRunTimeException() noexcept = default;

    const char* InvalidTimeStepException::what() const noexcept {
      return "invalid time step";
    }  // end of InvalidTimeStepException::what

    InvalidTimeStepException::~InvalidTimeStepException() noexcept = default;

    MathDivergenceException::~MathDivergenceException() noexcept = default;

    const char* MaximumNumberOfIterationsReachedException::what()
        const noexcept {
      return "maximum number of iterations reached";
    }  // end of MaximumNumberOfIterationsReachedException::what

    MaximumNumberOfIterationsReachedException::
        ~MaximumNumberOfIterationsReachedException() noexcept = default;

    const char* SingularJacobianException::what() const noexcept {
      return "jacobian is not invertible";
    }  // end of SingularJacobianException::what

    SingularJacobianException::~SingularJacobianException() noexcept = default;

  }  // end of namespace math

}  // end of namespace tfel
