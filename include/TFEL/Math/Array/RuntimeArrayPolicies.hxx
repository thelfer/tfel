/*!
 * \file  include/TFEL/Math/Array/RuntimeArrayPolicies.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_RUNTIMEARRAYPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_RUNTIMEARRAYPOLICIES_HXX

#include "TFEL/ContractViolation.hxx"
#include "TFEL/Math/Array/ArrayPolicies.hxx"
#include "TFEL/Math/Array/RuntimeIndexingPolicies.hxx"

namespace tfel::math {

  //! \brief an array policy based on the runtime vector indexing policy
  template <typename ValueType>
  struct RuntimeVectorArrayPolicy
      : StandardArrayPolicy<ValueType, RuntimeVectorIndexingPolicy> {
  };  // end of struct RuntimeVectorArrayPolicy

  /*!
   * \brief an array policy based on the runtime row-major matrix indexing
   * policy
   */
  template <typename ValueType>
  struct RuntimeRowMajorMatrixArrayPolicy
      : StandardArrayPolicy<ValueType, RuntimeRowMajorMatrixIndexingPolicy> {
  };  // end of struct RuntimeRowMajorMatrixArrayPolicy

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_RUNTIMEARRAYPOLICIES_HXX */
