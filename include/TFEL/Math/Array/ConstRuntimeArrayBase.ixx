/*!
 * \file  include/TFEL/Math/Array/ConstRuntimeArrayBase.ixx
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

#ifndef LIB_TFEL_MATH_ARRAY_CONSTRUNTIMEARRAYBASE_IXX
#define LIB_TFEL_MATH_ARRAY_CONSTRUNTIMEARRAYBASE_IXX

#include "TFEL/Math/Array/IterateOverMultipleIndices.hxx"

namespace tfel::math {

  template <typename Child, typename ArrayPolicy>
  constexpr ConstRuntimeArrayBase<Child, ArrayPolicy>::ConstRuntimeArrayBase(
      const typename ArrayPolicy::IndexingPolicy& i)
      : ArrayPolicy::IndexingPolicy(i) {}  // end of ConstRuntimeArrayBase

  template <typename Child, typename ArrayPolicy>
  constexpr auto& ConstRuntimeArrayBase<Child, ArrayPolicy>::getIndexingPolicy()
      const noexcept {
    return static_cast<const typename ArrayPolicy::IndexingPolicy&>(*this);
  }  // end of getIndexingPolicy

  template <typename Child, typename ArrayPolicy>
  template <typename Functor>
  void ConstRuntimeArrayBase<Child, ArrayPolicy>::iterate(
      const Functor& f) const {
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

}  // namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_CONSTRUNTIMEARRAYBASE_IXX */
