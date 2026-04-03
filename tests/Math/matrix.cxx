/*!
 * \file   tests/Math/matrix.cxx
 * \author Thomas Helfer
 * \date   29/04/2024
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

struct MatrixTest final : public tfel::tests::TestCase {
  MatrixTest()
      : tfel::tests::TestCase("TFEL/Math", "MatrixTest") {
  }  // end of STensorTest8
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace tfel::math;
    const matrix<int> m = {{0, 1}, {2, 3}};
    TFEL_TESTS_ASSERT(m.getNbRows() == 2);
    TFEL_TESTS_ASSERT(m.getNbCols() == 2);
    TFEL_TESTS_ASSERT(m(0, 0) == 0);
    TFEL_TESTS_ASSERT(m(0, 1) == 1);
    TFEL_TESTS_ASSERT(m(1, 0) == 2);
    TFEL_TESTS_ASSERT(m(1, 1) == 3);
  }
  void test2() {
    using namespace tfel::math;
    matrix<int> m(2, 2);
    m = {{0, 1}, {2, 3}};
    TFEL_TESTS_ASSERT(m.getNbRows() == 2);
    TFEL_TESTS_ASSERT(m.getNbCols() == 2);
    TFEL_TESTS_ASSERT(m(0, 0) == 0);
    TFEL_TESTS_ASSERT(m(0, 1) == 1);
    TFEL_TESTS_ASSERT(m(1, 0) == 2);
    TFEL_TESTS_ASSERT(m(1, 1) == 3);
  }
  void test3() {
    using namespace tfel::math;
#ifdef __NVCOMPILER
    const auto m = matrix<int>{{0, 1}, { 2, 3 }};
#else  /* __NVCOMPILER */
    const auto m = matrix{{0, 1},  //
                          {2, 3}};
#endif /* __NVCOMPILER */
    TFEL_TESTS_ASSERT(m.getNbRows() == 2);
    TFEL_TESTS_ASSERT(m.getNbCols() == 2);
    TFEL_TESTS_ASSERT(m(0, 0) == 0);
    TFEL_TESTS_ASSERT(m(0, 1) == 1);
    TFEL_TESTS_ASSERT(m(1, 0) == 2);
    TFEL_TESTS_ASSERT(m(1, 1) == 3);
  }
};

TFEL_TESTS_GENERATE_PROXY(MatrixTest, "MatrixTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("matrix.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
