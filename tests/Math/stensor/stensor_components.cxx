/*!
 * \file   tests/Math/stensor_components.cxx
 * \brief
 * \author Antoine Martin
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

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/Matrix/tmatrixIO.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct STensorTestComponents final : public tfel::tests::TestCase {
  STensorTestComponents()
      : tfel::tests::TestCase("TFEL/Math", "STensorTestComponents") {
  }  // end of STensorTestComponents
  tfel::tests::TestResult execute() override {
    using stress = typename tfel::config::Types<1u, double, true>::stress;
    static constexpr auto eps = std::numeric_limits<double>::epsilon();
    auto seps = stress(eps);
    using namespace tfel::math;
    const double sqrt2 = sqrt(2.);
    stensor<2u, stress> s = {stress(8.2), stress(4.5), stress(7.2),
                             stress(2.3 * sqrt2)};
    auto s11 = getComponent(s, 0, 0);
    auto s12 = getComponent(s, 0, 1);
    TFEL_TESTS_ASSERT(my_abs(s11 - s(0)) < seps);
    TFEL_TESTS_ASSERT(my_abs(s12 - s(3) / sqrt2) < seps);

    setComponent<stress, stress>(s, 1, 0, stress(1e9));
    TFEL_TESTS_ASSERT(my_abs(stress(1e9) - s(3) / sqrt2) < 1e9 * seps);
    stensor<3u, stress> sig = {stress(8.2),         stress(4.5),
                               stress(7.2),         stress(2.3 * sqrt2),
                               stress(1.3 * sqrt2), stress(5.3 * sqrt2)};
    auto sig11 = getComponent(sig, 0, 0);
    auto sig12 = getComponent(sig, 0, 1);
    auto sig23 = getComponent(sig, 1, 2);
    TFEL_TESTS_ASSERT(my_abs(sig11 - sig(0)) < seps);
    TFEL_TESTS_ASSERT(my_abs(sig12 - sig(3) / sqrt2) < seps);
    TFEL_TESTS_ASSERT(my_abs(sig23 - sig(5) / sqrt2) < 5 * seps);
    setComponent<stress, stress>(sig, 1, 0, stress(1e9));
    TFEL_TESTS_ASSERT(my_abs(stress(1e9) - sig(3) / sqrt2) < 1e9 * seps);

    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(STensorTestComponents, "STensorTestComponents");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("stensorComponents.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
