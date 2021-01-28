/*!pe
 * \file   include/TFEL/Math/General/ComputeBinaryResult.hxx
 * \brief  This file declares the ComputeBinaryResult metafunction
 * \author Thomas Helfer
 * \date   13 Oct 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_COMPUTEBINARYRESULT_HXX
#define LIB_TFEL_COMPUTEBINARYRESULT_HXX

#include <type_traits>
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/Math/Forward/Expr.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"

namespace tfel::math {

  struct OpPlus;
  struct OpMinus;
  struct OpMult;
  struct OpDiv;

  /*!
   * \class ComputeBinaryOperationHandler
   * \brief A helper class for ComputeBinaryResult.
   * This default version returns InvalidType both for
   * Result and Handle.
   * \tparam TagA, tag of the type of the first
   * argument of the operation.
   * \tparam TagB, tag of the type of second
   * argument of the operation.
   * \tparam A, type of the first argument of the
   * operation.
   * \tparam B, type of the second argument of the
   * operation.
   * \tparam Op, operation.
   * \return Result, type of the result of the operation.
   * \return Handle, type that will handle the operation.
   * \see ComputeBinaryResult.
   * \see ResultType.
   * \see ComputeObjectTag.
   */
  template <typename TagA, typename TagB, typename A, typename B, typename Op>
  struct ComputeBinaryOperationHandler {
    //! \brief result of the binary operation.
    using Result = tfel::meta::InvalidType;
    //! \brief type that will handle the operation.
    using Handle = tfel::meta::InvalidType;
  };  // end of ComputeBinaryOperationHandler

  /*!
   * Partial Specialisation of ComputeBinaryOperationHandler for scalars.
   * In that case, Result.
   * \tparam A, type of the first argument of the
   * operation.
   * \tparam B, type of the second argument of the
   * operation.
   * \tparam Op, operation.
   */
  template <typename A, typename B>
  struct ComputeBinaryOperationHandler<ScalarTag, ScalarTag, A, B, OpPlus> {
    //! \brief the result.
    using Result = result_type<std::decay_t<A>, std::decay_t<B>, OpPlus>;
    //! \brief the handle.
    using Handle = Result;
  };  // end of ComputeBinaryOperationHandler.

  template <typename A, typename B>
  struct ComputeBinaryOperationHandler<ScalarTag, ScalarTag, A, B, OpMinus> {
    //! \brief the result.
    using Result = result_type<std::decay_t<A>, std::decay_t<B>, OpMinus>;
    //! \brief the handle.
    using Handle = Result;
  };  // end of ComputeBinaryOperationHandler.

  template <typename A, typename B>
  struct ComputeBinaryOperationHandler<ScalarTag, ScalarTag, A, B, OpMult> {
    //! \brief the result.
    using Result = result_type<std::decay_t<A>, std::decay_t<B>, OpMult>;
    //! \brief the handle.
    using Handle = Result;
  };  // end of ComputeBinaryOperationHandler.

  template <typename A, typename B>
  struct ComputeBinaryOperationHandler<ScalarTag, ScalarTag, A, B, OpDiv> {
    //! \brief the result.
    using Result = result_type<std::decay_t<A>, std::decay_t<B>, OpDiv>;
    //! \brief the handle.
    using Handle = Result;
  };  // end of ComputeBinaryOperationHandler.

  /*!
   * \brief partial specialisation of ComputeBinaryOperationHandler for
   * scalar-vector operations.
   */
  template <typename MathObjectTag, typename A, typename B>
  struct ComputeBinaryOperationHandler<ScalarTag, MathObjectTag, A, B, OpMult> {
    struct DummyHandle {};
    using MathObjectTagTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<std::decay_t<A>, MathObjectTagTypeB, OpMult>;
    using Handle =
        std::conditional_t<tfel::typetraits::isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, ScalarObjectOperation<A, B, OpMult>>>;
  };

  /*!
   * \brief partial specialisation of ComputeBinaryOperationHandler for
   * vector-scalar operations.
   */
  template <typename MathObjectTag, typename A, typename B>
  class ComputeBinaryOperationHandler<MathObjectTag, ScalarTag, A, B, OpMult> {
    struct DummyHandle {};
    using MathObjectTagTypeA = EvaluationResult<A>;

   public:
    using Result = result_type<MathObjectTagTypeA, B, OpMult>;
    using Handle =
        std::conditional_t<tfel::typetraits::isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, ObjectScalarOperation<A, B, OpMult>>>;
  };

  /*!
   * \brief partial specialisation of ComputeBinaryOperationHandler for
   * vector-scalar operations
   */
  template <typename MathObjectTag, typename A, typename B>
  class ComputeBinaryOperationHandler<MathObjectTag, ScalarTag, A, B, OpDiv> {
    struct DummyHandle {};
    using MathObjectTagTypeA = EvaluationResult<A>;

   public:
    using Result = result_type<MathObjectTagTypeA, B, OpDiv>;
    using Handle =
        std::conditional_t<tfel::typetraits::isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, ObjectScalarOperation<A, B, OpDiv>>>;
  };

  /*!
   * \brief partial specialisation of ComputeBinaryOperationHandler for vector's
   * operation
   */
  template <typename MathObjectTag, typename A, typename B>
  class ComputeBinaryOperationHandler<MathObjectTag,
                                      MathObjectTag,
                                      A,
                                      B,
                                      OpPlus> {
    struct DummyHandle {};
    using MathObjectTagTypeA = EvaluationResult<A>;
    using MathObjectTagTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<MathObjectTagTypeA, MathObjectTagTypeB, OpPlus>;
    using Handle =
        std::conditional_t<tfel::typetraits::isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, BinaryOperation<A, B, OpPlus>>>;
  };

  /*!
   * \brief partial specialisation of ComputeBinaryOperationHandler for vector's
   * operation
   */
  template <typename MathObjectTag, typename A, typename B>
  class ComputeBinaryOperationHandler<MathObjectTag,
                                      MathObjectTag,
                                      A,
                                      B,
                                      OpMinus> {
    struct DummyHandle {};
    using MathObjectTagTypeA = EvaluationResult<A>;
    using MathObjectTagTypeB = EvaluationResult<B>;

   public:
    using Result = result_type<MathObjectTagTypeA, MathObjectTagTypeB, OpMinus>;
    using Handle =
        std::conditional_t<tfel::typetraits::isInvalid<Result>(),
                           DummyHandle,
                           Expr<Result, BinaryOperation<A, B, OpMinus>>>;
  };

  /*!
   * \class ComputeBinaryResult
   * A metafunction to manage binary operations.
   * Binary operations are central part of tfel::math which makes
   * heavy use of expression templates.
   * Expression templates differentiates:
   * - the result of a binary operation, which in our view is the
   * type to what the result of the binary operation will be
   * affected.
   * - the type created by the binary operation, the handle, which
   * is often a temporary that should normally be thrown away by the
   * compiler.
   * This is not true for scalars where the result and the handle
   * are the same are the same.
   * The computation of the Result and Handle types is normally be
   * delegated to ComputeBinaryOperationHandler metafunction which is called
   * by ComputeBinaryResult class after having computed the tag
   * associated with each arguments of the binary operations thanks
   * to the ComputeObjectTag.
   * \tparam A, type of the first argument of the
   * operation.
   * \tparam B, type of the second argument of the
   * operation.
   * \tparam Op, operation.
   * \return Result, type of the result of the operation.
   * \return Handle, type that will handle the operation.
   * \see ResultType.
   * \see ComputeObjectTag.
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult {
    //! a simple alias
    using A_ = std::decay_t<A>;
    //! a simple alias
    using B_ = std::decay_t<B>;
    //! tag of the left hand side
    typedef typename ComputeObjectTag<A_>::type TagA;
    //! tag of the right hand side
    typedef typename ComputeObjectTag<B_>::type TagB;

   public:
    //! call to ComputeBinaryOperationHandler to get the Result type.
    typedef typename ComputeBinaryOperationHandler<TagA, TagB, A, B, Op>::Result
        Result;
    //! call to ComputeBinaryOperationHandler to get the Handle type.
    typedef typename ComputeBinaryOperationHandler<TagA, TagB, A, B, Op>::Handle
        Handle;
  };  // end of ComputeBinaryResult.

  //! an alias for the result of an binary operation
  template <typename T1, typename T2, typename Op>
  using BinaryOperationResult =
      typename ComputeBinaryResult<T1, T2, Op>::Result;
  //! a metafunction returning true if the result of the binary operation is
  //! valid
  template <typename T1, typename T2, typename Op>
  using isBinaryOperationResultTypeValid = std::integral_constant<
      bool,
      !tfel::typetraits::IsInvalid<BinaryOperationResult<T1, T2, Op>>::cond>;
  //! an alias of the handler of an binary operation
  template <typename T1, typename T2, typename Op>
  using BinaryOperationHandler =
      typename ComputeBinaryResult<T1, T2, Op>::Handle;

}  // namespace tfel::math

#endif /* LIB_TFEL_COMPUTEBINARYRESULT_HXX */
