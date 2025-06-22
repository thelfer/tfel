/*!
 * \file  Expr5.cxx
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

struct Expr5Test final : public tfel::tests::TestCase {
  Expr5Test()
      : tfel::tests::TestCase("TFEL/Math", "Expr5Test") {
  }  // end of Expr5Test()
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using tvector = tvector<3u, int>;
    using res = BinaryOperationResult<const tvector&, const tvector&, OpMinus>;
    using handler =
        Expr<tvector, BinaryOperation<const tvector&, const tvector&, OpMinus>>;
    using res2 = BinaryOperationResult<const tvector&, const handler&, OpPlus>;
    using handler2 =
        BinaryOperationHandler<const tvector&, const handler&, OpPlus>;
    const tvector v1{3, 4, 5};
    const tvector v2{7, 3, 2};
    const auto expr = v1 - v2;
    TFEL_TESTS_STATIC_ASSERT((is_same<res, tvector>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::lhs_type, const tvector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::lhs_storage_type, const tvector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::rhs_type, const tvector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::rhs_storage_type, const tvector&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::result_type, tvector>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<decltype(expr), const handler>::value));
    TFEL_TESTS_ASSERT(abs(expr(0) + 4) == 0);
    TFEL_TESTS_ASSERT(abs(expr(1) - 1) == 0);
    TFEL_TESTS_ASSERT(abs(expr(2) - 3) == 0);
    const tvector v3{expr};
    TFEL_TESTS_ASSERT(abs(v3(0) + 4) == 0);
    TFEL_TESTS_ASSERT(abs(v3(1) - 1) == 0);
    TFEL_TESTS_ASSERT(abs(v3(2) - 3) == 0);
    const auto expr2 = v1 + expr;
    TFEL_TESTS_STATIC_ASSERT((is_same<res2, tvector>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<tvector, handler, OpPlus>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<decltype(expr2), const handler2>::value));
    TFEL_TESTS_ASSERT(abs(expr2(0) + 1) == 0);
    TFEL_TESTS_ASSERT(abs(expr2(1) - 5) == 0);
    TFEL_TESTS_ASSERT(abs(expr2(2) - 8) == 0);
    const tvector v4{expr2};
    TFEL_TESTS_ASSERT(abs(v4(0) + 1) == 0);
    TFEL_TESTS_ASSERT(abs(v4(1) - 5) == 0);
    TFEL_TESTS_ASSERT(abs(v4(2) - 8) == 0);
    return this->result;
  }
  virtual ~Expr5Test() = default;
};

TFEL_TESTS_GENERATE_PROXY(Expr5Test, "Expr5Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Expr5.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
