/*!
 * \file   tests/Math/st2tost2/st2tost2_components.cxx
 * \brief
 *
 * \author Antoine Martin
 * \date   6 june 2025
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
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
#include <cassert>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/stensor.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct ST2ToST2TestComponents final : public tfel::tests::TestCase {
  ST2ToST2TestComponents()
      : tfel::tests::TestCase("TFEL/Math", "ST2ToST2TestComponents") {
  }  // end of ST2ToST2TestComponents
  tfel::tests::TestResult execute() override {
    using stress =
        typename tfel::config::Types<1u, double, true>::stress;
    static constexpr auto eps = std::numeric_limits<double>::epsilon();
    auto seps=stress(eps);    
    using namespace tfel::math;
    const double sqrt2 = sqrt(2.);
    const auto J2=tfel::math::st2tost2<2u,double>::J();
    const auto K2=tfel::math::st2tost2<2u,double>::K();
    auto M2=K2;
    M2(1,3)=double(6);
    st2tost2<2u, stress> A = stress(2)*J2+stress(3)*K2+stress(1)*M2;
    auto A1111=getComponent(A,0,0,0,0);
    auto A1212=getComponent(A,0,1,0,1);
    auto A2212=getComponent(A,1,1,0,1);
    TFEL_TESTS_ASSERT(my_abs(A1111 - A(0,0)) < seps);
    TFEL_TESTS_ASSERT(my_abs(A1212 - A(3,3)/2) < 10*seps);
    TFEL_TESTS_ASSERT(my_abs(A2212 - A(1,3)/sqrt2) < 10*seps);
    setComponent<stress,stress>(A,1,0,0,1,stress(1e9));
    TFEL_TESTS_ASSERT(my_abs(stress(1e9) - A(3,3)/2) < 1e9*seps);
    setComponent<stress,stress>(A,1,1,0,1,stress(1e9));
    TFEL_TESTS_ASSERT(my_abs(stress(1e9) - A(1,3)/sqrt2) < 1e9*seps);
    TFEL_TESTS_ASSERT(my_abs(stress(0) - A(3,1)/sqrt2) < seps);
    
    const auto J=tfel::math::st2tost2<3u,double>::J();
    const auto K=tfel::math::st2tost2<3u,double>::K();
    auto M=K;
    M(1,4)=double(6);
    st2tost2<3u, stress> C = stress(2)*J+stress(3)*K+stress(1)*M;
    auto C1111=getComponent(C,0,0,0,0);
    auto C1212=getComponent(C,0,1,0,1);
    auto C2323=getComponent(C,1,2,1,2);
    auto C2213=getComponent(C,1,1,0,2);
    TFEL_TESTS_ASSERT(my_abs(C1111 - C(0,0)) < seps);
    TFEL_TESTS_ASSERT(my_abs(C1212 - C(3,3)/2) < 10*seps);
    TFEL_TESTS_ASSERT(my_abs(C2323 - C(5,5)/2) < 5*seps);
    TFEL_TESTS_ASSERT(my_abs(C2213 - C(1,4)/sqrt2) < 10*seps);
    setComponent<stress,stress>(C,1,0,1,0,stress(1e9));
    TFEL_TESTS_ASSERT(my_abs(stress(1e9) - C(3,3)/2) < 1e9*seps);
    setComponent<stress,stress>(C,1,1,0,1,stress(1e9));
    TFEL_TESTS_ASSERT(my_abs(stress(1e9) - C(1,3)/sqrt2) < 1e9*seps);
    TFEL_TESTS_ASSERT(my_abs(stress(0) - C(3,1)/sqrt2) < seps);
    
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ST2ToST2TestComponents, "ST2ToST2TestComponents");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("st2tost2Components.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
