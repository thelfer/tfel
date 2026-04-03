/*!
 * \file  include/TFEL/Math/Array/MutableFixedSizeArrayBase.ixx
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

#ifndef LIB_TFEL_MATH_ARRAY_MUTABLEFIXEDSIZEARRAYBASE_IXX
#define LIB_TFEL_MATH_ARRAY_MUTABLEFIXEDSIZEARRAYBASE_IXX

#include "TFEL/Math/Array/IterateOverMultipleIndices.hxx"

namespace tfel::math {

  template <typename Child, typename ArrayPolicyType>
  constexpr const auto&
  MutableFixedSizeArrayBase<Child, ArrayPolicyType>::getIndexingPolicy() const {
    return static_cast<const typename ArrayPolicyType::IndexingPolicy&>(*this);
  }  // end of getIndexingPolicy

  template <typename Child, typename ArrayPolicyType>
  template <typename Functor>
  constexpr void MutableFixedSizeArrayBase<Child, ArrayPolicyType>::iterate(
      const Functor& f) {
    constexpr auto g = [](const typename Child::size_type i) {
      typename ArrayPolicyType::IndexingPolicy p;
      return p.size(i);
    };
    if constexpr (ArrayPolicyType::IndexingPolicy::unRollLoop) {
      IterateOverMultipleIndices<typename Child::size_type, 0, Child::arity,
                                 true>::exe(f, g);
    } else {
      IterateOverMultipleIndices<typename Child::size_type, 0, Child::arity,
                                 false>::exe(f, g);
    }
  }  // end of iterate

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_MUTABLEFIXEDSIZEARRAYBASE_HXX */
