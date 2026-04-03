/*!
 * \file   tests/Math/TinyVectorOfTinyVectorFromTinyVectorView.cxx
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
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Array/ViewsArray.hxx"

struct TinyVectorOfTinyVectorFromTinyVectorViewTest final
    : public tfel::tests::TestCase {
  TinyVectorOfTinyVectorFromTinyVectorViewTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "TinyVectorOfTinyVectorFromTinyVectorView") {
  }  // end of TinyVectorOfTinyVectorFromTinyVectorViewTest

  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    const double eps = 100 * std::numeric_limits<double>::epsilon();
    const tvector<10, double> values = {1.1, 10.1, 2.9, 9.2, 3.8,
                                        8.3, 4.7,  7.4, 5.6, 6.5};
    tvector<10, double> v(values);
    auto view =
        tfel::math::map_array<tvector<2, tvector<2u, double>>>(v.data() + 2u);
    const auto& s1 = view(0);
    auto s2 = view(1);
    tvector<2, double>::size_type i;
    for (i = 0; i != s1.size(); ++i) {
      auto idx = static_cast<unsigned short>(i + 2u);
      TFEL_TESTS_ASSERT(std::abs(s1(i) - v(idx)) < eps);
      idx = static_cast<unsigned short>(i + 4u);
      TFEL_TESTS_ASSERT(std::abs(s2(i) - v(idx)) < eps);
    }
    s2 += tvector<2, double>(1.5);
    for (i = 0; i != s1.size(); ++i) {
      const auto idx = static_cast<unsigned short>(i + 4u);
      TFEL_TESTS_ASSERT(std::abs(s2(i) - values[idx] - 1.5) < eps);
      TFEL_TESTS_ASSERT(std::abs(v(idx) - values[idx] - 1.5) < eps);
    }
    return this->result;
  }  // end of execute

};  // end of TinyVectorOfTinyVectorFromTinyVectorViewTest

TFEL_TESTS_GENERATE_PROXY(TinyVectorOfTinyVectorFromTinyVectorViewTest,
                          "TinyVectorOfTinyVectorFromTinyVectorViewTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TinyVectorOfTinyVectorFromTinyVectorView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
