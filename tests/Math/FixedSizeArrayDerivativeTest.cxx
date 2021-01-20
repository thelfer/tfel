/*!
 * \file   FixedSizeArrayDerivativeTest.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17/01/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */


#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Array/FixedSizeArrayDerivative.hxx"

struct MyStensor4
    : tfel::math::FixedSizeArrayDerivative<tfel::math::stensor<2u, double>,
                                           tfel::math::stensor<2u, double>> {
  //! a simple alias
  using DerivativeArrayBase =
      tfel::math::FixedSizeArrayDerivative<tfel::math::stensor<2u, double>,
                                           tfel::math::stensor<2u, double>>;
  // inheriting constructors
  using FixedSizeArrayDerivative<
      tfel::math::stensor<2u, double>,
      tfel::math::stensor<2u, double>>::FixedSizeArrayDerivative;
  // inherting access operator
  using DerivativeArrayBase::operator();
  using DerivativeArrayBase::operator[];
  // inherting assignement operator
  using DerivativeArrayBase::operator=;
};

struct FSDerivativeArrayTest final : public tfel::tests::TestCase {
  FSDerivativeArrayTest()
      : tfel::tests::TestCase("TFEL/Math", "FSDerivativeArrayTest") {
  }  // end of FSDerivativeArrayTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute
private:
 void test1() {
    using namespace tfel::math;
    constexpr auto eps = double{1e-14};
    using Stensor6 =
        FixedSizeArrayDerivative<st2tost2<2u, double>, stensor<2u, double>>;
    constexpr Stensor6 d(1);
    TFEL_TESTS_STATIC_ASSERT(Stensor6::arity == 3);
    TFEL_TESTS_STATIC_ASSERT(d.size() == 64);
    TFEL_TESTS_STATIC_ASSERT(d.size(0) == 4);
    TFEL_TESTS_STATIC_ASSERT(d.size(1) == 4);
    TFEL_TESTS_STATIC_ASSERT(d.size(2) == 4);
    TFEL_TESTS_STATIC_ASSERT(std::abs(d(0, 0, 0) - 1) < eps);
 }
 void test2() {
   constexpr auto eps = double{1e-14};
   constexpr MyStensor4 d(2);
   TFEL_TESTS_STATIC_ASSERT(MyStensor4::arity == 2);
   TFEL_TESTS_STATIC_ASSERT(d.size() == 16);
   TFEL_TESTS_STATIC_ASSERT(d.size(0) == 4);
   TFEL_TESTS_STATIC_ASSERT(d.size(1) == 4);
   TFEL_TESTS_ASSERT(std::abs(d(0, 0) - 2) < eps);
 }
};

TFEL_TESTS_GENERATE_PROXY(FSDerivativeArrayTest, "FSDerivativeArrayTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("fixedsizederivativearray.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}

