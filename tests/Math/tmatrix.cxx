/*!
 * \file   tests/Math/tmatrix.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   03 jui 2006
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
#include "TFEL/Math/tmatrix.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::math;
  tfel::math::tmatrix<2, 2, double> m;
  tfel::math::tmatrix<2, 2, double> m2;
  m(0, 0) = 0.;
  m(0, 1) = 1.;
  m(1, 0) = 2.;
  m(1, 1) = 3.;
  m2 = 2 * m;
  return EXIT_SUCCESS;
}
