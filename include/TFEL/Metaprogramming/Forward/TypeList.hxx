/*!
 * \file  include/TFEL/Metaprogramming/Forward/TypeList.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 12 janv. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_METAPROGRAMMING_FORWARD_TYPELIST_HXX
#define LIB_TFEL_METAPROGRAMMING_FORWARD_TYPELIST_HXX

namespace tfel::meta {

  /*!
   * Node of TypeList
   * \param T : current type
   * \param U : next type
   */
  template <typename T, typename U>
  struct TLNode;

  struct TLE;

}  // end of namespace tfel::meta

#endif /* LIB_TFEL_METAPROGRAMMING_FORWARD_TYPELIST_HXX */
