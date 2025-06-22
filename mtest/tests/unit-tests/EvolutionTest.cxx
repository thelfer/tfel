/*!
 * \file   EvolutionTest.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   21 févr. 2016
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
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "MTest/Evolution.hxx"

struct EvolutionTestUnitTest final : public tfel::tests::TestCase {
  EvolutionTestUnitTest()
      : tfel::tests::TestCase("Test", "EvolutionTestUnitTest") {
  }  // end of EvolutionTestUnitTest

  tfel::tests::TestResult execute() override {
    auto cev = mtest::make_evolution(12.);
    auto lev = mtest::make_evolution({{0., 2.}, {1., 3.}, {2., 4.}});
    auto lev2 = mtest::make_evolution({{0., 1.}});
    TFEL_TESTS_ASSERT(cev->isConstant());
    TFEL_TESTS_ASSERT(!lev->isConstant());
    TFEL_TESTS_ASSERT(std::abs(cev->operator()(0.) - 12.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(lev->operator()(-1) - 2.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(lev->operator()(0.) - 2.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(lev->operator()(0.5) - 2.5) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(lev->operator()(1.) - 3.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(lev->operator()(1.5) - 3.5) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(lev->operator()(2.) - 4.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(lev->operator()(3.) - 4.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(lev2->operator()(0.) - 1.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(lev2->operator()(0.5) - 1.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(lev2->operator()(1.) - 1.) < 1.e-14);
    TFEL_TESTS_CHECK_THROW(lev->setValue(4.), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(cev->setValue(2., 4.), std::runtime_error);
    TFEL_TESTS_CHECK_THROW(mtest::LPIEvolution({}, {})(2.), std::runtime_error);
    return this->result;
  }  // end of execute()
  //! destructor
  ~EvolutionTestUnitTest() = default;
};

TFEL_TESTS_GENERATE_PROXY(EvolutionTestUnitTest, "EvolutionTestUnitTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("EvolutionTest.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
