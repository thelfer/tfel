/*!
 * \file   tests/Math/stensor11.cxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <cassert>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/Matrix/tmatrixIO.hxx"

struct STensorTest11 final : public tfel::tests::TestCase {
  STensorTest11()
      : tfel::tests::TestCase("TFEL/Math", "STensorTest11") {
  }  // end of STensorTest11
  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    const double eps = 1.e-12;
    const double sqrt2 = sqrt(2.);
    stensor<2u, double> s = {8.2, 4.5, 7.2, 2.3 * sqrt2};
    tvector<3u, double> vp;
    tmatrix<3u, 3u, double> m;
    s.computeEigenVectors(vp, m);
    TFEL_TESTS_ASSERT(std::abs(vp[0] - 9.30169442862909) < eps);
    TFEL_TESTS_ASSERT(std::abs(vp[1] - 3.39830557137091) < eps);
    TFEL_TESTS_ASSERT(std::abs(vp[2] - 7.2) < eps);
    std::cout << "m : " << m << std::endl;
    s.changeBasis(m);
    std::cout << "s : " << s << std::endl;
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(STensorTest11, "STensorTest11");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("stensor11.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
