/*!
 * \file   include/TFEL/Math/Forward/Expr.hxx
 * \brief
 * \author Thomas Helfer
 * \date   18/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_EXPR_HXX
#define LIB_TFEL_MATH_FORWARD_EXPR_HXX

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
  struct Expr;
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
   * objects representing a lazy expression.
   * \tparam ResultType: result of the evaluation of the lazy expression
   * \tparam Operation: operation performed by
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

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_EXPR_HXX */
