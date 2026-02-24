/*!
 * \file   tests/Math/hill.cxx
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

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/Hill.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::material;

  st2tost2<3u, double> H(hillTensor<3u>(2., 3., 4., 5., 6., 7.));
  stensor<3u, double> s(1.);
  //   cout << "H : " << endl;
  //   cout << (s|H*s) << endl;

  return EXIT_SUCCESS;
}
