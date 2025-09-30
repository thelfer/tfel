/*!
 * \file  include/TFEL/Math/Array/ArrayPolicies.ixx
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

#ifndef LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_IXX
#define LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_IXX

namespace tfel::math {

  template <typename ArrayPolicyType>
  constexpr bool hasArrayPolicyFixedSizes() {
    return ArrayPolicyType::IndexingPolicy::hasFixedSizes;
  }  // end of hasArrayPolicyFixedSizes

  template <typename ArrayPolicyType>
  constexpr auto getArrayPolicySize() {
    static_assert(hasArrayPolicyFixedSizes<ArrayPolicyType>(),
                  "array policy must have fixed sizes");
    const typename ArrayPolicyType::IndexingPolicy p;
    return p.size();
  }  // end of getArrayPolicySize

  template <typename ArrayPolicyType>
  constexpr auto getArrayPolicyMinimalDataSize() {
    static_assert(hasArrayPolicyFixedSizes<ArrayPolicyType>(),
                  "array policy must have fixed sizes");
    const typename ArrayPolicyType::IndexingPolicy p;
    return p.getUnderlyingArrayMinimalSize();
  }  // end of getArrayPolicyMinimalDataSize

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_HXX */
