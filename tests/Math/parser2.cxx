/*!
 * \file   tests/Math/parser2.cxx
 * \brief
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
  Evaluator f(var, "exp(cos(x))");

#if __GNUC__ != 3
  shared_ptr<ExternalFunction> df = f.differentiate(0);
  df->setVariableValue(0, 2.);
  assert(abs(df->getValue() + sin(2.) * exp(cos(2.))) < 1.e-12);
#endif

  auto manager = std::make_shared<ExternalFunctionManager>();
  manager->operator[]("a") = std::make_shared<Evaluator>("12", manager);
  manager->operator[]("f") =
      std::make_shared<Evaluator>(var, "2*exp(a*x)", manager);
  var.push_back("y");
  manager->operator[]("g") =
      std::make_shared<Evaluator>(var, "y*f(x)", manager);

  assert(abs(manager->operator[]("a")->getValue() - 12.) < 1.e-12);
  manager->operator[]("f")->setVariableValue(0, 0.4);
  assert(abs(manager->operator[]("f")->getValue() - 2 * exp(12 * 0.4)) <
         1.e-12);
  manager->operator[]("g")->setVariableValue(0, 0.4);
  manager->operator[]("g")->setVariableValue(1, 3);
  assert(abs(manager->operator[]("g")->getValue() - 3 * 2 * exp(12 * 0.4)) <
         1.e-12);
  // Changing a function
  manager->operator[]("a") = std::make_shared<Evaluator>("24", manager);
  assert(abs(manager->operator[]("a")->getValue() - 24.) < 1.e-12);
  assert(abs(manager->operator[]("g")->getValue() - 3 * 2 * exp(24 * 0.4)) <
         1.e-12);
  // Changing f function
  manager->operator[]("f") = std::make_shared<Evaluator>("2*cos(x)", manager);
  manager->operator[]("g")->setVariableValue(0, 12.);
  assert(abs(manager->operator[]("g")->getValue() - 3 * 2 * cos(12.)) < 1.e-12);

  return EXIT_SUCCESS;
}
