/*!
 * \file  include/TFEL/Math/Array/ConstFixedSizeArrayBase.ixx
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

#ifndef LIB_TFEL_MATH_ARRAY_CONSTFIXEDSIZEARRAYBASE_IXX
#define LIB_TFEL_MATH_ARRAY_CONSTFIXEDSIZEARRAYBASE_IXX

#include "TFEL/Math/Array/IterateOverMultipleIndices.hxx"

namespace tfel::math {

  template <typename Child, typename ArrayPolicy>
  constexpr auto&
  ConstFixedSizeArrayBase<Child, ArrayPolicy>::getIndexingPolicy() const {
    return static_cast<const typename ArrayPolicy::IndexingPolicy&>(*this);
  }  // end of getIndexingPolicy

  template <typename Child, typename ArrayPolicy>
  template <typename Functor>
  constexpr void ConstFixedSizeArrayBase<Child, ArrayPolicy>::iterate(
      const Functor& f) const {
    const auto g = [](typename Child::size_type i) constexpr {
      typename ArrayPolicy::IndexingPolicy p;
      return p.size(i);
    };
    if constexpr (ArrayPolicy::IndexingPolicy::unRollLoop) {
      IterateOverMultipleIndices<typename Child::size_type, 0, Child::arity,
                                 true>::exe(f, g);
    } else {
      IterateOverMultipleIndices<typename Child::size_type, 0, Child::arity,
                                 false>::exe(f, g);
    }
  }  // end of iterate

}  // namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_CONSTFIXEDSIZEARRAYBASE_IXX */
