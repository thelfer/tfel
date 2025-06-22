/*!
 * \file  ConceptRebind.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12/02/2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CONCEPTREBIND_HXX
#define LIB_TFEL_MATH_CONCEPTREBIND_HXX

#include "TFEL/Metaprogramming/InvalidType.hxx"

namespace tfel::math {

  /*!
   * \brief the `ConceptRebind` metafunction use the given tag to define a
   * base class for the `Type` class.
   */
  template <typename Tag, typename Type>
  struct ConceptRebind;

  /*!
   * \brief the `ConceptRebind` metafunction use the given tag to define a
   * base class for the `Type` class.
   */
  template <typename Type>
  struct ConceptRebind<tfel::meta::InvalidType, Type> {
    struct EmptyType {};
    using type = EmptyType;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_CONCEPTREBIND_HXX */
