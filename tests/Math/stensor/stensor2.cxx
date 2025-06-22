/*!
 * \file   tests/Math/stensor2.cxx
 * \brief
 * \author Helfer Thomas
 * \date   09 Jun 2006
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

#include "TFEL/Math/General/Complex.hxx"
#include "TFEL/Math/stensor.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::math;
  using namespace std;

  stensor<1, Complex<float>> s;
  stensor<1, Complex<float>> s2;

  s(0) = Complex<float>(2.f);
  s(1) = Complex<float>(3.1415929f);
  s(2) = Complex<float>(4231.421f);

  s2 = 0.5f * s;

  assert((abs(real(s2(0) - Complex<float>(1.f))) < 1.e-3f));
  assert(abs(imag(s2(0))) < 1.e-3f);

  assert((abs(real(s2(1) - Complex<float>(1.5708f))) < 1.e-3f));
  assert(abs(imag(s2(1))) < 1.e-3f);

  assert((abs(real(s2(2) - Complex<float>(2115.7f))) < 1.f));
  assert(abs(imag(s2(2))) < 1.e-3f);

  return EXIT_SUCCESS;
}
