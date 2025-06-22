/*!
 * \file   tests/Math/stensor_eigenvectors.cxx
 * \brief
 * \author Helfer Thomas
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
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/General/floating_point_exceptions.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"

struct StensorComputeEigenvectors final : public tfel::tests::TestCase {
  StensorComputeEigenvectors()
      : tfel::tests::TestCase("TFEL/Math", "StensorComputeEigenvectors") {
  }  // end of StensorComputeEigenvectors

  virtual tfel::tests::TestResult execute() override {
    this->test<float>();
    this->test<double>();
    this->test2<float>();
    this->test2<double>();
#if (!defined __CYGWIN__) && (!defined __FreeBSD__)
    this->test2<long double>();
#endif /* (!defined __CYGWIN__) && (!defined __FreeBSD__) */
    this->test3<float>();
    this->test3<double>();
    return this->result;
  }  // end of execute

 private:
  template <typename T>
  void test() {
    using namespace tfel::math;
    using namespace tfel::math::internals;
    using namespace std;

    T vp1;
    T vp2;
    T vp3;
    T vec1[3];
    T vec2[3];
    T vec3[3];
    tmatrix<3u, 3u, T> m(T(0));
    tvector<3u, T> vp(T(0));

    stensor<3, T> s(static_cast<T>(0.));
    s(0) = static_cast<T>(1.232);
    s(1) = static_cast<T>(2.5198);
    s(2) = static_cast<T>(0.234);
    s(3) = static_cast<T>(1.5634);
    s(4) = static_cast<T>(3.3425);
    s(5) = static_cast<T>(0.9765);

    s.computeEigenVectors(vp, m);
    vp1 = vp(0);
    vp2 = vp(1);
    vp3 = vp(2);

    vec1[0] = m(0, 0);
    vec1[1] = m(1, 0);
    vec1[2] = m(2, 0);

    vec2[0] = m(0, 1);
    vec2[1] = m(1, 1);
    vec2[2] = m(2, 1);

    vec3[0] = m(0, 2);
    vec3[1] = m(1, 2);
    vec3[2] = m(2, 2);

    TFEL_TESTS_ASSERT(abs(vp1 - static_cast<T>(4.16709379934921)) <
                      20 * numeric_limits<T>::epsilon());
    TFEL_TESTS_ASSERT(abs(vp2 + static_cast<T>(1.68923153093191)) <
                      20 * numeric_limits<T>::epsilon());
    TFEL_TESTS_ASSERT(abs(vp3 - static_cast<T>(1.50793773158270)) <
                      20 * numeric_limits<T>::epsilon());

    TFEL_TESTS_ASSERT(abs(vec1[0] - static_cast<T>(0.6208263966073649)) <
                      20 * numeric_limits<T>::epsilon());
    TFEL_TESTS_ASSERT(abs(vec1[1] - static_cast<T>(0.6185290894233862)) <
                      20 * numeric_limits<T>::epsilon());
    TFEL_TESTS_ASSERT(abs(vec1[2] - static_cast<T>(0.4816599950303030)) <
                      20 * numeric_limits<T>::epsilon());

    TFEL_TESTS_ASSERT(abs(vec2[0] + static_cast<T>(0.6378665158240716)) <
                      20 * numeric_limits<T>::epsilon());
    TFEL_TESTS_ASSERT(abs(vec2[1] - static_cast<T>(0.0413740968617118)) <
                      20 * numeric_limits<T>::epsilon());
    TFEL_TESTS_ASSERT(abs(vec2[2] - static_cast<T>(0.7690347795121735)) <
                      20 * numeric_limits<T>::epsilon());

    TFEL_TESTS_ASSERT(abs(vec3[0] - static_cast<T>(0.4557421346177839)) <
                      20 * numeric_limits<T>::epsilon());
    TFEL_TESTS_ASSERT(abs(vec3[1] + static_cast<T>(0.7846718738721010)) <
                      20 * numeric_limits<T>::epsilon());
    TFEL_TESTS_ASSERT(abs(vec3[2] - static_cast<T>(0.4202251266738716)) <
                      20 * numeric_limits<T>::epsilon());
  }

  template <typename T>
  void test2() {
    using namespace tfel::math;
    using namespace tfel::math::internals;
    const unsigned int nb_boucles = 1000000;
    const T one_half = T(1) / T(2);
    unsigned int i;
    unsigned int failure;
    unsigned int unprecised_results;

    stensor<3, T> s(T(0));
    tmatrix<3u, 3u, T> m(T(0));
    tvector<3u, T> vp(T(0));

    failure = 0;
    unprecised_results = 0;
    for (i = 0; i < nb_boucles; ++i) {
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
      if (s.computeEigenVectors(vp, m) == false) {
        failure += 1u;
      }
      if (StensorComputeEigenVectors_<3>::test(s.begin(), vp, m) == false) {
        unprecised_results += 1u;
      }
    }

    TFEL_TESTS_ASSERT(failure == 0);
    std::cout << "stat : "
              << (static_cast<double>(unprecised_results) /
                  static_cast<double>(nb_boucles))
              << std::endl;
    TFEL_TESTS_ASSERT((static_cast<double>(unprecised_results) /
                           static_cast<double>(nb_boucles) <
                       0.003));
  }

  template <typename T>
  void test3() {
    using namespace tfel::math;
    const unsigned int nb_boucles = 1000000;
    const T one_half = T(1) / T(2);
    unsigned int i;
    unsigned int failure;

    stensor<2u, T> s(T(0));
    tmatrix<3u, 3u, T> m(T(0));
    tvector<3u, T> vp(T(0));

    failure = 0;
    for (i = 0; i < nb_boucles; ++i) {
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
      if (s.computeEigenVectors(vp, m) == false) {
        failure += 1;
      }
    }
    TFEL_TESTS_ASSERT(failure == 0);
    std::cout << "stat : "
              << (static_cast<double>(failure) /
                  static_cast<double>(nb_boucles))
              << std::endl;
    TFEL_TESTS_ASSERT(
        (static_cast<double>(failure) / static_cast<double>(nb_boucles) <
         0.003));
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
