/*!
 * \file   tests/Math/tvector5.cxx
 * \brief
 * \author Helfer Thomas
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

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Vector/tvectorIO.hxx"

struct TVectorTest5 final : public tfel::tests::TestCase {
  TVectorTest5()
      : tfel::tests::TestCase("TFEL/Math", "TVectorTesT5") {
  }  // end of TVectorTest

  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double v1_values[3u] = {1.2, -4.2, -0.3};
    tvector<3u, double> v1(v1_values);
    cout << "v1      : " << v1 << endl;
    cout << "abs(v1) : " << abs(v1) << endl;
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TVectorTest5, "TVectorTest5");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("tvector5.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
