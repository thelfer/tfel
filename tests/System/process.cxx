/*!
 * \file   tests/System/process.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 12/11/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/System/SystemError.hxx"
#include "TFEL/System/ProcessManager.hxx"

struct ProcessTest final : public tfel::tests::TestCase {
  ProcessTest()
      : tfel::tests::TestCase("TFEL/System", "ProcessTest") {
  }  // end of ProcessTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::system;
    ProcessManager m;
    const auto pid = m.createProcess("./process_test_target");
    TFEL_TESTS_ASSERT(pid > 0);
    m.wait(pid);
    // execute waits for the child and checks its exit status
    auto success = true;
    try {
      m.execute("./process_test_target");
    } catch (std::exception&) {
      success = false;
    }
    TFEL_TESTS_ASSERT(success);
    TFEL_TESTS_CHECK_THROW(m.execute("false"), SystemError);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ProcessTest, "ProcessTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("process.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
