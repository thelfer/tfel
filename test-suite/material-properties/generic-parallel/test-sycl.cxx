/*!
 * \file   test-sycl.cxx
 * \brief
 * \author Thomas Helfer
 * \date 19/05/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <array>
#include <cmath>
#include <memory>
#include <vector>
#include <iostream>
#include <sycl/sycl.hpp>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "MFront/GenericMaterialProperty/MaterialProperty.h"
#include "MFront/GenericParallel/SyclBackendExecutionPolicy.hxx"
#include "PoissonRatioTest-generic-parallel-sycl.hxx"
#include "BoundsOrderTest-generic-parallel-sycl.hxx"
#include "Inconel600_YoungModulus-generic-parallel-sycl.hxx"
#include "Inconel600_YoungModulus_qt-generic-parallel-sycl.hxx"
#include "UO2_YoungModulus_Martin1989-generic-parallel-sycl.hxx"
#include "LinearDataInterpolationTest-generic-parallel-sycl.hxx"

#ifdef TFEL_SYCL_DEVICE
#if TFEL_SYCL_DEVICE == 0
#define TFEL_SYCL_DEVICE_SELECTOR sycl::cpu_selector_v
#elif TFEL_SYCL_DEVICE == 1
#define TFEL_SYCL_DEVICE_SELECTOR sycl::gpu_selector_v
#else
#error "unsupported value for TFEL_SYCL_DEVICE"
#endif
#else
#define TFEL_SYCL_DEVICE_SELECTOR sycl::cpu_selector_v
#endif

//! \brief name of the backend
#define MFRONT_GPMP_TEST_BACKEND_NAME "sycl"

//! \brief array of values given to the material properties (host memory)
template <typename T>
using test_vector = std::vector<T>;

//! \return the queue of the synchronous evaluations
static sycl::queue& getTestQueue() {
  static auto q = sycl::queue{TFEL_SYCL_DEVICE_SELECTOR};
  return q;
}  // end of getTestQueue

//! \return the execution policy of the synchronous evaluations
static const mfront::generic_parallel::SyclBackendExecutionPolicy&
getTestPolicy() {
  static const auto p =
      mfront::generic_parallel::makeSyclBackendExecutionPolicy(getTestQueue());
  return p;
}  // end of getTestPolicy

#include "batch.hxx"

//! \return the execution of a synchronous evaluation
static TestExecution testExecution() {
  return TestExecution{.policy = &getTestPolicy()};
}  // end of testExecution

/*!
 * \return an execution policy of the `sycl` backend on the queue of the
 * tests
 * \param[in] o: common options
 */
static std::unique_ptr<mfront::generic_parallel::AbstractBackendExecutionPolicy>
makeTestPolicy(
    const mfront::generic_parallel::BackendExecutionCommonPolicy& o) {
  return std::make_unique<mfront::generic_parallel::SyclBackendExecutionPolicy>(
      getTestQueue(), o);
}  // end of makeTestPolicy

#include "tests.hxx"

//! \brief tests specific to the `sycl` backend
struct SYCLTest : public tfel::tests::TestCase {
  SYCLTest()
      : tfel::tests::TestCase("MFront/MaterialProperty/GenericParallel",
                              "SYCLTest") {}  // end of SYCLTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    return this->result;
  }  // end of execute

 private:
  [[nodiscard]] static double fct(const double TK) noexcept {
    const double TC = TK - 273.15;
    return (-3.1636e-3 * TC * TC - 3.8654 * TC + 2.1421e+4) * 1e7;
  }  // end of fct

  void test1() {
    // SYCL has no default queue: an execution policy holding a queue must be
    // given
    auto output = mfront_gpmp_OutputStatus{};
    auto E = std::vector<double>(4);
    const auto T = std::vector<double>{300, 500, 300, 800};
    const auto args = std::array<const double *, 1u>{T.data()};
    TFEL_TESTS_CHECK_EQUAL(
        evaluate(Inconel600_YoungModulus_generic_parallel, output, E.data(), 1,
                 args, contiguous<1>(), 4, mfront_gmp_OutOfBoundsPolicy{}),
        -5);
    TFEL_TESTS_CHECK_EQUAL(
        std::string(output.msg),
        "no execution policy given: the sycl backend requires a queue");
  }

  void test2() {
    // asynchronous evaluation: the values and the statistics are in shared
    // memory (no value is copied), and the statistics are reported once the
    // queue is synchronized. The second temperature is out of its physical
    // bounds.
    constexpr auto eps = double{1e-2};
    // an asynchronous evaluation requires an in-order queue
    auto Q = sycl::queue{TFEL_SYCL_DEVICE_SELECTOR,
                         sycl::property::queue::in_order{}};
    auto* const statistics = sycl::malloc_shared<mfront_gpmp_Statistics>(1, Q);
    auto* const T = sycl::malloc_shared<double>(4, Q);
    auto* const E = sycl::malloc_shared<double>(4, Q);
    mfront_gpmp_initializeStatistics(statistics);
    const auto values = std::array<double, 4u>{300, -1, 500, 800};
    std::copy(values.begin(), values.end(), T);
    const auto args = std::array<const double*, 1u>{T};
    const auto execution_policy =
        mfront::generic_parallel::makeSyclBackendExecutionPolicy(
            Q, {.asynchronous = true});
    const auto options =
        TestExecution{.statistics = statistics, .policy = &execution_policy};
    auto output = mfront_gpmp_OutputStatus{};
    TFEL_TESTS_CHECK_EQUAL(
        evaluate(Inconel600_YoungModulus_generic_parallel, output, E, 1, args,
                 contiguous<1>(), 4, mfront_gmp_OutOfBoundsPolicy{}, options),
        0);
    Q.wait();
    TFEL_TESTS_CHECK_EQUAL(
        Inconel600_YoungModulus_generic_parallel_report(&output, statistics),
        -1);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, -1);
    TFEL_TESTS_CHECK_EQUAL(output.number_of_failures, mfront_gmp_size_type{1});
    TFEL_TESTS_CHECK_EQUAL(output.first_failure_index, mfront_gmp_size_type{1});
    TFEL_TESTS_ASSERT(std::isnan(E[1]));
    for (const auto i : {0, 2, 3}) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(values[i])) < eps);
    }
    sycl::free(statistics, Q);
    sycl::free(T, Q);
    sycl::free(E, Q);
  }

  void test3() {
    // an asynchronous evaluation is refused with an out-of-order queue
    auto Q = sycl::queue{TFEL_SYCL_DEVICE_SELECTOR};
    TFEL_TESTS_ASSERT(!Q.is_in_order());
    auto* const statistics = sycl::malloc_shared<mfront_gpmp_Statistics>(1, Q);
    mfront_gpmp_initializeStatistics(statistics);
    auto* const T = sycl::malloc_shared<double>(4, Q);
    auto* const E = sycl::malloc_shared<double>(4, Q);
    std::fill(T, T + 4, 300.);
    const auto args = std::array<const double*, 1u>{T};
    const auto execution_policy =
        mfront::generic_parallel::makeSyclBackendExecutionPolicy(
            Q, {.asynchronous = true});
    const auto options =
        TestExecution{.statistics = statistics, .policy = &execution_policy};
    auto output = mfront_gpmp_OutputStatus{};
    TFEL_TESTS_CHECK_EQUAL(
        evaluate(Inconel600_YoungModulus_generic_parallel, output, E, 1, args,
                 contiguous<1>(), 4, mfront_gmp_OutOfBoundsPolicy{}, options),
        -5);
    sycl::free(statistics, Q);
    sycl::free(T, Q);
    sycl::free(E, Q);
  }

  void test4() {
    // asynchronous evaluation with the values and the statistics in host
    // memory. With automatic data transfers on a GPU, they are copied in
    // device memory and copied back, and the evaluation is synchronous. As
    // with the `generic` interface, the output is `NaN` at the point where
    // the evaluation fails.
    constexpr auto eps = double{1e-2};
    auto Q = sycl::queue{TFEL_SYCL_DEVICE_SELECTOR,
                         sycl::property::queue::in_order{}};
    auto statistics = std::vector<mfront_gpmp_Statistics>(1);
    mfront_gpmp_initializeStatistics(statistics.data());
    const auto T = std::vector<double>{300, -1, 500, 800};
    auto E = std::vector<double>(4, -12.);
    const auto args = std::array<const double *, 1u>{T.data()};
    const auto execution_policy =
        mfront::generic_parallel::makeSyclBackendExecutionPolicy(
            Q, {.asynchronous = true});
    const auto options = TestExecution{.statistics = statistics.data(),
                                       .policy = &execution_policy};
    auto output = mfront_gpmp_OutputStatus{};
    TFEL_TESTS_CHECK_EQUAL(evaluate(Inconel600_YoungModulus_generic_parallel,
                                    output, E.data(), 1, args, contiguous<1>(),
                                    4, mfront_gmp_OutOfBoundsPolicy{}, options),
                           0);
    Q.wait_and_throw();
    TFEL_TESTS_CHECK_EQUAL(Inconel600_YoungModulus_generic_parallel_report(
                               &output, statistics.data()),
                           -1);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, -1);
    TFEL_TESTS_CHECK_EQUAL(output.number_of_failures, mfront_gmp_size_type{1});
    TFEL_TESTS_CHECK_EQUAL(output.first_failure_index, mfront_gmp_size_type{1});
    for (const auto i : {0, 2, 3}) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[i])) < eps);
    }
    TFEL_TESTS_ASSERT(std::isnan(E[1]));
  }
};

TFEL_TESTS_GENERATE_PROXY(SYCLTest, "SYCLTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc, const char *const *argv) {
  if (argc != 2) {
    std::cerr << "no test name given on the command line (argc: " << argc
              << ")";
    return EXIT_FAILURE;
  }
  auto &m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput(std::string{argv[1]} + ".xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
