/*!
 * \file   src/Tests/TestResult.cxx
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

#include <iostream>

#include "TFEL/Tests/TestResult.hxx"

namespace tfel::tests {

  TestResult::TestResult() = default;
  TestResult::TestResult(const TestResult&) = default;
  TestResult::TestResult(TestResult&&) noexcept = default;
  TestResult& TestResult::operator=(const TestResult&) = default;
  TestResult& TestResult::operator=(TestResult&&) = default;

  TestResult::TestResult(const bool b)
      : status(b) {}  // end of TestResult::TestResult

  TestResult::TestResult(const bool b,
                         const std::string_view c,
                         const std::string_view e)
      : description(c),
        failure_description(b ? "" : e),
        status(b) {}  // end of TestResult::TestResult

  bool TestResult::success() const {
    return this->status;
  }  // end of TestResult::succees

  const std::string& TestResult::getDescription() const {
    return this->description;
  }  // end of TestResult::getDescription

  const std::string& TestResult::getFailureDescription() const {
    return this->failure_description;
  }  // end of TestResult::getFailureDescription

  TestResult::const_iterator TestResult::begin() const {
    return this->results.begin();
  }  // end of begin

  TestResult::const_iterator TestResult::end() const {
    return this->results.end();
  }  // end of end

  void TestResult::append(const TestResult& r) {
    if (!r.success()) {
      this->status = false;
    }
    this->results.push_back(r);
  }  // end of append

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

  TestResult::~TestResult() = default;

  std::ostream& operator<<(std::ostream& os, const TestResult& r) {
    os << "Result : ";
    if (r.success()) {
      os << "SUCCESS\n";
    } else {
      os << "FAILED\n";
    }
    os << "\nDetails : " << r.getDescription() << '\n';
    for (const auto& t : r) {
      os << t << '\n';
    }
    return os;
  }  // end of operator <<

}  // end of namespace tfel::tests
