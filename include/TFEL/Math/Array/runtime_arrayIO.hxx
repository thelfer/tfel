/*!
 * \file  runtime_arrayIO.hxx
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

#ifndef LIB_TFEL_MATH_RUNTIME_ARRAY_IO_HXX
#define LIB_TFEL_MATH_RUNTIME_ARRAY_IO_HXX

#include <ostream>
#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/Math/runtime_array.hxx"

namespace tfel::math {

  template <typename T>
  std::ostream& operator<<(std::ostream&, const runtime_array<T>&);

  template <typename T>
  std::ostream& operator<<(std::ostream& os, const runtime_array<T>& a) {
    os << "( ";
    for (const auto& v : a) {
      os << a << " ";
    }
    os << ")";
    return os;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_RUNTIME_ARRAY_IO_HXX */
