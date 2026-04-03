/*!
 * \file   tests/FSAlgorithms/iota.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28 Aug 2006
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

#include <iostream>
#include <cstdlib>
#include <cassert>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::fsalgo;

  int A1[3];
  iota<3>::exe(A1, 12);
  assert(A1[0] == 12);
  assert(A1[1] == 13);
  assert(A1[2] == 14);

  return EXIT_SUCCESS;
}
