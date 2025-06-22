/*!
 * \file   src/Tests/TestResult.cxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>

#include "TFEL/Tests/TestResult.hxx"

namespace tfel {

  namespace tests {

    TestResult::TestResult() = default;

    TestResult::TestResult(const bool b)
        : s(b) {}  // end of TestResult::TestResult

    TestResult::TestResult(const bool b, const char* const c)
        : d(c), s(b) {}  // end of TestResult::TestResult

    TestResult::TestResult(const bool b, const std::string& c)
        : d(c), s(b) {}  // end of TestResult::TestResult

    TestResult::TestResult(const TestResult&) = default;
    TestResult::TestResult(TestResult&&) = default;
    TestResult& TestResult::operator=(const TestResult&) = default;
    TestResult& TestResult::operator=(TestResult&&) = default;

    bool TestResult::success() const {
      return this->s;
    }  // end of TestResult::succees

    const std::string& TestResult::details() const {
      return this->d;
    }  // end of TestResult::succees

    TestResult::const_iterator TestResult::begin() const {
      return std::vector<TestResult>::begin();
    }  // end of TestResult::begin()

    TestResult::const_iterator TestResult::end() const {
      return std::vector<TestResult>::end();
    }  // end of TestResult::end()

    void TestResult::append(const TestResult& r) {
      if (!r.success()) {
        this->s = false;
      }
      std::vector<TestResult>::push_back(r);
    }  // end of TestResult::end()

    void TestResult::setTestDuration(const double td) {
      this->test_duration = td;
    }

    double TestResult::duration() const {
      auto r = this->test_duration;
      for (const auto& t : *this) {
        r += t.duration();
      }
      return r;
    }

    TestResult::~TestResult(void) = default;

    std::ostream& operator<<(std::ostream& os, const TestResult& r) {
      os << "Result : ";
      if (r.success()) {
        os << "SUCCESS\n";
      } else {
        os << "FAILED\n";
      }
      os << '\n';
      os << "Details : " << r.details() << '\n';
      for (const auto& t : r) {
        os << t << '\n';
      }
      return os;
    }  // end of operator <<

  }  // end of namespace tests

}  // end of namespace tfel
