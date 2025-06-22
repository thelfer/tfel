/*!
 * \file   include/TFEL/TypeTraits/IsInvalid.hxx
 * \brief  This file declares the IsInvalid traits class.
 * \author Thomas Helfer
 * \date   14 oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TYPETRAITS_ISINVALID_HXX
#define LIB_TFEL_TYPETRAITS_ISINVALID_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"

namespace tfel::typetraits {

  /*!
   * \brief  Traits class which says if its argument is invalid.
   *
   * \tparam  T, type to be tested.
   * \return bool cond, true if T is invalid, false otherwise.
   *
   * \author Thomas Helfer
   * \date   Apr 2006
   */
  template <typename T>
  struct IsInvalid {
    //! \brief result
    static constexpr auto cond = std::is_same_v<tfel::meta::InvalidType, T>;
  };

  //! \brief a simple wrapper around `IsInvalid`
  template <typename T>
  constexpr auto isInvalid() {
    return IsInvalid<std::decay_t<T>>::cond;
  }  // end of isInvalid

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_TYPETRAITS_ISINVALID_HXX */
