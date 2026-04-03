/*!
 * \file   tests/FSAlgorithms/inner_product.cxx
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

#include <iostream>

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::fsalgo;
  int A1[] = {1, 2, 4};
  int A2[] = {4, 1, -2};
  std::cout << inner_product<3>::exe(A1, A2, 0) << std::endl;
  assert(inner_product<3>::exe(A1, A2, 0) == -2);
  return EXIT_SUCCESS;
}
