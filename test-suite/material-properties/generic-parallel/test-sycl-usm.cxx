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

// kind of the unified shared memory holding the arrays: host (`0`, by
// default) or shared (`1`)
#ifdef TFEL_SYCL_USM_ALLOCATOR
#if TFEL_SYCL_USM_ALLOCATOR == 0
static constexpr auto test_usm_kind = sycl::usm::alloc::host;
#elif TFEL_SYCL_USM_ALLOCATOR == 1
static constexpr auto test_usm_kind = sycl::usm::alloc::shared;
#else
#error "unsupported value for TFEL_SYCL_USM_ALLOCATOR"
#endif
#else
static constexpr auto test_usm_kind = sycl::usm::alloc::host;
#endif

//! \brief name of the backend
#define MFRONT_GPMP_TEST_BACKEND_NAME "sycl"

//! \return the queue of the evaluations, which gives the context of the
//! unified shared memory
static sycl::queue& getTestQueue() {
  static auto q = sycl::queue{TFEL_SYCL_DEVICE_SELECTOR};
  return q;
}  // end of getTestQueue

//! \brief allocator of unified shared memory on the queue of the tests
template <typename T>
struct TestAllocator : sycl::usm_allocator<T, test_usm_kind> {
  TestAllocator() : sycl::usm_allocator<T, test_usm_kind>(getTestQueue()) {}
  template <typename T2>
  TestAllocator(const TestAllocator<T2>&) noexcept : TestAllocator() {}
  //! \brief the allocator of `sycl::usm_allocator` is not inherited
  template <typename T2>
  struct rebind {
    using other = TestAllocator<T2>;
  };
};

//! \brief array of values given to the material properties
template <typename T>
using test_vector = std::vector<T, TestAllocator<T>>;

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
