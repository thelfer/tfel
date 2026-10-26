/*!
 * \file   ThreadPoolTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date 09/11/2016
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

struct ThreadPoolTest final : public tfel::tests::TestCase {
  ThreadPoolTest()
      : tfel::tests::TestCase("TFEL/System", "ThreadPoolTest") {
  }  // end of ThreadPoolTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::system;
    ThreadPool pool(2);
    auto results = std::vector<std::future<ThreadedTaskResult<int>>>{};
    for (int i = 0; i < 5; ++i) {
      auto r = pool.addTask([i] { return i * i; });
      results.emplace_back(std::move(r));
    }
    int value = 0;
    for (auto&& r : results) {
      value += *(r.get());
    }
    TFEL_TESTS_ASSERT(value == 30);
    // a callable passed as an lvalue is copied
    auto counter = [n = 0]() mutable { return ++n; };
    TFEL_TESTS_ASSERT(*(pool.addTask(counter).get()) == 1);
    TFEL_TESTS_ASSERT(counter() == 1);
    // copy of a result from a non-const lvalue
    auto tr = ThreadedTaskResult<int>{};
    tr = 42;
    TFEL_TESTS_ASSERT(*(ThreadedTaskResult<int>(tr)) == 42);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ThreadPoolTest, "ThreadPoolTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ThreadPoolTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
