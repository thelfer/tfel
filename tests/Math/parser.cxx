/*!
 * \file   tests/Math/parser.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   21 sep 2007
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

#include "TFEL/Math/Evaluator.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int, const char* const* const) {
  using namespace std;
  using namespace tfel::math;

  Evaluator ev("min(x,y)*x");
  ev.setVariableValue("x", 1.5);
  ev.setVariableValue("y", 2.54);

  return EXIT_SUCCESS;
}
