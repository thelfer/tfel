/*!
 * \file  tvectorIO.hxx
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

#ifndef LIB_TFEL_MATH_TVECTORIO_HXX
#define LIB_TFEL_MATH_TVECTORIO_HXX

#include <ostream>
#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Array/CoalescedView.hxx"
#include "TFEL/Math/Array/StridedCoalescedView.hxx"
#include "TFEL/Math/tvector.hxx"

namespace tfel::math {

  template <unsigned short N, typename T>
  std::ostream& operator<<(std::ostream&, const tvector<N, T>&);

  template <unsigned short N, typename T>
  std::ostream& operator<<(std::ostream& os, const tvector<N, T>& s) {
    os << "( ";
    tfel::fsalgo::copy<N>::exe(s.begin(), std::ostream_iterator<T>(os, " "));
    os << ")";
    return os;
  }

  template <unsigned short N, typename T>
  std::ostream& operator<<(std::ostream& os, const View<tvector<N, T>>& s) {
    os << "( ";
    tfel::fsalgo::copy<N>::exe(s.begin(), std::ostream_iterator<T>(os, " "));
    os << ")";
    return os;
  }

  template <unsigned short N, typename T>
  std::ostream& operator<<(std::ostream& os,
                           const View<const tvector<N, T>>& s) {
    os << "( ";
    tfel::fsalgo::copy<N>::exe(s.begin(), std::ostream_iterator<T>(os, " "));
    os << ")";
    return os;
  }

  template <unsigned short N, typename T>
  std::ostream& operator<<(std::ostream& os,
                           const CoalescedView<tvector<N, T>>& s) {
    os << "(";
    for (unsigned short i = 0; i != N; ++i) {
      os << " " << s(i);
    }
    os << " )";
    return os;
  }

  template <unsigned short N, typename T>
  std::ostream& operator<<(std::ostream& os,
                           const CoalescedView<const tvector<N, T>>& s) {
    os << "(";
    for (unsigned short i = 0; i != N; ++i) {
      os << " " << s(i);
    }
    os << " )";
    return os;
  }

  template <unsigned short N, typename T>
  std::ostream& operator<<(std::ostream& os,
                           const StridedCoalescedView<tvector<N, T>>& s) {
    os << "(";
    for (unsigned short i = 0; i != N; ++i) {
      os << " " << s(i);
    }
    os << " )";
    return os;
  }

  template <unsigned short N, typename T>
  std::ostream& operator<<(std::ostream& os,
                           const StridedCoalescedView<const tvector<N, T>>& s) {
    os << "(";
    for (unsigned short i = 0; i != N; ++i) {
      os << " " << s(i);
    }
    os << " )";
    return os;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TVECTORIO_HXX */
