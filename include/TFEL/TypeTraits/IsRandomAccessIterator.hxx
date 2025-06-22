/*!
 * \file   include/TFEL/TypeTraits/IsRandomAccessIterator.hxx
 * \brief  This file declares the IsRandomAccessIterator class.
 * \author Thomas Helfer
 * \date   15/08/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TYPETRAITS_ISRANDOMAACCESSITERATOR_HXX
#define LIB_TFEL_TYPETRAITS_ISRANDOMAACCESSITERATOR_HXX

#include <iterator>
#include <type_traits>

namespace tfel::typetraits {

  /*!
   * \brief Traits class which says if its argument is a random
   * access iterator
   *
   * \param  T, type to be tested.
   * \return bool cond, true if T stands for a random access
   * iterator, false otherwise.
   *
   * \author Thomas Helfer
   * \date   04/07/2006
   */
  template <typename T>
  struct IsRandomAccessIterator {
    //! \brief result
    static constexpr bool cond =
        std::is_same_v<typename std::iterator_traits<T>::iterator_category,
                       std::random_access_iterator_tag>;
  };

  /*!
   * \brief a simple helper function
   * \tparam T: type tested
   */
  template <typename T>
  constexpr bool isRandomAccessIterator() {
    return IsRandomAccessIterator<std::decay_t<T>>::cond;
  }  // end of isRandomAccessIterator

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_TYPETRAITS_ISRANDOMAACCESSITERATOR_HXX */
