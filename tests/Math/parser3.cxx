/*!
 * \file   tests/Math/parser3.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 20/12/2008
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

#include "TFEL/Math/Evaluator.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::math::parser;
  vector<string> var(1, "x");
  Evaluator f(var, "diff(cos(x),x)");
#if __GNUC__ != 3
  shared_ptr<ExternalFunction> df = f.differentiate(0);
  df->setVariableValue(0, 2.);
  assert(abs(df->getValue() + cos(2.)) < 1.e-14);
#endif
  const Evaluator g({"x", "y"}, "diff(x*y,x)");
  assert(abs(Evaluator(g).getValue({{"x", 2.}, {"y", 3.}}) - 3.) < 1.e-14);
  Evaluator h("diff(x*x*y*y,x,y)");
  assert(abs(h.getValue({{"x", 2.}, {"y", 3.}}) - 24.) < 1.e-14);
  Evaluator h1("diff<1>(x*x*y,x)");
  assert(abs(h1.getValue({{"x", 2.}, {"y", 3.}}) - 12.) < 1.e-14);
  Evaluator h2("diff(y*x*x,x)");
  assert(abs(h2.getValue({{"x", 2.}, {"y", 3.}}) - 12.) < 1.e-14);
  return EXIT_SUCCESS;
}
