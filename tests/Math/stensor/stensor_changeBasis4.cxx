/*!
 * \file  tests/Math/stensor_changeBasis4.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"

struct StensorChangeBasis4Test final : public tfel::tests::TestCase {
  StensorChangeBasis4Test()
      : tfel::tests::TestCase("TFEL/Math", "StensorChangeBasis4Test") {
  }  // end of StensorChangeBasis4Test

  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    tfel::math::tmatrix<3u, 3u, real> m;
    tfel::math::tvector<3u, real> vp;
    stensor<3, real> s(0.);
    s(0) = 1.232;
    s(1) = 2.5198;
    s(2) = 0.234;
    s(3) = 1.5634;
    s(4) = 3.3425;
    s(5) = 0.9765;

    s.computeEigenVectors(vp, m);
    s.changeBasis(m);
    TFEL_TESTS_ASSERT(abs(vp(0) - 4.16709379934921) <
                      20 * numeric_limits<real>::epsilon());
    TFEL_TESTS_ASSERT(abs(vp(1) + 1.68923153093191) <
                      20 * numeric_limits<real>::epsilon());
    TFEL_TESTS_ASSERT(abs(vp(2) - 1.50793773158270) <
                      20 * numeric_limits<real>::epsilon());
    TFEL_TESTS_ASSERT(abs(s(0) - vp(0)) < 20 * numeric_limits<real>::epsilon());
    TFEL_TESTS_ASSERT(abs(s(1) - vp(1)) < 20 * numeric_limits<real>::epsilon());
    TFEL_TESTS_ASSERT(abs(s(2) - vp(2)) < 20 * numeric_limits<real>::epsilon());
    TFEL_TESTS_ASSERT(abs(s(3)) < 20 * numeric_limits<real>::epsilon());
    TFEL_TESTS_ASSERT(abs(s(4)) < 20 * numeric_limits<real>::epsilon());
    TFEL_TESTS_ASSERT(abs(s(5)) < 20 * numeric_limits<real>::epsilon());
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(StensorChangeBasis4Test, "StensorChangeBasis4Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("StensorChangeBasis4.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
