/*!
 * \file  tmatrixIO.hxx
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

#ifndef LIB_TFEL_MATH_TMATRIXIO_HXX
#define LIB_TFEL_MATH_TMATRIXIO_HXX

#include <ostream>
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N, unsigned short M, typename T>
    std::ostream& operator<<(std::ostream&, const tmatrix<N, M, T>&);

    template <unsigned short N, unsigned short M, typename T>
    std::ostream& operator<<(std::ostream& os, const tmatrix<N, M, T>& m) {
      unsigned short i, j;
      os << "[";
      for (i = 0; i < N; ++i) {
        os << "[";
        for (j = 0; j < M; ++j) {
          os << m(i, j);
          if (j != M - 1) {
            os << ",";
          }
        }
        os << "]";
      }
      os << "]";
      return os;
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TMATRIXIO_HXX */
