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

#ifndef LIB_TFEL_TYPETRAITS_ISUNARYOPERATOR_HXX
#define LIB_TFEL_TYPETRAITS_ISUNARYOPERATOR_HXX

#include <type_traits>

namespace tfel::typetraits {

  /*!
   * \brief Traits class which says if its argument stands for a
   * unary operator
   *
   * \tparam  T: type to be tested.
   * \return bool cond, true if T stands for a unary operator, false
   * otherwise.
   *
   * \author Thomas Helfer
   * \date   08/01/07
   */
  template <typename T>
  struct IsUnaryOperator {
    //! \brief result
    static constexpr bool cond = false;
  };

  /*!
   * \brief a simple helper function
   * \tparam  T: type to be tested.
   */
  template <typename T>
  constexpr auto isUnaryOperator() {
    return IsUnaryOperator<std::remove_cv_t<T>>::cond;
  }  // end of isUnaryOperator

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_TYPETRAITS_ISUNARYOPERATOR_HXX */
