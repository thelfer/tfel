/*!
 * \file  qtIO.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24/01/2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QTIO_HXX
#define LIB_TFEL_MATH_QTIO_HXX

#include <ostream>
#include "TFEL/Math/qt.hxx"

namespace tfel::math {

  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  std::ostream& operator<<(
      std::ostream&, const Quantity<UnitType, ValueType, OwnershipPolicy>& q) {
    os << q.getValue();
    return os;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_QTIO_HXX */
