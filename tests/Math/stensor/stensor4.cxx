/*!
 * \file   tests/Math/stensor4.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date 01/02/2010
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

struct STensorTest4 final : public tfel::tests::TestCase {
  STensorTest4()
      : tfel::tests::TestCase("TFEL/Math", "STensorTest4") {
  }  // end of STensorTest4
  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    const double eps = 1.e-14;
    constexpr stensor<1> Id = stensor<1>::Id();
    constexpr st2tost2<1> IxI = st2tost2<1>::IxI();
    stensor<1> a;
    a(0) = 1.;
    a(1) = 2.;
    a(2) = 3.;
    const st2tost2<1> IdxId = Id ^ Id;
    for (unsigned short i = 0; i != 3; ++i) {
      for (unsigned short j = 0; j != 3; ++j) {
        TFEL_TESTS_ASSERT(std::abs(IdxId(i, j) - 1) < eps);
        TFEL_TESTS_ASSERT(std::abs(IxI(i, j) - 1) < eps);
      }
    }
    // a | (Id ^ Id) = tr(a) Id
    const stensor<1> b = a | (Id ^ Id);
    for (unsigned short i = 0; i != 3; ++i) {
      TFEL_TESTS_ASSERT(std::abs(b(i) - 6) < eps);
    }
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(STensorTest4, "STensorTest4");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("stensor4.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
