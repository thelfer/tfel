/*!
 * \file   src/Tests/TestSuite.cxx
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

#include "TFEL/Tests/TestSuite.hxx"

namespace tfel {

  namespace tests {

    TestSuite::TestSuite() = default;

    TestSuite::TestSuite(const std::string& n)
        : sname(n) {}  // end of TestSuite::TestSuite

    TestSuite::TestSuite(const char* const n)
        : sname(n) {}  // end of TestSuite::TestSuite

    std::string TestSuite::name() const {
      return this->sname;
    }  // end of TestSuite::name

    void TestSuite::add(const TestSuite::TestPtr& t) {
      this->tests.push_back(t);
    }  // end of TestSuite::execute

    TestResult TestSuite::execute() {
      using namespace std::chrono;
      TestResult r;
      for (const auto& t : this->tests) {
        TestResult r1;
        const auto start = high_resolution_clock::now();
        try {
          r1 = t->execute();
        } catch (std::exception& e) {
          r1 = TestResult(false, "test '" + t->name() +
                                     "' has thrown "
                                     "an exception (" +
                                     std::string(e.what()) + ")");
        } catch (...) {
          r1 = TestResult(false, "test '" + t->name() +
                                     "' has thrown "
                                     "an unknown exception");
        }
        const auto stop = high_resolution_clock::now();
        const auto nsec = duration_cast<nanoseconds>(stop - start).count();
        r1.setTestDuration(1.e-9 * nsec);
        r.append(r1);
      }
      return r;
    }  // end of TestSuite::execute

    TestResult TestSuite::execute(TestOutput& o) {
      using namespace std::chrono;
      o.beginTestSuite(this->name());
      TestResult r;
      auto success = true;
      for (const auto& t : this->tests) {
        TestResult r1;
        const auto start = high_resolution_clock::now();
        try {
          r1 = t->execute();
          if (!r1.success()) {
            success = false;
          }
        } catch (std::exception& e) {
          r1 = TestResult(false, "test '" + t->name() +
                                     "' has thrown an exception "
                                     "(" +
                                     std::string(e.what()) + ")");
          success = false;
        } catch (...) {
          r1 = TestResult(false, "test '" + t->name() +
                                     "' has thrown an "
                                     "unknown exception");
          success = false;
        }
        const auto stop = high_resolution_clock::now();
        const auto nsec = duration_cast<nanoseconds>(stop - start).count();
        r1.setTestDuration(1.e-9 * nsec);
        o.addTest(t->classname(), t->name(), r1);
        r.append(r1);
      }
      o.endTestSuite(TestResult(success));
      return r;
    }  // end of TestSuite::execute

    TestSuite::~TestSuite() = default;

  }  // end of namespace tests

}  // end of namespace tfel
