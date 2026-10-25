/*!
 * \file   tests/System/process_signal.cxx
 * \brief  check that a signalled child is waited for and reaped
 * \author Tristan Chenaille
 * \date 25/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/System/ProcessManager.hxx"

struct ProcessSignalTest final : public tfel::tests::TestCase {
  ProcessSignalTest()
      : tfel::tests::TestCase("TFEL/System", "ProcessSignalTest") {
  }  // end of ProcessSignalTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::system;
    ProcessManager m;
    // a killed child is reaped by wait
    const auto pid = m.createProcess("sleep 30");
    m.killProcess(pid);
    m.wait(pid);
    TFEL_TESTS_ASSERT(!exists(pid));
    // a signal which does not terminate the child does not stop wait
    const auto pid2 = m.createProcess("sleep 1");
    m.sendSignal(pid2, SIGCONT);
    m.wait(pid2);
    TFEL_TESTS_ASSERT(!exists(pid2));
    return this->result;
  }  // end of execute
 private:
  //! \return if the given process exists, including as a zombie
  static bool exists(const tfel::system::ProcessManager::ProcessId pid) {
    return !((::kill(pid, 0) == -1) && (errno == ESRCH));
  }
};

TFEL_TESTS_GENERATE_PROXY(ProcessSignalTest, "ProcessSignalTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("process_signal.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
