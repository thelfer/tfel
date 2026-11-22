/*!
 * \file   include/TFEL/Math/ExpressionTemplates/SparseProductExpr.hxx
 * \brief  This file declares the `SparsityPattern` metafunction and the
 * expression describing the product of two objects when some of their
 * components are null by construction.
 * \author Tristan Chenaille
 * \date   22/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_EXPRESSIONTEMPLATES_SPARSEPRODUCTEXPR_HXX
#define LIB_TFEL_MATH_EXPRESSIONTEMPLATES_SPARSEPRODUCTEXPR_HXX

#include <utility>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/fsarray.hxx"
#include "TFEL/Math/Forward/Expr.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"

namespace tfel::math {

  /*!
   * \brief a metafunction describing the components of a mathematical object
   * which are null by construction, i.e. whatever the values of the objects
   * used to build it.
   *
   * The terms associated with those components are not computed by the
   * products of fourth order tensors, see `SparseProductExpr` for details.
   * By default, no component is known to be null.
   *
   * \tparam T: type of the mathematical object
   */
  template <typename T>
  struct SparsityPattern {
    //! \brief boolean stating if some components are null by construction
    static constexpr bool isSparse = false;
    /*!
     * \return true if the given component is null by construction
     * \param[in] i: row index
     * \param[in] j: column index
     */
    [[nodiscard]] static constexpr bool isNull(const unsigned short,
                                               const unsigned short) noexcept {
      return false;
    }
  };  // end of struct SparsityPattern

  /*!
   * \brief partial specialisation of the `SparsityPattern` metafunction for
   * the opposite of an object, whose null components are the ones of this
   * object.
   */
  template <typename ResultType, typename T1>
  struct SparsityPattern<Expr<ResultType, UnaryOperation<T1, OpNeg>>>
      : SparsityPattern<std::decay_t<T1>> {};

  /*!
   * \brief partial specialisation of the `SparsityPattern` metafunction for
   * the product of a scalar and an object, whose null components are the
   * ones of this object.
   */
  template <typename ResultType, typename T1, typename T2>
  struct SparsityPattern<
      Expr<ResultType, ScalarObjectOperation<T1, T2, OpMult>>>
      : SparsityPattern<std::decay_t<T2>> {};

  /*!
   * \brief partial specialisation of the `SparsityPattern` metafunction for
   * the product and the division of an object by a scalar, whose null
   * components are the ones of this object.
   */
  template <typename ResultType, typename T1, typename T2, typename Op>
  struct SparsityPattern<Expr<ResultType, ObjectScalarOperation<T1, T2, Op>>>
      : SparsityPattern<std::decay_t<T1>> {};

  /*!
   * \brief sparsity pattern of an expression built from a single argument.
   *
   * The components which are null by construction are the null components of
   * the expression built from an argument whose components are all equal to
   * one. This assumes that the other components do not vanish for this
   * argument, which is the case when each of them is proportional to a
   * component of the argument.
   *
   * \tparam ExprType: type of the expression
   * \tparam ArgumentType: type of the argument
   */
  template <typename ExprType, typename ArgumentType>
  struct SingleArgumentExprSparsityPattern {
    //! \brief boolean stating if some components are null by construction
    static constexpr bool isSparse = true;
    /*!
     * \return true if the given component is null by construction
     * \param[in] i: row index
     * \param[in] j: column index
     */
    [[nodiscard]] static constexpr bool isNull(
        const unsigned short i, const unsigned short j) noexcept {
      return !(tfel::math::abs(pattern(i, j)) > 0);
    }

   private:
    //! \brief expression built from an argument filled with ones
    static constexpr auto pattern = ExprType(ArgumentType(1));
  };  // end of struct SingleArgumentExprSparsityPattern

  /*!
   * \brief empty structure used for partial specialisation of the `Expr`
   * class.
   *
   * The associated expression describes the product of two objects with
   * matrix-like access operators, such as the fourth order tensors, when some
   * components of one of them at least are null by construction (see
   * `SparsityPattern`). The terms associated with those components are not
   * computed. The other terms are summed in the same order as in the standard
   * product, the first two ones being swapped when this leads a compiler
   * contracting floating-point expressions to the operations of the standard
   * product, which does not change their sum. The results are thus the same,
   * except that:
   *
   * - a non finite value of one object is only propagated by the terms which
   *   are computed.
   * - a null result may have a different sign.
   * - a compiler contracting floating-point expressions may fuse different
   *   multiplications and additions.
   */
  struct SparseProductExpr {};

  /*!
   * \brief a simple alias giving the operation describing the product of two
   * objects: `SparseProductExpr` if some components of one of those objects at
   * least are null by construction, the given operation otherwise.
   * \tparam A: type of the left hand side
   * \tparam B: type of the right hand side
   * \tparam StandardProductExpr: operation describing the standard product
   */
  template <typename A, typename B, typename StandardProductExpr>
  using ProductOperation =
      std::conditional_t<SparsityPattern<std::decay_t<A>>::isSparse ||
                             SparsityPattern<std::decay_t<B>>::isSparse,
                         SparseProductExpr,
                         StandardProductExpr>;

  namespace internals {

    /*!
     * \return the size of a mathematical object along the given dimension
     * \tparam T: type of the mathematical object
     * \param[in] d: dimension
     */
    template <typename T>
    [[nodiscard]] constexpr unsigned short getSize(const unsigned short d) {
      return typename EvaluationResult<T>::indexing_policy().size(d);
    }  // end of getSize

    /*!
     * \brief structure in charge of computing the component (i, j) of the
     * product of two objects, i.e. the sum of the terms a(i, k) * b(k, j)
     * which are not null by construction.
     * \tparam A: type of the left hand side
     * \tparam B: type of the right hand side
     * \tparam i: row index
     * \tparam j: column index
     */
    template <typename A, typename B, unsigned short i, unsigned short j>
    struct SparseProductComponent {
      //! \brief number of terms of the standard product
      static constexpr auto n = getSize<A>(1);
      /*!
       * \return the index of the first term which is not null by
       * construction, starting from the given one, or n if there is no such
       * term.
       * \param[in] k: index of the first term considered
       */
      [[nodiscard]] static constexpr unsigned short next(
          unsigned short k) noexcept {
        while ((k < n) && (SparsityPattern<A>::isNull(i, k) ||
                           SparsityPattern<B>::isNull(k, j))) {
          ++k;
        }
        return k;
      }  // end of next
      /*!
       * \return the sum of the given partial sum and of the terms following
       * the k-th one
       * \tparam k: index of the last term of the partial sum
       * \param[in] s: partial sum
       * \param[in] a: left hand side
       * \param[in] b: right hand side
       */
      template <unsigned short k>
      [[nodiscard]] TFEL_HOST_DEVICE static constexpr auto add(
          const auto& s, const A& a, const B& b) noexcept {
        constexpr auto k2 = next(k + 1);
        if constexpr (k2 == n) {
          return s;
        } else {
          return add<k2>(s + a(i, k2) * b(k2, j), a, b);
        }
      }  // end of add
      /*!
       * \return the component (i, j) of the product
       * \tparam ValueType: type of the result
       * \param[in] a: left hand side
       * \param[in] b: right hand side
       */
      template <typename ValueType>
      [[nodiscard]] TFEL_HOST_DEVICE static constexpr ValueType exe(
          const A& a, const B& b) noexcept {
        constexpr auto k1 = next(0);
        constexpr auto k2 = k1 == n ? n : next(k1 + 1);
        if constexpr (k1 == n) {
          return ValueType{};
        } else if constexpr (k2 == n) {
          return a(i, k1) * b(k1, j);
        } else if constexpr (k2 == 1) {
          return add<k2>(a(i, k1) * b(k1, j) + a(i, k2) * b(k2, j), a, b);
        } else {
          // The standard product sums every term in order. A compiler
          // contracting this sum fuses the multiplication of each term with
          // the addition of the partial sum, except for the second term,
          // which is rounded: the first one is fused with its addition. The
          // first term which is not null is thus rounded as soon as one of
          // the first two terms is null, the following ones being fused.
          // Writing the second term which is not null first leads the
          // compiler to the same operations.
          return add<k2>(a(i, k2) * b(k2, j) + a(i, k1) * b(k1, j), a, b);
        }
      }  // end of exe
    };  // end of struct SparseProductComponent

  }  // end of namespace internals

  /*!
   * \brief partial specialisation of the `Expr` class for the product of two
   * objects when some of their components are null by construction.
   * \tparam ResultType: type of the result of the product
   */
  template <typename ResultType>
  struct Expr<ResultType, SparseProductExpr>
      : public ConceptRebind<typename ComputeObjectTag<ResultType>::type,
                             Expr<ResultType, SparseProductExpr>>::type,
        public array_holder<internals::getSize<ResultType>(0) *
                                internals::getSize<ResultType>(1),
                            numeric_type<ResultType>> {
    //! \brief a simple alias
    using RunTimeProperties = EmptyRunTimeProperties;
    //! \brief a simple alias
    using value_type = numeric_type<ResultType>;
    /*!
     * \param[in] a: first term of the product
     * \param[in] b: second term of the product
     */
    template <typename A, typename B>
    TFEL_HOST_DEVICE constexpr Expr(const A& a, const B& b) noexcept {
      static_assert(internals::getSize<A>(0) == rows);
      static_assert(internals::getSize<A>(1) == internals::getSize<B>(0));
      static_assert(internals::getSize<B>(1) == columns);
      this->compute(a, b, std::make_index_sequence<rows * columns>());
    }  // end of Expr
    /*!
     * \brief access operator
     * \param[in] i: row index
     * \param[in] j: column index
     */
    [[nodiscard]] TFEL_HOST_DEVICE constexpr const value_type& operator()(
        const unsigned short i, const unsigned short j) const noexcept {
      return this->v[i * columns + j];
    }  // end of operator()
    //! \return the runtime properties
    [[nodiscard]] TFEL_HOST_DEVICE constexpr auto getRunTimeProperties()
        const noexcept {
      return RunTimeProperties();
    }

   private:
    //! \brief number of rows
    static constexpr auto rows = internals::getSize<ResultType>(0);
    //! \brief number of columns
    static constexpr auto columns = internals::getSize<ResultType>(1);
    /*!
     * \brief compute the components of the product
     * \tparam c: indices of the components
     * \param[in] a: first term of the product
     * \param[in] b: second term of the product
     */
    template <typename A, typename B, std::size_t... c>
    TFEL_HOST_DEVICE constexpr void compute(
        const A& a, const B& b, const std::index_sequence<c...>&) noexcept {
      ((this->v[c] = internals::SparseProductComponent<
            A, B, c / columns, c % columns>::template exe<value_type>(a, b)),
       ...);
    }  // end of compute
  };  // end of struct Expr<ResultType, SparseProductExpr>

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_EXPRESSIONTEMPLATES_SPARSEPRODUCTEXPR_HXX */
