/*!
 * \file  StensorConceptIO.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORCONCEPTIO_HXX
#define LIB_TFEL_MATH_STENSORCONCEPTIO_HXX

#include <ostream>
#include "TFEL/Math/Forward/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel::math {

  /*!
   * \brief serialisation operator
   * \param[in] os: output stream
   * \param[in] s: symmetric tensor
   */
  TFEL_HOST std::ostream& operator<<(std::ostream& os,
                                     const StensorConcept auto& s) {
    constexpr auto size =
        StensorDimeToSize<getSpaceDimension<decltype(s)>()>::value;
    os << "[ ";
    for (unsigned short i = 0; i != size; ++i) {
      os << s(i) << " ";
    }
    os << "]";
    return os;
  }  // end of operator <<

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSORCONCEPTIO_HXX */
