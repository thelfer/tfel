/*!
 * \file   tests/Math/solve.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   01 Aug 2006
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
#include <cassert>
#include <iterator>

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/TinyMatrixSolve.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  tmatrix<3, 3> m{{0.132, 0.60, 0.57, 0.324, 0.25, 0.99, 0.26, 0.78, 0.29}};
  tmatrix<3, 3> m2{{0.132, 0.60, 0., 0.324, 0.25, 0., 0.26, 0.78, 0.}};
  tvector<3> v{{1.0, 2.0, 3.0}};
  try {
    TinyMatrixSolve<3, double>::exe(m, v);
    assert(std::abs(v(0) - 10.6118682566723) <= 1.e-13);
    assert(std::abs(v(1) - 0.93696763202725) <= 1.e-13);
    assert(std::abs(v(2) + 1.68938103350369) <= 1.e-13);
  } catch (LUException&) {
    assert(false);
  }

  try {
    TinyMatrixSolve<3, double>::exe(m2, v);
    assert(false);
  } catch (LUException&) {
    assert(true);
  }

  return EXIT_SUCCESS;
}
