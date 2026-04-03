/*!
 * \file   tests/Math/parser8.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  18 jan 2011
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
#include <limits>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/Evaluator.hxx"

struct ParserTest final : public tfel::tests::TestCase {
  ParserTest()
      : tfel::tests::TestCase("TFEL/Math", "Parser8") {}  // end of MyTest
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using namespace tfel::math::parser;
    const double eps = 1.e-14;
    const std::string f("12*x*y*cos(u)*sin(v)");
    map<string, double> values;
    vector<string> vars;
    vector<string> v1;
    vector<string>::const_iterator p;
    vector<string>::iterator p2;
    map<string, double>::const_iterator p3;
    values["u"] = 3.987;
    values["v"] = 4.32;
    auto manager = std::make_shared<ExternalFunctionManager>();
    auto fmanager = std::make_shared<ExternalFunctionManager>();
    vars.push_back("x");
    vars.push_back("y");
    try {
      Evaluator ev1(f);
      v1 = ev1.getVariablesNames();
      for (p = vars.begin(); p != vars.end(); ++p) {
        p2 = find(v1.begin(), v1.end(), *p);
        tfel::raise_if(p2 == v1.end(), "unsued variable '" + *p + "'");
        v1.erase(p2);
      }
      for (p = v1.begin(); p != v1.end(); ++p) {
        p3 = values.find(*p);
        tfel::raise_if(p3 == values.end(),
                       "unknown external value '" + *p + "'");
        ostringstream os;
        os << p3->second;
        fmanager->operator[](*p) = std::make_shared<Evaluator>(os.str());
      }
      manager->operator[]("f") = std::make_shared<Evaluator>(vars, f, fmanager);
      manager->operator[]("u") = std::make_shared<Evaluator>("1.2");
      manager->operator[]("v") = std::make_shared<Evaluator>("2.");
      Evaluator ev(vector<string>(), "f(u,v)", manager);
      ev.removeDependencies();
      TFEL_TESTS_ASSERT(ev.getVariablesNames().empty());
      TFEL_TESTS_ASSERT(abs(ev.getValue() - 12 * 2.4 * cos(3.987) * sin(4.32)) <
                        eps);
    } catch (runtime_error& e) {
      cout << e.what() << endl;
      return false;
    }
    return this->result;
  }  // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(ParserTest, "Parser");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("parser8.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
