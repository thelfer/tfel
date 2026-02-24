/*!
 * \file  fsarrayIO.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21/05/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FSARRAYIO_HXX
#define LIB_TFEL_MATH_FSARRAYIO_HXX

#include <ostream>
#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/Math/fsarray.hxx"

namespace tfel::math {

  template <unsigned short N, typename T>
  std::ostream& operator<<(std::ostream&, const fsarray<N, T>&);

  template <unsigned short N, typename T>
  std::ostream& operator<<(std::ostream& os, const fsarray<N, T>& s) {
    using copy = tfel::fsalgo::copy<N>;
    os << "( ";
    copy::exe(s.begin(), std::ostream_iterator<T>(os, " "));
    os << ")";
    return os;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FSARRAYIO_HXX */
