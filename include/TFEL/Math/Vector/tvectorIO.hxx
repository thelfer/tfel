/*!
 * \file  tvectorIO.hxx
 * \brief
 * \author Helfer Thomas
 * \date   24 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TVECTORIO_H_
#define LIB_TFEL_MATH_TVECTORIO_H_

#include <ostream>
#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/Math/tvector.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N, typename T>
    std::ostream& operator<<(std::ostream&, const tvector<N, T>&);

    template <unsigned short N, typename T>
    std::ostream& operator<<(std::ostream& os, const tvector<N, T>& s) {
      using copy = tfel::fsalgo::copy<N>;
      os << "( ";
      copy::exe(s.begin(), std::ostream_iterator<T>(os, " "));
      os << ")";
      return os;
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TVECTORIO_H_ */
