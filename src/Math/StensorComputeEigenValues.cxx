/*!
 * \file  StensorComputeEigenValues.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Math/stensor.hxx"

namespace tfel {

  namespace math {

    namespace internals {

      const char* StensorEigenValuesComputationFailureException::what()
          const noexcept {
        return "eigen value computation failed";
      }

      StensorEigenValuesComputationFailureException::
          ~StensorEigenValuesComputationFailureException() noexcept = default;

    }  // end of namespace internals
  }    // end of namespace math
}  // end of namespace tfel
