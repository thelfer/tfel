/*!
 * \file   tests/FSAlgorithms/generate.cxx
 * \brief
 * \author Thomas Helfer
 * \date 28/08/2006
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
#include <vector>
#include <algorithm>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using tfel::fsalgo::generate;
  vector<int> V(10);
  srand(1);
  generate<10>::exe(V.begin(), rand);
  // same sequence as the standard algorithm
  vector<int> V2(10);
  srand(1);
  std::generate(V2.begin(), V2.end(), rand);
  assert(V == V2);
  return EXIT_SUCCESS;
}
