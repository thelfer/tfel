/*!
 * \file   include/TFEL/Metaprogramming/IsSubClassOf.hxx
 * \brief  This file implements the IsSubClassOf metafunction.
 * \author Helfer Thomas
 * \date   28 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_ISSUBCLASSOF_H_
#define LIB_TFEL_ISSUBCLASSOF_H_

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace meta {

    /*!
     * \brief  The metafunction IsSubClassOf test if a given
     * class T publically inherits from type U.
     * This idea of this class was found in Andrei
     * Alexandrescu's book
     * \latexonly\cite{alexandrescu01:_c_desig}\endlatexonly
     * \htmlonly, modern C++ Design : generic programming
     * and design patterns applied\endhtmlonly.
     *
     * \param T, type to be tested.
     * \param U, we want to know if T publically inherits from U.
     * \return bool, true if T publically inherits to U, false otherwise.
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    template <typename T, typename U>
    struct TFEL_VISIBILITY_LOCAL IsSubClassOf {
      /*!
       * \brief the result of the metafunction.
       * True if T is publically inherits from U, false otherwise.
       * \see IsSuperClassOf
       */
      static constexpr bool cond = std::is_base_of<U, T>::value;
    };

  }  // end of namespace meta

}  // end of namespace tfel

#endif /* LIB_TFEL_ISSUBCLASSOF_H_ */
