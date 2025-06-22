/*!
 * \file   include/TFEL/TypeTraits/IsUnaryOperator.hxx
 * \brief  This file declares the IsUnaryOperator traits class.
 * \author Thomas Helfer
 * \date   07 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_ISOPERATOR_HXX
#define LIB_TFEL_ISOPERATOR_HXX

namespace tfel {

  namespace typetraits {

    /*!
     * \brief Traits class which says if its argument stands for a
     * unary operator
     *
     * \param  T, type to be tested.
     * \return bool cond, true if T stands for a unary operator, false
     * otherwise.
     *
     * \author Thomas Helfer
     * \date   08/01/07
     */
    template <typename T>
    struct IsUnaryOperator {
      /*!
       *  Result
       */
      static constexpr bool cond = false;
    };

  }  // end of namespace typetraits

}  // end of namespace tfel

#endif /* LIB_TFEL_ISOPERATOR_HXX */
