/*!
 * \file  Expr2.cxx
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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <array>
#include <cmath>

#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/tvector.hxx"

struct Expr2Test final : public tfel::tests::TestCase {
  Expr2Test()
      : tfel::tests::TestCase("TFEL/Math", "Expr2Test") {
  }  // end of Expr2Test()
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using vector = tvector<3u, double>;
    using res = BinaryOperationResult<vector, vector, OpPlus>;
    using handler = BinaryOperationHandler<vector, vector, OpPlus>;
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<vector, vector, OpPlus>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<res, vector>::value));
    TFEL_TESTS_STATIC_ASSERT((
        is_same<handler,
                Expr<vector, BinaryOperation<vector, vector, OpPlus>>>::value));
    using res2 = BinaryOperationResult<vector, res, OpPlus>;
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<vector, res, OpPlus>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<res2, vector>::value));

    return this->result;
  }
  virtual ~Expr2Test() = default;
};

TFEL_TESTS_GENERATE_PROXY(Expr2Test, "Expr2Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Expr2.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
