/*!
 * \file   include/TFEL/Metaprogramming/Implements.hxx
 * \brief  This file describes the Implements metafunction
 * \author Helfer Thomas
 * \date   28 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_IMPLEMENTS_H_
#define LIB_TFEL_IMPLEMENTS_H_

#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace meta {

    /*!
     * \class Implements
     *
     * \param T, class to be tested.
     * \param concept, concept template class.
     * \return bool.
     *
     * \see concepts_sec for more details about how concepts are modelled in
     * TFEL.
     *
     * \brief the Implements metafunction returns true if the
     * class T is a subclass of concept<T>.
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    template <typename T, template <typename> class concept>
    struct TFEL_VISIBILITY_LOCAL Implements {
      /*!
       * The result of the metafunction.
       */
      static constexpr bool cond = std::is_base_of<concept<T>, T>::value;
    };

    template <typename T, template <typename> class concept>
    constexpr bool implements(void) {
      return Implements<T, concept>::cond;
    }

  }  // end of namespace meta

}  // end of namespace tfel

#endif /* LIB_TFEL_IMPLEMENTS_H_ */
