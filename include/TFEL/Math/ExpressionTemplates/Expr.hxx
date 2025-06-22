/*!
 * \file   include/TFEL/Math/ExpressionTemplates/Expr.hxx
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

#ifndef LIB_TFEL_MATH_EXPRESSION_EXPR_H_
#define LIB_TFEL_MATH_EXPRESSION_EXPR_H_

#include <type_traits>
#include "TFEL/Metaprogramming/ResultOf.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"
#include "TFEL/Math/General/ComputeBinaryResult.hxx"

namespace tfel {

  namespace math {

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
     * an helper class
     */
    struct ExprBase {
     protected:
      /*!
       * a simple metafunction defining how an argument of an
       * operation is stored in the Expr class.
       */
      template <typename T>
      using ArgumentStorage =
          typename std::conditional<std::is_rvalue_reference<T>::value,
                                    const typename std::decay<T>::type,
                                    const typename std::decay<T>::type&>::type;
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
      typedef typename Operation::value_type value_type;
      //! a simple alias
      typedef typename Operation::size_type size_type;
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
            std::is_same<decltype(Operation::operator()(i)),
                         value_type&>::value,
            "unexpected return value for Operation::operator(size_type)");
        return Operation::operator()(i);
      }

      using Operation::operator();
      using Operation::operator=;
    };

    /*!
     * an Expr object allows the lazy evaluation of an operation.
     * \tparam ResultType :
     * \tparam T1 : type of the left  argument of the binary operation
     * \tparam T2 : type of the right argument of the binary operation
     * \tparam Op : the binary operation
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
              typename std::conditional<
                  std::is_lvalue_reference<
                      typename tfel::meta::ResultOf<argument_storage_type,
                                                    Indexes...>::type>::value,
                  typename tfel::meta::ResultOf<argument_storage_type,
                                                Indexes...>::type,
                  typename tfel::meta::ResultOf<argument_storage_type,
                                                Indexes...>::type&&>::type,
              Op> {
        static_assert(isUnaryOperationResultTypeValid<
                          typename tfel::meta::ResultOf<argument_storage_type,
                                                        Indexes...>::type,
                          Op>::value,
                      "invalid call to unary operation operator()");
        return Op::apply(this->a(i...));
      }  // end of operator()
     protected:
      argument_storage_type a;  //<! argument of the operation
    };  // end of struct Expr<ResultType,UnaryOperation<T1,Op> >

    /*!
     * an Expr object allows the lazy evaluation of a binary
     * operation.
     * \tparam ResultType :
     * \tparam T1 : type of the left  argument of the binary operation
     * \tparam T2 : type of the right argument of the binary operation
     * \tparam Op : the binary operation
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
        static_assert(isBinaryOperationResultTypeValid<
                          typename tfel::meta::ResultOf<lhs_storage_type,
                                                        size_type>::type,
                          typename tfel::meta::ResultOf<rhs_storage_type,
                                                        size_type>::type,
                          Op>::value,
                      "invalid call to unary operation operator[]");
        return Op::apply(this->a(i), this->b(i));
      }
      /*!
       * multi-dimensional operator acces
       */
      template <typename... Indexes>
      TFEL_MATH_INLINE BinaryOperationHandler<
          typename std::conditional<
              std::is_lvalue_reference<
                  typename tfel::meta::ResultOf<lhs_storage_type,
                                                Indexes...>::type>::value,
              typename tfel::meta::ResultOf<lhs_storage_type, Indexes...>::type,
              typename tfel::meta::ResultOf<lhs_storage_type,
                                            Indexes...>::type&&>::type,
          typename std::conditional<
              std::is_lvalue_reference<
                  typename tfel::meta::ResultOf<rhs_storage_type,
                                                Indexes...>::type>::value,
              typename tfel::meta::ResultOf<rhs_storage_type, Indexes...>::type,
              typename tfel::meta::ResultOf<rhs_storage_type,
                                            Indexes...>::type&&>::type,
          Op>
      operator()(const Indexes... i) const {
        static_assert(isBinaryOperationResultTypeValid<
                          typename tfel::meta::ResultOf<lhs_storage_type,
                                                        Indexes...>::type,
                          typename tfel::meta::ResultOf<rhs_storage_type,
                                                        Indexes...>::type,
                          Op>::value,
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
       * array-like access operator
       */
      TFEL_MATH_INLINE value_type operator[](const size_type i) const {
        static_assert(
            isBinaryOperationResultTypeValid<
                typename std::add_lvalue_reference<lhs_storage_type>::type,
                typename tfel::meta::ResultOf<rhs_storage_type,
                                              size_type>::type,
                Op>::value,
            "invalid call");
        return Op::apply(this->a, this->b(i));
      }
      /*!
       * multi-dimensional operator acces
       */
      template <typename... Indexes>
      TFEL_MATH_INLINE BinaryOperationHandler<
          typename std::add_lvalue_reference<lhs_storage_type>::type,
          typename std::conditional<
              std::is_lvalue_reference<
                  typename tfel::meta::ResultOf<rhs_storage_type,
                                                Indexes...>::type>::value,
              typename tfel::meta::ResultOf<rhs_storage_type, Indexes...>::type,
              typename tfel::meta::ResultOf<rhs_storage_type,
                                            Indexes...>::type&&>::type,
          Op>
      operator()(const Indexes... i) const {
        static_assert(
            isBinaryOperationResultTypeValid<
                typename std::add_lvalue_reference<lhs_storage_type>::type,
                typename tfel::meta::ResultOf<rhs_storage_type,
                                              Indexes...>::type,
                Op>::value,
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
                typename tfel::meta::ResultOf<lhs_storage_type,
                                              size_type>::type,
                typename std::add_lvalue_reference<rhs_storage_type>::type,
                Op>::value,
            "invalid call");
        return Op::apply(this->a(i), this->b);
      }
      /*!
       * multi-dimensional operator acces
       */
      template <typename... Indexes>
      TFEL_MATH_INLINE BinaryOperationHandler<
          typename std::conditional<
              std::is_lvalue_reference<
                  typename tfel::meta::ResultOf<lhs_storage_type,
                                                Indexes...>::type>::value,
              typename tfel::meta::ResultOf<lhs_storage_type, Indexes...>::type,
              typename tfel::meta::ResultOf<lhs_storage_type,
                                            Indexes...>::type&&>::type,
          typename std::add_lvalue_reference<rhs_storage_type>::type,
          Op>
      operator()(const Indexes... i) const {
        static_assert(
            isBinaryOperationResultTypeValid<
                typename tfel::meta::ResultOf<lhs_storage_type,
                                              Indexes...>::type,
                typename std::add_lvalue_reference<rhs_storage_type>::type,
                Op>::value,
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
      BinaryOperationHandler<
          typename tfel::meta::ResultOf<lhs_storage_type, size_type>::type,
          typename tfel::meta::ResultOf<rhs_storage_type, size_type>::type,
          OpMult>
      operator()(const size_type i, const size_type j) const

      {
        return (this->a(i)) * (this->b(j));
      }

     protected:
      lhs_storage_type a;  //<! left  hand side argument of the operation
      rhs_storage_type b;  //<! right hand side argument of the operation
    };  // end of struct Expr<ResultType,DiadicProductOperation<T1,T2,Op>>

    /*!
     * \brief evaluate an expression
     * \return the computed value
     * \param[in] e: expression
     */
    template <typename ResultType, typename Operation>
    ResultType eval(const Expr<ResultType, Operation>& e) {
      return {e};
    }  // end of eval

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_EXPRESSION_EXPR_H_ */
