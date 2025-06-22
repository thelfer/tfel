/*!
 * \file   include/TFEL/Metaprogramming/HasIterator.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   19 oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_HASITERATOR_HXX
#define LIB_TFEL_HASITERATOR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include <type_traits>

namespace tfel {

  namespace meta {

    /*!
     * \class HasIterator
     * \param typename A, type tested.
     * \return bool cond, true if the type defines a iterator.
     */
    template <typename A>
    class TFEL_VISIBILITY_LOCAL HasIterator {
      /*!
       * \brief A first type.
       */
      typedef char Small;
      /*!
       * \brief A Second type which size is higher than Small
       */
      class TFEL_VISIBILITY_LOCAL Big {
        Small dummy[2];
      };
      /*
       * \brief a substitute for classes that have only protected constructors
       */
      template <typename B>
      struct Subs {};

     protected:
      /*
       * A Test fonction which returns a Small.
       * Can only be called if B defines an iterator.
       */
      template <typename B>
      static typename std::enable_if<sizeof(typename B::iterator) != 0,
                                     Small>::type
      Test(const Subs<B>);

      /*
       * A Test fonction which returns a Big.
       * It is called only if B does not defines an iterator.
       */
      static Big Test(...);

      /*!
       * \brief A function returning a T.
       * A small trick for classes that are not default constructible.
       */
      static Subs<A> MakeSubsA();

     public:
      /*
       * The result of the metafunction.
       */
      static constexpr bool cond = sizeof(Test(MakeSubsA())) == sizeof(Small);
    };

  }  // end of namespace meta

}  // end of namespace tfel

#endif /* LIB_TFEL_HASITERATOR_HXX */
