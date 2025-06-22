/*!
 * \file   tests/Math/cadna/stensor_eigenvalues.cxx
 * \brief
 * \author Helfer Thomas
 * \date   29 sept. 2015
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

#include <cmath>
#include <cassert>
#include <cstdlib>

#include "TFEL/Math/cadna.hxx"
#include "TFEL/Math/stensor.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main(void) {
  using real = cadna::numeric_type<double>;
  real vp1, vp2, vp3;
  tfel::math::stensor<3, real> s(0.);
  s.computeEigenValues(vp1, vp2, vp3);
  assert(std::abs(vp1 - 0) < 1.e-14);
  assert(std::abs(vp2 - 0) < 1.e-14);
  assert(std::abs(vp3 - 0) < 1.e-14);
  return EXIT_SUCCESS;
}
