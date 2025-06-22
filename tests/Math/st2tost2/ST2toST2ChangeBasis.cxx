/*!
 * \file   ChangeBasis.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   13 oct. 2015
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

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <iostream>

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/st2tost2.hxx"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct ST2toST2ChangeBasisTest : public tfel::tests::TestCase {
  ST2toST2ChangeBasisTest()
      : tfel::tests::TestCase("TFEL/Math", "ST2toST2ChangeBasis") {
  }  // end of ST2toST2ChangeBasis

  tfel::tests::TestResult execute() {
    auto from_euler = [](const double psi, const double the, const double phi) {
      const double cospsi = std::cos(psi);
      const double costhe = std::cos(the);
      const double cosphi = std::cos(phi);
      const double sinpsi = std::sin(psi);
      const double sinthe = std::sin(the);
      const double sinphi = std::sin(phi);
      tfel::math::tmatrix<3, 3, double> r;
      r(0, 0) = cosphi * cospsi - sinphi * costhe * sinpsi;
      r(0, 1) = cosphi * sinpsi + sinphi * costhe * cospsi;
      r(0, 2) = sinphi * sinthe;
      r(1, 0) = -sinphi * cospsi - cosphi * costhe * sinpsi;
      r(1, 1) = -sinphi * sinpsi + cosphi * costhe * cospsi;
      r(1, 2) = cosphi * sinthe;
      r(2, 0) = sinthe * sinpsi;
      r(2, 1) = -sinthe * cospsi;
      r(2, 2) = costhe;
      return r;
    };
    // check that the identity does not change the tensor
    this->test<1>({1, 0, 0, 0, 1, 0, 0, 0, 1});
    this->test<2>({1, 0, 0, 0, 1, 0, 0, 0, 1});
    this->test<3>({1, 0, 0, 0, 1, 0, 0, 0, 1});
    // check that an isotropic stiffness tensor is invariant under
    // rotation
    this->test<2>(from_euler(0.23 * M_PI, 0., 0.));
    this->test<3>(from_euler(0.23 * M_PI, 0.32 * M_PI, 0.13 * M_PI));
    // real tests begins here
    this->test<1>({1, 0, 0, 0, 1, 0, 0, 0, 1},
                  {0.23, 0.32, 0.13, 0.94, 0.65, 0.87, 0.45, 0.71, 0.18},
                  {1., 2., 3.});
    this->test<2>(from_euler(0.23 * M_PI, 0., 0.),
                  {0.23, 0.32, 0.13, 0.76, 0.94, 0.65, 0.87, 0.19, 0.45, 0.71,
                   0.18, 0.82, 0.54, 0.17, 0.81, 0.28},
                  {1., 2., 3., 0.98});
    this->test<3>(from_euler(0.23 * M_PI, 0.32 * M_PI, 0.13 * M_PI),
                  {0.23, 0.32, 0.13, 0.76, 0.54, 0.17, 0.94, 0.65, 0.87,
                   0.19, 0.56, 0.78, 0.45, 0.71, 0.18, 0.82, 0.31, 0.67,
                   0.54, 0.17, 0.81, 0.28, 0.81, 0.28, 0.49, 0.56, 0.78,
                   0.91, 0.65, 0.87, 0.32, 0.23, 0.31, 0.67, 0.45, 0.71},
                  {1., 2., 3., 0.98, 0.32, 2.34});
    return this->result;
  }  // end of execute

 private:
  /*!
   * check that a rotation has no effect on an isotropic tensor
   */
  template <unsigned short N>
  void test(const tfel::math::tmatrix<3u, 3u, double>& r) {
    using st2tost2 = tfel::math::st2tost2<N, double>;
    TFEL_CONSTEXPR const double e =
        100 * std::numeric_limits<double>::epsilon();
    TFEL_CONSTEXPR const double y = 150.e9;
    TFEL_CONSTEXPR const double n = 0.3;
    TFEL_CONSTEXPR const double l = n * y / ((1 + n) * (1 - 2 * n));
    TFEL_CONSTEXPR const double m = y / (2 * (1 + n));
    const st2tost2 D = l * st2tost2::IxI() + m * st2tost2::Id();
    const st2tost2 D2 = change_basis(D, r);
    auto p = D.begin();
    auto p2 = D2.begin();
    for (; p2 != D2.end(); ++p2, ++p) {
      TFEL_TESTS_ASSERT(std::abs(*p - *p2) < y * e);
    }
  }
  /*!
   * check that st2tost2 rotation works appropriately
   * \param[in] r : rotation matrix
   * \param[in] e : strain
   */
  template <unsigned short N>
  void test(const tfel::math::tmatrix<3u, 3u, double>& r,
            const tfel::math::st2tost2<N, double>& D,
            const tfel::math::stensor<N, double>& e) {
    using stensor = tfel::math::stensor<N, double>;
    TFEL_CONSTEXPR const double eps =
        100 * std::numeric_limits<double>::epsilon();
    // first computation of the stress
    const stensor ns = change_basis(D * e, r);
    // second computation of the stress
    const auto ne = change_basis(e, r);
    const auto nD = change_basis(D, r);
    const stensor ns2 = nD * ne;
    auto p = ns.begin();
    auto p2 = ns2.begin();
    for (; p2 != ns2.end(); ++p, ++p2) {
      TFEL_TESTS_ASSERT(std::abs(*p - *p2) < eps);
    }
  }
};  // end of ST2toST2ChangeBasisTest

TFEL_TESTS_GENERATE_PROXY(ST2toST2ChangeBasisTest, "ST2toST2ChangeBasisTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("ST2toST2ChangeBasis.xml");
  TestResult r = manager.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
