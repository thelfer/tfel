/*!
 * \file   test-sycl.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19/05/2026
 */

#include <array>
#include <vector>
#include <iostream>
#include <sycl/sycl.hpp>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "MFront/GenericMaterialProperty/MaterialProperty.h"
#include "PoissonRatioTest-generic-parallel-sycl.hxx"
#include "Inconel600_YoungModulus-generic-parallel-sycl.hxx"
#include "Inconel600_YoungModulus_qt-generic-parallel-sycl.hxx"

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
    auto E = std::vector<double>(4);
    auto E_qt = std::vector<double>(4);
    const auto T = std::vector<double>{300, 500, 300, 800};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.data()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
    auto Q = sycl::queue{sycl::cpu_selector_v};
    Inconel600_YoungModulus(&output, E.data(), 1, &Q, args.data(),
                            args_strides.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt(&output_qt, E_qt.data(), 1, &Q, args.data(),
                               args_strides.data(), 1, 4, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, 0);
    for (std::size_t i = 0; i != E.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[i])) < eps);
      TFEL_TESTS_ASSERT(std::abs(E_qt[i] - fct(T[i])) < eps);
    }
  }

  void test2() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = std::vector<double>(4);
    auto E_qt = std::vector<double>(4);
    const auto T = std::vector<double>{300};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.data()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{0};
    auto Q = sycl::queue{sycl::cpu_selector_v};
    Inconel600_YoungModulus(&output, E.data(), 1, &Q, args.data(),
			    args_strides.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt(&output_qt, E_qt.data(), 1, &Q, args.data(),
			       args_strides.data(), 1, 4, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, 0);
    for (std::size_t i = 0; i != E.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[0])) < eps);
      TFEL_TESTS_ASSERT(std::abs(E_qt[i] - fct(T[0])) < eps);
    }
  }
  
  void test3() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = std::vector<double>(1);
    auto E_qt = std::vector<double>(1);
    const auto T = std::vector<double>{300};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.data()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{0};
    auto Q = sycl::queue{sycl::cpu_selector_v};
    Inconel600_YoungModulus(&output, E.data(), 0, &Q, args.data(),
			    args_strides.data(), 1, 1, policy);
    Inconel600_YoungModulus_qt(&output_qt, E_qt.data(), 0, &Q, args.data(),
			       args_strides.data(), 1, 1, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, 0);
    TFEL_TESTS_ASSERT(std::abs(E[0] - fct(T[0])) < eps);
    TFEL_TESTS_ASSERT(std::abs(E_qt[0] - fct(T[0])) < eps);
  }
  
  void test4() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = std::vector<double>(4);
    auto E_qt = std::vector<double>(4);
    const auto T = std::vector<double>{300, 500, 300, 800};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.data()};
    auto Q = sycl::queue{sycl::cpu_selector_v};
    Inconel600_YoungModulus2(&output, E.data(), &Q, args.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt2(&output_qt, E_qt.data(), &Q, args.data(), 1, 4,
				policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, 0);
    for (std::size_t i = 0; i != E.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[i])) < eps);
      TFEL_TESTS_ASSERT(std::abs(E_qt[i] - fct(T[i])) < eps);
    }
  }
  // check detection of physical bounds violation
  void test5() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = std::vector<double>(4);
    auto E_qt = std::vector<double>(4);
    const auto T = std::vector<double>{-300, 500, 300, 800};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.data()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
    auto Q = sycl::queue{sycl::cpu_selector_v};
    Inconel600_YoungModulus(&output, E.data(), 1, &Q, args.data(),
			    args_strides.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt(&output_qt, E_qt.data(), 1, &Q, args.data(),
			       args_strides.data(), 1, 4, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, -1);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, -1);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, -1);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, -1);
    TFEL_TESTS_ASSERT(std::abs(E[0]) < eps);
    TFEL_TESTS_ASSERT(std::abs(E_qt[0]) < eps);
    for (std::size_t i = 1; i != E.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[i])) < eps);
      TFEL_TESTS_ASSERT(std::abs(E_qt[i] - fct(T[i])) < eps);
    }
  }
  
  void test6() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = std::vector<double>(4);
    auto E_qt = std::vector<double>(4);
    const auto T = std::vector<double>{-300, 500, 300, 800};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.data()};
    auto Q = sycl::queue{sycl::cpu_selector_v};
    Inconel600_YoungModulus2(&output, E.data(), &Q, args.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt2(&output_qt, E_qt.data(), &Q, args.data(), 1, 4,
				policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, -1);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, -1);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, -1);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, -1);
    TFEL_TESTS_ASSERT(std::abs(E[0]) < eps);
    TFEL_TESTS_ASSERT(std::abs(E_qt[0]) < eps);
    for (std::size_t i = 1; i != E.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[i])) < eps);
      TFEL_TESTS_ASSERT(std::abs(E_qt[i] - fct(T[i])) < eps);
    }
  }
  
  void test7() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = std::vector<double>(4 * 3);
    auto E_qt = std::vector<double>(4 * 3);
    const auto T =
      std::vector<double>{300, -300, 500, -500, 300, -300, 800, -800};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.data()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{2};
    auto Q = sycl::queue{sycl::cpu_selector_v};
    Inconel600_YoungModulus(&output, E.data(), 3, &Q, args.data(),
			    args_strides.data(), 1, 4, policy);
    Inconel600_YoungModulus_qt(&output_qt, E_qt.data(), 3, &Q, args.data(),
			       args_strides.data(), 1, 4, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output_qt.bounds_status, 0);
    for (std::size_t i = 0; i != 4; ++i) {
      TFEL_TESTS_ASSERT(std::abs(E[i * 3] - fct(T[i * 2])) < eps);
      TFEL_TESTS_ASSERT(std::abs(E_qt[i * 3] - fct(T[i * 2])) < eps);
    }
  }
  
  void test8() {
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto E = std::vector<double>(1);
    const auto T = std::vector<double>{300};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.data()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
    auto Q = sycl::queue{sycl::cpu_selector_v};
    Inconel600_YoungModulus(&output, E.data(), 1, &Q, args.data(),
			    args_strides.data(), 3, 1, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, -5);
    TFEL_TESTS_CHECK_EQUAL(std::string(output.msg),
			   "invalid number of arguments (3 given, 1 expected)");
    //
    Inconel600_YoungModulus(&output, E.data(), 0, &Q, args.data(),
			    args_strides.data(), 1, 1, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, -7);
    TFEL_TESTS_CHECK_EQUAL(std::string(output.msg),
			   "if the output is uniform, all the arguments must " "be uniform (i.e. their strides must be null)");
    //
    const auto args_strides2 = std::array<mfront_gmp_size_type, 1u>{0};
    Inconel600_YoungModulus(&output, E.data(), 0, &Q, args.data(),
			    args_strides2.data(), 1, 10, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, -7);
    TFEL_TESTS_CHECK_EQUAL(std::string(output.msg),
			   "if the output is uniform, the number of points " "must be equal to one (10 given)");
  }
  
  void test9() {
    constexpr auto eps = double{1e-14};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto nu = std::vector<double>(4);
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    auto Q = sycl::queue{sycl::cpu_selector_v};
    PoissonRatioTest(&output, nu.data(), 1, &Q, nullptr, nullptr, 0, 4,
		     policy); TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    for (std::size_t i = 0; i != nu.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(nu[i] - 0.39991) < eps);
    }
    nu.resize(1);
    nu.at(0) = 0;
    PoissonRatioTest(&output, nu.data(), 0, &Q, nullptr, nullptr, 0, 1,
		     policy);
    TFEL_TESTS_ASSERT(std::abs(nu[0] - 0.39991) < eps);
  }
  
  void test10() {
    constexpr auto eps = double{1e-14};
    auto output = mfront_gmp_OutputStatus{};
    auto output_qt = mfront_gmp_OutputStatus{};
    auto nu = std::vector<double>(4);
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    auto Q = sycl::queue{sycl::cpu_selector_v};
    PoissonRatioTest2(&output, nu.data(), &Q, nullptr, 0, 4, policy);
    TFEL_TESTS_CHECK_EQUAL(output.status, 0);
    TFEL_TESTS_CHECK_EQUAL(output.c_error_number, 0);
    TFEL_TESTS_CHECK_EQUAL(output.bounds_status, 0);
    for (std::size_t i = 0; i != nu.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(nu[i] - 0.39991) < eps);
    }
    nu.resize(1);
    nu.at(0) = 0;
    PoissonRatioTest2(&output, nu.data(), &Q, nullptr, 0, 1, policy);
    TFEL_TESTS_ASSERT(std::abs(nu[0] - 0.39991) < eps);
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
