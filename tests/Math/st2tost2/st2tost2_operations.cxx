/*!
 * \file   tests/Math/st2tost2/st2tost2_operations.cxx
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

struct ST2ToST2TestOperations final : public tfel::tests::TestCase {
  ST2ToST2TestOperations()
      : tfel::tests::TestCase("TFEL/Math", "ST2ToST2TestOperations") {
  }  // end of ST2ToST2TestOperations
  tfel::tests::TestResult execute() override {
    using stress =
        typename tfel::config::Types<1u, double, true>::stress;
    static constexpr auto eps = std::numeric_limits<double>::epsilon();
    auto seps=stress(eps);    
    using namespace tfel::math;

    const auto J2=tfel::math::st2tost2<2u,double>::J();
    const auto K2=tfel::math::st2tost2<2u,double>::K();
    auto M2=K2;
    M2(1,3)=double(6);
    auto A = stress(2)*J2+stress(3)*K2+stress(1)*M2;
    auto trA=trace(A);
    auto quad1=stress(quaddot(A,M2));
    auto quad2=quaddot(A,J2);
    auto n1=norm(J2);
    
    TFEL_TESTS_ASSERT(my_abs(trA - stress(14)) < seps);
    TFEL_TESTS_ASSERT(my_abs(quad1 - stress(12)) < seps);
    TFEL_TESTS_ASSERT(my_abs(quad2 - stress(2)) < 10*seps);
    TFEL_TESTS_ASSERT(my_abs(n1 - double(0.5)) < eps);

    
    const auto J=tfel::math::st2tost2<3u,double>::J();
    const auto K=tfel::math::st2tost2<3u,double>::K();
    auto M=K;
    M(1,4)=double(6);
    auto C = stress(2)*J+stress(3)*K+stress(1)*M;
    auto trC=trace(C);
    auto quad3=quaddot(C,M);
    auto quad4=quaddot(C,J);
    auto n2=norm(J);
    
    TFEL_TESTS_ASSERT(my_abs(trC - stress(22)) < seps);
    TFEL_TESTS_ASSERT(my_abs(quad3 - stress(20)) < seps);
    TFEL_TESTS_ASSERT(my_abs(quad4 - stress(2)) < 10*seps);
    TFEL_TESTS_ASSERT(my_abs(n2 - double(1/sqrt(6))) < eps);
    
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ST2ToST2TestOperations, "ST2ToST2TestOperations");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("st2tost2Operations.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
