/*!
 * \file   tests/Math/stensor4.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   01/02/2010
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

#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdlib>

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  const stensor<1> Id = stensor<1>::Id();
  const st2tost2<1> IxI = st2tost2<1>::IxI();
  stensor<1> a;
  a(0) = 1.;
  a(1) = 2.;
  a(2) = 3.;
  cout << (Id ^ Id) << endl;
  cout << IxI << endl;
  cout << (a | (Id ^ Id)) << endl;
  return EXIT_SUCCESS;
}
