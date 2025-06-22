/*!
 * \file  include/TFEL/Math/Array/ArrayPolicies.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_HXX

#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/IndexingPolicies.hxx"

namespace tfel::math {

  //! \brief a base class defining some aliases for standard array policies
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

  //! \brief a standard array policy.
  template <typename ValueType, typename IndexingPolicyType>
  struct StandardArrayPolicy : StandardArrayPolicyAliases<ValueType> {
    //! \brief type of the values hold by the array.
    using storage_type = ValueType;
    //!
    static constexpr auto isMakeConstReferenceTrivial = true;
    //! \brief make a const_reference from a const reference to a base type
    static constexpr typename StandardArrayPolicy::const_reference
    make_const_reference(
        const typename StandardArrayPolicy::storage_type& v) noexcept {
      return v;
    }
    //!
    static constexpr auto isMakeReferenceTrivial = true;
    //! \brief make a reference from a reference to a base type
    static constexpr typename StandardArrayPolicy::reference make_reference(
        typename StandardArrayPolicy::storage_type& v) noexcept {
      return v;
    }
    //! \brief the underlying indexing policy
    using IndexingPolicy = IndexingPolicyType;
  };

  /*!
   * \brief an helper structure to define a math
   * object traits from an array policy.
   */
  template <typename ArrayPolicyType>
  struct MathObjectTraitsFromArrayPolicy
      : MathObjectTraitsBase<
            typename ArrayPolicyType::value_type,
            typename ArrayPolicyType::IndexingPolicy::size_type> {
  };  // end of struct MathObjectTraitsFromArrayPolicy

  /*!
   * \return if the given array policy has fixed sizes.
   * \tparam ArrayPolicyType: the array policy
   */
  template <typename ArrayPolicyType>
  constexpr bool hasArrayPolicyFixedSizes();

  /*!
   * \return the logical size of the given array policy, i.e. the number of
   * values that can be accessed by the given policy.
   * \tparam ArrayPolicyType: the array policy
   * \pre the array policy must have fixed sizes.
   */
  template <typename ArrayPolicyType>
  constexpr auto getArrayPolicySize();

  /*!
   * \return the minimal number of values required to stored the given array
   * policy
   * \tparam ArrayPolicyType: the array policy
   * \pre the array policy must have fixed sizes.
   */
  template <typename ArrayPolicyType>
  constexpr auto getArrayPolicyMinimalDataSize();

}  // end of namespace tfel::math

#include "TFEL/Math/Array/ArrayPolicies.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_ARRAYPOLICIES_HXX */
