/*!pe
 * \file   include/TFEL/Math/General/ComputeBinaryResult.hxx
 * \brief  This file declares the ComputeBinaryResult metafunction
 * \author Helfer Thomas
 * \date   13 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_COMPUTEBINARYRESULT_H_
#define LIB_TFEL_COMPUTEBINARYRESULT_H_

#include <type_traits>
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"

namespace tfel {

  namespace math {

    /*!
     * \class ComputeBinaryResult_
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
    struct ComputeBinaryResult_ {
      /*!
       * Result of the binary operation.
       */
      typedef tfel::meta::InvalidType Result;
      /*!
       * Type that will handle the operation.
       */
      typedef tfel::meta::InvalidType Handle;
    };  // end of ComputeBinaryResult_

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
     * delegated to ComputeBinaryResult_ metafunction which is called
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
      using A_ = typename std::decay<A>::type;
      //! a simple alias
      using B_ = typename std::decay<B>::type;
      //! tag of the left hand side
      typedef typename ComputeObjectTag<A_>::type TagA;
      //! tag of the right hand side
      typedef typename ComputeObjectTag<B_>::type TagB;

     public:
      //! call to ComputeBinaryResult_ to get the Result type.
      typedef
          typename ComputeBinaryResult_<TagA, TagB, A, B, Op>::Result Result;
      //! call to ComputeBinaryResult_ to get the Handle type.
      typedef
          typename ComputeBinaryResult_<TagA, TagB, A, B, Op>::Handle Handle;
    };  // end of ComputeBinaryResult.

    /*!
     * Partial Specialisation of ComputeBinaryResult_ for scalars.
     * In that case, Result.
     * \tparam A, type of the first argument of the
     * operation.
     * \tparam B, type of the second argument of the
     * operation.
     * \tparam Op, operation.
     */
    template <typename A, typename B, typename Op>
    class ComputeBinaryResult_<ScalarTag, ScalarTag, A, B, Op> {
     private:
      //! bare type
      typedef typename std::decay<A>::type A_;
      //! bare type
      typedef typename std::decay<B>::type B_;

     public:
      //! the result.
      typedef typename ResultType<A_, B_, Op>::type Result;
      //! the handle.
      typedef typename ResultType<A_, B_, Op>::type Handle;
    };  // end of ComputeBinaryResult_.

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

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_COMPUTEBINARYRESULT_H_ */
