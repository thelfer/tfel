/*!
 * \file   tests/Math/conj.cxx
 *
 * \brief
 * \author Helfer Thomas
 * \date   01 Sep 2006
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
#include <cstdlib>
#include <cassert>

#include "TFEL/Math/General/Complex.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"

#include <iostream>

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::math;

  assert(real(conj(12) - Complex<int>(12, 0)) == 0);
  assert(imag(conj(12) - Complex<int>(12, 0)) == 0);
  assert(real(conj(Complex<int>(0, 12)) - Complex<int>(0, -12)) == 0);
  assert(imag(conj(Complex<int>(0, 12)) - Complex<int>(0, -12)) == 0);

  return EXIT_SUCCESS;
}
