/*!
 * \file  IsConstCallable.hxx
 * \brief
 * \author Thomas Helfer
 * \date   27 févr. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_METAPROGRAMMING_ISCONSTCALLABLE_HXX
#define LIB_TFEL_METAPROGRAMMING_ISCONSTCALLABLE_HXX

#include <type_traits>

namespace tfel {

  namespace meta {

    /*!
     * a metafunction which determines of the
     * T::operator()(declval<Args>()...) const exists.
     */
    template <typename T, typename... Args>
    struct IsConstCallable {
      //! a simple alias
      using Small = char;
      //! a type type which size is higher than Small's one
      struct TFEL_VISIBILITY_LOCAL Big {
        Small dummy[2];
      };

     protected:
      /*!
       * A Test fonction which returns a Small.
       * Can only be called if B defines an iterator.
       */
      template <typename T2, typename... Args2>
      static typename std::enable_if<
          !std::is_same<decltype(std::declval<const T2&>().operator()(
                            std::declval<Args2>()...)),
                        InvalidType>::value,
          Small>::type
      test(const T2&, const Args2&...);
      /*!
       * A Test fonction which returns a Big.
       * It is called only if B does not defines an iterator.
       */
      static Big test(...);

     public:
      //! The result of the metafunction.
      static constexpr bool cond =
          sizeof(test(std::declval<typename std::decay<T>::type>(),
                      std::declval<typename std::decay<Args>::type>()...)) ==
          sizeof(Small);
    };  // end of struct IsConstCallable

  }  // end of namespace meta

}  // end of namespace tfel

#endif /* LIB_TFEL_METAPROGRAMMING_ISCONSTCALLABLE_HXX */
