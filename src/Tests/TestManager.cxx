/*!
 * \file   src/Tests/TestManager.cxx
 * \author Thomas Helfer
 * \date   10 Apr 10
 * \brief
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <chrono>
#include <stdexcept>
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Tests/XMLTestOutput.hxx"
#include "TFEL/Tests/StdStreamTestOutput.hxx"

namespace tfel::tests {

  TestManager& TestManager::getTestManager() {
    static TestManager m;
    return m;
  }  // end of TestManager::getTestManager()

  void TestManager::addTestOutput(TestOutputPtr o) {
    if (this->default_outputs == nullptr) {
      this->default_outputs = std::make_shared<MultipleTestOutputs>();
    }
    this->default_outputs->addTestOutput(o);
  }  // end of TestManager::addTestOutput

  void TestManager::addTestOutput(const std::string& n, TestOutputPtr o) {
    auto p = this->outputs.find(n);
    if (p == this->outputs.end()) {
      auto out = std::make_shared<MultipleTestOutputs>();
      p = this->outputs.insert({n, out}).first;
    }
    p->second->addTestOutput(o);
  }  // end of TestManager::addTestOutput

  void TestManager::addTestOutput(std::ostream& o, const bool b) {
    this->addTestOutput(std::make_shared<StdStreamTestOutput>(o, b));
  }  // end of TestManager::addTestOutput

  void TestManager::addTestOutput(const std::string& n,
                                  std::ostream& o,
                                  const bool b) {
    this->addTestOutput(n, std::make_shared<StdStreamTestOutput>(o, b));
  }  // end of TestManager::addTestOutput

  void TestManager::addTestOutput(const std::string& f) {
    this->addTestOutput(std::make_shared<StdStreamTestOutput>(f));
  }  // end of TestManager::addTestOutput

  void TestManager::addXMLTestOutput(const std::string& f) {
    this->addTestOutput(std::make_shared<XMLTestOutput>(f));
  }  // end of TestManager::addTestOutput

  void TestManager::addTestOutput(const std::string& n, const std::string& f) {
    this->addTestOutput(n, std::make_shared<StdStreamTestOutput>(f));
  }  // end of TestManager::addTestOutput

  void TestManager::addTest(const std::string& n, TestPtr t) {
    auto p = this->tests.find(n);
    if (p == this->tests.end()) {
      auto s = std::make_shared<TestSuite>(n);
      p = this->tests.insert({n, s}).first;
    }
    p->second->add(t);
  }  // end of TestManager::addTest

  TestResult TestManager::execute() {
    using namespace std::chrono;
    TestResult r;
    for (const auto& t : this->tests) {
      MultipleTestOutputsPtr output;
      const auto& n = t.first;
      auto p2 = this->outputs.find(n);
      if (p2 != this->outputs.end()) {
        output = p2->second;
      } else {
        if (this->default_outputs != nullptr) {
          output = this->default_outputs;
        }
      }
      const auto start = high_resolution_clock::now();
      if (output != nullptr) {
        r.append(t.second->execute(*output));
      } else {
        r.append(t.second->execute());
      }
      const auto stop = high_resolution_clock::now();
      const auto nsec = duration_cast<nanoseconds>(stop - start).count();
      r.setTestDuration(1.e-9 * nsec);
    }
    return r;
  }  // end of TestManager::execute()

  TestManager::TestManager() = default;

}  // end of namespace tfel::tests
