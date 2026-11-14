/*!
 * \file   test-hip.hip
 * \brief
 * \author Thomas Helfer
 * \date   22/04/2026
 */

#include <new>
#include <array>
#include <cmath>
#include <tuple>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <initializer_list>

#include "hip/hip_runtime.h"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "MFront/GenericMaterialProperty/MaterialProperty.h"
#include "MFront/GenericParallel/HIPBackendExecutionPolicy.hxx"
#include "PoissonRatioTest-generic-parallel-hip.hxx"
#include "BoundsOrderTest-generic-parallel-hip.hxx"
#include "Inconel600_YoungModulus-generic-parallel-hip.hxx"
#include "Inconel600_YoungModulus_qt-generic-parallel-hip.hxx"
#include "UO2_YoungModulus_Martin1989-generic-parallel-hip.hxx"
#include "LinearDataInterpolationTest-generic-parallel-hip.hxx"

[[nodiscard]] std::unique_ptr<double, void (*)(double *const) noexcept>
createManagedData(const std::size_t s) {
  double *ptr;
  const auto e = hipMallocManaged(&ptr, s * sizeof(double));
  if (e != hipSuccess) {
    std::cerr << hipGetErrorString(e) << '\n';
    std::exit(EXIT_FAILURE);
  }
  // the memory returned by hipMallocManaged is not initialized
  std::fill_n(ptr, s, 0.);
  return {ptr, +[](double *p) noexcept { std::ignore = hipFree(p); }};
}  // end of createManagedData

[[nodiscard]] std::unique_ptr<double, void (*)(double *const) noexcept>
createManagedData(const std::initializer_list<double> l) {
  auto ptr = createManagedData(l.size());
  std::copy(l.begin(), l.end(), ptr.get());
  return {std::move(ptr)};
}  // end of createManagedData

[[nodiscard]] double getValue(
    const std::unique_ptr<double, void (*)(double *const) noexcept> &ptr,
    const std::size_t i) {
  return (ptr.get())[i];
}  // end of getValue

/*!
 * \brief kernel writing the given values after a delay, so that the
 * computations submitted after it on the same stream must wait for it
 * \param[out] T: written values
 * \param[in] values: values
 * \param[in] cycles: number of clock cycles of the delay
 */
__global__ void writeAfterDelay(double* const T,
                                const double* const values,
                                const long long cycles) {
  const auto start = clock64();
  while (clock64() - start < cycles) {
  }
  T[threadIdx.x] = values[threadIdx.x];
}  // end of writeAfterDelay

//! \brief name of the backend
#define MFRONT_GPMP_TEST_BACKEND_NAME "hip"

//! \brief allocator of managed memory
template <typename T>
struct TestAllocator {
  using value_type = T;
  TestAllocator() = default;
  template <typename T2>
  TestAllocator(const TestAllocator<T2>&) noexcept {}
  T* allocate(const std::size_t n) {
    T* p = nullptr;
    if (hipMallocManaged(&p, n * sizeof(T)) != hipSuccess) {
      throw std::bad_alloc{};
    }
    return p;
  }
  void deallocate(T* const p, const std::size_t) noexcept {
    std::ignore = hipFree(p);
  }
  template <typename T2>
  bool operator==(const TestAllocator<T2>&) const noexcept {
    return true;
  }
};

//! \brief array of values given to the material properties
template <typename T>
using test_vector = std::vector<T, TestAllocator<T>>;

#include "batch.hxx"

//! \return the execution of a synchronous evaluation (default policy)
static TestExecution testExecution() {
  return TestExecution{};
}  // end of testExecution

/*!
 * \return an execution policy of the `hip` backend (default stream)
 * \param[in] o: common options
 */
static std::unique_ptr<mfront::generic_parallel::AbstractBackendExecutionPolicy>
makeTestPolicy(
    const mfront::generic_parallel::BackendExecutionCommonPolicy& o) {
  using namespace mfront::generic_parallel;
  return std::make_unique<HIPBackendExecutionPolicy>(
      HIPBackendExecutionOptions{.common_options = o});
}  // end of makeTestPolicy

#include "tests.hxx"

//! \brief tests specific to the `hip` backend
struct HIPTest : public tfel::tests::TestCase {
  HIPTest()
      : tfel::tests::TestCase("MFront/MaterialProperty/GenericParallel",
                              "HIPTest") {}  // end of HIPTest

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
    // computations on a stream given by the execution policy, which is not
    // the default stream
    constexpr auto eps = double{1e-2};
    auto stream = hipStream_t{};
    TFEL_TESTS_ASSERT(hipStreamCreateWithFlags(&stream, hipStreamNonBlocking) ==
                      hipSuccess);
    auto E = createManagedData(4);
    const auto values = createManagedData({300, 500, 300, 800});
    const auto T = createManagedData(4);
    // the temperatures are written on the stream, after a delay: the
    // material property must be evaluated after this kernel, without
    // synchronization by the host
    writeAfterDelay<<<1, 4, 0, stream>>>(T.get(), values.get(), 100'000'000);
    const auto args = std::array<const double *, 1u>{T.get()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
    const auto execution_policy =
        mfront::generic_parallel::makeHIPBackendExecutionPolicy(
            {.stream = stream});
    const auto execution = TestExecution{.policy = &execution_policy};
    auto output = mfront_gpmp_OutputStatus{};
    TFEL_TESTS_CHECK_EQUAL(evaluate(Inconel600_YoungModulus_generic_parallel,
                                    output, E.get(), 1, args, args_strides, 4,
                                    mfront_gmp_OutOfBoundsPolicy{}, execution),
                           0);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    for (std::size_t i = 0; i != 4; ++i) {
      TFEL_TESTS_ASSERT(std::abs(getValue(E, i) - fct(getValue(values, i))) <
                        eps);
    }
    TFEL_TESTS_ASSERT(hipStreamDestroy(stream) == hipSuccess);
  }

  void test2() {
    // asynchronous evaluation on a stream: the statistics, in managed memory,
    // are given by the calling code and reported once the stream is
    // synchronized. The second temperature is out of its physical bounds.
    constexpr auto eps = double{1e-2};
    auto stream = hipStream_t{};
    TFEL_TESTS_ASSERT(hipStreamCreateWithFlags(&stream, hipStreamNonBlocking) ==
                      hipSuccess);
    mfront_gpmp_Statistics* statistics = nullptr;
    TFEL_TESTS_ASSERT(
        hipMallocManaged(&statistics, sizeof(mfront_gpmp_Statistics)) ==
        hipSuccess);
    mfront_gpmp_initializeStatistics(statistics);
    auto E = createManagedData(4);
    const auto values = createManagedData({300, -1, 500, 800});
    const auto T = createManagedData(4);
    // the temperatures are written on the stream after a delay
    writeAfterDelay<<<1, 4, 0, stream>>>(T.get(), values.get(), 100'000'000);
    const auto args = std::array<const double *, 1u>{T.get()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto execution_policy =
        mfront::generic_parallel::makeHIPBackendExecutionPolicy(
            {.common_options = {.asynchronous = true}, .stream = stream});
    const auto execution =
        TestExecution{.statistics = statistics, .policy = &execution_policy};
    auto output = mfront_gpmp_OutputStatus{};
    TFEL_TESTS_CHECK_EQUAL(
        evaluate(Inconel600_YoungModulus_generic_parallel, output, E.get(), 1,
                 args, args_strides, 4, policy, execution),
        0);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    // the function returns once the computations are submitted
    TFEL_TESTS_ASSERT(hipStreamQuery(stream) == hipErrorNotReady);
    TFEL_TESTS_ASSERT(hipStreamSynchronize(stream) == hipSuccess);
    TFEL_TESTS_CHECK_EQUAL(
        Inconel600_YoungModulus_generic_parallel_report(&output, statistics),
        -1);
    TFEL_TESTS_CHECK_EQUAL(output.status, -1);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, -1);
    TFEL_TESTS_CHECK_EQUAL(output.number_of_failures, mfront_gmp_size_type{1});
    TFEL_TESTS_CHECK_EQUAL(output.first_failure_index, mfront_gmp_size_type{1});
    for (const auto i : {0, 2, 3}) {
      TFEL_TESTS_ASSERT(std::abs(getValue(E, i) - fct(getValue(values, i))) <
                        eps);
    }
    TFEL_TESTS_ASSERT(std::isnan(getValue(E, 1)));
    TFEL_TESTS_ASSERT(hipFree(statistics) == hipSuccess);
    TFEL_TESTS_ASSERT(hipStreamDestroy(stream) == hipSuccess);
  }

  void test3() {
    // stack size of the threads preserved by the execution policy: an
    // error is reported if it is too small for the kernel. Otherwise, the
    // stack size is increased if required.
    constexpr auto eps = double{1e-2};
    auto initial_stack_size = std::size_t{};
    TFEL_TESTS_ASSERT(hipDeviceGetLimit(&initial_stack_size,
                                        hipLimitStackSize) == hipSuccess);
    // the kernels require at least 1024 bytes (see
    // mfront::generic_parallel::gpu::ensureStackSize)
    TFEL_TESTS_ASSERT(hipDeviceSetLimit(hipLimitStackSize, 256) == hipSuccess);
    auto stack_size = std::size_t{};
    TFEL_TESTS_ASSERT(hipDeviceGetLimit(&stack_size, hipLimitStackSize) ==
                      hipSuccess);
    TFEL_TESTS_ASSERT(stack_size < 1024);
    auto E = createManagedData(2);
    const auto T = createManagedData({300, 500});
    const auto args = std::array<const double *, 1u>{T.get()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    auto output = mfront_gpmp_OutputStatus{};
    const auto preserve_policy = makeTestPolicy({.preserve_stack_size = true});
    const auto preserve = TestExecution{.policy = preserve_policy.get()};
    TFEL_TESTS_CHECK_EQUAL(
        evaluate(Inconel600_YoungModulus_generic_parallel, output, E.get(), 1,
                 args, args_strides, 2, policy, preserve),
        -7);
    TFEL_TESTS_CHECK_EQUAL(output.status, -7);
    TFEL_TESTS_ASSERT(std::string(output.msg)
                          .starts_with("the stack size of the threads of the "
                                       "device must be at least"));
    TFEL_TESTS_ASSERT(hipDeviceGetLimit(&stack_size, hipLimitStackSize) ==
                      hipSuccess);
    TFEL_TESTS_ASSERT(stack_size < 1024);
    // default policy: the stack size is increased
    TFEL_TESTS_CHECK_EQUAL(
        evaluate(Inconel600_YoungModulus_generic_parallel, output, E.get(), 1,
                 args, args_strides, 2, policy),
        0);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_ASSERT(hipDeviceGetLimit(&stack_size, hipLimitStackSize) ==
                      hipSuccess);
    TFEL_TESTS_ASSERT(stack_size >= 1024);
    for (std::size_t i = 0; i != 2; ++i) {
      TFEL_TESTS_ASSERT(std::abs(getValue(E, i) - fct(getValue(T, i))) < eps);
    }
    TFEL_TESTS_ASSERT(
        hipDeviceSetLimit(hipLimitStackSize, initial_stack_size) == hipSuccess);
  }

  void test4() {
    // the number of blocks of threads is checked before any computation:
    // 2^40 points need 2^33 blocks of 128 threads, more than the 2^31 - 1
    // blocks of the grid. The arrays are never read.
    auto output = mfront_gpmp_OutputStatus{};
    auto E = test_vector<double>(1, 3);
    const auto T = test_vector<double>(1, 300);
    auto statuses = test_vector<int>(1, 12);
    const auto n = mfront_gmp_size_type{1} << 40;
    const auto args = std::array<const double*, 1u>{T.data()};
    const auto r =
        evaluate(Inconel600_YoungModulus_generic_parallel, output, E.data(), 1,
                 args, contiguous<1>(), n, mfront_gmp_OutOfBoundsPolicy{},
                 testExecution(), statuses.data());
    TFEL_TESTS_CHECK_EQUAL(r, -5);
    TFEL_TESTS_CHECK_EQUAL(output.status, -5);
    TFEL_TESTS_CHECK_EQUAL(std::string(output.msg),
                           "too many points for the grid of the device (" +
                               std::to_string(n) + " given)");
    TFEL_TESTS_CHECK_EQUAL(E[0], 3.);
    TFEL_TESTS_CHECK_EQUAL(statuses[0], 12);
  }
};

TFEL_TESTS_GENERATE_PROXY(HIPTest, "HIPTest");

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
