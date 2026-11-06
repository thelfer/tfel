/*!
 * \file   test-cxx.cxx
 * \brief
 * \author Thomas Helfer
 * \date 22/04/2026
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
#include <string>
#include <string_view>
#include "MFront/GenericParallel/ParallelSTLBackendExecutionPolicy.hxx"

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "MFront/GenericMaterialProperty/MaterialProperty.h"

// This test is shared by the backends based on the C++ programming model,
// selected by the `MFRONT_GPMP_TEST_BACKEND` macro (`stlpar` by default).
// The arrays are allocated by `std::vector` (on the host, or in managed
// memory with `nvc++ -gpu=mem:managed`).
#ifndef MFRONT_GPMP_TEST_BACKEND
#define MFRONT_GPMP_TEST_BACKEND stlpar
#endif /* MFRONT_GPMP_TEST_BACKEND */
#define MFRONT_GPMP_TEST_STRINGIFY2(x) #x
#define MFRONT_GPMP_TEST_STRINGIFY(x) MFRONT_GPMP_TEST_STRINGIFY2(x)
// clang-format off: the spaces would appear in the name of the header
#define MFRONT_GPMP_TEST_HEADER(name) \
  MFRONT_GPMP_TEST_STRINGIFY(name-generic-parallel-MFRONT_GPMP_TEST_BACKEND.hxx)
// clang-format on

#include MFRONT_GPMP_TEST_HEADER(PoissonRatioTest)
#include MFRONT_GPMP_TEST_HEADER(BoundsOrderTest)
#include MFRONT_GPMP_TEST_HEADER(Inconel600_YoungModulus)
#include MFRONT_GPMP_TEST_HEADER(Inconel600_YoungModulus_qt)
#include MFRONT_GPMP_TEST_HEADER(UO2_YoungModulus_Martin1989)
#include MFRONT_GPMP_TEST_HEADER(LinearDataInterpolationTest)

template <typename T>
using TestAllocator = std::allocator<T>;

//! \brief array of values given to the material properties
template <typename T>
using test_vector = std::vector<T, TestAllocator<T>>;

//! \brief name of the backend
#define MFRONT_GPMP_TEST_BACKEND_NAME \
  MFRONT_GPMP_TEST_STRINGIFY(MFRONT_GPMP_TEST_BACKEND)

#include "batch.hxx"

//! \return the execution of a synchronous evaluation: default policy
static TestExecution testExecution() {
  return TestExecution{};
}  // end of testExecution

/*!
 * \return an execution policy of the tested backend
 * \param[in] o: common options
 */
static std::unique_ptr<mfront::generic_parallel::AbstractBackendExecutionPolicy>
makeTestPolicy(
    const mfront::generic_parallel::BackendExecutionCommonPolicy& o) {
  using namespace mfront::generic_parallel;
  return std::make_unique<ParallelSTLBackendExecutionPolicy>(o);
}  // end of makeTestPolicy

#include "tests.hxx"

//! \brief tests specific to the backends based on the C++ programming model
struct BackendTest : public tfel::tests::TestCase {
  BackendTest()
      : tfel::tests::TestCase("MFront/MaterialProperty/GenericParallel",
                              "BackendTest") {}  // end of BackendTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute

 private:
  [[nodiscard]] static double fct(const double TK) noexcept {
    const double TC = TK - 273.15;
    return (-3.1636e-3 * TC * TC - 3.8654 * TC + 2.1421e+4) * 1e7;
  }  // end of fct

  void test1() {
    // execution policy given explicitly
    constexpr auto eps = double{1e-2};
    const auto execution_policy_holder = makeTestPolicy({});
    const auto& execution_policy = *execution_policy_holder;
    const auto execution = TestExecution{.policy = &execution_policy};
    auto output = mfront_gpmp_OutputStatus{};
    auto E = test_vector<double>(4);
    const auto T = test_vector<double>{300, 500, 300, 800};
    const auto args = std::array<const double*, 1u>{T.data()};
    const auto r = evaluate(Inconel600_YoungModulus_generic_parallel, output,
                            E.data(), 1, args, contiguous<1>(), 4,
                            mfront_gmp_OutOfBoundsPolicy{}, execution);
    TFEL_TESTS_CHECK_EQUAL(r, 0);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    for (std::size_t i = 0; i != E.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[i])) < eps);
    }
  }

  void test2() {
    // asynchronous evaluation: the statistics are given by the calling code,
    // the output status only reports the errors of the batch, and the result
    // is given by the report function once the computations are completed
    // (the backends computing on the host compute synchronously). The second
    // temperature is out of its physical bounds.
    constexpr auto eps = double{1e-2};
    auto statistics = test_vector<mfront_gpmp_Statistics>(1);
    mfront_gpmp_initializeStatistics(statistics.data());
    const auto execution_policy_holder = makeTestPolicy({.asynchronous = true});
    const auto& execution_policy = *execution_policy_holder;
    const auto execution = TestExecution{.statistics = statistics.data(),
                                         .policy = &execution_policy};
    auto output = mfront_gpmp_OutputStatus{};
    auto E = test_vector<double>(4);
    const auto T = test_vector<double>{300, -1, 500, 800};
    const auto args = std::array<const double*, 1u>{T.data()};
    const auto r = evaluate(Inconel600_YoungModulus_generic_parallel, output,
                            E.data(), 1, args, contiguous<1>(), 4,
                            mfront_gmp_OutOfBoundsPolicy{}, execution);
    TFEL_TESTS_CHECK_EQUAL(r, 0);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    const auto r2 = Inconel600_YoungModulus_generic_parallel_report(
        &output, statistics.data());
    TFEL_TESTS_CHECK_EQUAL(r2, -1);
    TFEL_TESTS_CHECK_EQUAL(output.status, -1);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, -1);
    TFEL_TESTS_CHECK_EQUAL(output.number_of_failures, mfront_gmp_size_type{1});
    TFEL_TESTS_CHECK_EQUAL(output.first_failure_index, mfront_gmp_size_type{1});
    TFEL_TESTS_ASSERT(std::isnan(E[1]));
    for (const auto i : {0, 2, 3}) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[i])) < eps);
    }
  }

  void test3() {
    // stack size of the threads of the device preserved by the execution
    // policy. On the host, the option has no effect.
    constexpr auto eps = double{1e-2};
    auto output = mfront_gpmp_OutputStatus{};
    auto E = test_vector<double>(2);
    const auto T = test_vector<double>{300, 500};
    const auto args = std::array<const double*, 1u>{T.data()};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto preserve_policy = makeTestPolicy({.preserve_stack_size = true});
    const auto preserve = TestExecution{.policy = preserve_policy.get()};
    const auto f = Inconel600_YoungModulus_generic_parallel;
    const auto r = evaluate(f, output, E.data(), 1, args, contiguous<1>(), 2,
                            policy, preserve);
    TFEL_TESTS_CHECK_EQUAL(r, 0);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    for (std::size_t i = 0; i != E.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[i])) < eps);
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(BackendTest, "BackendTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc, const char* const* argv) {
  if (argc != 2) {
    std::cerr << "no test name given on the command line (argc: " << argc
              << ")";
    return EXIT_FAILURE;
  }
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput(std::string{argv[1]} + ".xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
