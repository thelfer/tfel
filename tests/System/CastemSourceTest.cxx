/*!
 * \file  tests/System/CastemSourceTest.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 12 déc. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"

struct CastemSourceTest : public tfel::tests::TestCase {
  CastemSourceTest()
      : tfel::tests::TestCase("TFEL/System", "CastemSourceTest") {
  }  // end of CastemSourceTest

  tfel::tests::TestResult execute() override {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    const auto f = elm.getSource(".libs/libCastemMaterialLaw.so", "Test");
    TFEL_TESTS_ASSERT(f == "Test.mfront");
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(CastemSourceTest, "CastemSourceTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CastemSource.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
