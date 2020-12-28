/*!
 * \file   include/TFEL/Math/ExpressionTemplates/Expr.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 févr. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_EXPRESSION_EXPR_HXX
#define LIB_TFEL_MATH_EXPRESSION_EXPR_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/ResultOf.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"
#include "TFEL/Math/General/ComputeBinaryResult.hxx"

namespace tfel::math {

  /*!
   * an helper structure used for the partial specialisation of the
   * Expr class.
   * \tparam T  : type of unary operation argument
   * \tparam Op : the unary operation
   */
  template <typename T1, typename Op>
  struct UnaryOperation;
  /*!
   * an helper structure used for the partial specialisation of the
   * Expr class.
   * \tparam T1 : type of the left  argument of the binary operation
   * \tparam T2 : type of the right argument of the binary operation
   * \tparam Op : the binary operation
   */
  template <typename T1, typename T2, typename Op>
  struct BinaryOperation;
  /*!
   * an helper structure used for the partial specialisation of the
   * Expr class.
   * \tparam T1 : type of the left  argument of the binary operation
   * \tparam T2 : type of the right argument of the binary operation
   * \tparam Op : the binary operation
   * \pre T1 must be a scalar
   */
  template <typename T1, typename T2, typename Op>
  struct ScalarObjectOperation;
  /*!
   * an helper structure used for the partial specialisation of the
   * Expr class.
   * \tparam T1 : type of the left  argument of the binary operation
   * \tparam T2 : type of the right argument of the binary operation
   * \tparam Op : the binary operation
   * \pre T1 must be a scalar
   */
  template <typename T1, typename T2, typename Op>
  struct ObjectScalarOperation;
  /*!
   * an helper structure used for the partial specialisation of the
   * Expr class.
   * \tparam T1 : type of the left  argument of the diadic product
   * \tparam T2 : type of the right argument of the diadic product
   */
  template <typename T1, typename T2>
  struct DiadicProductOperation;

  /*!
   * \brief an helper class
   */
  struct ExprBase {
   protected:
    /*!
     * a simple metafunction defining how an argument of an
     * operation is stored in the Expr class.
     */
    template <typename T>
    using ArgumentStorage = std::conditional_t<std::is_rvalue_reference_v<T>,
                                               const std::decay_t<T>,
                                               const std::decay_t<T>&>;
  };  // end of strut ExprBase

  /*!
   * \brief An helper class which defines array-like access operator
   * by relying on the operator() of the derived class.
   * \tparam Child : child class
   */
  template <typename Child>
  struct ExprWithArrayAccessOperator {
    // /*!
    //  * \brief array like access operator
    //  * \param[in] i : index
    //  */
    // auto operator[](const typename Child::size_type i) const
    // 	-> decltype(std::declval<const Child>()(i))
    // {
    // 	return static_cast<const Child&>(this).operator()(i);
    // }
    // /*!
    //  * \brief array like access operator
    //  * \param[in] i : index
    //  */
    // auto operator[](const size_type i)
    // 	-> decltype(std::declval<Child>()(i))
    // {
    // 	return static_cast<const Child&>(this).operator()(i);
    // }
  };  // end of struct ExprWithArrayAccessOperator

  /*!
   * \brief an Expr object allows the lazy evaluation of a mathematical
   * operation.
   * \tparam ResultType : the type of the result of the operation
   * \tparam Operation  : Operation to be performed
   *
   * Partial specialisation are provided for:
   * - unary operations (negation, function call)
   * - binary operations
   */
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
    explicit TFEL_MATH_INLINE Expr(Args&&... args)
        : Operation(std::forward<Args>(args)...) {}
    /*!
     * \brief array like access operator
     * \param[in] i : index
     * \note This operator is always defined, even tough it might not be
     * valid
     */
    value_type operator[](const size_type i) const {
      // may not be defined
      static_assert(tfel::meta::IsConstCallable<Expr, size_type>::cond,
                    "Expr is not callable");
      return Operation::operator()(i);
    }
    /*!
     * \brief array like access operator
     * \param[in] i : index
     * \note This operator is always defined, even tough it might not be
     * valid
     */
    value_type& operator[](const size_type i) {
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
    Expr(const Expr&) = default;
    Expr(Expr&&) = default;
    Expr& operator=(const Expr&) = delete;
    Expr& operator=(Expr&&) = delete;
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
     * \param l : left argument of the binary operation
     * \param r : right argument of the binary operation
     */
    TFEL_MATH_INLINE Expr(T1 l) : a(l) {}  // end of Expr
    /*!
     * multidimensional access operator
     */
    template <typename... Indexes>
    TFEL_MATH_INLINE auto operator()(const Indexes... i) const
        -> UnaryOperationHandler<
            std::conditional_t<
                std::is_lvalue_reference_v<
                    tfel::meta::result_of<argument_storage_type, Indexes...>>,
                tfel::meta::result_of<argument_storage_type, Indexes...>,
                tfel::meta::result_of<argument_storage_type, Indexes...>&&>,
            Op> {
      static_assert(
          isUnaryOperationResultTypeValid<
              tfel::meta::result_of<argument_storage_type, Indexes...>,
              Op>::value,
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
    Expr(const Expr&) = default;
    Expr(Expr&&) = default;
    Expr& operator=(const Expr&) = delete;
    Expr& operator=(Expr&&) = delete;
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
    TFEL_MATH_INLINE Expr(T1 l, T2 r) : a(l), b(r) {}  // end of Expr
    /*!
     * array-like access operator
     */
    TFEL_MATH_INLINE value_type operator[](const size_type i) const {
      static_assert(
          isBinaryOperationResultTypeValid<
              tfel::meta::result_of<lhs_storage_type, size_type>,
              tfel::meta::result_of<rhs_storage_type, size_type>, Op>::value,
          "invalid call to unary operation operator[]");
      return Op::apply(this->a(i), this->b(i));
    }
    /*!
     * multi-dimensional operator acces
     */
    template <typename... Indexes>
    TFEL_MATH_INLINE BinaryOperationHandler<
        std::conditional_t<
            std::is_lvalue_reference_v<
                tfel::meta::result_of<lhs_storage_type, Indexes...>>,
            tfel::meta::result_of<lhs_storage_type, Indexes...>,
            tfel::meta::result_of<lhs_storage_type, Indexes...>&&>,
        std::conditional_t<
            std::is_lvalue_reference_v<
                tfel::meta::result_of<rhs_storage_type, Indexes...>>,
            tfel::meta::result_of<rhs_storage_type, Indexes...>,
            tfel::meta::result_of<rhs_storage_type, Indexes...>&&>,
        Op>
    operator()(const Indexes... i) const {
      static_assert(
          isBinaryOperationResultTypeValid<
              tfel::meta::result_of<lhs_storage_type, Indexes...>,
              tfel::meta::result_of<rhs_storage_type, Indexes...>, Op>::value,
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
    Expr(const Expr&) = default;
    Expr(Expr&&) = default;
    Expr& operator=(const Expr&) = delete;
    Expr& operator=(Expr&&) = delete;
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
    TFEL_MATH_INLINE Expr(T1 l, T2 r) : a(l), b(r) {}  // end of Expr
    /*!
     * \brief array-like access operator
     */
    TFEL_MATH_INLINE value_type operator[](const size_type i) const {
      static_assert(
          isBinaryOperationResultTypeValid<
              std::add_lvalue_reference_t<lhs_storage_type>,
              tfel::meta::result_of<rhs_storage_type, size_type>, Op>::value,
          "invalid call");
      return Op::apply(this->a, this->b(i));
    }
    /*!
     * \brief multi-dimensional operator acces
     */
    template <typename... Indexes>
    TFEL_MATH_INLINE BinaryOperationHandler<
        std::add_lvalue_reference_t<lhs_storage_type>,
        std::conditional_t<
            std::is_lvalue_reference_v<
                tfel::meta::result_of<rhs_storage_type, Indexes...>>,
            tfel::meta::result_of<rhs_storage_type, Indexes...>,
            tfel::meta::result_of<rhs_storage_type, Indexes...>&&>,
        Op>
    operator()(const Indexes... i) const {
      static_assert(
          isBinaryOperationResultTypeValid<
              std::add_lvalue_reference_t<lhs_storage_type>,
              tfel::meta::result_of<rhs_storage_type, Indexes...>, Op>::value,
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
    Expr(const Expr&) = default;
    Expr(Expr&&) = default;
    Expr& operator=(const Expr&) = delete;
    Expr& operator=(Expr&&) = delete;
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
    TFEL_MATH_INLINE Expr(T1 l, T2 r) : a(l), b(r) {}  // end of Expr
    /*!
     * array-like access operator
     */
    TFEL_MATH_INLINE value_type operator[](const size_type i) const {
      static_assert(
          isBinaryOperationResultTypeValid<
              tfel::meta::result_of<lhs_storage_type, size_type>,
              std::add_lvalue_reference_t<rhs_storage_type>, Op>::value,
          "invalid call");
      return Op::apply(this->a(i), this->b);
    }
    /*!
     * multi-dimensional operator acces
     */
    template <typename... Indexes>
    TFEL_MATH_INLINE BinaryOperationHandler<
        std::conditional_t<
            std::is_lvalue_reference_v<
                tfel::meta::result_of<lhs_storage_type, Indexes...>>,
            tfel::meta::result_of<lhs_storage_type, Indexes...>,
            tfel::meta::result_of<lhs_storage_type, Indexes...>&&>,
        std::add_lvalue_reference_t<rhs_storage_type>,
        Op>
    operator()(const Indexes... i) const {
      static_assert(
          isBinaryOperationResultTypeValid<
              tfel::meta::result_of<lhs_storage_type, Indexes...>,
              std::add_lvalue_reference_t<rhs_storage_type>, Op>::value,
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
    Expr(const Expr&) = default;
    Expr(Expr&&) = default;
    Expr& operator=(const Expr&) = delete;
    Expr& operator=(Expr&&) = delete;
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
    TFEL_MATH_INLINE Expr(T1 l, T2 r) : a(l), b(r) {}  // end of Expr
    /*!
     * access operator
     * \param[in] i : row index
     * \param[in] i : column index
     */
    TFEL_MATH_INLINE
    BinaryOperationHandler<tfel::meta::result_of<lhs_storage_type, size_type>,
                           tfel::meta::result_of<rhs_storage_type, size_type>,
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
   * \brief a metafunction giving the result of a the evaluation of an object.
   * \tparam ObjectType: type of the object evaluated
   */
  template <typename ObjectType>
  struct ResultOfEvaluation {
    //! \brief result of the metafunction
    using type = ObjectType;
  };  // end of struct ResultOfEvaluation

  /*!
   * \brief partial specialisation of the `ResultOfEvaluation` class for
   * objects representing a lazy expression. \tparam ResultType: result of the
   * evaluation of the lazy expression \tparam Operation: operation performed by
   * the lazy expression
   */
  template <typename ResultType, typename Operation>
  struct ResultOfEvaluation<Expr<ResultType, Operation>> {
    //! \brief result of the metafunction
    using type = ResultType;
  };  // end of struct ResultOfEvaluation

  //! \brief a simple alias
  template <typename ObjectType>
  using EvaluationResult =
      typename ResultOfEvaluation<std::decay_t<ObjectType>>::type;

  /*!
   * \brief evaluate an expression
   * \return the computed value
   * \param[in] e: expression
   */
  template <typename ResultType, typename Operation>
  ResultType eval(const Expr<ResultType, Operation>& e) {
    return {e};
  }  // end of eval

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_EXPRESSION_EXPR_HXX */
