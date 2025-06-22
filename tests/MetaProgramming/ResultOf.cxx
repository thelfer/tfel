/*!
 * \file   ResultOf.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <cstdlib>
#include <iostream>
#include <type_traits>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Metaprogramming/ResultOf.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

struct F {
  double operator()(const double) const;
  int operator()(const double, const double) const;
};

struct ResultOfTest final : public tfel::tests::TestCase {
  ResultOfTest()
      : tfel::tests::TestCase("TFEL/Math", "ResultOfTest") {
  }  // end of ResultOfTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::meta;
    using namespace tfel::math;
    using my_vector = tvector<1u, double>;
    using handler1 = BinaryOperationHandler<my_vector, double, OpMult>;
    using handler2 =
        BinaryOperationHandler<my_vector, handler1, OpDiadicProduct>;
    TFEL_TESTS_ASSERT((std::is_same<double, ResultOf<F, double>::type>::value));
    TFEL_TESTS_ASSERT(
        (std::is_same<int, ResultOf<F, double, double>::type>::value));
    TFEL_TESTS_ASSERT(
        (std::is_same<double&,
                      ResultOf<my_vector, unsigned short>::type>::value));
    TFEL_TESTS_ASSERT(
        (std::is_same<const double&,
                      ResultOf<const my_vector, unsigned short>::type>::value));
    TFEL_TESTS_ASSERT((
        std::is_same<double, ResultOf<handler1, unsigned short>::type>::value));
    TFEL_TESTS_ASSERT(
        (std::is_same<double, ResultOf<handler2, unsigned short,
                                       unsigned short>::type>::value));
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ResultOfTest, "ResultOfTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("ResultOf.xml");
  return manager.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
