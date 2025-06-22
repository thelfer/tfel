/*!
 * \file  Expr3.cxx
 * \brief
 * \author Thomas Helfer
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
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/vector.hxx"

struct Expr3Test final : public tfel::tests::TestCase {
  Expr3Test()
      : tfel::tests::TestCase("TFEL/Math", "Expr3Test") {
  }  // end of Expr3Test()
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using tfel::math::vector;
    using tvector = tvector<3u, double>;
    using stensor = stensor<3u, double>;
    using svector = vector<stensor>;
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<int, double, OpMult>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<ResultType<int, double, OpMult>::type, double>::value));
    using res2 = ResultType<int, stensor, OpMult>::type;
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<int, stensor, OpMult>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<res2, stensor>::value));
    using res3 = ResultType<int, svector, OpMult>::type;
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<int, svector, OpMult>::value));
    TFEL_TESTS_STATIC_ASSERT((is_same<res3, svector>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<VectorTraits<svector>::NumType, stensor>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<VectorTraits<svector>::NumType, stensor>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<stensor,
                 ResultType<int, svector, OpMult>::type::value_type>::value));
    using handle = ComputeBinaryResult<int, svector, OpMult>::Handle;
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<Expr<svector, ScalarObjectOperation<int, svector, OpMult>>,
                 handle>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<tvector, ResultType<tvector&, tvector, OpPlus>::A_>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<tvector,
                 ResultType<const tvector&, tvector, OpPlus>::A_>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<tvector, ResultType<const tvector&, const tvector&,
                                     OpPlus>::B_>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<tvector,
                 ComputeBinaryResult_<VectorTag, VectorTag, tvector, tvector,
                                      OpPlus>::Result>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<tvector, tvector, OpPlus>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<tvector&, tvector&, OpPlus>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<const tvector, const tvector,
                                          OpPlus>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<const tvector&, const tvector&,
                                          OpPlus>::value));

    return this->result;
  }
  ~Expr3Test() override = default;
};

TFEL_TESTS_GENERATE_PROXY(Expr3Test, "Expr3Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Expr3.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
