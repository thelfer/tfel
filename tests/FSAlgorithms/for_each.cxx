/*!
 * \file   tests/FSAlgorithms/for_each.cxx
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

#include <cstdlib>
#include <cassert>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

template <class T>
struct count : public std::unary_function<T, void> {
  count() : c(0) {}
  void operator()(const T&) { ++c; }
  int c;
};

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::fsalgo;
  int A[] = {1, 4, 2, 8, 5, 7};
  count<int> P;
  for_each<6>::exe(A, P);
  assert(P.c == 6);
  return EXIT_SUCCESS;
}
