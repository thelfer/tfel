/*!
 * \file   tests/FSAlgorithms/transform.cxx
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

#include <iostream>
#include <functional>
#include <iterator>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <cmath>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using tfel::fsalgo::fill;
  using tfel::fsalgo::iota;
  using tfel::fsalgo::transform;

  constexpr unsigned short N = 10;
  vector<int> V1(N);
  vector<int> V2(N);
  vector<int> V3(N);
  double A[N];

  iota<N>::exe(A, 1);
  for (int i = 0; i != N; ++i) {
    assert(abs(A[i] - i - 1) < 1.e-14);
  }

  transform<N>::exe(A, A, negate<double>());
  for (int i = 0; i != N; ++i) {
    assert(abs(A[i] + i + 1) < 1.e-14);
  }

  iota<N>::exe(V1.begin(), 1);
  int v = 0;
  for (vector<int>::size_type i = 0; i != N; ++i) {
    assert(V1[i] == ++v);
  }

  fill<N>::exe(V2.begin(), 75);
  for (vector<int>::size_type i = 0; i != N; ++i) {
    assert(V2[i] == 75);
  }

  transform<N>::exe(V1.begin(), V2.begin(), V3.begin(), plus<int>());
  for (vector<int>::size_type i = 0; i != N; ++i) {
    assert(V3[i] == V1[i] + V2[i]);
  }

  return EXIT_SUCCESS;
}
