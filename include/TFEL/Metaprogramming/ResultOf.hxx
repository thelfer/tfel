/*!
 * \file include/TFEL/Metaprogramming/ResultOf.hxx
 * \brief
 * \author Thomas Helfer
 * \date   14 févr. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_METAPROGRAMMING_RESULTOF_HXX
#define LIB_TFEL_METAPROGRAMMING_RESULTOF_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Metaprogramming/IsConstCallable.hxx"

namespace tfel::meta {

  template <bool isCallable, typename, typename...>
  struct ResultOfDispatch {
    using type = InvalidType;
  };

  template <typename T, typename... Args>
  struct ResultOfDispatch<true, T, Args...> {
    using type = decltype(std::declval<T>()(std::declval<Args>()...));
  };

  template <typename T, typename... Args>
  struct ResultOf
      : ResultOfDispatch<IsConstCallable<T, Args...>::cond, T, Args...> {
  };  // end of struct ResultOf

  template <typename... Args>
  struct ResultOf<InvalidType, Args...> {
    using type = InvalidType;
  };  // end of struct ResultOf

  //! \brief a simple alias
  template <typename T, typename... Args>
  using result_of = typename ResultOf<T, Args...>::type;

}  // end of namespace tfel::meta

#endif /* LIB_TFEL_METAPROGRAMMING_RESULTOF_HXX */
