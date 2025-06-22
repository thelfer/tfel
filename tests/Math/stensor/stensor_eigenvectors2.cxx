/*!
 * \file   tests/Math/stensor_eigenvectors2.cxx
 * \brief
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

#include <cmath>
#include <limits>
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "TFEL/Math/General/floating_point_exceptions.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"

template <typename T>
void test() {
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::math::internals;
  T vp1;
  T vp2;
  T vp3;
  T vec1[3];
  T vec2[3];
  T vec3[3];
  tfel::math::tmatrix<3u, 3u, T> m;
  tfel::math::tvector<3u, T> vp;

  stensor<3, T> s(static_cast<T>(0.));
  s(0) = static_cast<T>(1.232);
  s(1) = static_cast<T>(2.5198);
  s(2) = static_cast<T>(0.234);
  s(3) = static_cast<T>(1.5634);
  s(4) = static_cast<T>(3.3425);
  s(5) = static_cast<T>(0.9765);

  s.computeEigenVectors(vp, m);
  vp1 = vp(0);
  vp2 = vp(1);
  vp3 = vp(2);

  vec1[0] = m(0, 0);
  vec1[1] = m(1, 0);
  vec1[2] = m(2, 0);

  vec2[0] = m(0, 1);
  vec2[1] = m(1, 1);
  vec2[2] = m(2, 1);

  vec3[0] = m(0, 2);
  vec3[1] = m(1, 2);
  vec3[2] = m(2, 2);

  assert(abs(vp1 - static_cast<T>(4.16709379934921)) <
         20 * numeric_limits<T>::epsilon());
  assert(abs(vp2 + static_cast<T>(1.68923153093191)) <
         20 * numeric_limits<T>::epsilon());
  assert(abs(vp3 - static_cast<T>(1.50793773158270)) <
         20 * numeric_limits<T>::epsilon());

  assert(abs(vec1[0] - static_cast<T>(0.6208263966073649)) <
         20 * numeric_limits<T>::epsilon());
  assert(abs(vec1[1] - static_cast<T>(0.6185290894233862)) <
         20 * numeric_limits<T>::epsilon());
  assert(abs(vec1[2] - static_cast<T>(0.4816599950303030)) <
         20 * numeric_limits<T>::epsilon());

  assert(abs(vec2[0] + static_cast<T>(0.6378665158240716)) <
         20 * numeric_limits<T>::epsilon());
  assert(abs(vec2[1] - static_cast<T>(0.0413740968617118)) <
         20 * numeric_limits<T>::epsilon());
  assert(abs(vec2[2] - static_cast<T>(0.7690347795121735)) <
         20 * numeric_limits<T>::epsilon());

  assert(abs(vec3[0] - static_cast<T>(0.4557421346177839)) <
         20 * numeric_limits<T>::epsilon());
  assert(abs(vec3[1] + static_cast<T>(0.7846718738721010)) <
         20 * numeric_limits<T>::epsilon());
  assert(abs(vec3[2] - static_cast<T>(0.4202251266738716)) <
         20 * numeric_limits<T>::epsilon());
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::math;
  //
  init_floating_point_exceptions();
#ifndef __clang__
#ifdef TFEL_VERBOSE
  std::cerr << " Beginning test<float>()" << std::endl;
#endif
  test<float>();
#endif
#ifdef TFEL_VERBOSE
  std::cerr << " Beginning test<double>()" << std::endl;
#endif
  test<double>();

  return EXIT_SUCCESS;
}
