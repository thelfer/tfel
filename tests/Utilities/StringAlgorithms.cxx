/*!
 * \file  tests/Utilities/StringAlgorithms.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mar 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <cerrno>
#include <limits>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

struct StringAlgorithmsTest final : public tfel::tests::TestCase {
  StringAlgorithmsTest()
      : tfel::tests::TestCase("TFEL/Utilities", "StringAlgorithmsTest") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace tfel::utilities;
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "o", "a"), "faa bar");
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "u", "a"), "foo bar");
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "fo", "a"), "ao bar");
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "ar", "u"), "foo bu");
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "foo ", ""), "bar");
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "", "test"), "foo bar");
    TFEL_TESTS_CHECK_EQUAL(replace_all("foo bar", "o", "a", 2), "foa bar");
    TFEL_TESTS_ASSERT(starts_with("foobar", "foo"));
    TFEL_TESTS_ASSERT(!starts_with("foobar", "bar"));
    TFEL_TESTS_ASSERT(ends_with("foobar", "bar"));
    TFEL_TESTS_ASSERT(!ends_with("foobar", "foo"));
  }  // end of execute()
  void test2() {
    const auto s = "a:bbc::d";
    const auto tokens1 = tfel::utilities::tokenize(s, ':');
    const auto tokens2 = tfel::utilities::tokenize(s, ':', true);
    TFEL_TESTS_ASSERT(tfel::utilities::tokenize(":a", ':').size() == 1);
    TFEL_TESTS_ASSERT(tfel::utilities::tokenize("a:b", "").size() == 1);
    TFEL_TESTS_ASSERT(tokens1.size() == 3);
    if (tokens1.size() == 3) {
      TFEL_TESTS_ASSERT(tokens1[0] == "a");
      TFEL_TESTS_ASSERT(tokens1[1] == "bbc");
      TFEL_TESTS_ASSERT(tokens1[2] == "d");
    }
    TFEL_TESTS_ASSERT(tokens2.size() == 4);
    if (tokens2.size() == 4) {
      TFEL_TESTS_ASSERT(tokens2[0] == "a");
      TFEL_TESTS_ASSERT(tokens2[1] == "bbc");
      TFEL_TESTS_ASSERT(tokens2[2] == "");
      TFEL_TESTS_ASSERT(tokens2[3] == "d");
    }
  }  // end of test2
  //! \brief underflows are accepted, overflows rejected
  void test3() {
    using namespace tfel::utilities;
    // a difference below the smallest subnormal number is null
    constexpr auto dmin = std::numeric_limits<double>::denorm_min();
    constexpr auto ldmin = std::numeric_limits<long double>::denorm_min();
    TFEL_TESTS_ASSERT(std::abs(convert<double>("1.5") - 1.5) < dmin);
    TFEL_TESTS_ASSERT(std::abs(convert<double>("1e-310") - 1e-310) < dmin);
    TFEL_TESTS_ASSERT(std::abs(convert<double>("4.9e-324") - dmin) < dmin);
    TFEL_TESTS_ASSERT(std::abs(convert<double>("1e-400")) < dmin);
    TFEL_TESTS_CHECK_THROW(convert<double>("1e400"), std::exception);
    TFEL_TESTS_CHECK_THROW(convert<double>("1.5x"), std::exception);
    TFEL_TESTS_ASSERT(std::abs(convert<long double>("1e-5000")) < ldmin);
    TFEL_TESTS_CHECK_THROW(convert<long double>("1e5000"), std::exception);
    // errno is preserved
    errno = EDOM;
    static_cast<void>(convert<double>("1e-400"));
    TFEL_TESTS_ASSERT(errno == EDOM);
  }  // end of test3
};

TFEL_TESTS_GENERATE_PROXY(StringAlgorithmsTest, "StringAlgorithms");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StringAlgorithms.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
