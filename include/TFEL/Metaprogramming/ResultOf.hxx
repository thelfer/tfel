/*!
 * \file  ResultOf.hxx
 * \brief
 * \author Helfer Thomas
 * \date   14 févr. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_METAPROGRAMMING_RESULTOF_H_
#define LIB_TFEL_METAPROGRAMMING_RESULTOF_H_

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Metaprogramming/IsConstCallable.hxx"

namespace tfel {

  namespace meta {

    template <bool isCallable, typename, typename...>
    struct ResultOfDispatch {
      typedef InvalidType type;
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
      typedef InvalidType type;
    };  // end of struct ResultOf

  }  // end of namespace meta

}  // end of namespace tfel

#endif /* LIB_TFEL_METAPROGRAMMING_RESULTOF_H_ */
