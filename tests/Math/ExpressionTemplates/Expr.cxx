/*!
 * \file  Expr.cxx
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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <array>
#include <cmath>

#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"

namespace tfel {

  namespace math {

    struct MyVectorTag {};

    template <typename T>
    struct ConceptRebind<MyVectorTag, T> {
      struct Dummy {};
      using type = Dummy;
    };

    struct MyVector {
      using ConceptTag = MyVectorTag;
      using value_type = int;
      using size_type = unsigned short;
      constexpr value_type operator()(const size_type i) const {
        return this->values[i];
      }
      value_type values[3];
    };

    template <typename A, typename B>
    struct ComputeBinaryResult_<MyVectorTag, MyVectorTag, A, B, OpPlus> {
      using AType = typename std::decay<A>::type;
      using BType = typename std::decay<B>::type;
      using Result = typename ResultType<AType, BType, OpPlus>::type;
      using Handle = Expr<Result, BinaryOperation<A, B, OpPlus>>;
    };

    template <typename A, typename B>
    struct ResultType_<MyVectorTag, MyVectorTag, A, B, OpPlus> {
      using type = MyVector;
    };

    MyVector& getMyVector();

    template <int N>
    struct structify {
      static constexpr int value = N;
    };

  }  // end of namespace math

}  // end of namespace tfel

struct ExprTest final : public tfel::tests::TestCase {
  ExprTest()
      : tfel::tests::TestCase("TFEL/Math", "ExprTest") {}  // end of ExprTest()
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using handler =
        decltype(std::declval<MyVector>() + std::declval<MyVector>());
    using handler2 = decltype(std::declval<MyVector>() + getMyVector());
    using handler3 = decltype(getMyVector() + std::declval<MyVector>());
    using handler4 = decltype(getMyVector() + getMyVector());
    TFEL_TESTS_STATIC_ASSERT(
        tfel::math::internals::HasConceptTag<MyVector>::cond);
    TFEL_TESTS_STATIC_ASSERT(
        (isBinaryOperationResultTypeValid<MyVector, MyVector, OpPlus>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same<typename ResultType<MyVector, MyVector, OpPlus>::type,
                      MyVector>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same<BinaryOperationResult<MyVector, MyVector, OpPlus>,
                      MyVector>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same<BinaryOperationHandler<MyVector, MyVector, OpPlus>,
                      Expr<MyVector, BinaryOperation<MyVector, MyVector,
                                                     OpPlus>>>::value));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<typename handler::lhs_storage_type,
                                           const MyVector>::value));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<typename handler::rhs_storage_type,
                                           const MyVector>::value));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<typename handler2::lhs_storage_type,
                                           const MyVector>::value));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<typename handler2::rhs_storage_type,
                                           const MyVector&>::value));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<typename handler3::lhs_storage_type,
                                           const MyVector&>::value));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<typename handler3::rhs_storage_type,
                                           const MyVector>::value));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<typename handler4::lhs_storage_type,
                                           const MyVector&>::value));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<typename handler4::rhs_storage_type,
                                           const MyVector&>::value));
    constexpr MyVector a{{0, 2, 0}};
    constexpr MyVector b{{4, 0, 1}};
    auto c = a + b;
    TFEL_TESTS_ASSERT(c(0u) == 4);
    TFEL_TESTS_ASSERT(c(1u) == 2);
    TFEL_TESTS_ASSERT(c(2u) == 1);
    return this->result;
  }
  ~ExprTest() override = default;
};

TFEL_TESTS_GENERATE_PROXY(ExprTest, "ExprTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Expr.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
