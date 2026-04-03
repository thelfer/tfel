/*!
 * \file   tests/Math/tvector2.cxx
 * \brief
 * \author Thomas Helfer
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
  tvector<2> v;
  tvector<2> v2;
  tmatrix<2, 2> m(0.);

  v(0) = 1.2;
  v(1) = 2.;
  m(0, 0) = 1.;
  m(0, 1) = 0.5;
  m(1, 0) = 0.5;
  m(1, 1) = 1.;

  v2 = m * v;

  return EXIT_SUCCESS;
}
