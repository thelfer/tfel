/*!
 * \file   CompatibilityModeTest.cxx
 * \brief    
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
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "MFront/CompatibilityMode.hxx"

struct CompatibilityModeTest final : public tfel::tests::TestCase {
  CompatibilityModeTest()
      : tfel::tests::TestCase("MFront", "CompatibilityModeTest") {
  }  // end of CompatibilityModeTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }

 private:
  static inline constexpr auto make_version = [](const std::size_t major,
                                            const std::size_t minor,
                                            const std::size_t patch) constexpr {
    return mfront::CompatibilityMode::Version{
        .major = major, .minor = minor, .patch = patch};
  };
  //
  void test1() {
    constexpr auto v1 = make_version(1, 0, 0);
    constexpr auto v2 = make_version(1, 0, 1);
    constexpr auto v3 = make_version(2, 0, 0);
    constexpr auto v4 = make_version(2, 2, 0);
    TFEL_TESTS_STATIC_ASSERT(v1 < v2);
    TFEL_TESTS_STATIC_ASSERT(v1 < v3);
    TFEL_TESTS_STATIC_ASSERT(v2 < v3);
    TFEL_TESTS_STATIC_ASSERT(v3 < v4);
    TFEL_TESTS_STATIC_ASSERT(v1 == make_version(1, 0, 0));
  }
  //
  void test2() {
    using namespace mfront;
    TFEL_TESTS_CHECK_THROW(setDefaultCompatibilityMode(make_version(1, 0, 0)),
                           std::runtime_error);
    try {
      // this version will never exist
      setDefaultCompatibilityMode(make_version(5, 3, 0));
      const auto v = getDefaultCompatibilityMode();
      TFEL_TESTS_CHECK_EQUAL(v, CompatibilityMode::v5_2_0);
      setDefaultCompatibilityMode("future");
      const auto v2 = getDefaultCompatibilityMode();
      TFEL_TESTS_CHECK_EQUAL(v2, CompatibilityMode::future);
    } catch (std::exception&) {
      this->registerResult("setting the default compatibility mode faile",
                           false);
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(CompatibilityModeTest, "CompatibilityModeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CompatibilityMode.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
