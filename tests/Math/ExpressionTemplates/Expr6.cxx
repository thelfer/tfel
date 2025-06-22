/*!
 * \file  Expr6.cxx
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

struct Expr6Test final : public tfel::tests::TestCase {
  Expr6Test()
      : tfel::tests::TestCase("TFEL/Math", "Expr6Test") {
  }  // end of Expr6Test()
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using tvector = tvector<3u, int>;
    using res = BinaryOperationResult<int&&, const tvector&, OpMult>;
    using handler =
        Expr<tvector, ScalarObjectOperation<int&&, const tvector&, OpMult>>;
    using res2 = BinaryOperationResult<const tvector&, int&&, OpMult>;
    using handler2 =
        Expr<tvector, ObjectScalarOperation<const tvector&, int&&, OpMult>>;
    const tvector v1{3, 4, 5};
    // scalar-vector operations
    const auto expr = 2 * v1;
    TFEL_TESTS_STATIC_ASSERT((is_same<res, tvector>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<decltype(expr)::lhs_type, int&&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::lhs_storage_type, const int>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::rhs_type, const tvector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::rhs_storage_type, const tvector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::result_type, tvector>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<decltype(expr), const handler>::value));
    TFEL_TESTS_ASSERT(abs(expr(0) - 6) == 0);
    TFEL_TESTS_ASSERT(abs(expr(1) - 8) == 0);
    TFEL_TESTS_ASSERT(abs(expr(2) - 10) == 0);
    const tvector v3{expr};
    TFEL_TESTS_ASSERT(abs(v3(0) - 6) == 0);
    TFEL_TESTS_ASSERT(abs(v3(1) - 8) == 0);
    TFEL_TESTS_ASSERT(abs(v3(2) - 10) == 0);
    // vector-scalar operations
    const auto expr2 = v1 * 2;
    TFEL_TESTS_STATIC_ASSERT((is_same<res2, tvector>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr2)::lhs_type, const tvector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr2)::lhs_storage_type, const tvector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr2)::rhs_type, int&&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr2)::rhs_storage_type, const int>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr2)::result_type, tvector>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<decltype(expr2), const handler2>::value));
    TFEL_TESTS_ASSERT(abs(expr2(0) - 6) == 0);
    TFEL_TESTS_ASSERT(abs(expr2(1) - 8) == 0);
    TFEL_TESTS_ASSERT(abs(expr2(2) - 10) == 0);
    const tvector v4{expr2};
    TFEL_TESTS_ASSERT(abs(v4(0) - 6) == 0);
    TFEL_TESTS_ASSERT(abs(v4(1) - 8) == 0);
    TFEL_TESTS_ASSERT(abs(v4(2) - 10) == 0);
    return this->result;
  }
  virtual ~Expr6Test() = default;
};

TFEL_TESTS_GENERATE_PROXY(Expr6Test, "Expr6Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Expr6.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
