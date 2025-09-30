/*!
 * \file   tests/Math/stensor_eigenvectors.cxx
 * \brief
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
#include <chrono>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/General/floating_point_exceptions.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Vector/tvectorIO.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Matrix/tmatrixIO.hxx"
#include "TFEL/Math/stensor.hxx"

struct StensorComputeEigenvectors final : public tfel::tests::TestCase {
  //! a simple alias
  using EigenSolver = tfel::math::stensor_common::EigenSolver;

  StensorComputeEigenvectors()
      : tfel::tests::TestCase("TFEL/Math", "StensorComputeEigenvectors") {
  }  // end of StensorComputeEigenvectors

  tfel::tests::TestResult execute() override {
    this->test<float>();
    this->test<double>();
    this->check<EigenSolver::TFELEIGENSOLVER>();
    this->check<EigenSolver::GTESYMMETRICQREIGENSOLVER>();
    this->check<EigenSolver::FSESJACOBIEIGENSOLVER>();
    this->check<EigenSolver::FSESQLEIGENSOLVER>();
    // this->check<EigenSolver::FSESCUPPENEIGENSOLVER>();
    // this->check<EigenSolver::FSESHYBRIDEIGENSOLVER>();
    // this->check<EigenSolver::FSESANALYTICALEIGENSOLVER>();
    this->check<EigenSolver::HARARIEIGENSOLVER>();
    return this->result;
  }  // end of execute

 private:
  template <EigenSolver es>
  void check() {
    this->test2<float, es>();
    this->test2<double, es>();
    // on FreeBSD 11.1, powl seems broken: the linker emits a
    // warning about its precision
#if (!defined __CYGWIN__) && (!defined __FreeBSD__)
    this->test2<long double, es>();
#endif /* (!defined __CYGWIN__) && (!defined __FreeBSD__) */
    this->test3<float, es>();
    this->test3<double, es>();
  }  // end of check

  template <typename T, typename ColumnView>
  void check_eigenvector(const ColumnView& v,
                         const tfel::math::tvector<3u, T>& v2,
                         const T prec) {
    tfel::math::tvector<3u, T> a = {v[0] + v2[0], v[1] + v2[1], v[2] + v2[2]};
    tfel::math::tvector<3u, T> d = {v[0] - v2[0], v[1] - v2[1], v[2] - v2[2]};
    TFEL_TESTS_ASSERT((norm(a) < prec) || (norm(d) < prec));
  }

  template <typename T>
  static T check_eigenvectors(const tfel::math::stensor<2u, T>& s,
                              const tfel::math::tvector<3u, T>& vp,
                              const tfel::math::tmatrix<3u, 3u, T>& m) {
    constexpr auto icste = tfel::math::Cste<T>::isqrt2;
    // first eigenvalue
    auto y0 = s[0] * m(0, 0) + s[3] * icste * m(1, 0) - vp(0) * m(0, 0);
    auto y1 = s[1] * m(1, 0) + s[3] * icste * m(0, 0) - vp(0) * m(1, 0);
    const auto n0 = std::sqrt(y0 * y0 + y1 * y1);
    // second eigenvalue
    y0 = s[0] * m(0, 1) + s[3] * icste * m(1, 1) - vp(1) * m(0, 1);
    y1 = s[1] * m(1, 1) + s[3] * icste * m(0, 1) - vp(1) * m(1, 1);
    const auto n1 = std::sqrt(y0 * y0 + y1 * y1);
    return std::max(n0, n1);
  }

  template <typename T>
  static T check_eigenvectors(const tfel::math::stensor<3u, T>& s,
                              const tfel::math::tvector<3u, T>& vp,
                              const tfel::math::tmatrix<3u, 3u, T>& m) {
    constexpr auto icste = tfel::math::Cste<T>::isqrt2;
    // first eigenvalue
    auto y0 = s[0] * m(0, 0) + s[3] * icste * m(1, 0) + s[4] * icste * m(2, 0) -
              vp(0) * m(0, 0);
    auto y1 = s[1] * m(1, 0) + s[3] * icste * m(0, 0) + s[5] * icste * m(2, 0) -
              vp(0) * m(1, 0);
    auto y2 = s[2] * m(2, 0) + s[4] * icste * m(0, 0) + s[5] * icste * m(1, 0) -
              vp(0) * m(2, 0);
    const auto n0 = std::sqrt(y0 * y0 + y1 * y1 + y2 * y2);
    // second eigenvalue
    y0 = s[0] * m(0, 1) + s[3] * icste * m(1, 1) + s[4] * icste * m(2, 1) -
         vp(1) * m(0, 1);
    y1 = s[1] * m(1, 1) + s[3] * icste * m(0, 1) + s[5] * icste * m(2, 1) -
         vp(1) * m(1, 1);
    y2 = s[2] * m(2, 1) + s[4] * icste * m(0, 1) + s[5] * icste * m(1, 1) -
         vp(1) * m(2, 1);
    const auto n1 = std::sqrt(y0 * y0 + y1 * y1 + y2 * y2);
    // third eigenvalue
    y0 = s[0] * m(0, 2) + s[3] * icste * m(1, 2) + s[4] * icste * m(2, 2) -
         vp(2) * m(0, 2);
    y1 = s[1] * m(1, 2) + s[3] * icste * m(0, 2) + s[5] * icste * m(2, 2) -
         vp(2) * m(1, 2);
    y2 = s[2] * m(2, 2) + s[4] * icste * m(0, 2) + s[5] * icste * m(1, 2) -
         vp(2) * m(2, 2);
    const auto n2 = std::sqrt(y0 * y0 + y1 * y1 + y2 * y2);
    return std::max(n0, std::max(n1, n2));
  }

  template <typename T>
  void test() {
    using tvector = tfel::math::tvector<3, T>;
    using stensor = tfel::math::stensor<3, T>;
    constexpr const auto prec = 20 * std::numeric_limits<T>::epsilon();
    stensor s = {T(1.232),  T(2.5198), T(0.234),
                 T(1.5634), T(3.3425), T(0.9765)};
    const tvector v0 = {-T(0.6378665158240716), T(0.0413740968617118),
                        T(0.7690347795121735)};
    const tvector v1 = {T(0.4557421346177839), -T(0.7846718738721010),
                        T(0.4202251266738716)};
    const tvector v2 = {T(0.6208263966073649), T(0.6185290894233862),
                        T(0.4816599950303030)};
    tfel::math::tmatrix<3u, 3u, T> m;
    tfel::math::tvector<3u, T> vp;
    s.computeEigenVectors(vp, m);
    TFEL_TESTS_ASSERT(std::abs(vp(0) - T(4.16709379934921)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp(1) + T(1.68923153093191)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp(2) - T(1.50793773158270)) < prec);
    check_eigenvector(m.template column_view<0>(), v2, prec);
    check_eigenvector(m.template column_view<1>(), v0, prec);
    check_eigenvector(m.template column_view<2>(), v1, prec);
    // using ascending ordering
    tfel::math::tmatrix<3u, 3u, T> m2;
    tfel::math::tvector<3u, T> vp2;
    s.computeEigenVectors(vp2, m2, stensor::ASCENDING);
    TFEL_TESTS_ASSERT(std::abs(vp2(0) + T(1.68923153093191)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp2(1) - T(1.50793773158270)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp2(2) - T(4.16709379934921)) < prec);
    check_eigenvector(m2.template column_view<0>(), v0, prec);
    check_eigenvector(m2.template column_view<1>(), v1, prec);
    check_eigenvector(m2.template column_view<2>(), v2, prec);
    // // using descending ordering
    tfel::math::tmatrix<3u, 3u, T> m3;
    tfel::math::tvector<3u, T> vp3;
    s.computeEigenVectors(vp3, m3, stensor::DESCENDING);
    TFEL_TESTS_ASSERT(std::abs(vp3(0) - T(4.16709379934921)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp3(1) - T(1.50793773158270)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp3(2) + T(1.68923153093191)) < prec);
    check_eigenvector(m3.template column_view<2>(), v0, prec);
    check_eigenvector(m3.template column_view<1>(), v1, prec);
    check_eigenvector(m3.template column_view<0>(), v2, prec);
    // iterative eigen solver, ascending ordering
    tfel::math::tmatrix<3u, 3u, T> m4;
    tfel::math::tvector<3u, T> vp4;
    s.template computeEigenVectors<stensor::GTESYMMETRICQREIGENSOLVER>(
        vp4, m4, stensor::ASCENDING);
    TFEL_TESTS_ASSERT(std::abs(vp4(0) + T(1.68923153093191)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp4(1) - T(1.50793773158270)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp4(2) - T(4.16709379934921)) < prec);
    check_eigenvector(m4.template column_view<0>(), v0, prec);
    check_eigenvector(m4.template column_view<1>(), v1, prec);
    check_eigenvector(m4.template column_view<2>(), v2, prec);
    // iterative eigen solver, descding ordering
    this->template test1b<T, stensor::GTESYMMETRICQREIGENSOLVER>(s);
    // non iterative eigen solver, descding ordering
    this->template test1b<T, stensor::FSESANALYTICALEIGENSOLVER>(s);
    // iterative eigen solver, descding ordering
    this->template test1b<T, stensor::FSESJACOBIEIGENSOLVER>(s);
    // iterative eigen solver, descding ordering
    this->template test1b<T, stensor::FSESQLEIGENSOLVER>(s);
    // iterative eigen solver, descding ordering
    this->template test1b<T, stensor::FSESCUPPENEIGENSOLVER>(s);
    // hybrid eigen solver, descding ordering
    this->template test1b<T, stensor::FSESHYBRIDEIGENSOLVER>(s);
  }

  template <typename T, EigenSolver es>
  void test1b(const tfel::math::stensor<3u, T>& s) {
    using tvector = tfel::math::tvector<3, T>;
    using stensor = tfel::math::stensor<3, T>;
    const tvector v0 = {-T(0.6378665158240716), T(0.0413740968617118),
                        T(0.7690347795121735)};
    const tvector v1 = {T(0.4557421346177839), -T(0.7846718738721010),
                        T(0.4202251266738716)};
    const tvector v2 = {T(0.6208263966073649), T(0.6185290894233862),
                        T(0.4816599950303030)};
    constexpr const auto prec = 20 * std::numeric_limits<T>::epsilon();
    tfel::math::tmatrix<3u, 3u, T> m;
    tfel::math::tvector<3u, T> vp;
    s.template computeEigenVectors<stensor::GTESYMMETRICQREIGENSOLVER>(
        vp, m, stensor::DESCENDING);
    TFEL_TESTS_ASSERT(std::abs(vp(0) - T(4.16709379934921)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp(1) - T(1.50793773158270)) < prec);
    TFEL_TESTS_ASSERT(std::abs(vp(2) + T(1.68923153093191)) < prec);
    check_eigenvector(m.template column_view<2>(), v0, prec);
    check_eigenvector(m.template column_view<1>(), v1, prec);
    check_eigenvector(m.template column_view<0>(), v2, prec);
  }

  template <typename T, EigenSolver es>
  void test2() {
    using namespace std::chrono;
    const unsigned int nb_boucles = 1000000;
    constexpr const auto one_half = 1 / T(2);
    constexpr const auto prec = 10 * std::numeric_limits<T>::epsilon();
    tfel::math::stensor<3, T> s(T(0));
    tfel::math::tmatrix<3u, 3u, T> m(T(0));
    tfel::math::tvector<3u, T> vp(T(0));
    auto nsec = nanoseconds{};
    unsigned int unprecised_results = 0;
    auto e = T(0);
    for (unsigned int i = 0; i < nb_boucles; ++i) {
      s(0) = static_cast<T>(
          (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - one_half) *
          2);
      s(1) = static_cast<T>(
          (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - one_half) *
          2);
      s(2) = static_cast<T>(
          (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - one_half) *
          2);
      s(3) = static_cast<T>(
          (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - one_half) *
          2);
      s(4) = static_cast<T>(
          (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - one_half) *
          2);
      s(5) = static_cast<T>(
          (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - one_half) *
          2);
      const auto start = high_resolution_clock::now();
      s.template computeEigenVectors<es>(vp, m);
      const auto stop = high_resolution_clock::now();
      nsec += duration_cast<nanoseconds>(stop - start);
      const auto d = check_eigenvectors(s, vp, m);
      e = std::max(d, e);
      if (d > prec) {
        unprecised_results += 1u;
      }
    }
    std::cout << "stat : " << (double(unprecised_results) / double(nb_boucles))
              << " " << e << " " << nsec.count() << std::endl;
    TFEL_TESTS_ASSERT(
        (double(unprecised_results) / double(nb_boucles) < 0.003));
  }

  template <typename T, EigenSolver es>
  void test3() {
    const unsigned int nb_boucles = 1000000;
    constexpr const auto one_half = T(1) / T(2);
    constexpr const auto prec = 10 * std::numeric_limits<T>::epsilon();
    tfel::math::stensor<2u, T> s(T(0));
    tfel::math::tmatrix<3u, 3u, T> m(T(0));
    tfel::math::tvector<3u, T> vp(T(0));
    unsigned int unprecised_results = 0;
    for (unsigned int i = 0; i < nb_boucles; ++i) {
      s(0) = static_cast<T>(
          (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - one_half) *
          2);
      s(1) = static_cast<T>(
          (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - one_half) *
          2);
      s(2) = static_cast<T>(
          (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - one_half) *
          2);
      s(3) = static_cast<T>(
          (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - one_half) *
          2);
      s.template computeEigenVectors<es>(vp, m);
      const auto d = check_eigenvectors(s, vp, m);
      if (d > prec) {
        std::cout << "d: " << d << std::endl;
        unprecised_results += 1u;
      }
    }
    TFEL_TESTS_ASSERT(
        (double(unprecised_results) / double(nb_boucles) < 0.003));
  }
};

TFEL_TESTS_GENERATE_PROXY(StensorComputeEigenvectors,
                          "StensorComputeEigenvectors");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("StensorComputeEigenvectors.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
