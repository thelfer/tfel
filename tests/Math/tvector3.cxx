/*!
 * \file   tests/Math/tvector3.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   08 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  tvector<2> v1;
  tvector<2> v2;
  tmatrix<2, 2> m;
  v1(0) = 0.5;
  v1(1) = 0.25;
  v2(0) = 4.;
  v2(1) = 2.;
  m = v1 ^ v2;
  return EXIT_SUCCESS;
}
