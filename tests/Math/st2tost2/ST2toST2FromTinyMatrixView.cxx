/*!
 * \file   tests/Math/st2tost2/ST2toST2FromTinyMatrixView.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   17 oct 2008
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

#include <cassert>
#include <cstdlib>
#include <iostream>
#include "TFEL/Math/ST2toST2/ST2toST2FromTinyMatrixView.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  tmatrix<6, 6> m(0.);
  ST2toST2FromTinyMatrixView<1, 6, 6, 0, 0>::type ms(m);
  ST2toST2FromTinyMatrixView<1, 6, 6, 3, 3>::type ms2(m);
  unsigned short i;
  unsigned short j;
  for (i = 0; i != 6; ++i) {
    m(i, i) = 1.;
  }

  for (i = 0; i != 3; ++i) {
    for (j = 0; j != 3; ++j) {
      if (i == j) {
        assert(abs(ms(i, i) - 1.) < 1.e-14);
      } else {
        assert(abs(ms(i, j)) < 1.e-14);
      }
    }
  }

  for (i = 0; i != 3; ++i) {
    for (j = 0; j != 3; ++j) {
      if (i == j) {
        assert(abs(ms2(i, i) - 1.) < 1.e-14);
      } else {
        assert(abs(ms2(i, j)) < 1.e-14);
      }
    }
  }

  return EXIT_SUCCESS;
}
