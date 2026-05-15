/*!
 * \file   test-stlpar.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22/04/2026
 */

#include <array>
#include <vector>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "MFront/GenericMaterialProperty/MaterialProperty.h"
#include "Inconel600_YoungModulus-generic-parallel-stlpar.hxx"

struct Iconel600YoungModulusTest : public tfel::tests::TestCase {
  Iconel600YoungModulusTest()
      : tfel::tests::TestCase("MFront/MaterialProperty/GenericParallel",
                              "Iconel600YoungModulusTest") {
  }  // end of Iconel600YoungModulusTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
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
    auto E = std::vector<double>(4);
    const auto T = std::vector<double>{300, 500, 300, 800};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.data()};
    const auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
    Inconel600_YoungModulus(&output, E.data(), 1, args.data(),
                            args_strides.data(), 1, 4, policy);
    for (std::size_t i = 0; i != E.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[i])) < eps);
    }
  }

  void test2() {
    constexpr auto eps = double{1e-2};
    auto output = mfront_gmp_OutputStatus{};
    auto E = std::vector<double>(4);
    const auto T = std::vector<double>{300, 500, 300, 800};
    const auto policy = mfront_gmp_OutOfBoundsPolicy{};
    const auto args = std::array<const double *, 1u>{T.data()};
    Inconel600_YoungModulus2(&output, E.data(), args.data(), 1, 4, policy);
    for (std::size_t i = 0; i != E.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(E[i] - fct(T[i])) < eps);
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(Iconel600YoungModulusTest, "Iconel600YoungModulusTest");

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
