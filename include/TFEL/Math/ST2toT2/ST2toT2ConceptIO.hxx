/*!
 * \file  ST2toT2ConceptIO.hxx
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

#ifndef LIB_TFEL_MATH_ST2TOT2CONCEPTIO_HXX
#define LIB_TFEL_MATH_ST2TOT2CONCEPTIO_HXX

#include <ostream>
#include "TFEL/Math/ST2toT2/ST2toT2Concept.hxx"

namespace tfel::math {

  // Serialisation operator
  template <typename ST2toT2Type>
  std::enable_if_t<implementsST2toT2Concept<ST2toT2Type>(), std::ostream&>
  operator<<(std::ostream& os, const ST2toT2Type& s) {
    constexpr auto N = getSpaceDimension<ST2toT2Type>();
    constexpr auto stensor_size = StensorDimeToSize<N>::value;
    constexpr auto tensor_size = TensorDimeToSize<N>::value;
    os << "[";
    for (unsigned short i = 0; i < tensor_size;) {
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
      if (++i != tensor_size) {
        os << "]\n";
      } else {
        os << "]";
      }
    }
    os << "]";
    return os;
  }  // end of operator <<

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOT2CONCEPTIO_HXX */
