/*!
 * \file   include/TFEL/Math/General/AbsCompare.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03/08/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ABS_COMPARE_HXX
#define LIB_TFEL_MATH_ABS_COMPARE_HXX

#include "TFEL/Math/General/Abs.hxx"

namespace tfel::math {

  /*
   * \class absCompare
   * \brief a adaptable binary predicate which compare the absolute value
   * of two objects
   * \see http://www.sgi.com/tech/stl/AdaptableBinaryFunction.html for details.
   */
  template <typename T>
  struct absCompare {
    //! \brief a simple alias
    using first_argument_type = T;
    //! \brief a simple alias
    using second_argument_type = T;
    //! \brief a simple alias
    using result_type = bool;
    //!
    bool operator()(const T& a, const T& b) {
      return tfel::math::abs(a) > tfel::math::abs(b);
    }  // end of operation()

  };  // end of absCompare

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ABS_COMPARE_HXX */
