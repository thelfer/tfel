/*!
 * \file   tests/Material/boundsCheck.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   08 Mar 2007
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

#include "TFEL/Material/BoundsCheck.hxx"
#include "TFEL/Material/MaterialException.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::material;
  using mass = qt<unit::Mass>;
  stensor<3, mass> s(mass(12.));
  stensor<3, float> s2(12.f);
  long double d = 12.L;
  bool catched;

  catched = false;
  try {
    BoundsCheck<3>::upperBoundCheck("s", s, 13.);
    BoundsCheck<3>::lowerBoundCheck("s", s, 11.);
    BoundsCheck<3>::lowerAndUpperBoundsChecks("s", s, 11., 13.);
  } catch (const OutOfBoundsException&) {
    catched = true;
  }
  assert(!catched);

  catched = false;
  try {
    BoundsCheck<3>::upperBoundCheck("s", s, 10.);
  } catch (const OutOfBoundsException&) {
    catched = true;
  }
  assert(catched);

  catched = false;
  try {
    BoundsCheck<3>::lowerBoundCheck("s", s, 24.);
  } catch (const OutOfBoundsException&) {
    catched = true;
  }
  assert(catched);

  catched = false;
  try {
    BoundsCheck<3>::lowerAndUpperBoundsChecks("s", s, 24., 13.);
  } catch (const OutOfBoundsException&) {
    catched = true;
  }
  assert(catched);

  catched = false;
  try {
    BoundsCheck<3>::upperBoundCheck("s2", s2, 10.f);
  } catch (const OutOfBoundsException&) {
    catched = true;
  }
  assert(catched);

  catched = false;
  try {
    BoundsCheck<3>::lowerBoundCheck("s2", s2, 24.f);
  } catch (const OutOfBoundsException&) {
    catched = true;
  }
  assert(catched);

  catched = false;
  try {
    BoundsCheck<3>::lowerAndUpperBoundsChecks("s2", s2, 24.f, 13.f);
  } catch (const OutOfBoundsException&) {
    catched = true;
  }
  assert(catched);

  catched = false;
  try {
    BoundsCheck<3>::upperBoundCheck("d", d, 10.L);
  } catch (const OutOfBoundsException&) {
    catched = true;
  }
  assert(catched);

  catched = false;
  try {
    BoundsCheck<3>::lowerBoundCheck("d", d, 24.L);
  } catch (const OutOfBoundsException&) {
    catched = true;
  }
  assert(catched);

  catched = false;
  try {
    BoundsCheck<3>::lowerAndUpperBoundsChecks("d", d, 24.L, 13.L);
  } catch (const OutOfBoundsException&) {
    catched = true;
  }
  assert(catched);

  return EXIT_SUCCESS;
}
