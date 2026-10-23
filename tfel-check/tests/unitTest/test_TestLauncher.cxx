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

#include <cstdio>
#include <string>
#include <utility>
#include <fstream>
#include <iterator>
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
    // results stopping before the end of the reference. Extending the
    // results by their last value would hide the difference.
    write("test_TestLauncher_H.ref", "# t y\n0 0\n2 2\n4 2\n");
    write("test_TestLauncher_E.res",
          "# t y\n0 0\n0.5 0.5\n1 1\n1.5 1.5\n2 2\n2.5 2\n");
    TFEL_TESTS_ASSERT(!run("test_TestLauncher7",
                           "@Interpolation Linear using 1;\n"
                           "@Precision 1.e-12;\n"
                           "@Test 'test_TestLauncher_H.ref' "
                           "'test_TestLauncher_E.res' 2;\n"));
    // empty files
    write("test_TestLauncher_F.res", "# t y\n");
    TFEL_TESTS_ASSERT(!run("test_TestLauncher8",
                           "@Interpolation Linear using 1 AllowLessResults;\n"
                           "@Precision 1.e-12;\n"
                           "@Test 'test_TestLauncher_A.ref' "
                           "'test_TestLauncher_F.res' 2;\n"));
    TFEL_TESTS_ASSERT(!run("test_TestLauncher9",
                           "@Interpolation Linear using 1;\n"
                           "@Precision 1.e-12;\n"
                           "@Test 'test_TestLauncher_F.res' "
                           "'test_TestLauncher_A.ref' 2;\n"));
    // duplicated last line
    write("test_TestLauncher_G.res", "# t y\n0 0\n2 2\n4 4\n4 4\n");
    TFEL_TESTS_ASSERT(run("test_TestLauncher10",
                          "@Interpolation Spline using 1;\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_G.res' "
                          "'test_TestLauncher_G.res' 2;\n"));
    // local splines on results with two lines
    write("test_TestLauncher_I.ref", "# t y\n1 1\n3 3\n");
    TFEL_TESTS_ASSERT(run("test_TestLauncher11",
                          "@Interpolation LocalSpline using 1;\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_I.ref' "
                          "'test_TestLauncher_C.res' 2;\n"));
    // results stopping early, even with the AllowLessResults option
    write("test_TestLauncher_J.res", "# t y\n0 0\n2 2\n");
    TFEL_TESTS_ASSERT(!run("test_TestLauncher14",
                           "@Interpolation Linear using 1 AllowLessResults;\n"
                           "@Precision 1.e-12;\n"
                           "@Test 'test_TestLauncher_B.res' "
                           "'test_TestLauncher_J.res' 2;\n"));
    // the number of values is the number of distinct abscissas
    write("test_TestLauncher_K.ref", "# t y\n0 0\n2 2\n2 2\n4 4\n");
    TFEL_TESTS_ASSERT(run("test_TestLauncher15",
                          "@Interpolation Linear using 1;\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_K.ref' "
                          "'test_TestLauncher_A.ref' 2;\n"));
    // a new @Interpolation keyword resets the AllowLessResults option
    TFEL_TESTS_ASSERT(!run("test_TestLauncher12",
                           "@Interpolation Linear using 1 AllowLessResults;\n"
                           "@Interpolation Linear using 1;\n"
                           "@Precision 1.e-12;\n"
                           "@Test 'test_TestLauncher_A.ref' "
                           "'test_TestLauncher_C.res' 2;\n"));
    // the interpolation of the Area comparison does not depend on the
    // @Interpolation keyword
    TFEL_TESTS_ASSERT(run("test_TestLauncher13",
                          "@TestType Area interpolation Linear using 1;\n"
                          "@Interpolation None;\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_A.ref' "
                          "'test_TestLauncher_B.res' 2;\n"));
    // Mixed comparison of negative values: the relative tolerance is
    // 1.e-3 * 100 = 0.1
    write("test_TestLauncher_M1.ref", "# t y\n0 -100\n1 -100\n");
    write("test_TestLauncher_M2.res", "# t y\n0 -100.05\n1 -100.05\n");
    for (const auto f :
         {"test_TestLauncher_M1.ref", "test_TestLauncher_M2.res"}) {
      TFEL_TESTS_ASSERT(run("test_TestLauncher_mixed",
                            "@TestType Mixed;\n"
                            "@Precision 1.e-3 1.e-12;\n"
                            "@Test 'test_TestLauncher_M1.ref' '" +
                                std::string{f} + "' 2;\n"));
    }
    // a NaN in the results makes all comparisons fail
    write("test_TestLauncher_N.res", "# t y\n0 0\n2 nan\n4 4\n");
    const char* const types[] = {
        "@TestType Absolute;\n@Precision 1.e-12;\n",
        "@TestType Relative;\n@Precision 1.e-12;\n",
        "@TestType RelativeAndAbsolute;\n@Precision 1.e-12 1.e-12;\n",
        "@TestType Mixed;\n@Precision 1.e-12 1.e-12;\n"};
    auto n = 0;
    for (const auto t : types) {
      TFEL_TESTS_ASSERT(!run("test_TestLauncher_nan" + std::to_string(n),
                             std::string{t} +
                                 "@Test 'test_TestLauncher_A.ref' "
                                 "'test_TestLauncher_N.res' 2;\n"));
      ++n;
    }
    // the NaN is reported in the log
    TFEL_TESTS_ASSERT(contains("test_TestLauncher_nan0.checklog",
                               "Maximum absolute error : nan"));
    // a NaN in a duplicated point makes the comparison fail
    write("test_TestLauncher_N2.res", "# t y\n0 0\n2 2\n2 nan\n4 4\n");
    TFEL_TESTS_ASSERT(!run("test_TestLauncher_nan_duplicated",
                           "@Interpolation Linear using 1;\n"
                           "@Precision 1.e-12;\n"
                           "@Test 'test_TestLauncher_A.ref' "
                           "'test_TestLauncher_N2.res' 2;\n"));
    // a NaN between two abscissas of the reference makes the comparison
    // fail
    write("test_TestLauncher_N3.res", "# t y\n0 0\n1 nan\n2 2\n3 3\n4 4\n");
    TFEL_TESTS_ASSERT(!run("test_TestLauncher_nan_between",
                           "@Interpolation Linear using 1;\n"
                           "@Precision 1.e-12;\n"
                           "@Test 'test_TestLauncher_A.ref' "
                           "'test_TestLauncher_N3.res' 2;\n"));
    // the location of the maximum error is given by the lines of the
    // files from which the values were read
    write("test_TestLauncher_L1.ref", "# t y\n0 0\n2 2\n4 4\n");
    write("test_TestLauncher_L2.res", "t y\n# comment\n0 0\n2 5\n4 4\n");
    TFEL_TESTS_ASSERT(!run("test_TestLauncher_lines",
                           "@Precision 1.e-12;\n"
                           "@Test 'test_TestLauncher_L1.ref' "
                           "'test_TestLauncher_L2.res' 2;\n"));
    TFEL_TESTS_ASSERT(
        contains("test_TestLauncher_lines.checklog",
                 "at line 3 of file 'test_TestLauncher_L1.ref' "
                 "and line 4 of file 'test_TestLauncher_L2.res'"));
    // after an interpolation, only the line of the reference is known
    TFEL_TESTS_ASSERT(
        contains("test_TestLauncher3.checklog",
                 "at line 2 of file 'test_TestLauncher_A.ref'\n"));
    // without interpolation, the abscissas are checked if a column is
    // given
    write("test_TestLauncher_P.res", "# t y\n0 0\n1 2\n4 4\n");
    TFEL_TESTS_ASSERT(run("test_TestLauncher_none1",
                          "@Interpolation None using 1;\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_A.ref' "
                          "'test_TestLauncher_L1.ref' 2;\n"));
    TFEL_TESTS_ASSERT(!run("test_TestLauncher_none2",
                           "@Interpolation None using 1;\n"
                           "@Precision 1.e-12;\n"
                           "@Test 'test_TestLauncher_A.ref' "
                           "'test_TestLauncher_P.res' 2;\n"));
    TFEL_TESTS_ASSERT(
        contains("test_TestLauncher_none2.checklog", "the abscissas of files"));
    TFEL_TESTS_ASSERT(run("test_TestLauncher_none3",
                          "@Interpolation None;\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_A.ref' "
                          "'test_TestLauncher_P.res' 2;\n"));
    // the column of a previous @Interpolation keyword is not used
    TFEL_TESTS_ASSERT(run("test_TestLauncher_none4",
                          "@Interpolation Linear using 1;\n"
                          "@Interpolation None;\n"
                          "@Precision 1.e-12;\n"
                          "@Test 'test_TestLauncher_A.ref' "
                          "'test_TestLauncher_P.res' 2;\n"));
    // L2 comparison: the difference between y = t and y = t + 1 is 1 on
    // [0:4], so the error is sqrt(4) / 4 = 0.5
    for (const auto& [p, b] :
         {std::pair{"0.49", false}, std::pair{"0.51", true}}) {
      TFEL_TESTS_ASSERT(run("test_TestLauncher_l2",
                            "@TestType L2 interpolation Linear using 1;\n"
                            "@Precision " +
                                std::string{p} +
                                ";\n"
                                "@Test 'test_TestLauncher_A.ref' "
                                "'test_TestLauncher_D.res' 2;\n") == b);
    }
    return this->result;
  }

 private:
  //! \return true if the given file contains the given text
  static bool contains(const std::string& f, const std::string& t) {
    std::ifstream file{f};
    const auto content = std::string{std::istreambuf_iterator<char>{file},
                                     std::istreambuf_iterator<char>{}};
    return content.find(t) != std::string::npos;
  }
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
    auto r = false;
    {
      tfel::check::TestLauncher l(configuration, n + ".check");
      r = l.execute(configuration);
    }
    // some tests are expected to fail: their JUnit report, written by the
    // destructor of the launcher, is removed so that the CI does not
    // report them
    std::remove(("TEST-" + n + ".xml").c_str());
    return r;
  }
};

TFEL_TESTS_GENERATE_PROXY(test_TestLauncher, "TestLauncher");
