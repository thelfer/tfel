/*!
 * \file  include/TFEL/Math/Array/MutableFixedSizeArrayBase.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_MUTABLEFIXEDSIZEARRAYBASE_HXX
#define LIB_TFEL_MATH_ARRAY_MUTABLEFIXEDSIZEARRAYBASE_HXX

#include "TFEL/Math/Array/ArrayPolicies.hxx"
#include "TFEL/Math/Array/ArrayCommonMethods.hxx"
#include "TFEL/Math/Array/FixedSizeArrayPolicies.hxx"
#include "TFEL/Math/Array/StandardTemplateLibraryCompatibilityLayers.hxx"

namespace tfel::math {

  /*!
   * \brief a base class for mutable fixed size arrays.
   */
  template <typename Child, typename ArrayPolicyType>
  struct MutableFixedSizeArrayBase
      : public ArrayPolicyType::IndexingPolicy,
        public MutableArrayCommonMethods<Child, ArrayPolicyType>,
        public MutableArraySTLCompatibilityLayer<Child, ArrayPolicyType> {
    // some checks
    static_assert(std::is_empty_v<typename ArrayPolicyType::IndexingPolicy>);
    static_assert(hasArrayPolicyFixedSizes<ArrayPolicyType>(),
                  "array policy must have fixed sizes");
    static_assert(getArrayPolicyMinimalDataSize<ArrayPolicyType>() != 0,
                  "invalid minimal container size");
    // exposing MutableArrayCommonMethods assignement operators
    using MutableArrayCommonMethods<Child, ArrayPolicyType>::operator=;
    //! \return the current indexing policy
    constexpr auto& getIndexingPolicy() const;
    /*!
     * \brief apply a multi-indices functor
     * \param[in] f: functor
     * \tparam Function: type of the functor
     */
    template <typename Functor>
    TFEL_HOST_DEVICE constexpr void iterate(const Functor&);
  };  // end of struct MutableFixedSizeArrayBase

}  // end of namespace tfel::math

#include "TFEL/Math/Array/MutableFixedSizeArrayBase.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_MUTABLEFIXEDSIZEARRAYBASE_HXX */
