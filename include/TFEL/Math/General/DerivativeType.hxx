/*!
 * \file   include/TFEL/Math/General/DerivativeType.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24/06/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_DERIVATIVETYPE_HXX
#define LIB_TFEL_MATH_DERIVATIVETYPE_HXX

namespace tfel {

  namespace math {

    /*!
     * \brief a meta function returning the type of the derivative of
     * a variable of type T1 with respect to a variable of type T2
     * \tparam T1: type
     * \tparam T2: type
     */
    template <typename T1, typename T2>
    struct DerivativeType;

    //! a simple alias
    template <typename T1, typename T2>
    using derivative_type = typename DerivativeType<T1, T2>::type;

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/General/DerivativeType.ixx"

#endif /* LIB_TFEL_MATH_DERIVATIVETYPE_HXX */
