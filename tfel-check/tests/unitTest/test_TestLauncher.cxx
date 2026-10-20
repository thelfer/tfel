/*!
 * \file tfel-check/tests/unitTest/test_TestLauncher.cxx
 * \brief tests the TestLauncher class
 * \date 20/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <fstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Check/Configuration.hxx"
#include "TFEL/Check/TestLauncher.hxx"

struct test_TestLauncher final : public tfel::tests::TestCase {
  test_TestLauncher() : tfel::tests::TestCase("TFEL/Check", "TestLauncher") {}
  tfel::tests::TestResult execute() override {
    // y = t, sampled differently
    write("test_TestLauncher_A.ref", "# t y\n0 0\n2 2\n4 4\n");
    write("test_TestLauncher_B.res", "# t y\n0 0\n1 1\n2 2\n3 3\n4 4\n");
    write("test_TestLauncher_C.res", "# t y\n0 0\n4 4\n");
    // same files, with legends. With the format used by tfel-check, the
    // legends are given by the first line, which shall not be a comment
    write("test_TestLauncher_A2.ref", "t y\n0 0\n2 2\n4 4\n");
    write("test_TestLauncher_B2.res", "t y\n0 0\n1 1\n2 2\n3 3\n4 4\n");
    // y = t + 1
    write("test_TestLauncher_D.res", "# t y\n0 1\n1 2\n2 3\n3 4\n4 5\n");
    // the results are interpolated at the times of the reference
    TFEL_TESTS_ASSERT(run("test_TestLauncher1",
                          "@Interpolation Linear using 1;\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_A.ref' "
                          "'test_TestLauncher_B.res' 2;\n"));
    TFEL_TESTS_ASSERT(run("test_TestLauncher2",
                          "@Interpolation Spline using 't';\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_A2.ref' "
                          "'test_TestLauncher_B2.res' 'y';\n"));
    TFEL_TESTS_ASSERT(run("test_TestLauncher6",
                          "@Interpolation LocalSpline using 1;\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_A.ref' "
                          "'test_TestLauncher_B.res' 2;\n"));
    TFEL_TESTS_ASSERT(!run("test_TestLauncher3",
                           "@Interpolation Linear using 1;\n"
                           "@Precision 1.e-12;\n"
                           "@Test 'test_TestLauncher_A.ref' "
                           "'test_TestLauncher_D.res' 2;\n"));
    // fewer results than references
    TFEL_TESTS_ASSERT(run("test_TestLauncher5",
                          "@Interpolation Linear using 1 AllowLessResults;\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_A.ref' "
                          "'test_TestLauncher_C.res' 2;\n"));
    return this->result;
  }

 private:
  static void write(const std::string& f, const std::string& c) {
    std::ofstream file{f};
    tfel::raise_if(!file,
                   "test_TestLauncher::write: "
                   "could not open file '" +
                       f + "'");
    file << c;
  }
  /*!
   * \return true if the tests described by the given content succeed
   * \param[in] n: name of the test
   * \param[in] c: content of the `.check` file
   */
  static bool run(const std::string& n, const std::string& c) {
    write(n + ".check", c);
    auto configuration = tfel::check::Configuration{};
    tfel::check::TestLauncher l(configuration, n + ".check");
    return l.execute(configuration);
  }
};

TFEL_TESTS_GENERATE_PROXY(test_TestLauncher, "TestLauncher");
