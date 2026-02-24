/*!
 * \file  T2toST2ConceptIO.hxx
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

#ifndef LIB_TFEL_MATH_T2TOST2CONCEPTIO_HXX
#define LIB_TFEL_MATH_T2TOST2CONCEPTIO_HXX

#include <ostream>
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"

namespace tfel::math {

  //! \brief serialisation operator
  TFEL_HOST std::ostream& operator<<(std::ostream& os,
                                     const T2toST2Concept auto& t) {
    using T2toST2Type = decltype(t);
    constexpr auto N = getSpaceDimension<T2toST2Type>();
    constexpr auto ssize = StensorDimeToSize<N>::value;
    constexpr auto tsize = TensorDimeToSize<N>::value;
    os << "[";
    for (unsigned short i = 0; i < ssize;) {
      if (i != 0) {
        os << " [";
      } else {
        os << "[";
      }
      for (unsigned short j = 0; j < tsize;) {
        os << t(i, j);
        if (++j != tsize) {
          os << ",";
        }
      }
      if (++i != ssize) {
        os << "]\n";
      } else {
        os << "]";
      }
    }
    os << "]";
    return os;
  }  // end of operator <<

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOST2CONCEPTIO_HXX */
