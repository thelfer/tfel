/*!
 * \file   tests/Math/TinyVectorOfStensorFromTinyVectorView.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  08 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Array/ViewsArray.hxx"

struct TinyVectorOfStensorFromTinyVectorViewTest final
    : public tfel::tests::TestCase {
  TinyVectorOfStensorFromTinyVectorViewTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "TinyVectorOfStensorFromTinyVectorView") {
  }  // end of TinyVectorOfStensorFromTinyVectorViewTest

  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    const double eps = 100 * std::numeric_limits<double>::epsilon();
    const tvector<10, double> values = {1.1, 10.1, 2.9, 9.2, 3.8,
                                        8.3, 4.7,  7.4, 5.6, 6.5};
    tvector<10, double> v(values);
    auto tsftv = map_array<tvector<2u, stensor<2u, double>>>(v.data() + 2u);
    const auto& s1 = tsftv(0);
    auto s2 = tsftv(1);
    for (stensor<2, double>::size_type i = 0; i != 4u; ++i) {
      auto idx = static_cast<unsigned short>(i + 2u);
      TFEL_TESTS_ASSERT(std::abs(s1(i) - v(idx)) < eps);
      idx = static_cast<unsigned short>(i + 6u);
      TFEL_TESTS_ASSERT(std::abs(s2(i) - v(idx)) < eps);
    }
    s2 += stensor<2, double>::Id();
    for (stensor<2, double>::size_type i = 0; i != 3u; ++i) {
      const auto idx = static_cast<unsigned short>(i + 6u);
      TFEL_TESTS_ASSERT(std::abs(s2(i) - values[idx] - 1) < eps);
      TFEL_TESTS_ASSERT(std::abs(v(idx) - values[idx] - 1) < eps);
    }
    TFEL_TESTS_ASSERT(std::abs(s2(3) - values[9u]) < eps);
    return this->result;
  }  // end of execute

};  // end of TinyVectorOfStensorFromTinyVectorViewTest

TFEL_TESTS_GENERATE_PROXY(TinyVectorOfStensorFromTinyVectorViewTest,
                          "TinyVectorOfStensorFromTinyVectorViewTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TinyVectorOfStensorFromTinyVectorView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
