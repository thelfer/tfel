/*!
 * \file tfel-check/tests/unitTest/test_AreaComparison.cxx
 * \brief tests the AreaComparison class
 * \date 20/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <memory>
#include <string>
#include <fstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Check/Column.hxx"
#include "TFEL/Check/Test.hxx"
#include "TFEL/Check/AreaComparison.hxx"
#include "TFEL/Check/L2Comparison.hxx"
#include "TFEL/Check/NoInterpolation.hxx"
#include "TFEL/Check/LinearInterpolation.hxx"
#include "TFEL/Check/SplineInterpolation.hxx"
#include "TFEL/Check/SplineLocalInterpolation.hxx"

struct test_AreaComparison final : public tfel::tests::TestCase {
  test_AreaComparison()
      : tfel::tests::TestCase("TFEL/Check", "AreaComparison") {}
  tfel::tests::TestResult execute() override {
    // y = t, sampled differently
    write("test_AreaComparison_A.res", "# t y\n0 0\n1 1\n2 2\n4 4\n");
    write("test_AreaComparison_B.res", "# t y\n0 0\n3 3\n4 4\n");
    // y = 1
    write("test_AreaComparison_C.res", "# t y\n0 1\n4 1\n");
    const auto linear = std::make_shared<tfel::check::LinearInterpolation>();
    // same curve with different samplings: the area is null
    TFEL_TESTS_ASSERT(check("test_AreaComparison_A.res",
                            "test_AreaComparison_B.res", 1.e-12, linear));
    // the area between y = t and y = 1 on [0:4] is 5. It is divided by
    // the maximum value of the first column, i.e. 4
    TFEL_TESTS_ASSERT(!check("test_AreaComparison_A.res",
                             "test_AreaComparison_C.res", 1.25 - 1.e-10,
                             linear));
    TFEL_TESTS_ASSERT(check("test_AreaComparison_A.res",
                            "test_AreaComparison_C.res", 1.25 + 1.e-10,
                            linear));
    // duplicated and unsorted abscissas
    write("test_AreaComparison_D.res", "# t y\n2 2\n0 0\n4 4\n1 1\n4 4\n");
    TFEL_TESTS_ASSERT(check("test_AreaComparison_D.res",
                            "test_AreaComparison_B.res", 1.e-12, linear));
    const auto spline = std::make_shared<tfel::check::SplineInterpolation>();
    TFEL_TESTS_ASSERT(check("test_AreaComparison_D.res",
                            "test_AreaComparison_B.res", 1.e-10, spline));
    // without interpolation, the abscissas must be the same
    const auto none = std::make_shared<tfel::check::NoInterpolation>();
    TFEL_TESTS_CHECK_THROW(check("test_AreaComparison_A.res",
                                 "test_AreaComparison_B.res", 1.e-12, none),
                           std::runtime_error);
    // y = t + 1 with other abscissas. Each curve must be evaluated with
    // its own local splines
    write("test_AreaComparison_F.res", "# t y\n0 0\n1 1\n2 2\n3 3\n4 4\n");
    write("test_AreaComparison_G.res", "# t y\n0 1\n0.5 1.5\n2.5 3.5\n4 5\n");
    TFEL_TESTS_ASSERT(!check(
        "test_AreaComparison_F.res", "test_AreaComparison_G.res", 1 - 1.e-10,
        std::make_shared<tfel::check::SplineLocalInterpolation>()));
    TFEL_TESTS_ASSERT(check(
        "test_AreaComparison_F.res", "test_AreaComparison_G.res", 1 + 1.e-10,
        std::make_shared<tfel::check::SplineLocalInterpolation>()));
    // negative curves: the area between y = -4 and y = -1 on [0:4] is 12.
    // It is divided by the maximum absolute value of the first column
    write("test_AreaComparison_N1.res", "# t y\n0 -4\n4 -4\n");
    write("test_AreaComparison_N2.res", "# t y\n0 -1\n2 -1\n4 -1\n");
    TFEL_TESTS_ASSERT(!check("test_AreaComparison_N1.res",
                             "test_AreaComparison_N2.res", 3 - 1.e-10, linear));
    TFEL_TESTS_ASSERT(check("test_AreaComparison_N1.res",
                            "test_AreaComparison_N2.res", 3 + 1.e-10, linear));
    // null first column: the area between y = 0 and y = 1 on [0:4] is 4,
    // and is not normalized
    write("test_AreaComparison_Z.res", "# t y\n0 0\n2 0\n4 0\n");
    TFEL_TESTS_ASSERT(!check("test_AreaComparison_Z.res",
                             "test_AreaComparison_C.res", 4 - 1.e-10, linear));
    TFEL_TESTS_ASSERT(check("test_AreaComparison_Z.res",
                            "test_AreaComparison_C.res", 4 + 1.e-10, linear));
    // a NaN in the values makes the check fail
    write("test_AreaComparison_NaN.res", "# t y\n0 0\n1 nan\n2 2\n4 4\n");
    TFEL_TESTS_ASSERT(!check("test_AreaComparison_A.res",
                             "test_AreaComparison_NaN.res", 1.e10, linear));
    // a NaN in the abscissas is rejected
    write("test_AreaComparison_NaN2.res", "# t y\nnan 0\n1 1\n2 2\n4 4\n");
    TFEL_TESTS_CHECK_THROW(check("test_AreaComparison_A.res",
                                 "test_AreaComparison_NaN2.res", 1.e10, linear),
                           std::runtime_error);
    // a NaN in a duplicated point makes the check fail
    write("test_AreaComparison_NaN3.res", "# t y\n0 0\n1 1\n1 nan\n2 2\n4 4\n");
    TFEL_TESTS_ASSERT(!check("test_AreaComparison_A.res",
                             "test_AreaComparison_NaN3.res", 1.e10, linear));
    // at least two distinct abscissas are required
    write("test_AreaComparison_S1.res", "# t y\n1 5\n");
    write("test_AreaComparison_S2.res", "# t y\n1 100\n");
    TFEL_TESTS_CHECK_THROW(check("test_AreaComparison_S1.res",
                                 "test_AreaComparison_S2.res", 1.e10, linear),
                           std::runtime_error);
    // L2 norm: on the abscissas 0, 1, 2 and 4, the differences between
    // y = t and y = 1 are 1, 0, 1 and 3. The trapezoidal rule gives 11 for
    // the integral of their squares, and the error is sqrt(11) / 4
    const auto l2 = std::make_shared<tfel::check::L2Comparison>();
    TFEL_TESTS_ASSERT(!check("test_AreaComparison_A.res",
                             "test_AreaComparison_C.res",
                             std::sqrt(11.) / 4 - 1.e-10, linear, l2));
    TFEL_TESTS_ASSERT(check("test_AreaComparison_A.res",
                            "test_AreaComparison_C.res",
                            std::sqrt(11.) / 4 + 1.e-10, linear, l2));
    return this->result;
  }

 private:
  static void write(const std::string& f, const std::string& c) {
    std::ofstream file{f};
    tfel::raise_if(!file,
                   "test_AreaComparison::write: "
                   "could not open file '" +
                       f + "'");
    file << c;
  }
  /*!
   * \return true if the area check succeeds
   * \param[in] f1: first file
   * \param[in] f2: second file
   * \param[in] prec: precision
   * \param[in] i: interpolation used for the integration
   * \param[in] c: comparison
   */
  static bool check(const std::string& f1,
                    const std::string& f2,
                    const double prec,
                    const std::shared_ptr<tfel::check::Interpolation>& i,
                    const std::shared_ptr<tfel::check::Comparison>& c =
                        std::make_shared<tfel::check::AreaComparison>()) {
    tfel::check::Test t;
    t.setComparison(c);
    t.setPrec(prec);
    t.setInterpolation(std::make_shared<tfel::check::NoInterpolation>());
    t.setIntegralInterpolation(i);
    t.setColIntegralInterpolated(std::make_shared<tfel::check::Column>(1));
    t.setColA(std::make_shared<tfel::check::Column>(2));
    t.setColB(std::make_shared<tfel::check::Column>(2));
    t.setFileA(f1);
    t.setFileB(f2);
    t.applyInterpolation();
    t.compare();
    return t.hasSucceed();
  }
};

TFEL_TESTS_GENERATE_PROXY(test_AreaComparison, "AreaComparison");
