/*!
 * \file   tests/Math/hill.cxx
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
#endif /* NDEBUG */

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/Hill.hxx"

struct HillTensorTest final : public tfel::tests::TestCase {
  HillTensorTest()
      : tfel::tests::TestCase("TFEL/Material", "HillTensorTest") {
  }  // end of HillTensorTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    using namespace tfel::material;
    const double eps = 1.e-14;
    const double sqrt2 = std::sqrt(2.);
    st2tost2<3u, double> H(hillTensor<3u>(2., 3., 4., 5., 6., 7.));
    // matrix given in the documentation of the hillTensor function
    const double Href[6][6] = {{6, -2, -4, 0, 0, 0}, {-2, 5, -3, 0, 0, 0},
                               {-4, -3, 7, 0, 0, 0}, {0, 0, 0, 5, 0, 0},
                               {0, 0, 0, 0, 6, 0},   {0, 0, 0, 0, 0, 7}};
    for (unsigned short i = 0; i != 6; ++i) {
      for (unsigned short j = 0; j != 6; ++j) {
        TFEL_TESTS_ASSERT(std::abs(H(i, j) - Href[i][j]) < eps);
      }
    }
    // Hill stress: F (s11-s22)^2 + G (s22-s33)^2 + H (s33-s11)^2
    //              + 2 L s12^2 + 2 M s13^2 + 2 N s23^2 = 167
    const stensor<3u, double> s = {1.,          2.,          4.,
                                   0.5 * sqrt2, 1.5 * sqrt2, 2.5 * sqrt2};
    TFEL_TESTS_ASSERT(std::abs((s | H * s) - 167) < 167 * eps);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(HillTensorTest, "HillTensorTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("hill.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
