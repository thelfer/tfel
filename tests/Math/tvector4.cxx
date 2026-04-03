/*!
 * \file   tests/Math/tvector4.cxx
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
#endif

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/tvector.hxx"

#include "function_unary_tests.hxx"

const double prec = 1.e-12;

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::math;

  typedef tvector<3, double> vector1;
  typedef tvector<3, int> vector2;

  vector1 v1 = {4, 1, 25};
  vector2 v2 = {2, 1, 5};

  function(v1, 4., 1., 25., prec, 1);
  function(2 * v1, 8., 2., 50., prec, 2);
  function(v1 * 2, 8., 2., 50., prec, 3);
  function(v2, 2, 1, 5, 0, 4);
  function(v1 + v2, 6., 2., 30., prec, 5);
  function(v1 - v2, 2., 0., 20., prec, 6);
  function(v1 + v2 + v1, 10., 3., 55., prec, 7);
  function(v1 + v2 - v1, 2., 1., 5., prec, 8);
  function(2. * v1 + v2, 10., 3., 55., prec, 9);
  function(v1 + (2. * (v1 + v2)), 16., 5., 85., prec, 10);
  function(3 * v1 + (2. * (v1 + v2)), 24., 7., 135., prec, 11);

  return EXIT_SUCCESS;
}
