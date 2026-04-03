/*!
 * \file   tests/Math/CubicRoots.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   01 jui 2006
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

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cassert>

#include "TFEL/Math/General/CubicRoots.hxx"

#include "CubicRoots-header.hxx"

#ifndef M_E
#define M_E 2.7182818284590452354 /* e */
#endif                            /* M_E */

#ifndef M_LOG2E
#define M_LOG2E 1.4426950408889634074 /* log_2 e */
#endif                                /* M_LOG2E */

#ifndef M_LOG10E
#define M_LOG10E 0.43429448190325182765 /* log_10 e */
#endif                                  /* M_LOG10E */

#ifndef M_LN2
#define M_LN2 0.69314718055994530942 /* log_e 2 */
#endif                               /* M_LN2 */

#ifndef M_LN10
#define M_LN10 2.30258509299404568402 /* log_e 10 */
#endif                                /* M_LN10 */

#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif                              /* M_PI */

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923 /* pi/2 */
#endif                                /* M_PI_2 */

#ifndef M_PI_4
#define M_PI_4 0.78539816339744830962 /* pi/4 */
#endif                                /* M_PI_4 */

#ifndef M_1_PI
#define M_1_PI 0.31830988618379067154 /* 1/pi */
#endif                                /* M_1_PI */

#ifndef M_2_PI
#define M_2_PI 0.63661977236758134308 /* 2/pi */
#endif                                /* M_2_PI */

#ifndef M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551257390 /* 2/sqrt(pi) */
#endif                                    /* M_2_SQRTPI */

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880 /* sqrt(2) */
#endif                                 /* M_SQRT2 */

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.70710678118654752440 /* 1/sqrt(2) */
#endif                                   /* M_SQRT1_2 */

double cubic(const double a3,
             const double a2,
             const double a1,
             const double a0,
             const double x) {
  return a3 * x * x * x + a2 * x * x + a1 * x + a0;
}

void generate_coef(double& a3,
                   double& a2,
                   double& a1,
                   double& a0,
                   const double x1,
                   const double x2,
                   const double x3) {
  a3 = 2.;
  a2 = -2. * (x1 + x2 + x3);
  a1 = 2. * (x1 * x2 + x1 * x3 + x2 * x3);
  a0 = -2. * x1 * x2 * x3;
}

void solve(const double a3, const double a2, const double a1, const double a0) {
  using namespace tfel::math;

  double x = 0.;
  double y = 0.;
  double z = 0.;
  static constexpr double prec = 1.e-14;

  assert(CubicRoots::exe(x, y, z, a3, a2, a1, a0) == 3);
  if (!((std::abs(cubic(a3, a2, a1, a0, x)) < prec) ||
        (std::abs(cubic(a3, a2, a1, a0, y)) < prec) ||
        (std::abs(cubic(a3, a2, a1, a0, z)) < prec))) {
    assert(false);
  }
#ifdef TFEL_VERBOSE
  std::cout << "f(x) = " << cubic(a3, a2, a1, a0, x) << std::endl;
  std::cout << "f(y) = " << cubic(a3, a2, a1, a0, y) << std::endl;
  std::cout << "f(z) = " << cubic(a3, a2, a1, a0, z) << std::endl;
#endif /* TFEL_VERBOSE */
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;

  double a0, a1, a2, a3;
  int i;
  for (i = -5; i <= 0; ++i) {
    solve(1., 0., static_cast<double>(i), 0.);
  }

  generate_coef(a3, a2, a1, a0, -M_E, M_LOG2E, -M_LOG10E);
  solve(a3, a2, a1, a0);

  generate_coef(a3, a2, a1, a0, M_LN2, -M_LN10, M_PI);
  solve(a3, a2, a1, a0);

  generate_coef(a3, a2, a1, a0, -M_PI_2, M_PI_4, M_1_PI);
  solve(a3, a2, a1, a0);

  generate_coef(a3, a2, a1, a0, M_2_PI, M_2_SQRTPI, -M_SQRT2);
  solve(a3, a2, a1, a0);

  generate_coef(a3, a2, a1, a0, M_2_SQRTPI, -M_SQRT2, M_SQRT1_2);
  solve(a3, a2, a1, a0);

  return EXIT_SUCCESS;
}
