/*!
 * \file  tests/Utilities/StringAlgorithms.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mar 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <cstdlib>
#include <string>

#include <cassert>

#include "TFEL/Utilities/StringAlgorithms.hxx"

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

struct StringAlgorithmsTest final : public tfel::tests::TestCase {
  StringAlgorithmsTest()
      : tfel::tests::TestCase("TFEL/Utilities", "StringAlgorithmsTest") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::utilities;

    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "o", "a"), "faa bar");
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "u", "a"), "foo bar");
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "fo", "a"), "ao bar");
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "ar", "u"), "foo bu");

    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "foo ", ""), "bar");
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "", "test"), "foo bar");

    return this->result;
  }  // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(StringAlgorithmsTest, "StringAlgorithms");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace std;

  using namespace tfel::tests;
  using namespace tfel::utilities;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("gentype.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
