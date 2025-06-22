/*!
 * \file  Expr8.cxx
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

#include "TFEL/Math/stensor.hxx"

struct Expr8Test final : public tfel::tests::TestCase {
  Expr8Test()
      : tfel::tests::TestCase("TFEL/Math", "Expr8Test") {
  }  // end of Expr8Test()
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::meta;
    using namespace tfel::math;
    constexpr const auto eps = 10 * numeric_limits<double>::epsilon();
    using stensor = stensor<2u, double>;
    const stensor s1{3., 4., 5., 0.};
    const stensor id = stensor::Id();
    const double tr = trace(s1);
    using res = BinaryOperationResult<int&&, const stensor&, OpMult>;
#ifndef _MSC_VER
    using handler =
        Expr<stensor, ScalarObjectOperation<double&&, const stensor&, OpMult>>;
#endif
    const auto expr = (tr / 3) * id;
    TFEL_TESTS_ASSERT(std::abs(expr(0) - 4) < eps);
    TFEL_TESTS_ASSERT(std::abs(expr(1) - 4) < eps);
    TFEL_TESTS_ASSERT(std::abs(expr(2) - 4) < eps);
    TFEL_TESTS_ASSERT(std::abs(expr(3) - 0) < eps);
    TFEL_TESTS_STATIC_ASSERT((is_same<res, stensor>::value));
#ifndef _MSC_VER
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::lhs_type, double&&>::value));
#endif
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::lhs_storage_type, const double>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::rhs_type, const stensor&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::rhs_storage_type, const stensor&>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<decltype(expr)::result_type, stensor>::value));
#ifndef _MSC_VER
    TFEL_TESTS_STATIC_ASSERT((is_same<decltype(expr), const handler>::value));
#endif
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<ComputeObjectTag<res>::type, StensorTag>::value));
#ifndef _MSC_VER
    TFEL_TESTS_STATIC_ASSERT(
        (is_base_of<StensorConcept<handler>, handler>::value));
#endif
    TFEL_TESTS_STATIC_ASSERT(
        (is_same<ConceptRebind<ComputeObjectTag<res>::type, double>::type,
                 StensorConcept<double>>::value));
#ifndef _MSC_VER
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<const stensor&, const handler&,
                                          OpMinus>()));
#endif
    const auto expr2 = s1 - expr;
    TFEL_TESTS_ASSERT(std::abs(expr2(0) + 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(expr2(1) + 0) < eps);
    TFEL_TESTS_ASSERT(std::abs(expr2(2) - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(expr2(3) - 0) < eps);
    const stensor s3{expr2};
    TFEL_TESTS_ASSERT(std::abs(s3(0) + 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(s3(1) + 0) < eps);
    TFEL_TESTS_ASSERT(std::abs(s3(2) - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(s3(3) - 0) < eps);
    return this->result;
  }
  ~Expr8Test() override = default;
};

TFEL_TESTS_GENERATE_PROXY(Expr8Test, "Expr8Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Expr8.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
