/*!
 * \file  Expr.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19 févr. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/tensor.hxx"

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

struct Expr9Test final : public tfel::tests::TestCase {
  Expr9Test()
      : tfel::tests::TestCase("TFEL/Math", "Expr9Test") {
  }  // end of Expr9Test()
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::meta;
    using namespace tfel::math;
    using expr = Expr<t2tot2<2u, double>, TensorProductLeftDerivativeExpr<2u>>;
    TFEL_TESTS_STATIC_ASSERT((Implements<expr, T2toT2ConceptBase>::cond));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same_v<EvaluationResult<expr>, t2tot2<2u, double>>));
    TFEL_TESTS_STATIC_ASSERT(
        (MathObjectTraits<EvaluationResult<expr>>::dime == 2u));
    TFEL_TESTS_STATIC_ASSERT((getSpaceDimension<expr>() == 2u));
    using expr2 =
        BinaryOperationHandler<tensor<1u, double>&, tensor<1u, double>, OpMult>;
    tensor<1u, double> t1;
    tensor<1u, double> t2;
    expr2 e(t1, t2);
    return this->result;
  }
  ~Expr9Test() override = default;
};

TFEL_TESTS_GENERATE_PROXY(Expr9Test, "Expr9Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("ResultOf.xml");
  return manager.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
