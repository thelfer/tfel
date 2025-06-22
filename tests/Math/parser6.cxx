/*!
 * \file   tests/Math/parser6.cxx
 * \brief
 *
 * \author Helfer Thomas
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

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::math::parser;
  vector<string> var(1, "x");
  vector<string> nvar(1, "a");
  auto manager = std::make_shared<ExternalFunctionManager>();
  manager->operator[]("f") =
      std::make_shared<Evaluator>(var, "sin(a*x)", manager);
  auto g = std::make_shared<Evaluator>(var, "f(x)", manager);
  auto h = g->createFunctionByChangingParametersIntoVariables(nvar);
  h = h->resolveDependencies();
  assert(h->getNumberOfVariables() == 2);
  h->setVariableValue(0, 1.5);
  h->setVariableValue(1, 2.54);
  assert(abs(h->getValue() - sin(1.5 * 2.54)) < 1.e-14);

#if __GNUC__ != 3
  auto dh_dx = h->differentiate(0);
  dh_dx->setVariableValue(0, 12.5);
  dh_dx->setVariableValue(1, 3.5412);
  assert(abs(dh_dx->getValue() - 3.5412 * cos(3.5412 * 12.5)) < 1.e-14);
  auto dh_da = h->differentiate(1);
  dh_da->setVariableValue(0, 7.98);
  dh_da->setVariableValue(1, 4.1);
  assert(abs(dh_da->getValue() - 7.98 * cos(4.1 * 7.98)) < 1.e-14);
#endif

  return EXIT_SUCCESS;
}
