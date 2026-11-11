/*!
 * \file   CompatibilityModeTest.cxx
 * \brief  Unit test for the `CompatibilityMode` structure and the
 * associated functions.
 * \author Thomas Helfer
 * \date   08/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "MFront/CompatibilityMode.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontWarningMode.hxx"

struct CompatibilityModeTest final : public tfel::tests::TestCase {
  CompatibilityModeTest()
      : tfel::tests::TestCase("MFront", "CompatibilityModeTest") {
  }  // end of CompatibilityModeTest

  tfel::tests::TestResult execute() override {
    // the default compatibility mode is a global variable, which must be
    // tested before the first call to `setDefaultCompatibilityMode`
    this->testInitiallyUndefinedDefaultCompatibilityMode();
    this->testVersionComparisons();
    this->testKnownCompatibilityModes();
    this->testCheck();
    this->testToString();
    this->testFromString();
    // those tests modify a global variable: they must be the last ones
    this->testSetDefaultCompatibilityMode();
    return this->result;
  }

 private:
  using Version = mfront::CompatibilityMode::Version;
  //! \brief greatest value a `std::size_t` can hold
  static inline constexpr auto max_size =
      std::numeric_limits<std::size_t>::max();
  static inline constexpr auto make_version =
      [](const std::size_t major,
         const std::size_t minor,
         const std::size_t patch) constexpr {
    return mfront::CompatibilityMode::Version{
        .major = major, .minor = minor, .patch = patch};
  };
  /*!
   * \return true if the first string contains the second one
   * \param[in] s: string
   * \param[in] t: sub-string
   */
  static bool contains(const std::string_view s, const std::string_view t) {
    return s.find(t) != std::string::npos;
  }  // end of contains
  /*!
   * \return true if the given function does not throw any exception
   * \param[in] f: function
   */
  template <typename F>
  static bool no_throw(F f) {
    try {
      f();
    } catch (...) {
      return false;
    }
    return true;
  }  // end of no_throw
  /*!
   * \brief call the given function, redirecting the log stream to a string
   * stream to capture the warnings it reports
   * \return the warnings which have been reported
   * \param[in] f: function
   */
  template <typename F>
  static std::string capture_warnings(F f) {
    auto os = std::ostringstream{};
    mfront::setLogStream(os);
    try {
      f();
    } catch (...) {
      mfront::setLogStream(std::cout);
      throw;
    }
    const auto msg = os.str();
    mfront::setLogStream(std::cout);
    return msg;
  }  // end of capture_warnings
  // The default compatibility mode is stored in a global variable which is
  // not initialised: it is undefined until it is explicitly set.
  void testInitiallyUndefinedDefaultCompatibilityMode() {
    const auto v = mfront::getDefaultCompatibilityMode();
    TFEL_TESTS_ASSERT(!v.has_value());
  }  // end of testInitiallyUndefinedDefaultCompatibilityMode
  //
  void testVersionComparisons() {
    using mfront::CompatibilityMode;
    constexpr auto v1 = make_version(1, 0, 0);
    constexpr auto v2 = make_version(1, 0, 1);
    constexpr auto v3 = make_version(2, 0, 0);
    constexpr auto v4 = make_version(2, 2, 0);
    TFEL_TESTS_STATIC_ASSERT(v1 < v2);
    TFEL_TESTS_STATIC_ASSERT(v1 < v3);
    TFEL_TESTS_STATIC_ASSERT(v2 < v3);
    TFEL_TESTS_STATIC_ASSERT(v3 < v4);
    TFEL_TESTS_STATIC_ASSERT(v1 == make_version(1, 0, 0));
    // the major number is the most significant one
    TFEL_TESTS_STATIC_ASSERT(make_version(1, 9, 9) < make_version(2, 0, 0));
    TFEL_TESTS_STATIC_ASSERT(make_version(1, 0, 9) < make_version(1, 1, 0));
    TFEL_TESTS_STATIC_ASSERT(make_version(1, 0, 0) < make_version(1, 0, 1));
    // the other operators are those implicitly defined by the comparison
    // operator
    TFEL_TESTS_STATIC_ASSERT(v2 > v1);
    TFEL_TESTS_STATIC_ASSERT(v2 != v1);
    TFEL_TESTS_STATIC_ASSERT(!(v2 == v1));
    TFEL_TESTS_STATIC_ASSERT(v1 <= v1);
    TFEL_TESTS_STATIC_ASSERT(v1 >= v1);
    TFEL_TESTS_STATIC_ASSERT(v1 <= v2);
    TFEL_TESTS_STATIC_ASSERT(v2 >= v1);
    // the `future` compatibility mode is greater than any version
    TFEL_TESTS_STATIC_ASSERT(CompatibilityMode::future > v4);
    TFEL_TESTS_STATIC_ASSERT(CompatibilityMode::future >
                             make_version(max_size - 1, max_size, max_size));
    TFEL_TESTS_STATIC_ASSERT(CompatibilityMode::future ==
                             make_version(max_size, max_size, max_size));
  }  // end of testVersionComparisons
  //
  void testKnownCompatibilityModes() {
    using mfront::CompatibilityMode;
    // the numbers of the first compatibility mode
    TFEL_TESTS_CHECK_EQUAL(CompatibilityMode::v5_2_0.major, std::size_t{5});
    TFEL_TESTS_CHECK_EQUAL(CompatibilityMode::v5_2_0.minor, std::size_t{2});
    TFEL_TESTS_CHECK_EQUAL(CompatibilityMode::v5_2_0.patch, std::size_t{0});
    // the `future` compatibility mode is the next major version, which is
    // the greatest version which can be described
    TFEL_TESTS_CHECK_EQUAL(CompatibilityMode::future.major, max_size);
    TFEL_TESTS_CHECK_EQUAL(CompatibilityMode::future.minor, max_size);
    TFEL_TESTS_CHECK_EQUAL(CompatibilityMode::future.patch, max_size);
    // the compatibility modes are given in increasing order, they are all
    // valid compatibility modes
    for (std::size_t i = 0u;
         i != CompatibilityMode::known_compatibility_modes.size(); ++i) {
      const auto& v = CompatibilityMode::known_compatibility_modes[i];
      TFEL_TESTS_CHECK(v >= CompatibilityMode::v5_2_0);
      TFEL_TESTS_CHECK(v < CompatibilityMode::future);
      TFEL_TESTS_CHECK(no_throw([&v] { check(v); }));
      if (i != 0u) {
        TFEL_TESTS_CHECK(v >
                         CompatibilityMode::known_compatibility_modes[i - 1u]);
      }
    }
    // the first compatibility mode is the first version in which the
    // compatibility modes were introduced
    TFEL_TESTS_CHECK_EQUAL(CompatibilityMode::known_compatibility_modes.front(),
                           CompatibilityMode::v5_2_0);
    // a file which declares no compatibility mode uses the greatest known
    // compatibility mode, `5.2.0` in Version 5.2, which preserves the
    // compatibility with Version 5.1. This test shall only change when the
    // compatibility mode of a new major version is introduced.
    TFEL_TESTS_CHECK(mfront::getGreatestCompatibilityMode() ==
                     CompatibilityMode::v5_2_0);
    TFEL_TESTS_CHECK(mfront::getEffectiveCompatibilityMode(
                         mfront::MaterialPropertyDescription{}) ==
                     CompatibilityMode::v5_2_0);
  }  // end of testKnownCompatibilityModes
  //
  void testCheck() {
    using mfront::CompatibilityMode;
    // the first compatibility mode is valid
    TFEL_TESTS_CHECK(no_throw([] { check(CompatibilityMode::v5_2_0); }));
    // so is the `future` compatibility mode
    TFEL_TESTS_CHECK(no_throw([] { check(CompatibilityMode::future); }));
    // `check` does not require the version to be a known compatibility
    // mode: any version greater than or equal to 5.2.0 is valid
    TFEL_TESTS_CHECK(no_throw([] { check(make_version(5, 2, 1)); }));
    TFEL_TESTS_CHECK(no_throw([] { check(make_version(5, 3, 0)); }));
    TFEL_TESTS_CHECK(no_throw([] { check(make_version(100, 0, 0)); }));
    // a version strictly lower than 5.2.0 is invalid
    TFEL_TESTS_CHECK_THROW(check(make_version(5, 1, 9)), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(check(make_version(5, 0, 0)), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(check(make_version(1, 0, 0)), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(check(make_version(0, 0, 0)), std::runtime_error);
  }  // end of testCheck
  //
  void testToString() {
    using mfront::CompatibilityMode;
    TFEL_TESTS_CHECK_EQUAL(to_string(CompatibilityMode::v5_2_0), "5.2.0");
    TFEL_TESTS_CHECK_EQUAL(to_string(make_version(5, 3, 0)), "5.3.0");
    TFEL_TESTS_CHECK_EQUAL(to_string(make_version(10, 11, 12)), "10.11.12");
    // the `future` compatibility mode has a specific representation
    TFEL_TESTS_CHECK_EQUAL(to_string(CompatibilityMode::future), "future");
    // `to_string` does not clamp the version to a known compatibility mode
    TFEL_TESTS_CHECK_EQUAL(
        to_string(make_version(max_size, max_size, max_size - 1)),
        "18446744073709551615.18446744073709551615."
        "18446744073709551614");
    // a version which is not a valid compatibility mode can't be converted
    // to a string
    TFEL_TESTS_CHECK_THROW(to_string(make_version(5, 1, 9)),
                           std::runtime_error);
    TFEL_TESTS_CHECK_THROW(to_string(make_version(1, 0, 0)),
                           std::runtime_error);
    // the description of a compatibility mode is a valid input
    for (const auto& v : CompatibilityMode::known_compatibility_modes) {
      TFEL_TESTS_CHECK_EQUAL(CompatibilityMode::fromString(to_string(v)), v);
    }
  }  // end of testToString
  //
  void testFromString() {
    using mfront::CompatibilityMode;
    // the `future` compatibility mode is not described by a version
    TFEL_TESTS_CHECK_EQUAL(CompatibilityMode::fromString("future"),
                           CompatibilityMode::future);
    TFEL_TESTS_CHECK_EQUAL(
        CompatibilityMode::fromString(std::string_view{"future", 6}),
        CompatibilityMode::future);
    // a string which describes a known compatibility mode
    TFEL_TESTS_CHECK_EQUAL(CompatibilityMode::fromString("5.2.0"),
                           CompatibilityMode::v5_2_0);
    // a version which is not a known compatibility mode is clamped to the
    // closest compatibility mode strictly lower, which is reported as a
    // warning
    const auto& m = CompatibilityMode::known_compatibility_modes.back();
    const auto d = to_string(m);
    for (const auto& v :
         {Version{.major = m.major, .minor = m.minor, .patch = m.patch + 1},
          Version{.major = m.major, .minor = m.minor + 1, .patch = m.patch},
          Version{.major = m.major + 1, .minor = m.minor, .patch = m.patch}}) {
      const auto s = to_string(v);
      auto o = std::optional<Version>{};
      const auto w = this->capture_warnings(
          [&o, s] { o = CompatibilityMode::fromString(s); });
      TFEL_TESTS_ASSERT(o.has_value());
      TFEL_TESTS_CHECK_EQUAL(*o, m);
      TFEL_TESTS_ASSERT(contains(w, "clamping version '" + s + "'"));
      TFEL_TESTS_ASSERT(contains(w, "closest compatibility mode '" + d + "'"));
    }
    // a version which is lower than the first compatibility mode is
    // rejected
    TFEL_TESTS_CHECK_THROW(CompatibilityMode::fromString("5.1.0"),
                           std::runtime_error);
    TFEL_TESTS_CHECK_THROW(CompatibilityMode::fromString("1.0.0"),
                           std::runtime_error);
    // strings which do not describe a version
    for (const auto s :
         {std::string_view{}, std::string_view{"5"}, std::string_view{"5.2"},
          std::string_view{"5.2.0.0"}, std::string_view{".5.2"},
          std::string_view{"5..2"}, std::string_view{"5.2."},
          std::string_view{"a.b.c"}, std::string_view{"5.x.0"},
          std::string_view{"5.2.x"}, std::string_view{"future.0.0"},
          std::string_view{"FUTURE"}, std::string_view{"+5.2.0"},
          std::string_view{"-5.2.0"}, std::string_view{" 5.2.0"},
          std::string_view{"5.2.0 "}, std::string_view{"5,2,0"},
          // the greatest version is not clamped but the
          // numbers are too big to be described by a
          // compatibility mode: this is an error
          std::string_view{"18446744073709551616.0.0"}}) {
      TFEL_TESTS_CHECK_THROW(CompatibilityMode::fromString(s),
                             std::runtime_error);
    }
  }  // end of testFromString
  //
  void testSetDefaultCompatibilityMode() {
    using mfront::CompatibilityMode;
    using mfront::getDefaultCompatibilityMode;
    using mfront::setDefaultCompatibilityMode;
    auto o = std::optional<Version>{};
    // an invalid compatibility mode is rejected: the default compatibility
    // mode is left undefined
    TFEL_TESTS_CHECK_THROW(setDefaultCompatibilityMode(make_version(1, 0, 0)),
                           std::runtime_error);
    TFEL_TESTS_CHECK_THROW(setDefaultCompatibilityMode("5.1.0"),
                           std::runtime_error);
    TFEL_TESTS_CHECK_THROW(setDefaultCompatibilityMode("5.2"),
                           std::runtime_error);
    TFEL_TESTS_ASSERT(!getDefaultCompatibilityMode().has_value());
    // the warning mode and the warning error mode are enabled by default
    TFEL_TESTS_ASSERT(mfront::getWarningMode());
    TFEL_TESTS_ASSERT(!mfront::getWarningErrorMode());
    // a string which describes a known compatibility mode
    const auto w1 = this->capture_warnings([&o] {
      setDefaultCompatibilityMode(to_string(CompatibilityMode::v5_2_0));
      o = getDefaultCompatibilityMode();
    });
    TFEL_TESTS_ASSERT(o.has_value());
    TFEL_TESTS_CHECK_EQUAL(*o, CompatibilityMode::v5_2_0);
    // no warning is reported when the version is a known compatibility mode
    TFEL_TESTS_ASSERT(w1.empty());
    // a version which describes a known compatibility mode
    const auto w2 = this->capture_warnings([&o] {
      setDefaultCompatibilityMode(CompatibilityMode::v5_2_0);
      o = getDefaultCompatibilityMode();
    });
    TFEL_TESTS_ASSERT(o.has_value());
    TFEL_TESTS_CHECK_EQUAL(*o, CompatibilityMode::v5_2_0);
    TFEL_TESTS_ASSERT(w2.empty());
    // the `future` compatibility mode is never clamped, whatever the way it
    // is given
    const auto w3 = this->capture_warnings([&o] {
      setDefaultCompatibilityMode("future");
      o = getDefaultCompatibilityMode();
    });
    TFEL_TESTS_ASSERT(o.has_value());
    TFEL_TESTS_CHECK_EQUAL(*o, CompatibilityMode::future);
    TFEL_TESTS_ASSERT(w3.empty());
    const auto w4 = this->capture_warnings([&o] {
      setDefaultCompatibilityMode(CompatibilityMode::future);
      o = getDefaultCompatibilityMode();
    });
    TFEL_TESTS_ASSERT(o.has_value());
    TFEL_TESTS_CHECK_EQUAL(*o, CompatibilityMode::future);
    TFEL_TESTS_ASSERT(w4.empty());
    // a version which is not a known compatibility mode is clamped to the
    // closest compatibility mode strictly lower, which is reported as a
    // warning, both when the version is given as a string or as a version
    const auto m = CompatibilityMode::known_compatibility_modes.back();
    const auto v =
        Version{.major = m.major, .minor = m.minor, .patch = m.patch + 1};
    const auto s = to_string(v);
    const auto w5 = this->capture_warnings([&o, s] {
      setDefaultCompatibilityMode(s);
      o = getDefaultCompatibilityMode();
    });
    TFEL_TESTS_ASSERT(o.has_value());
    TFEL_TESTS_CHECK_EQUAL(*o, m);
    TFEL_TESTS_ASSERT(contains(w5, "clamping version '" + s + "'"));
    const auto w6 = this->capture_warnings([&o, v] {
      setDefaultCompatibilityMode(v);
      o = getDefaultCompatibilityMode();
    });
    TFEL_TESTS_ASSERT(o.has_value());
    TFEL_TESTS_CHECK_EQUAL(*o, m);
    TFEL_TESTS_ASSERT(contains(w6, "clamping version '" + s + "'"));
    // an invalid compatibility mode is rejected: the default compatibility
    // mode is left unchanged
    TFEL_TESTS_CHECK_THROW(setDefaultCompatibilityMode("1.0.0"),
                           std::runtime_error);
    TFEL_TESTS_CHECK_THROW(setDefaultCompatibilityMode(make_version(5, 1, 0)),
                           std::runtime_error);
    o = getDefaultCompatibilityMode();
    TFEL_TESTS_ASSERT(o.has_value());
    TFEL_TESTS_CHECK_EQUAL(*o, m);
    // no warning is reported when the warning mode is disabled but the
    // compatibility mode is nonetheless clamped
    const auto wm = mfront::getWarningMode();
    mfront::setWarningMode(false);
    const auto w7 = this->capture_warnings([&o, v] {
      setDefaultCompatibilityMode(v);
      o = getDefaultCompatibilityMode();
    });
    mfront::setWarningMode(wm);
    TFEL_TESTS_ASSERT(o.has_value());
    TFEL_TESTS_CHECK_EQUAL(*o, m);
    TFEL_TESTS_ASSERT(w7.empty());
    // in the warning error mode, clamping a version is an error and the
    // default compatibility mode is left unchanged
    const auto wem = mfront::getWarningErrorMode();
    mfront::setWarningErrorMode(true);
    auto msg = std::string{};
    auto clamped = false;
    try {
      setDefaultCompatibilityMode(v);
    } catch (std::exception& e) {
      clamped = true;
      msg = e.what();
    }
    // the `future` compatibility mode is never clamped: it is available in
    // the warning error mode
    auto rejected = false;
    try {
      setDefaultCompatibilityMode("future");
    } catch (std::exception&) {
      rejected = true;
    }
    mfront::setWarningErrorMode(wem);
    o = getDefaultCompatibilityMode();
    TFEL_TESTS_ASSERT(clamped);
    TFEL_TESTS_ASSERT(contains(msg, "[warning]"));
    TFEL_TESTS_ASSERT(contains(msg, "clamping version '" + s + "'"));
    TFEL_TESTS_ASSERT(!rejected);
    TFEL_TESTS_ASSERT(o.has_value());
    TFEL_TESTS_CHECK_EQUAL(*o, CompatibilityMode::future);
  }  // end of testSetDefaultCompatibilityMode
};

TFEL_TESTS_GENERATE_PROXY(CompatibilityModeTest, "CompatibilityModeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CompatibilityMode.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
