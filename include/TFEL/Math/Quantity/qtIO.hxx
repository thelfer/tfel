/*!
 * \file  qtIO.hxx
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

#ifndef LIB_TFEL_MATH_QTIO_HXX
#define LIB_TFEL_MATH_QTIO_HXX

#include <ostream>
#include "TFEL/Math/qt.hxx"

namespace tfel {

  namespace math {

    template <typename unit, typename T>
    TFEL_MATH_INLINE2 std::ostream& operator<<(std::ostream&,
                                               const qt<unit, T>&);

    template <typename unit, typename T>
    std::ostream& operator<<(std::ostream& os, const qt<unit, T>& q) {
      os << q.getValue();
      return os;
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_QTIO_HXX */
