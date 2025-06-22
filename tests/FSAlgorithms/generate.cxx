/*!
 * \file   tests/FSAlgorithms/generate.cxx
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

#include <cstdlib>
#include <vector>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using tfel::fsalgo::generate;
  vector<int> V(10);
  generate<10>::exe(V.begin(), rand);
  return EXIT_SUCCESS;
}
