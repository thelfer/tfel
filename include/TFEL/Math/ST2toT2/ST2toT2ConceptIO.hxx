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

namespace tfel {

  namespace math {

    // Serialisation operator
    template <typename T>
    std::ostream& operator<<(std::ostream&, const ST2toT2Concept<T>&);

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const ST2toT2Concept<T>& s) {
      constexpr auto N = ST2toT2Traits<T>::dime;
      os << "[";
      for (unsigned short i = 0; i < TensorDimeToSize<N>::value;) {
        if (i != 0) {
          os << " [";
        } else {
          os << "[";
        }
        for (unsigned short j = 0; j < StensorDimeToSize<N>::value;) {
          os << s(i, j);
          if (++j != StensorDimeToSize<N>::value) {
            os << ",";
          }
        }
        if (++i != TensorDimeToSize<N>::value) {
          os << "]\n";
        } else {
          os << "]";
        }
      }
      os << "]";
      return os;
    }  // end of operator <<

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_ST2TOT2CONCEPTIO_HXX */
