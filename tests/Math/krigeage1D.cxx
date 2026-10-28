/*!
 * \file   tests/Math/krigeage1D.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  09 avr 2009
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
#include <algorithm>
#include <random>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/General/floating_point_exceptions.hxx"
#include "TFEL/Math/Kriging.hxx"

double rand(const double min, const double max) {
  // std::minstd_rand0 is fully specified by the standard: the data set is
  // the same on every platform, unlike the one given by std::rand
  static std::minstd_rand0 g;
  return min + (max - min) * static_cast<double>(g() - g.min()) /
                   static_cast<double>(g.max() - g.min());
}

struct Kriging1DTest final : public tfel::tests::TestCase {
  Kriging1DTest()
      : tfel::tests::TestCase("TFEL/Math", "Kriging1DTest") {
  }  // end of Kriging1DTest
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    Kriging<1> k;
    double x;
    double y;
    double xmin = 1.;
    double xmax = 0.;
    unsigned int i;
    ofstream data("data.txt");
    ofstream out("out.txt");

    init_floating_point_exceptions();

    for (i = 0; i != 10; ++i) {
      x = rand(0., 1.);
      y = exp(cos(x * x)) + 0.1 * (rand(0., 1.) - 0.5);
      k.addValue(x, y);
      xmin = std::min(xmin, x);
      xmax = std::max(xmax, x);
      data << x << " " << y << endl;
    }
    k.setNuggetEffect(0.02);
    k.buildInterpolation();
    for (x = -0.5; x < 1.5; x += 0.05) {
      out << x << " " << k(x) << endl;
    }
    // with the nugget effect, the noise of the data (whose amplitude is
    // 0.05) is smoothed out: the interpolation stays close to the
    // function without noise, in the range of the data (outside of it,
    // the kriging extrapolates)
    for (x = 0; x < 1.025; x += 0.05) {
      if ((x < xmin) || (x > xmax)) {
        continue;
      }
      TFEL_TESTS_ASSERT(std::abs(k(x) - exp(cos(x * x))) < 0.1);
    }
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(Kriging1DTest, "Kriging1DTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("krigeage1D.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
