/*!
 * \file   include/TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx
 * \brief  This file implements the IsSafelyReinterpretCastableTo traits class.
 * \author Thomas Helfer
 * \date   26 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_IS_SAFELY_REINTERPRET_CASTABLE_HXX
#define LIB_TFEL_IS_SAFELY_REINTERPRET_CASTABLE_HXX

namespace tfel {

  namespace typetraits {

    /*!
     * \class IsSafelyReinterpretCastableTo
     * \brief Traits class which says if the expressions:
     *  - reinterpret_cast<B&>(a)
     *  - reinterpret_cast<B*>(p)
     *  - reinterpret_cast<const B&>(a)
     *  - reinterpret_cast<const B* const>(p)
     * shall be considered valid for all object a of type A and for
     * all object of type A*.
     *
     * \param  A, first type
     * \param  B, second type
     * \return bool cond, true if the above expressions are valid,
     * false otherwise.
     *
     * \author Thomas Helfer
     * \date   28 jun 2006
     */
    template <typename A, typename B>
    struct IsSafelyReinterpretCastableTo;

    /*!
     * Partial specialisation if the two types in argument are valid
     */
    template <typename T>
    struct IsSafelyReinterpretCastableTo<T, T> {
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };  // end of struct IsSafelyReinterpretCastableTo

  }  // end of namespace typetraits

}  // end of namespace tfel

#endif /* LIB_TFEL_IS_SAFELY_REINTERPRET_CASTABLE_HXX */
