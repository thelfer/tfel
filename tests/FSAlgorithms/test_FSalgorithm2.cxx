/*!
 * \file   tests/FSAlgorithms/test_FSalgorithm2.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   15 aoû 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <vector>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iterator>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using uishort = unsigned short;
  std::vector<uishort> v(10);
  std::vector<uishort> v2(10);
  tfel::fsalgo::iota<10u>::exe(v.begin(), uishort{0});
  tfel::fsalgo::copy<10u>::exe(v.begin(), v2.begin());
  for (std::vector<uishort>::size_type i = 0; i != 10; ++i) {
    assert(v2[i] == i);
  }
  return EXIT_SUCCESS;
}
