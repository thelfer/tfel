/*!
 * \file   include/TFEL/Metaprogramming/HasRandomAccessConstIterator.hxx
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

#ifndef LIB_TFEL_HASRANDOMACCESSCONSTITERATOR_HXX
#define LIB_TFEL_HASRANDOMACCESSCONSTITERATOR_HXX

#include <iterator>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::meta {

  /*!
   * \class HasRandomAccessConstIterator
   * \param typename A, type tested.
   * \return bool cond, true if the type defines a random access const_iterator.
   */
  template <typename A>
  class TFEL_VISIBILITY_LOCAL HasRandomAccessConstIterator {
    /*!
     * \brief A first type.
     */
    typedef char Small;
    /*!
     * \brief A Second type whose size is higher than Small
     */
    class Big {
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
     * Can only be called if B defines a random access const iterator.
     */
    template <typename B>
    static typename std::enable_if<
        std::is_same<typename std::iterator_traits<
                         typename B::const_iterator>::iterator_category,
                     std::random_access_iterator_tag>::value,
        Small>::type
    Test(const Subs<B>);

    /*
     * A Test fonction which returns a Big.
     * It is called if B does not defines a random access const iterator.
     */
    static Big Test(...);

    /*!
     * \brief A function returning a T.
     * A small trick for classes that are not default constructible.
     */
    static Subs<A> MakeSubsA();

   public:
    //! \brief result of the metafunction.
    static constexpr bool cond = sizeof(Test(MakeSubsA())) == sizeof(Small);
  };

}  // end of namespace tfel::meta

#endif /* LIB_TFEL_HASRANDOMACCESSCONSTITERATOR_HXX */
