/*!
 * \file   test-hip.hip
 * \brief
 * \author Thomas Helfer
 * \date   22/04/2026
 */

#include <array>
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
#include "PoissonRatioTest-generic-parallel-hip.hxx"
#include "Inconel600_YoungModulus-generic-parallel-hip.hxx"
#include "Inconel600_YoungModulus_qt-generic-parallel-hip.hxx"

[[nodiscard]] std::unique_ptr<double, void (*)(double *const) noexcept>
createManagedData(const std::size_t s) {
  double *ptr;
  const auto e = hipMallocManaged(&ptr, s);
  if (e != hipSuccess) {
    std::cerr << hipGetErrorString(e) << '\n';
    std::exit(EXIT_FAILURE);
  }
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

struct Iconel600YoungModulusTest : public tfel::tests::TestCase {
  Iconel600YoungModulusTest()
      : tfel::tests::TestCase("MFront/MaterialProperty/GenericParallel",
                              "Iconel600YoungModulusTest") {
  }  // end of Iconel600YoungModulusTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
    this->test6();
    this->test7();
    this->test8();
    this->test9();
    this->test10();
    return this->result;
  }  // end of execute

 private:
  [[nodiscard]] static double fct(const double TK) noexcept {
    const double TC = TK - 273.15;
    return (-3.1636e-3 * TC * TC - 3.8654 * TC + 2.1421e+4) * 1e7;
  }  // end of fct

  void test1() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = createManagedData(4);
    const auto T = createManagedData({300, 500, 300, 800});
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.get()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
    Inconel600_YoungModulus(&output, E.get(), 1, args.data(),
                            args_strides.data(), 1, 4, policy);
    //
    auto E_qt = createManagedData(4);
    Inconel600_YoungModulus_qt(&output_qt, E_qt.get(), 1, args.data(),
                               args_strides.data(), 1, 4, policy);
    //
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, 0);
    for (std::size_t i = 0; i != 4; ++i) {
      TFEL_TESTS_ASSERT(std::abs(getValue(E, i) - fct(getValue(T, i))) < eps);
      TFEL_TESTS_ASSERT(std::abs(getValue(E_qt, i) - fct(getValue(T, i))) <
                        eps);
    }
  }

  void test2() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = createManagedData(4);
    auto E_qt = createManagedData(4);
    const auto T = createManagedData(300);
    //
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.get()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{0};
    //
    Inconel600_YoungModulus(&output, E.get(), 1, args.data(),
                            args_strides.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt(&output_qt, E_qt.get(), 1, args.data(),
                               args_strides.data(), 1, 4, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, 0);
    for (std::size_t i = 0; i != 4; ++i) {
      TFEL_TESTS_ASSERT(std::abs(getValue(E, i) - fct(*T)) < eps);
      TFEL_TESTS_ASSERT(std::abs(getValue(E_qt, i) - fct(*T)) < eps);
    }
  }

  void test3() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = createManagedData(1);
    auto E_qt = createManagedData(1);
    const auto T = createManagedData({300});
    //
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.get()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{0};
    Inconel600_YoungModulus(&output, E.get(), 0, args.data(),
                            args_strides.data(), 1, 1, policy);
    Inconel600_YoungModulus_qt(&output_qt, E_qt.get(), 0, args.data(),
                               args_strides.data(), 1, 1, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, 0);
    TFEL_TESTS_ASSERT(std::abs(*E - fct(*T)) < eps);
    TFEL_TESTS_ASSERT(std::abs(*E_qt - fct(*T)) < eps);
  }

  void test4() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = createManagedData(4);
    auto E_qt = createManagedData(4);
    const auto T = createManagedData({300, 500, 300, 800});
    //
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.get()};
    Inconel600_YoungModulus2(&output, E.get(), args.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt2(&output_qt, E_qt.get(), args.data(), 1, 4,
                                policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, 0);
    for (std::size_t i = 0; i != 4; ++i) {
      TFEL_TESTS_ASSERT(std::abs(getValue(E, i) - fct(getValue(T, i))) < eps);
      TFEL_TESTS_ASSERT(std::abs(getValue(E_qt, i) - fct(getValue(T, i))) <
                        eps);
    }
  }

  // check detection of physical bounds violation
  void test5() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = createManagedData(4);
    auto E_qt = createManagedData(4);
    const auto T = createManagedData({-300, 500, 300, 800});
    //
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.get()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
    Inconel600_YoungModulus(&output, E.get(), 1, args.data(),
                            args_strides.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt(&output_qt, E_qt.get(), 1, args.data(),
                               args_strides.data(), 1, 4, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, -1);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, -1);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, -1);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, -1);
    TFEL_TESTS_ASSERT(std::abs(*E) < eps);
    TFEL_TESTS_ASSERT(std::abs(*E_qt) < eps);
    for (std::size_t i = 1; i != 4; ++i) {
      TFEL_TESTS_ASSERT(std::abs(getValue(E, i) - fct(getValue(T, i))) < eps);
      TFEL_TESTS_ASSERT(std::abs(getValue(E_qt, i) - fct(getValue(T, i))) <
                        eps);
    }
  }

  void test6() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = createManagedData(4);
    auto E_qt = createManagedData(4);
    const auto T = createManagedData({-300, 500, 300, 800});
    //
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.get()};
    Inconel600_YoungModulus2(&output, E.get(), args.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt2(&output_qt, E_qt.get(), args.data(), 1, 4,
                                policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, -1);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, -1);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, -1);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, -1);
    TFEL_TESTS_ASSERT(std::abs(*E) < eps);
    TFEL_TESTS_ASSERT(std::abs(*E_qt) < eps);
    for (std::size_t i = 1; i != 4; ++i) {
      TFEL_TESTS_ASSERT(std::abs(getValue(E, i) - fct(getValue(T, i))) < eps);
      TFEL_TESTS_ASSERT(std::abs(getValue(E_qt, i) - fct(getValue(T, i))) <
                        eps);
    }
  }

  void test7() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = createManagedData(4 * 3);
    auto E_qt = createManagedData(4 * 3);
    const auto T =
        createManagedData({300, -300, 500, -500, 300, -300, 800, -800});
    //
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.get()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{2};
    Inconel600_YoungModulus(&output, E.get(), 3, args.data(),
                            args_strides.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt(&output_qt, E_qt.get(), 3, args.data(),
                               args_strides.data(), 1, 4, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, 0);
    for (std::size_t i = 0; i != 4; ++i) {
      TFEL_TESTS_ASSERT(std::abs(getValue(E, i * 3) - fct(getValue(T, i * 2))) <
                        eps);
      TFEL_TESTS_ASSERT(
          std::abs(getValue(E_qt, i * 3) - fct(getValue(T, i * 2))) < eps);
    }
  }

  void test8() {
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = createManagedData(1);
    const auto T = createManagedData({300});
    //
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.get()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
    Inconel600_YoungModulus(&output, E.get(), 1, args.data(),
                            args_strides.data(), 3, 1, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, -5);
    TFEL_TESTS_CHECK_EQUAL(std::string(output.msg),
                           "invalid number of arguments (3 given, 1 expected)");
    //
    Inconel600_YoungModulus(&output, E.get(), 0, args.data(),
                            args_strides.data(), 1, 1, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, -7);
    TFEL_TESTS_CHECK_EQUAL(std::string(output.msg),
                           "if the output is uniform, all the arguments must "
                           "be uniform (i.e. their strides must be null)");
    //
    const auto args_strides2 = std::array<mfront_gmp_size_type, 1u>{0};
    Inconel600_YoungModulus(&output, E.get(), 0, args.data(),
                            args_strides2.data(), 1, 10, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, -7);
    TFEL_TESTS_CHECK_EQUAL(std::string(output.msg),
                           "if the output is uniform, the number of points "
                           "must be equal to one (10 given)");
  }

  void test9() {
    constexpr auto eps = double{1e-14};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto nu = createManagedData(4);
    //
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    PoissonRatioTest(&output, nu.get(), 1, nullptr, nullptr, 0, 4, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    for (std::size_t i = 0; i != 4; ++i) {
      TFEL_TESTS_ASSERT(std::abs(getValue(nu, i) - 0.39991) < eps);
    }
    nu = createManagedData(1);
    PoissonRatioTest(&output, nu.get(), 0, nullptr, nullptr, 0, 1, policy);
    TFEL_TESTS_ASSERT(std::abs(*nu - 0.39991) < eps);
  }

  void test10() {
    constexpr auto eps = double{1e-14};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto nu = createManagedData(4);
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    //
    PoissonRatioTest2(&output, nu.get(), nullptr, 0, 4, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    for (std::size_t i = 0; i != 4; ++i) {
      TFEL_TESTS_ASSERT(std::abs(getValue(nu, i) - 0.39991) < eps);
    }
    nu = createManagedData(1);
    PoissonRatioTest2(&output, nu.get(), nullptr, 0, 1, policy);
    TFEL_TESTS_ASSERT(std::abs(*nu - 0.39991) < eps);
  }
};

TFEL_TESTS_GENERATE_PROXY(Iconel600YoungModulusTest,
                          "Iconel600YoungModulusTest");

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
