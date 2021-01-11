/*!
 * \file  include/TFEL/Math/Array/ArrayPolicies.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_HXX

#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/IndexingPolicies.hxx"

namespace tfel::math {

  /*!
   * \brief a base class defining some aliases for standard array policies
   */
  template <typename ValueType>
  struct StandardArrayPolicyAliases {
    //! \brief type of the values hold by the array.
    using value_type = ValueType;
    //! \brief const type of the values hold by the array.
    using const_value_type = const value_type;
    //! \brief type of a const reference to the value contained.
    using reference = value_type&;
    //! \brief type of a const reference to the value contained.
    using const_reference = const value_type&;
  };  // end of struct StandardArrayPolicyAliases

  /*!
   * \brief an helper structure to define a math object traits from an array
   * policy.
   */
  template <typename ArrayPolicy>
  struct MathObjectTraitsFromArrayPolicy
      : MathObjectTraitsBase<typename ArrayPolicy::value_type,
                             typename ArrayPolicy::IndexingPolicy::size_type> {
  };  // end of struct MathObjectTraitsFromArrayPolicy

  /*!
   * \return if the given array policy has fixed sizes.
   * \tparam ArrayPolicy: the array policy
   */
  template <typename ArrayPolicy>
  constexpr bool hasArrayPolicyFixedSizes();

  /*!
   * \return the logical size of the given array policy, i.e. the number of
   * values that can be accessed by the given policy.
   * \pre the array policy must have fixed sizes.
   * \tparam ArrayPolicy: the array policy
   */
  template <typename ArrayPolicy>
  constexpr typename ArrayPolicy::IndexingPolicy::size_type
  getArrayPolicySize();

  /*!
   * \return the minimal number of values required to stored the given array
   * policy
   * \pre the array policy must have fixed sizes.
   * \tparam ArrayPolicy: the array policy
   */
  template <typename ArrayPolicy>
  constexpr typename ArrayPolicy::IndexingPolicy::size_type
  getArrayPolicyMinimalDataSize();

}  // end of namespace tfel::math

#include "TFEL/Math/Array/ArrayPolicies.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_HXX */
