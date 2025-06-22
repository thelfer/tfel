/*!
 * \file   tests/FSAlgorithms/accumulate.cxx
 * \brief
 * \author Helfer Thomas
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

#include <cstdlib>
#include <cassert>
#include <functional>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using tfel::fsalgo::accumulate;
  int A[] = {1, 2, 3, 4, 5};
  const int N = sizeof(A) / sizeof(int);
  assert(accumulate<N>::exe(A, 0) == 15);
  assert(accumulate<N>::exe(A, 1, multiplies<int>()) == 120);
  return EXIT_SUCCESS;
}
