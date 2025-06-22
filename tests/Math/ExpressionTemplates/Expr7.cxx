/*!
 * \file  Expr7.cxx
 * \brief
 * \author Helfer Thomas
 * \date   04 févr. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <array>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/vector.hxx"

struct Expr7Test final : public tfel::tests::TestCase {
  Expr7Test()
      : tfel::tests::TestCase("TFEL/Math", "Expr7Test") {
  }  // end of Expr7Test()
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using tvector = tvector<3u, int>;
    using vector = tfel::math::vector<tvector>;
    const vector v1{{3, 4, 5}, {3, 4, 5}};
    const vector v2{{7, 3, 2}, {7, 3, 2}};
    using res = BinaryOperationResult<int&&, const tvector&, OpMult>;
    using handler =
        Expr<vector, ScalarObjectOperation<int&&, const vector&, OpMult>>;
    using res2 = BinaryOperationResult<int&&, handler&, OpMult>;
    // scalar-vector operations
    const auto expr = 2 * v1;
    TFEL_TESTS_STATIC_ASSERT((is_same<res, tvector>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<decltype(expr)::lhs_type, int&&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::lhs_storage_type, const int>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::rhs_type, const vector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::rhs_storage_type, const vector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::result_type, vector>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<decltype(expr), const handler>::value));
    TFEL_TESTS_ASSERT(abs(expr(0)(0) - 6) == 0);
    TFEL_TESTS_ASSERT(abs(expr(0)(1) - 8) == 0);
    TFEL_TESTS_ASSERT(abs(expr(0)(2) - 10) == 0);
    vector v3(2u);
    v3 = expr;
    TFEL_TESTS_ASSERT(abs(v3(0)(0) - 6) == 0);
    TFEL_TESTS_ASSERT(abs(v3(0)(1) - 8) == 0);
    TFEL_TESTS_ASSERT(abs(v3(0)(2) - 10) == 0);
    // vector-scalar operations
    const auto expr2 = v1 * 2;
    TFEL_TESTS_STATIC_ASSERT((is_same<res2, vector>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr2)::lhs_type, const vector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr2)::lhs_storage_type, const vector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr2)::rhs_type, int&&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr2)::rhs_storage_type, const int>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr2)::result_type, vector>::value));
    TFEL_TESTS_ASSERT(abs(expr2(0)(0) - 6) == 0);
    TFEL_TESTS_ASSERT(abs(expr2(0)(1) - 8) == 0);
    TFEL_TESTS_ASSERT(abs(expr2(0)(2) - 10) == 0);
    const tvector v4{expr2(0)};
    TFEL_TESTS_ASSERT(abs(v4(0) - 6) == 0);
    TFEL_TESTS_ASSERT(abs(v4(1) - 8) == 0);
    TFEL_TESTS_ASSERT(abs(v4(2) - 10) == 0);
    return this->result;
  }
  virtual ~Expr7Test() = default;
};

TFEL_TESTS_GENERATE_PROXY(Expr7Test, "Expr7Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Expr7.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
