/*!
 * \file  ST2toST2ConceptIO.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24 janv. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2CONCEPTIO_HXX
#define LIB_TFEL_MATH_ST2TOST2CONCEPTIO_HXX

#include <ostream>
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel::math {

  // Serialisation operator
  TFEL_HOST std::ostream& operator<<(std::ostream& os,
                                     const ST2toST2Concept auto& s) {
    using ST2toST2Type = decltype(s);
    constexpr auto stensor_size =
        StensorDimeToSize<getSpaceDimension<ST2toST2Type>()>::value;
    os << "[";
    for (unsigned short i = 0; i < stensor_size;) {
      if (i != 0) {
        os << " [";
      } else {
        os << "[";
      }
      for (unsigned short j = 0; j < stensor_size;) {
        os << s(i, j);
        if (++j != stensor_size) {
          os << ",";
        }
      }
      if (++i != stensor_size) {
        os << "]\n";
      } else {
        os << "]";
      }
    }
    os << "]";
    return os;
  }  // end of operator <<

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOST2CONCEPTIO_HXX */
