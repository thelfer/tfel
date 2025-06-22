/*!
 * \file  tests/System/CastemSourceTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 déc. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <TFEL/System/ExternalLibraryManager.hxx>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

struct CastemSourceTest : public tfel::tests::TestCase {
  CastemSourceTest()
      : tfel::tests::TestCase("TFEL/System", "CastemSourceTest") {
  }  // end of CastemSourceTest

  tfel::tests::TestResult execute() {
    using namespace std;
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    auto& elm = ELM::getExternalLibraryManager();
    const string f = elm.getSource(".libs/libCastemMaterialLaw.so", "Test");
    TFEL_TESTS_ASSERT(f == "Test.mfront");
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(CastemSourceTest, "CastemSourceTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main(void) {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("CastemSource.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
