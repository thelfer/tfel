/*!
 * \file   tests/Math/tmatrix2.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12 avr 2008
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

  tmatrix<2, 2, double> m;
  tvector<2, double> v;
  tvector<2, double> v2;

  m(0, 0) = 0.;
  m(0, 1) = 1.;
  m(1, 0) = 2.;
  m(1, 1) = 3.;

  v(0) = 0.5;
  v(1) = 0.25;

  v2 = v * m;

  return EXIT_SUCCESS;

}  // end of main
