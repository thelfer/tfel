/*!
 * \file  include/TFEL/Math/Array/FixedSizeArrayPolicies.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYPOLICIES_HXX

#include "TFEL/Math/Array/ArrayPolicies.hxx"
#include "TFEL/Math/Array/FixedSizeIndexingPolicies.hxx"

namespace tfel::math {

  //! \brief an array policy based on the fixed size vector indexing policy
  template <unsigned int N, typename ValueType>
  struct FixedSizeVectorPolicy
      : StandardArrayPolicy<ValueType,
                            FixedSizeVectorIndexingPolicy<unsigned int, N>> {
  };  // end of struct FixedSizeVectorPolicy

  /*!
   * \brief an array policy describing a fixed size matrix stored in row major
   * format based on the fixed size matrix indexing policy.
   */
  template <unsigned int N, unsigned int M, typename ValueType>
  struct FixedSizeRowMajorMatrixPolicy
      : StandardArrayPolicy<
            ValueType,
            FixedSizeRowMajorMatrixIndexingPolicy<unsigned int, N, M>> {
  };  // end of struct FixedSizeRowMajorMatrixPolicy

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYPOLICIES_HXX */
