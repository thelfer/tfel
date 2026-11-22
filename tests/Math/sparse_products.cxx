/*!
 * \file  tests/Math/sparse_products.cxx
 * \brief This file checks that the products of fourth order tensors which
 * do not compute the terms that are null by construction (derivatives of the
 * product of two tensors and of the Cauchy-Green tensors) give the results
 * of the standard products
 * \author Tristan Chenaille
 * \date 22/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <limits>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <type_traits>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tot2.hxx"
#include "TFEL/Math/t2tot2.hxx"

/*!
 * \brief fill a tensor with values which are neither null nor equal
 * \param[out] t: tensor
 * \param[in] s: seed
 */
template <tfel::math::TensorConcept TensorType>
constexpr void fill(TensorType& t, const unsigned short s) {
  using real = tfel::math::base_type<tfel::math::numeric_type<TensorType>>;
  for (unsigned short i = 0; i != t.size(); ++i) {
    t[i] = (real{1} + 2 * i + s) / 8;
  }
}  // end of fill

/*!
 * \brief fill a fourth order tensor with values which are not null
 * \param[out] t: fourth order tensor
 * \param[in] s: seed
 */
template <typename FourthOrderTensorType>
constexpr void fill(FourthOrderTensorType& t, const unsigned short s) {
  using real =
      tfel::math::base_type<tfel::math::numeric_type<FourthOrderTensorType>>;
  for (unsigned short i = 0; i != t.size(0); ++i) {
    for (unsigned short j = 0; j != t.size(1); ++j) {
      const auto k = static_cast<unsigned short>(7 * i + 3 * j + s);
      t(i, j) = tfel::math::numeric_type<FourthOrderTensorType>{
          (real{2} * (k % 11) - 9) / 8};
    }
  }
}  // end of fill

/*!
 * \return true if two fourth order tensors have the same type and the same
 * values, up to a few rounding errors: a compiler may contract differently
 * the two products
 * \param[in] a: first tensor
 * \param[in] b: second tensor
 */
template <typename FourthOrderTensorType>
constexpr bool same(const FourthOrderTensorType& a,
                    const FourthOrderTensorType& b) {
  using namespace tfel::math;
  using real = base_type<numeric_type<FourthOrderTensorType>>;
  auto d = real{};
  auto m = real{};
  for (unsigned short i = 0; i != a.size(0); ++i) {
    for (unsigned short j = 0; j != a.size(1); ++j) {
      const auto va = base_type_cast(a(i, j));
      const auto vb = base_type_cast(b(i, j));
      d = std::max(d, tfel::math::abs(va - vb));
      m = std::max(m, std::max(tfel::math::abs(va), tfel::math::abs(vb)));
    }
  }
  return d <= 100 * std::numeric_limits<real>::epsilon() * m;
}  // end of same

/*!
 * \return true if:
 * - the components of an expression which are declared null by construction
 *   are the null components of its evaluation.
 * - no row and no column of this expression is null by construction: a non
 *   finite value of an operand of a product thus always leads to a non
 *   finite result.
 * \param[in] e: expression, built from tensors without null components
 */
template <typename ExprType>
constexpr bool checkSparsityPattern(const ExprType& e) {
  using namespace tfel::math;
  using real = base_type<numeric_type<ExprType>>;
  using Pattern = SparsityPattern<ExprType>;
  const auto r = eval(e);
  auto sparse = false;
  for (unsigned short i = 0; i != r.size(0); ++i) {
    auto null_row = true;
    for (unsigned short j = 0; j != r.size(1); ++j) {
      const auto null = !(tfel::math::abs(base_type_cast(r(i, j))) > real{});
      if (Pattern::isNull(i, j) != null) {
        return false;
      }
      sparse = sparse || null;
      null_row = null_row && null;
    }
    if (null_row) {
      return false;
    }
  }
  for (unsigned short j = 0; j != r.size(1); ++j) {
    auto null_column = true;
    for (unsigned short i = 0; i != r.size(0); ++i) {
      null_column = null_column && Pattern::isNull(i, j);
    }
    if (null_column) {
      return false;
    }
  }
  return Pattern::isSparse == sparse;
}  // end of checkSparsityPattern

/*!
 * \brief operands of the products
 * \tparam N: space dimension
 * \tparam real: numeric type
 */
template <unsigned short N, typename real>
struct Operands {
  constexpr Operands() {
    fill(A, 0);
    fill(B, 5);
    fill(X, 1);
    fill(Y, 2);
    fill(Z, 3);
    fill(S, 4);
    ld = tfel::math::t2tot2<N, real>::tpld(B);
    rd = tfel::math::t2tot2<N, real>::tprd(A);
    dC = tfel::math::t2tost2<N, real>::dCdF(A);
    dB = tfel::math::t2tost2<N, real>::dBdF(A);
  }  // end of Operands
  //! \brief tensors
  tfel::math::tensor<N, real> A{}, B{};
  //! \brief fourth order tensors
  tfel::math::t2tot2<N, real> X{};
  tfel::math::t2tost2<N, real> Y{};
  tfel::math::st2tot2<N, real> Z{};
  tfel::math::st2tost2<N, real> S{};
  //! \brief evaluations of the derivatives, used by the standard products
  tfel::math::t2tot2<N, real> ld{}, rd{};
  tfel::math::t2tost2<N, real> dC{}, dB{};
};  // end of struct Operands

/*!
 * \return true if the sparsity patterns of the derivatives of the product of
 * two tensors and of the Cauchy-Green tensors are the expected ones
 * \tparam N: space dimension
 * \tparam real: numeric type
 */
template <unsigned short N, typename real>
constexpr bool checkSparsityPatterns() {
  using namespace tfel::math;
  const auto o = Operands<N, real>{};
  return checkSparsityPattern(t2tot2<N, real>::tpld(o.B)) &&
         checkSparsityPattern(t2tot2<N, real>::tprd(o.A)) &&
         checkSparsityPattern(t2tost2<N, real>::dCdF(o.A)) &&
         checkSparsityPattern(t2tost2<N, real>::dBdF(o.A));
}  // end of checkSparsityPatterns

/*!
 * \return true if the products of a `t2tot2` and a derivative give the
 * results of the standard products, which are the ones used with the
 * evaluations of the derivatives
 * \tparam N: space dimension
 * \tparam real: numeric type
 */
template <unsigned short N, typename real>
constexpr bool checkProductsWithT2toT2() {
  using namespace tfel::math;
  using T2toT2 = t2tot2<N, real>;
  using T2toST2 = t2tost2<N, real>;
  const auto [A, B, X, Y, Z, S, ld, rd, dC, dB] = Operands<N, real>{};
  return same(eval(X * T2toT2::tpld(B)), eval(X * ld)) &&
         same(eval(X * T2toT2::tprd(A)), eval(X * rd)) &&
         same(eval(T2toT2::tpld(B) * X), eval(ld * X)) &&
         same(eval(T2toT2::tprd(A) * X), eval(rd * X)) &&
         same(eval(T2toST2::dCdF(A) * X), eval(dC * X)) &&
         same(eval(T2toST2::dBdF(A) * X), eval(dB * X));
}  // end of checkProductsWithT2toT2

/*!
 * \return true if the products of a `t2tost2` or a `st2tost2` and a
 * derivative give the results of the standard products
 * \tparam N: space dimension
 * \tparam real: numeric type
 */
template <unsigned short N, typename real>
constexpr bool checkProductsWithT2toST2AndST2toST2() {
  using namespace tfel::math;
  using T2toT2 = t2tot2<N, real>;
  using T2toST2 = t2tost2<N, real>;
  const auto [A, B, X, Y, Z, S, ld, rd, dC, dB] = Operands<N, real>{};
  return same(eval(Y * T2toT2::tpld(B)), eval(Y * ld)) &&
         same(eval(Y * T2toT2::tprd(A)), eval(Y * rd)) &&
         same(eval(S * T2toST2::dCdF(A)), eval(S * dC)) &&
         same(eval(S * T2toST2::dBdF(A)), eval(S * dB));
}  // end of checkProductsWithT2toST2AndST2toST2

/*!
 * \return true if the products of a `st2tot2` and a derivative give the
 * results of the standard products
 * \tparam N: space dimension
 * \tparam real: numeric type
 */
template <unsigned short N, typename real>
constexpr bool checkProductsWithST2toT2() {
  using namespace tfel::math;
  using T2toT2 = t2tot2<N, real>;
  using T2toST2 = t2tost2<N, real>;
  const auto [A, B, X, Y, Z, S, ld, rd, dC, dB] = Operands<N, real>{};
  return same(eval(T2toT2::tpld(B) * Z), eval(ld * Z)) &&
         same(eval(T2toT2::tprd(A) * Z), eval(rd * Z)) &&
         same(eval(T2toST2::dCdF(A) * Z), eval(dC * Z)) &&
         same(eval(T2toST2::dBdF(A) * Z), eval(dB * Z)) &&
         same(eval(Z * T2toST2::dCdF(A)), eval(Z * dC)) &&
         same(eval(Z * T2toST2::dBdF(A)), eval(Z * dB));
}  // end of checkProductsWithST2toT2

/*!
 * \return true if the products of two derivatives give the results of the
 * standard products
 * \tparam N: space dimension
 * \tparam real: numeric type
 */
template <unsigned short N, typename real>
constexpr bool checkProductsOfDerivatives() {
  using namespace tfel::math;
  using T2toT2 = t2tot2<N, real>;
  using T2toST2 = t2tost2<N, real>;
  const auto [A, B, X, Y, Z, S, ld, rd, dC, dB] = Operands<N, real>{};
  return same(eval(T2toT2::tpld(B) * T2toT2::tprd(A)), eval(ld * rd)) &&
         same(eval(T2toT2::tprd(A) * T2toT2::tpld(B)), eval(rd * ld)) &&
         same(eval(T2toST2::dCdF(A) * T2toT2::tprd(A)), eval(dC * rd)) &&
         same(eval(T2toST2::dBdF(A) * T2toT2::tpld(B)), eval(dB * ld));
}  // end of checkProductsOfDerivatives

/*!
 * \return true if the products of expressions and the expressions using
 * products give the results obtained with the standard products
 * \tparam N: space dimension
 * \tparam real: numeric type
 */
template <unsigned short N, typename real>
constexpr bool checkExpressions() {
  using namespace tfel::math;
  using T2toT2 = t2tot2<N, real>;
  using T2toST2 = t2tost2<N, real>;
  const auto [A, B, X, Y, Z, S, ld, rd, dC, dB] = Operands<N, real>{};
  return same(eval((2 * Y - Y) * T2toT2::tprd(A + B)),
              eval(Y * T2toT2{T2toT2::tprd(A + B)})) &&
         same(eval(-(Y * T2toT2::tpld(B)) / 2 + Y), eval(-(Y * ld) / 2 + Y)) &&
         same(eval((S * T2toST2::dCdF(A)) * T2toT2::tpld(B)),
              eval((S * dC) * ld)) &&
         // compositions of the derivatives of the product of two tensors
         same(eval(T2toT2::tpld(B, T2toT2::tprd(A))),
              eval(T2toT2::tpld(B, rd))) &&
         same(eval(T2toT2::tprd(A, T2toT2::tpld(B))),
              eval(T2toT2::tprd(A, ld)));
}  // end of checkExpressions

/*!
 * \return true if the products with the opposite of a derivative, or with
 * its product or its division by a scalar, give the results of the standard
 * products
 * \tparam N: space dimension
 * \tparam real: numeric type
 */
template <unsigned short N, typename real>
constexpr bool checkOppositesAndScalarProducts() {
  using namespace tfel::math;
  using T2toT2 = t2tot2<N, real>;
  using T2toST2 = t2tost2<N, real>;
  const auto [A, B, X, Y, Z, S, ld, rd, dC, dB] = Operands<N, real>{};
  return same(eval(-T2toST2::dCdF(A) * X / 2), eval(-dC * X / 2)) &&
         same(eval((T2toST2::dBdF(A) / 2) * Z), eval((dB / 2) * Z)) &&
         same(eval(X * (2 * T2toT2::tprd(A))), eval(X * (2 * rd))) &&
         same(eval(Y * (T2toT2::tpld(B) * 2)), eval(Y * (ld * 2))) &&
         same(eval(-T2toST2::dCdF(A) * (-T2toT2::tpld(B) / 2)),
              eval(dC * ld / 2));
}  // end of checkOppositesAndScalarProducts

/*!
 * \brief check the types of the products: the standard products are
 * unchanged when no component of their operands is null by construction
 * \tparam N: space dimension
 * \tparam real: numeric type
 */
template <unsigned short N, typename real>
constexpr bool checkTypes() {
  using namespace tfel::math;
  using T2toT2 = t2tot2<N, real>;
  using T2toST2 = t2tost2<N, real>;
  using ST2toT2 = st2tot2<N, real>;
  using ST2toST2 = st2tost2<N, real>;
  const auto A = tensor<N, real>{};
  const auto X = T2toT2{};
  const auto Y = T2toST2{};
  const auto Z = ST2toT2{};
  const auto S = ST2toST2{};
  // standard products
  static_assert(std::is_same_v<decltype(X * X),
                               Expr<T2toT2, T2toT2T2toT2ProductExpr<N>>>);
  static_assert(std::is_same_v<decltype(Y * X),
                               Expr<T2toST2, T2toST2T2toT2ProductExpr<N>>>);
  static_assert(std::is_same_v<decltype(S * Y),
                               Expr<T2toST2, ST2toST2T2toST2ProductExpr<N>>>);
  static_assert(std::is_same_v<decltype(Y * Z),
                               Expr<ST2toST2, T2toST2ST2toT2ProductExpr<N>>>);
  static_assert(std::is_same_v<decltype(X * Z),
                               Expr<ST2toT2, T2toT2ST2toT2ProductExpr<N>>>);
  static_assert(std::is_same_v<decltype(Z * Y),
                               Expr<T2toT2, ST2toT2T2toST2ProductExpr<N>>>);
  // standard products of expressions, including the compositions of the
  // derivatives of the product of two tensors with a fourth order tensor
  static_assert(!SparsityPattern<T2toT2>::isSparse);
  static_assert(!SparsityPattern<decltype(T2toT2::tpld(A, X))>::isSparse);
  static_assert(!SparsityPattern<decltype(T2toT2::tprd(A, X))>::isSparse);
  static_assert(std::is_same_v<decltype((2 * Y) * (X + X)),
                               Expr<T2toST2, T2toST2T2toT2ProductExpr<N>>>);
  static_assert(std::is_same_v<decltype(Y * T2toT2::tpld(A, X)),
                               Expr<T2toST2, T2toST2T2toT2ProductExpr<N>>>);
  static_assert(std::is_same_v<decltype(Y * T2toT2::tprd(A, X)),
                               Expr<T2toST2, T2toST2T2toT2ProductExpr<N>>>);
  // products which do not compute the terms that are null by construction
  static_assert(std::is_same_v<decltype(X * T2toT2::tpld(A)),
                               Expr<T2toT2, SparseProductExpr>>);
  static_assert(std::is_same_v<decltype(T2toT2::tprd(A) * X),
                               Expr<T2toT2, SparseProductExpr>>);
  static_assert(std::is_same_v<decltype(Y * T2toT2::tprd(A)),
                               Expr<T2toST2, SparseProductExpr>>);
  static_assert(std::is_same_v<decltype(T2toST2::dCdF(A) * X),
                               Expr<T2toST2, SparseProductExpr>>);
  static_assert(std::is_same_v<decltype(S * T2toST2::dBdF(A)),
                               Expr<T2toST2, SparseProductExpr>>);
  static_assert(std::is_same_v<decltype(T2toST2::dCdF(A) * Z),
                               Expr<ST2toST2, SparseProductExpr>>);
  static_assert(std::is_same_v<decltype(T2toT2::tprd(A) * Z),
                               Expr<ST2toT2, SparseProductExpr>>);
  static_assert(std::is_same_v<decltype(Z * T2toST2::dCdF(A)),
                               Expr<T2toT2, SparseProductExpr>>);
  // the opposite of a derivative and its product or its division by a
  // scalar have the same null components
  static_assert(std::is_same_v<decltype(-T2toST2::dCdF(A) * X),
                               Expr<T2toST2, SparseProductExpr>>);
  static_assert(std::is_same_v<decltype(Y * (2 * T2toT2::tprd(A))),
                               Expr<T2toST2, SparseProductExpr>>);
  static_assert(std::is_same_v<decltype((T2toT2::tpld(A) * 2) * Z),
                               Expr<ST2toT2, SparseProductExpr>>);
  static_assert(std::is_same_v<decltype(S * (T2toST2::dBdF(A) / 2)),
                               Expr<T2toST2, SparseProductExpr>>);
  static_assert(!SparsityPattern<decltype(-X)>::isSparse);
  static_assert(!SparsityPattern<decltype(2 * X)>::isSparse);
  static_assert(!SparsityPattern<decltype(X / 2)>::isSparse);
  // the derivatives and the products still match the concepts of their
  // results
  static_assert(T2toT2Concept<decltype(T2toT2::tpld(A))>);
  static_assert(T2toT2Concept<decltype(T2toT2::tprd(A))>);
  static_assert(T2toT2Concept<decltype(X * T2toT2::tprd(A))>);
  static_assert(T2toST2Concept<decltype(Y * T2toT2::tprd(A))>);
  static_assert(ST2toST2Concept<decltype(T2toST2::dCdF(A) * Z)>);
  static_assert(ST2toT2Concept<decltype(T2toT2::tprd(A) * Z)>);
  return true;
}  // end of checkTypes

/*!
 * \return true if a product with a derivative of the product of two tensors
 * has the expected unit
 * \tparam N: space dimension
 */
template <unsigned short N>
constexpr bool checkQuantities() {
  using namespace tfel::math;
  using stress = qt<unit::Stress, double>;
  auto F = tensor<N, double>{};
  auto K = t2tost2<N, stress>{};
  fill(F, 0);
  fill(K, 1);
  const auto r = K * t2tot2<N, double>::tpld(F);
  static_assert(std::is_same_v<decltype(r), const Expr<t2tost2<N, stress>,  //
                                                       SparseProductExpr>>);
  const t2tot2<N, double> ld = t2tot2<N, double>::tpld(F);
  return same(eval(r), eval(K * ld));
}  // end of checkQuantities

struct SparseProductsTest final : public tfel::tests::TestCase {
  SparseProductsTest()
      : tfel::tests::TestCase("TFEL/Math", "SparseProductsTest") {
  }  // end of SparseProductsTest

  tfel::tests::TestResult execute() override {
    this->check<1u, double>();
    this->check<2u, double>();
    this->check<3u, double>();
    this->check<1u, float>();
    this->check<2u, float>();
    this->check<3u, float>();
    // a quantity
    TFEL_TESTS_STATIC_ASSERT(checkQuantities<1u>());
    TFEL_TESTS_STATIC_ASSERT(checkQuantities<2u>());
    TFEL_TESTS_STATIC_ASSERT(checkQuantities<3u>());
    return this->result;
  }  // end of execute

 private:
  /*!
   * \brief checks at compile-time, then with the code generated by the
   * compiler
   * \tparam N: space dimension
   * \tparam real: numeric type
   */
  template <unsigned short N, typename real>
  void check() {
    TFEL_TESTS_STATIC_ASSERT((checkTypes<N, real>()));
    TFEL_TESTS_STATIC_ASSERT((checkSparsityPatterns<N, real>()));
    TFEL_TESTS_STATIC_ASSERT((checkProductsWithT2toT2<N, real>()));
    TFEL_TESTS_STATIC_ASSERT((checkProductsWithT2toST2AndST2toST2<N, real>()));
    TFEL_TESTS_STATIC_ASSERT((checkProductsWithST2toT2<N, real>()));
    TFEL_TESTS_STATIC_ASSERT((checkProductsOfDerivatives<N, real>()));
    TFEL_TESTS_STATIC_ASSERT((checkExpressions<N, real>()));
    TFEL_TESTS_STATIC_ASSERT((checkOppositesAndScalarProducts<N, real>()));
    TFEL_TESTS_ASSERT((checkProductsWithT2toT2<N, real>()));
    TFEL_TESTS_ASSERT((checkProductsWithT2toST2AndST2toST2<N, real>()));
    TFEL_TESTS_ASSERT((checkProductsWithST2toT2<N, real>()));
    TFEL_TESTS_ASSERT((checkProductsOfDerivatives<N, real>()));
    TFEL_TESTS_ASSERT((checkExpressions<N, real>()));
    TFEL_TESTS_ASSERT((checkOppositesAndScalarProducts<N, real>()));
  }  // end of check
};

TFEL_TESTS_GENERATE_PROXY(SparseProductsTest, "SparseProductsTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("sparse_products.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
