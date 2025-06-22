/*!
 * \file   include/TFEL/Math/ExpressionTemplates/Expr.hxx
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

#ifndef LIB_TFEL_MATH_EXPRESSION_EXPR_HXX
#define LIB_TFEL_MATH_EXPRESSION_EXPR_HXX

#include <type_traits>
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"
#include "TFEL/Math/General/ComputeBinaryResult.hxx"
#include "TFEL/Math/Forward/Expr.hxx"

namespace tfel::math {

  /*!
   * \brief an helper class
   */
  struct ExprBase {
   protected:
    /*!
     * \brief a simple metafunction defining how an argument of an
     * operation is stored in the Expr class.
     */
    template <typename ValueType>
    using ArgumentStorage = std::conditional_t<
        tfel::typetraits::isScalar<ValueType>(),
        const std::decay_t<ValueType>,
        std::conditional_t<std::is_rvalue_reference_v<ValueType>,
                           const std::decay_t<ValueType>,
                           const std::decay_t<ValueType>&>>;
  };  // end of strut ExprBase

  template <typename ResultType, typename Operation>
  struct Expr
      : public ConceptRebind<typename ComputeObjectTag<ResultType>::type,
                             Expr<ResultType, Operation>>::type,
        public Operation {
    //! a simple alias
    using value_type = typename Operation::value_type;
    //! a simple alias
    using size_type = typename Operation::size_type;
    /*!
     * default constructor
     * \param[in] args : arguments of Operation constructor
     */
    template <typename... Args>
    TFEL_HOST_DEVICE constexpr explicit Expr(Args&&... args)
        : Operation(std::forward<Args>(args)...) {}
    /*!
     * \brief array like access operator
     * \param[in] i : index
     * \note This operator is always defined, even tough it might not be
     * valid
     */
    TFEL_HOST_DEVICE constexpr value_type operator[](const size_type i) const {
      // may not be defined
      static_assert(std::is_invocable_v<const Expr, size_type>,
                    "Expr is not callable");
      return Operation::operator()(i);
    }
    /*!
     * \brief array like access operator
     * \param[in] i : index
     * \note This operator is always defined, even tough it might not be
     * valid
     */
    TFEL_HOST_DEVICE constexpr value_type& operator[](const size_type i) {
      // may not be defined
      static_assert(
          std::is_same_v<decltype(Operation::operator()(i)), value_type&>,
          "unexpected return value for Operation::operator(size_type)");
      return Operation::operator()(i);
    }

    using Operation::operator();
    using Operation::operator=;
  };

  /*!
   * \brief an Expr object allowing the lazy evaluation of an unary operation.
   * \tparam ResultType: type of the result of the unary operation
   * \tparam T1 : type of the argument of the unary operation
   * \tparam Op : unary operation
   */
  template <typename ResultType, typename T1, typename Op>
  struct Expr<ResultType, UnaryOperation<T1, Op>>
      : public ExprBase,
        public ConceptRebind<typename ComputeObjectTag<ResultType>::type,
                             Expr<ResultType, UnaryOperation<T1, Op>>>::type {
    Expr() = delete;
    constexpr Expr(const Expr&) = default;
    constexpr Expr(Expr&&) = default;
    constexpr Expr& operator=(const Expr&) = delete;
    constexpr Expr& operator=(Expr&&) = delete;
    //! type return by the access operator(s)
    using value_type = typename ResultType::value_type;
    //! type used by access operator(s)
    using size_type = typename ResultType::size_type;
    //! result type
    using result_type = ResultType;
    //! argument type
    using argument_type = T1;
    //! storage type of the argument
    using argument_storage_type = ArgumentStorage<T1>;
    /*!
     * \param l : left argument of the unary operation
     */
    TFEL_HOST_DEVICE constexpr Expr(T1 l) : a(std::forward<T1>(l)) {}
    /*!
     * array-like access operator
     */
    TFEL_HOST_DEVICE constexpr value_type operator[](const size_type i) const {
      static_assert(
          isUnaryOperationResultTypeValid<
              std::invoke_result_t<argument_storage_type, size_type>, Op>(),
          "invalid call to unary operation operator[]");
      return Op::apply(this->a(i));
    }
    /*!
     * \brief multidimensional access operator
     */
    template <typename... Indexes>
    constexpr auto operator()(const Indexes... i) const
        -> UnaryOperationHandler<
            std::conditional_t<
                std::is_lvalue_reference_v<
                    std::invoke_result_t<argument_storage_type, Indexes...>>,
                std::invoke_result_t<argument_storage_type, Indexes...>,
                std::invoke_result_t<argument_storage_type, Indexes...>&&>,
            Op> {
      static_assert(
          isUnaryOperationResultTypeValid<
              std::invoke_result_t<argument_storage_type, Indexes...>, Op>(),
          "invalid call to unary operation operator()");
      return Op::apply(this->a(i...));
    }  // end of operator()
   protected:
    argument_storage_type a;  //<! argument of the operation
  };  // end of struct Expr<ResultType,UnaryOperation<T1,Op> >

  /*!
   * \brief an Expr object allowing the lazy evaluation of a binary
   * operation.
   * \tparam ResultType: type of the result of the binary operation
   * \tparam T1: type of the left  argument of the binary operation
   * \tparam T2: type of the right argument of the binary operation
   * \tparam Op: the binary operation
   */
  template <typename ResultType, typename T1, typename T2, typename Op>
  struct Expr<ResultType, BinaryOperation<T1, T2, Op>>
      : public ExprBase,
        public ConceptRebind<
            typename ComputeObjectTag<ResultType>::type,
            Expr<ResultType, BinaryOperation<T1, T2, Op>>>::type {
    Expr() = delete;
    constexpr Expr(const Expr&) = default;
    constexpr Expr(Expr&&) = default;
    constexpr Expr& operator=(const Expr&) = delete;
    constexpr Expr& operator=(Expr&&) = delete;
    //! type return by the access operator(s)
    using value_type = typename ResultType::value_type;
    //! type used by access operator(s)
    using size_type = typename ResultType::size_type;
    //! result type
    using result_type = ResultType;
    //! type of left hand side argument
    using lhs_type = T1;
    //! type of right hand side argument
    using rhs_type = T2;
    //! storage type of left hand side argument
    using lhs_storage_type = ArgumentStorage<T1>;
    //! storage type of right hand side argument
    using rhs_storage_type = ArgumentStorage<T2>;
    /*!
     * \param l : left argument of the binary operation
     * \param r : right argument of the binary operation
     */
    TFEL_HOST_DEVICE constexpr Expr(T1 l, T2 r)
        : a(std::forward<T1>(l)), b(std::forward<T2>(r)) {}  // end of Expr
    /*!
     * array-like access operator
     */
    TFEL_HOST_DEVICE constexpr value_type operator[](const size_type i) const {
      static_assert(
          isBinaryOperationResultTypeValid<
              std::invoke_result_t<lhs_storage_type, size_type>,
              std::invoke_result_t<rhs_storage_type, size_type>, Op>(),
          "invalid call to unary operation operator[]");
      return Op::apply(this->a(i), this->b(i));
    }
    /*!
     * multi-dimensional operator acces
     */
    template <typename... Indexes>
    TFEL_HOST_DEVICE constexpr BinaryOperationHandler<
        std::conditional_t<
            std::is_lvalue_reference_v<
                std::invoke_result_t<lhs_storage_type, Indexes...>>,
            std::invoke_result_t<lhs_storage_type, Indexes...>,
            std::invoke_result_t<lhs_storage_type, Indexes...>&&>,
        std::conditional_t<
            std::is_lvalue_reference_v<
                std::invoke_result_t<rhs_storage_type, Indexes...>>,
            std::invoke_result_t<rhs_storage_type, Indexes...>,
            std::invoke_result_t<rhs_storage_type, Indexes...>&&>,
        Op>
    operator()(const Indexes... i) const {
      static_assert(
          isBinaryOperationResultTypeValid<
              std::invoke_result_t<lhs_storage_type, Indexes...>,
              std::invoke_result_t<rhs_storage_type, Indexes...>, Op>(),
          "invalid call to binary operation operator()");
      return Op::apply(this->a(i...), this->b(i...));
    }  // end of operator()
   protected:
    lhs_storage_type a;  //<! left  hand side argument of the operation
    rhs_storage_type b;  //<! right hand side argument of the operation
  };  // end of struct Expr<ResultType,BinaryOperation<T1,T2,Op> >

  /*!
   * an Expr object allows the lazy evaluation of a binary
   * operation between a scalar and a mathematical object.
   * \tparam ResultType :
   * \tparam T1 : type of the left  argument of the binary operation
   * \tparam T2 : type of the right argument of the binary operation
   * \tparam Op : the binary operation
   */
  template <typename ResultType, typename T1, typename T2, typename Op>
  struct Expr<ResultType, ScalarObjectOperation<T1, T2, Op>>
      : public ExprBase,
        public ConceptRebind<
            typename ComputeObjectTag<ResultType>::type,
            Expr<ResultType, ScalarObjectOperation<T1, T2, Op>>>::type {
    Expr() = delete;
    constexpr Expr(const Expr&) = default;
    constexpr Expr(Expr&&) = default;
    constexpr Expr& operator=(const Expr&) = delete;
    constexpr Expr& operator=(Expr&&) = delete;
    //! type return by the access operator(s)
    using value_type = typename ResultType::value_type;
    //! type used by access operator(s)
    using size_type = typename ResultType::size_type;
    //! result type
    using result_type = ResultType;
    //! type of left hand side argument
    using lhs_type = T1;
    //! type of right hand side argument
    using rhs_type = T2;
    //! storage type of left hand side argument
    using lhs_storage_type = ArgumentStorage<T1>;
    //! storage type of right hand side argument
    using rhs_storage_type = ArgumentStorage<T2>;
    /*!
     * \param l : left argument of the binary operation
     * \param r : right argument of the binary operation
     */
    TFEL_HOST_DEVICE constexpr Expr(T1 l, T2 r)
        : a(std::forward<T1>(l)), b(std::forward<T2>(r)) {}  // end of Expr
    /*!
     * \brief array-like access operator
     */
    TFEL_HOST_DEVICE constexpr value_type operator[](const size_type i) const {
      static_assert(
          isBinaryOperationResultTypeValid<
              std::add_lvalue_reference_t<lhs_storage_type>,
              std::invoke_result_t<rhs_storage_type, size_type>, Op>(),
          "invalid call");
      return Op::apply(this->a, this->b(i));
    }
    /*!
     * \brief multi-dimensional operator acces
     */
    template <typename... Indexes>
    TFEL_HOST_DEVICE constexpr BinaryOperationHandler<
        std::add_lvalue_reference_t<lhs_storage_type>,
        std::conditional_t<
            std::is_lvalue_reference_v<
                std::invoke_result_t<rhs_storage_type, Indexes...>>,
            std::invoke_result_t<rhs_storage_type, Indexes...>,
            std::invoke_result_t<rhs_storage_type, Indexes...>&&>,
        Op>
    operator()(const Indexes... i) const {
      static_assert(
          isBinaryOperationResultTypeValid<
              std::add_lvalue_reference_t<lhs_storage_type>,
              std::invoke_result_t<rhs_storage_type, Indexes...>, Op>(),
          "invalid call");
      return Op::apply(this->a, this->b(i...));
    }  // end of operator()
   protected:
    const lhs_storage_type a;  //<! left  hand side argument of the operation
    const rhs_storage_type b;  //<! right hand side argument of the operation
  };  // end of struct Expr<ResultType,ScalarObjectOperation<T1,T2,Op>>

  /*!
   * an Expr object allows the lazy evaluation of a binary
   * operation between a mathematical object and scalar.
   * \tparam ResultType :
   * \tparam T1 : type of the left  argument of the binary operation
   * \tparam T2 : type of the right argument of the binary operation
   * \tparam Op : the binary operation
   */
  template <typename ResultType, typename T1, typename T2, typename Op>
  struct Expr<ResultType, ObjectScalarOperation<T1, T2, Op>>
      : public ExprBase,
        public ConceptRebind<
            typename ComputeObjectTag<ResultType>::type,
            Expr<ResultType, ObjectScalarOperation<T1, T2, Op>>>::type {
    Expr() = delete;
    constexpr Expr(const Expr&) = default;
    constexpr Expr(Expr&&) = default;
    constexpr Expr& operator=(const Expr&) = delete;
    constexpr Expr& operator=(Expr&&) = delete;
    //! type return by the access operator(s)
    using value_type = typename ResultType::value_type;
    //! type used by access operator(s)
    using size_type = typename ResultType::size_type;
    //! result type
    using result_type = ResultType;
    //! type of left hand side argument
    using lhs_type = T1;
    //! type of right hand side argument
    using rhs_type = T2;
    //! storage type of left hand side argument
    using lhs_storage_type = ArgumentStorage<T1>;
    //! storage type of right hand side argument
    using rhs_storage_type = ArgumentStorage<T2>;
    /*!
     * \param l : left argument of the binary operation
     * \param r : right argument of the binary operation
     */
    TFEL_HOST_DEVICE constexpr Expr(T1 l, T2 r)
        : a(std::forward<T1>(l)), b(std::forward<T2>(r)) {}  // end of Expr
    /*!
     * array-like access operator
     */
    TFEL_HOST_DEVICE constexpr value_type operator[](const size_type i) const {
      static_assert(isBinaryOperationResultTypeValid<
                        std::invoke_result_t<lhs_storage_type, size_type>,
                        std::add_lvalue_reference_t<rhs_storage_type>, Op>(),
                    "invalid call");
      return Op::apply(this->a(i), this->b);
    }
    /*!
     * multi-dimensional operator acces
     */
    template <typename... Indexes>
    TFEL_HOST_DEVICE constexpr BinaryOperationHandler<
        std::conditional_t<
            std::is_lvalue_reference_v<
                std::invoke_result_t<lhs_storage_type, Indexes...>>,
            std::invoke_result_t<lhs_storage_type, Indexes...>,
            std::invoke_result_t<lhs_storage_type, Indexes...>&&>,
        std::add_lvalue_reference_t<rhs_storage_type>,
        Op>
    operator()(const Indexes... i) const {
      static_assert(isBinaryOperationResultTypeValid<
                        std::invoke_result_t<lhs_storage_type, Indexes...>,
                        std::add_lvalue_reference_t<rhs_storage_type>, Op>(),
                    "invalid call");
      return Op::apply(this->a(i...), this->b);
    }  // end of operator()
   protected:
    lhs_storage_type a;  //<! left  hand side argument of the operation
    rhs_storage_type b;  //<! right hand side argument of the operation
  };  // end of struct Expr<ResultType,ObjectScalarOperation<T1,T2,Op>>

  /*!
   * an Expr object allows the lazy evaluation of a diadic product
   * \tparam ResultType :
   * \tparam T1 : type of the left  argument of the diadic product
   * \tparam T2 : type of the right argument of the diadic product
   */
  template <typename ResultType, typename T1, typename T2>
  struct Expr<ResultType, DiadicProductOperation<T1, T2>>
      : public ExprBase,
        public ConceptRebind<
            typename ComputeObjectTag<ResultType>::type,
            Expr<ResultType, DiadicProductOperation<T1, T2>>>::type {
    Expr() = delete;
    constexpr Expr(const Expr&) = default;
    constexpr Expr(Expr&&) = default;
    constexpr Expr& operator=(const Expr&) = delete;
    constexpr Expr& operator=(Expr&&) = delete;
    //! type return by the access operator(s)
    using value_type = typename ResultType::value_type;
    //! type used by access operator(s)
    using size_type = typename ResultType::size_type;
    //! result type
    using result_type = ResultType;
    //! type of left hand side argument
    using lhs_type = T1;
    //! type of right hand side argument
    using rhs_type = T2;
    //! storage type of left hand side argument
    using lhs_storage_type = ArgumentStorage<T1>;
    //! storage type of right hand side argument
    using rhs_storage_type = ArgumentStorage<T2>;
    /*!
     * \param l : left argument of the binary operation
     * \param r : right argument of the binary operation
     */
    TFEL_HOST_DEVICE constexpr Expr(T1 l, T2 r)
        : a(std::forward<T1>(l)), b(std::forward<T2>(r)) {}  // end of Expr
    /*!
     * access operator
     * \param[in] i : row index
     * \param[in] i : column index
     */
    TFEL_HOST_DEVICE constexpr BinaryOperationHandler<
        std::invoke_result_t<lhs_storage_type, size_type>,
        std::invoke_result_t<rhs_storage_type, size_type>,
        OpMult>
    operator()(const size_type i, const size_type j) const {
      return (this->a(i)) * (this->b(j));
    }

   protected:
    lhs_storage_type a;  //<! left  hand side argument of the operation
    rhs_storage_type b;  //<! right hand side argument of the operation
  };  // end of struct Expr<ResultType,DiadicProductOperation<T1,T2,Op>>

  /*!
   * \brief partial specialisation of the `MathObjectTraits`.
   * We assume that the expression has the same properties that the result
   * of its evaluation.
   */
  template <typename ResultType, typename Operation>
  struct MathObjectTraits<Expr<ResultType, Operation>>
      : public MathObjectTraits<ResultType> {
  };  // end of struct MathObjectTraits<Expr<ResultType, Operation>>

  /*!
   * \brief evaluate an expression
   * \return the computed value
   * \param[in] e: expression
   */
  template <typename ResultType, typename Operation>
  ResultType eval(const Expr<ResultType, Operation>& e) {
    return e;
  }  // end of eval

}  // end of namespace tfel::math

namespace tfel::typetraits {

  /*!
   * \brief generic partial specialisation for an expression
   * \tparam EvaluationResult: result of the evaluation of the expression
   */
  template <typename EvaluationResult, typename Operation>
  struct IsAssignableTo<tfel::math::Expr<EvaluationResult, Operation>,
                        EvaluationResult> {
    //! \brief result
    static constexpr bool cond = true;
  };  // end of struct IsAssignableTo

  /*!
   * \brief generic partial specialisation for an expression
   * \tparam EvaluationResult: result of the evaluation of the expression
   * \tparam DestinationType: destination
   */
  template <typename EvaluationResult,
            typename Operation,
            typename DestinationType>
  struct IsAssignableTo<tfel::math::Expr<EvaluationResult, Operation>,
                        DestinationType> {
    //! \brief result
    static constexpr bool cond =
        isAssignableTo<EvaluationResult, DestinationType>();
  };  // end of struct IsAssignableTo

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_EXPRESSION_EXPR_HXX */
