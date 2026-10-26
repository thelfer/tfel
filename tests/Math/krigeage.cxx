/*!
 * \file  tests/Math/krigeage.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 09 avr 2009
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
#include <vector>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/General/floating_point_exceptions.hxx"
#include "TFEL/Math/FactorizedKriging.hxx"

#ifndef _MSC_VER
#define KRIGING_STATIC static
#else
#define KRIGING_STATIC
#endif

KRIGING_STATIC double rand(const double min, const double max) {
  return min + ((max - min) * rand()) / RAND_MAX;
}

struct FactorizedKrigingTest final : public tfel::tests::TestCase {
  FactorizedKrigingTest()
      : tfel::tests::TestCase("TFEL/Math", "FactorizedKrigingTest") {
  }  // end of FactorizedKrigingTest
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using std::vector;
    FactorizedKriging<1, 1> k;
    double x;
    double t;
    unsigned int i;
    vector<double> xs;
    vector<double> ts;
    vector<double> fs;
    ofstream data("data.txt");
    ofstream out("out.txt");

    init_floating_point_exceptions();

    for (i = 0; i != 100; ++i) {
      x = rand(0., 1.);
      t = rand(0., 1.);
      double f = cos(x + t) * exp(x);
      k.addValue(x, t, f);
      xs.push_back(x);
      ts.push_back(t);
      fs.push_back(f);
      data << x << " " << t << " " << f << endl;
    }
    k.buildInterpolation();
    for (x = -0.5; x < 1.5; x += 0.05) {
      for (t = -0.5; t < 1.5; t += 0.05) {
        out << x << " " << t << " " << k(x, t) << endl;
      }
    }
    // without nugget effect, the data points are interpolated
    for (i = 0; i != 100; ++i) {
      TFEL_TESTS_ASSERT(std::abs(k(xs[i], ts[i]) - fs[i]) < 1.e-10);
    }
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(FactorizedKrigingTest, "FactorizedKrigingTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("krigeage.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
