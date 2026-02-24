/*!
 * \file   include/TFEL/Metaprogramming/HasIterator.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   19/10/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_META_HASITERATOR_HXX
#define LIB_TFEL_META_HASITERATOR_HXX

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::meta {

  template <typename T, typename = void>
  struct HasIterator : std::false_type {};

  template <typename T>
  struct HasIterator<T, std::void_t<typename T::iterator>> : std::true_type {};

}  // end of namespace tfel::meta

#endif /* LIB_TFEL_META_HASITERATOR_HXX */
