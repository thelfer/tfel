/*!
 * \file  include/TFEL/Math/Array/ArrayPolicies.ixx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_IXX
#define LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_IXX

namespace tfel::math {

  template <typename ArrayPolicy>
  constexpr bool hasArrayPolicyFixedSizes() {
    return ArrayPolicy::IndexingPolicy::hasFixedSizes;
  }  // end of hasArrayPolicyFixedSizes

  template <typename ArrayPolicy>
  constexpr typename ArrayPolicy::IndexingPolicy::size_type
  getArrayPolicySize() {
    static_assert(hasArrayPolicyFixedSizes<ArrayPolicy>(),
                  "array policy must have fixed sizes");
    const typename ArrayPolicy::IndexingPolicy p;
    return p.size();
  }  // end of getArrayPolicySize

  template <typename ArrayPolicy>
  constexpr typename ArrayPolicy::IndexingPolicy::size_type
  getArrayPolicyMinimalDataSize() {
    static_assert(hasArrayPolicyFixedSizes<ArrayPolicy>(),
                  "array policy must have fixed sizes");
    const typename ArrayPolicy::IndexingPolicy p;
    return p.getUnderlyingArrayMinimalSize();
  }  // end of getArrayPolicyMinimalDataSize

}  // end of namespace tfel::math

#include "TFEL/Math/Array/ArrayPolicies.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_HXX */
