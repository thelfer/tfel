/*!
 * \file   ThreadPoolTest2.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17 août 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/System/ThreadPool.hxx"

struct ThreadPoolTest2 final : public tfel::tests::TestCase {
  ThreadPoolTest2()
      : tfel::tests::TestCase("TFEL/System", "ThreadPoolTest2") {
  }  // end of ThreadPoolTest2
  tfel::tests::TestResult execute() override {
    std::atomic<int> res(0);
    auto task = [&res](const int i) { return [&res, i] { res += i; }; };
    tfel::system::ThreadPool p(2);
    p.addTask(task(-1));
    p.addTask(task(2));
    p.addTask(task(4));
    p.wait();
    TFEL_TESTS_ASSERT(res == 5);
    p.addTask(task(5));
    p.addTask(task(-8));
    p.wait();
    TFEL_TESTS_ASSERT(res == 2);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ThreadPoolTest2, "ThreadPoolTest2");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ThreadPoolTest2.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
