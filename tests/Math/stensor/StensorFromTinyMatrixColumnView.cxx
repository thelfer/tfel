/*!
 * \file   tests/Math/StensorFromTinyMatrixColumnView.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   16 oct 2008
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
#include <iostream>

#include "TFEL/Math/Stensor/StensorFromTinyMatrixColumnView.hxx"
#include "TFEL/Math/Stensor/StensorFromTinyMatrixRowView.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  tmatrix<3, 2> m(0.);
  StensorFromTinyMatrixColumnView<1, 3, 2, 0, 0>::type ms(m);
  StensorFromTinyMatrixColumnView<1, 3, 2, 0, 1>::type ms2(m);
  stensor<1> s(1.);
  ms(0) = 1.;
  ms(1) = 2.;
  ms(2) = 3.;
  s += ms;
  ms2 = ms + 2. * s;
  return EXIT_SUCCESS;
}  // end of main
