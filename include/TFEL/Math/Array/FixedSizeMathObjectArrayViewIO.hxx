/*!
 * \file  include/TFEL/Math/Array/FixedSizeMathObjectArrayViewIO.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12/05/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_FIXEDSIZEMATHOBJECTARRAYVIEWIO_HXX
#define LIB_TFEL_MATH_ARRAY_FIXEDSIZEMATHOBJECTARRAYVIEWIO_HXX

#include <ostream>
#include "TFEL/Math/Array/FixedSizeMathObjectArrayView.hxx"

namespace tfel::math {

  template <unsigned short N, typename MappedType, unsigned short stride>
  std::ostream& operator<<(
      std::ostream& os,
      const FixedSizeMathObjectArrayView<N, MappedType, stride>& v) {
    os << "[ ";
    for (unsigned short i = 0; i < N; ++i) {
      os << v[i] << " ";
    }
    os << "]";
    return os;
  }  // end of operator <<

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_FIXEDSIZEMATHOBJECTARRAYVIEWIO_HXX */
