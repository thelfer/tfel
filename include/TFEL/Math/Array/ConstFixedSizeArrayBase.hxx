/*!
 * \file  include/TFEL/Math/Array/ConstFixedSizeArrayBase.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_CONSTFIXEDSIZEARRAYBASE_HXX
#define LIB_TFEL_MATH_ARRAY_CONSTFIXEDSIZEARRAYBASE_HXX

#include "TFEL/Math/Array/ArrayPolicies.hxx"
#include "TFEL/Math/Array/ArrayCommonMethods.hxx"
#include "TFEL/Math/Array/StandardTemplateLibraryCompatibilityLayers.hxx"

namespace tfel::math {

  /*!
   * \brief a base class for fixed sized const arrays.
   */
  template <typename Child, typename ArrayPolicyType>
  struct ConstFixedSizeArrayBase
      : public ArrayPolicyType::IndexingPolicy,
        public ConstArrayCommonMethods<Child, ArrayPolicyType>,
        public ConstArraySTLCompatibilityLayer<Child, ArrayPolicyType> {
    //!
    static_assert(std::is_empty_v<typename ArrayPolicyType::IndexingPolicy>);
    //! \return the current indexing policy
    constexpr auto& getIndexingPolicy() const;
    /*!
     * \brief apply a multi-indices functor
     * \param[in] f: functor
     * \tparam Function: type of the functor
     */
    template <typename Functor>
    constexpr void iterate(const Functor&) const;
  };

}  // namespace tfel::math

#include "TFEL/Math/Array/ConstFixedSizeArrayBase.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_CONSTFIXEDSIZEARRAYBASE_HXX */
