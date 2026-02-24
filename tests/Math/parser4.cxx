/*!
 * \file   tests/Math/parser4.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   20 déc 2008
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

#include <iostream>
#include <algorithm>
#include <iterator>

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::math::parser;
  vector<string> var(1, "x");
  vector<string> nvar(1, "b");
  set<string> n;
  set<string> n2;
  auto manager = std::make_shared<ExternalFunctionManager>();
  manager->operator[]("a") = std::make_shared<Evaluator>("12", manager);
  auto f = std::make_shared<Evaluator>(var, "2*exp(a*x)", manager);
  auto g = std::make_shared<Evaluator>(var, "2*b*exp(a*x)", manager);
  auto h = g->createFunctionByChangingParametersIntoVariables(nvar);
  cout << "h : " << h->getNumberOfVariables() << endl;
  h->setVariableValue(0, 1.5);
  h->setVariableValue(1, 2.54);
  cout << "h : " << h->getValue() << endl;
  f->getParametersNames(n);
  g->getParametersNames(n2);
  copy(n.begin(), n.end(), ostream_iterator<string>(cout, " "));
  cout << endl;
  copy(n2.begin(), n2.end(), ostream_iterator<string>(cout, " "));
  cout << endl;
  return EXIT_SUCCESS;
}
