/*!
 * \file  include/TFEL/Math/Array/MutableRuntimeArrayBase.ixx
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

#ifndef LIB_TFEL_MATH_ARRAY_MUTABLERUNTIMEARRAYBASE_IXX
#define LIB_TFEL_MATH_ARRAY_MUTABLERUNTIMEARRAYBASE_IXX

#include "TFEL/Math/Array/IterateOverMultipleIndices.hxx"

namespace tfel::math {

  template <typename Child, typename ArrayPolicy>
  MutableRuntimeArrayBase<Child, ArrayPolicy>::MutableRuntimeArrayBase(
      const typename ArrayPolicy::IndexingPolicy& p)
      : ArrayPolicy::IndexingPolicy(p) {}  // end of MutableRuntimeArrayBase

  template <typename Child, typename ArrayPolicy>
  constexpr const typename ArrayPolicy::IndexingPolicy&
  MutableRuntimeArrayBase<Child, ArrayPolicy>::getIndexingPolicy()
      const noexcept {
    return *this;
  }  // end of getIndexingPolicy

  template <typename Child, typename ArrayPolicy>
  template <typename Functor>
  void MutableRuntimeArrayBase<Child, ArrayPolicy>::iterate(const Functor& f) {
    const auto g = [this](const typename Child::size_type i) {
      return this->size(i);
    };
    if constexpr (ArrayPolicy::IndexingPolicy::unRollLoop) {
      IterateOverMultipleIndices<typename Child::size_type, 0, Child::arity,
                                 true>::exe(f, g);
    } else {
      IterateOverMultipleIndices<typename Child::size_type, 0, Child::arity,
                                 false>::exe(f, g);
    }
  }  // end of iterate

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_MUTABLERUNTIMEARRAYBASE_HXX */
