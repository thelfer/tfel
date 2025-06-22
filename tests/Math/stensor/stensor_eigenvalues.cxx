/*!
 * \file   tests/Math/stensor_eigenvalues.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   03 jui 2006
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
#include <limits>
#include <cassert>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/Vector/tvectorIO.hxx"
#include "TFEL/Math/stensor.hxx"

struct StensorComputeEigenValues final : public tfel::tests::TestCase {
  StensorComputeEigenValues()
      : tfel::tests::TestCase("TFEL/Math", "StensorComputeEigenValues") {
  }  // end of StensorComputeEigenValues

  tfel::tests::TestResult execute() override {
    using EigenSolver = tfel::math::stensor_common::EigenSolver;
    this->test1<float>();
    this->test1<double>();
    this->test2<float, EigenSolver::TFELEIGENSOLVER>();
    this->test2<double, EigenSolver::TFELEIGENSOLVER>();
    this->test2<float, EigenSolver::GTESYMMETRICQREIGENSOLVER>();
    this->test2<double, EigenSolver::GTESYMMETRICQREIGENSOLVER>();
    return this->result;
  }  // end of execute

 private:
  template <typename T>
  void test1() {
    using namespace tfel::math;
    TFEL_CONSTEXPR const auto eps = std::numeric_limits<T>::epsilon();
    constexpr const auto icste = Cste<T>::isqrt2;
    constexpr const auto zero = T(0);
    const stensor<3u, T> s(zero);
    const auto vp = s.computeEigenValues();
    auto vp2 = tvector<3u, T>{};
    auto m2 = tmatrix<3u, 3u, T>{};
    internals::GteSymmetricEigensolver3x3<T>::exe(vp2, m2, s[0], s[3] * icste,
                                                  s[4] * icste, s[1],
                                                  s[5] * icste, s[2], false);
    TFEL_TESTS_ASSERT(std::abs(vp(0) - zero) < eps);
    TFEL_TESTS_ASSERT(std::abs(vp(1) - zero) < eps);
    TFEL_TESTS_ASSERT(std::abs(vp(2) - zero) < eps);
    TFEL_TESTS_ASSERT(std::abs(vp2(0) - zero) < eps);
    TFEL_TESTS_ASSERT(std::abs(vp2(1) - zero) < eps);
    TFEL_TESTS_ASSERT(std::abs(vp2(2) - zero) < eps);
  }
  template <typename T, tfel::math::stensor_common::EigenSolver es>
  void test2() {
    using stensor = tfel::math::stensor<3, T>;
    TFEL_CONSTEXPR const auto prec = 20 * std::numeric_limits<T>::epsilon();
    stensor s = {T(1.232),  T(2.5198), T(0.234),
                 T(1.5634), T(3.3425), T(0.9765)};
    const auto vp = s.template computeEigenValues<es>(stensor::DESCENDING);
    const auto vp2 = s.template computeEigenValues<es>(stensor::ASCENDING);
    const auto vp3 = s.template computeEigenValues<es>();
    TFEL_TESTS_ASSERT(std::abs(vp(0) - T(4.16709379934921)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp(1) - T(1.50793773158270)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp(2) + T(1.68923153093191)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp2(0) + T(1.68923153093191)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp2(1) - T(1.50793773158270)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp2(2) - T(4.16709379934921)) < prec);
    TFEL_TESTS_ASSERT((std::abs(vp3(0) + T(1.68923153093191)) < prec) ||
                      (std::abs(vp3(1) + T(1.68923153093191)) < prec) ||
                      (std::abs(vp3(2) + T(1.68923153093191)) < prec));
    TFEL_TESTS_ASSERT((std::abs(vp3(0) - T(1.50793773158270)) < prec) ||
                      (std::abs(vp3(1) - T(1.50793773158270)) < prec) ||
                      (std::abs(vp3(2) - T(1.50793773158270)) < prec));
    TFEL_TESTS_ASSERT((std::abs(vp3(0) - T(4.16709379934921)) < prec) ||
                      (std::abs(vp3(1) - T(4.16709379934921)) < prec) ||
                      (std::abs(vp3(2) - T(4.16709379934921)) < prec));
  }
};

TFEL_TESTS_GENERATE_PROXY(StensorComputeEigenValues,
                          "StensorComputeEigenValues");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& m = TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StensorComputeEigenValues.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
