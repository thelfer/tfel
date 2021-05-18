/*!
 * \file  include/TFEL/Math/Array/FixedSizeArrayViewIO.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYVIEWIO_HXX
#define LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYVIEWIO_HXX

#include <ostream>
#include "TFEL/Math/Array/FixedSizeArrayView.hxx"

namespace tfel::math {

  template <typename MappedArrayType, index_type<MappedArrayType> stride>
  std::ostream& operator<<(
      std::ostream& os, const FixedSizeArrayView<MappedArrayType, stride>& v) {
    os << "[ ";
    for (index_type<MappedArrayType> i = 0; i < v.size(); ++i) {
      os << v[i] << " ";
    }
    os << "]";
    return os;
  }  // end of operator <<

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_FIXEDSIZEARRAYVIEWIO_HXX */
