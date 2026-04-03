/*!
 * \file   cadna.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30 sept. 2015
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

#include <type_traits>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/cadna.hxx"

struct CadnaTest final : public tfel::tests::TestCase {
  //! a simple alias
  using cdouble = cadna::numeric_type<double>;
  CadnaTest()
      : tfel::tests::TestCase("TFEL/Math", "CadnaTest") {}  // end of CadnaTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace tfel::math;
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same<ResultType<cdouble, cdouble, OpDiv>::type,
                      cdouble>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same<ComputeBinaryResult<cdouble&&, cdouble&&, OpDiv>::Result,
                      cdouble>::value));
  }
  void test2() {
    using namespace tfel::math;
    const qt<Mass, cdouble> m1(100.);
    const qt<Mass, cdouble> m2(100.);
    const qt<Mass, cdouble> m3 = m1 + 0.5 * m2;
    const qt<Acceleration, cdouble> a(2);
    const qt<Force, cdouble> f = m1 * a;
    TFEL_TESTS_ASSERT(std::abs(m3.getValue() - 150.) < 1.e-14);
    TFEL_TESTS_ASSERT(std::abs(f.getValue() - 200.) < 1.e-14);
    TFEL_TESTS_ASSERT(
        (std::abs(std::cos(qt<NoUnit>(12.)) - std::cos(12.)) < 1.e-14));
  }

};  // end of struct CadnaTest

TFEL_TESTS_GENERATE_PROXY(CadnaTest, "CadnaTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& m = TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("cadna.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
