/*!
 * \file   tests/Math/UninitializedFixedSizeArrayTest.cxx
 * \brief  tests of the constructor of the fixed size arrays which leaves
 * their values uninitialized
 * \date   21/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/fsarray.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tot2.hxx"
#include "TFEL/Math/Array/FixedSizeArrayDerivative.hxx"

namespace tfel::math::tests {

  //! \brief concept satisfied if `{}` can be converted to the given type
  template <typename Type>
  concept ConvertibleFromEmptyBraces = requires(void (&f)(Type)) {
    f({});
  };

  /*!
   * \return if the call of the given function object is a constant
   * expression
   */
  template <typename Function, int = (Function{}(), 0)>
  constexpr bool isConstantExpression(Function) {
    return true;
  }
  constexpr bool isConstantExpression(...) { return false; }

  /*!
   * \return if the given values are equal, without comparing floating-point
   * numbers with `==`
   */
  template <typename ValueType>
  constexpr bool areEqual(const ValueType& a, const ValueType& b) {
    return (!(a < b)) && (!(b < a));
  }  // end of areEqual

  //! \return if all the values of the given array are null
  template <typename ArrayType>
  constexpr bool isNull(const ArrayType& a) {
    for (const auto& v : a) {
      if (!areEqual(v, typename ArrayType::value_type{})) {
        return false;
      }
    }
    return true;
  }  // end of isNull

  /*!
   * \return if an array built by the constructor leaving its values
   * uninitialized and then filled with the given value only holds this value.
   */
  template <typename ArrayType>
  constexpr bool isFilled(const numeric_type<ArrayType> value) {
    ArrayType a1(uninitialized);
    ArrayType a2{uninitialized};
    a1.fill(value);
    a2.fill(value);
    for (typename ArrayType::size_type i = 0; i != a1.size(); ++i) {
      if ((!areEqual(a1.data()[i], value)) ||
          (!areEqual(a2.data()[i], value))) {
        return false;
      }
    }
    return true;
  }  // end of isFilled

  //! \brief a data member left uninitialized by a default member initializer
  struct Workspace {
    tmatrix<2u, 2u, double> m{uninitialized};
  };

}  // end of namespace tfel::math::tests

struct UninitializedFixedSizeArrayTest final : public tfel::tests::TestCase {
  UninitializedFixedSizeArrayTest()
      : tfel::tests::TestCase("TFEL/Math", "UninitializedFixedSizeArrayTest") {
  }  // end of UninitializedFixedSizeArrayTest
  tfel::tests::TestResult execute() override {
    this->check<tfel::math::fsarray<3u, double>>();
    this->check<tfel::math::tvector<3u, double>>();
    this->check<tfel::math::tmatrix<2u, 3u, double>>();
    this->check<tfel::math::stensor<1u, double>>();
    this->check<tfel::math::stensor<2u, double>>();
    this->check<tfel::math::stensor<3u, double>>();
    this->check<tfel::math::tensor<3u, double>>();
    this->check<tfel::math::st2tost2<3u, double>>();
    this->check<tfel::math::t2tot2<3u, double>>();
    this->check<tfel::math::t2tost2<3u, double>>();
    this->check<tfel::math::st2tot2<3u, double>>();
    this->check<tfel::math::stensor<3u, float>>();
    this->check<tfel::math::FixedSizeArrayDerivative<
        tfel::math::tmatrix<2u, 2u, double>,
        tfel::math::stensor<3u, double>>>();
    this->checkTag();
    this->checkQuantities();
    this->checkDataMember();
    return this->result;
  }  // end of execute

 private:
  template <typename ArrayType>
  void check() {
    using namespace tfel::math;
    using namespace tfel::math::tests;
    // the constructor is explicit and does not throw
    static_assert(std::is_constructible_v<ArrayType, UninitializedTag>);
    static_assert(std::is_nothrow_constructible_v<ArrayType, UninitializedTag>);
    static_assert(!std::is_convertible_v<UninitializedTag, ArrayType>);
    // the default constructor still sets the values to zero
    TFEL_TESTS_STATIC_ASSERT(isNull(ArrayType()));
    TFEL_TESTS_STATIC_ASSERT(isNull(ArrayType{}));
    TFEL_TESTS_STATIC_ASSERT(
        isConstantExpression([] { return ArrayType().data()[0]; }));
    // the values are really left uninitialized: reading one of them is not a
    // constant expression
    TFEL_TESTS_STATIC_ASSERT(!isConstantExpression(
        [] { return ArrayType(uninitialized).data()[0]; }));
    // the values can be assigned, in constant expressions and at runtime
    using value_type = numeric_type<ArrayType>;
    TFEL_TESTS_STATIC_ASSERT(isFilled<ArrayType>(value_type{2}));
    TFEL_TESTS_ASSERT(isFilled<ArrayType>(value_type{3}));
  }  // end of check
  void checkTag() {
    using namespace tfel::math;
    using namespace tfel::math::tests;
    static_assert(
        std::is_same_v<decltype(uninitialized), const UninitializedTag>);
    static_assert(std::is_empty_v<UninitializedTag>);
    static_assert(std::is_trivially_copyable_v<UninitializedTag>);
    // `{}` is not converted to a tag, nor to an uninitialized array
    static_assert(!ConvertibleFromEmptyBraces<UninitializedTag>);
    static_assert(ConvertibleFromEmptyBraces<tvector<3u, double>>);
    constexpr tvector<3u, double> v = {};
    TFEL_TESTS_STATIC_ASSERT(isNull(v));
  }  // end of checkTag
  void checkQuantities() {
    using namespace tfel::math;
    using namespace tfel::math::tests;
    using stress = qt<unit::Stress, double>;
    // quantities are built by their default constructors
    TFEL_TESTS_STATIC_ASSERT(isNull(stensor<3u, stress>(uninitialized)));
    TFEL_TESTS_STATIC_ASSERT(isNull(tvector<2u, stress>{uninitialized}));
    // as the values of nested arrays
    constexpr fsarray<2u, stensor<2u, double>> a(uninitialized);
    TFEL_TESTS_STATIC_ASSERT(isNull(a[0]) && isNull(a[1]));
  }  // end of checkQuantities
  void checkDataMember() {
    using namespace tfel::math;
    using namespace tfel::math::tests;
    constexpr auto m = [] {
      Workspace w;
      w.m = tmatrix<2u, 2u, double>::Id();
      return w.m;
    }();
    TFEL_TESTS_STATIC_ASSERT(areEqual(m(0, 0), 1.) && areEqual(m(0, 1), 0.));
    TFEL_TESTS_STATIC_ASSERT(areEqual(m(1, 0), 0.) && areEqual(m(1, 1), 1.));
    TFEL_TESTS_STATIC_ASSERT(!isConstantExpression([] {
      Workspace w;
      return w.m(0, 0);
    }));
  }  // end of checkDataMember
};

TFEL_TESTS_GENERATE_PROXY(UninitializedFixedSizeArrayTest,
                          "UninitializedFixedSizeArrayTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("UninitializedFixedSizeArray.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
