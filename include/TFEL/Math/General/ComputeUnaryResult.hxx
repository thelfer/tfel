/*!
 * \file   include/TFEL/Math/General/ComputeUnaryResult.hxx
 * \brief  This file declares the ComputeUnaryResult metafunction
 * \author Thomas Helfer
 * \date   13 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_COMPUTEUNARYRESULT_HXX
#define LIB_TFEL_MATH_COMPUTEUNARYRESULT_HXX

#include <type_traits>

#include "TFEL/Math/General/UnaryResultType.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"

namespace tfel::math {

  template <typename TagA, typename TagOp, typename A, typename Op>
  struct ComputeUnaryResult_ {
    using Result = tfel::meta::InvalidType;
    using Handle = tfel::meta::InvalidType;
  };

  //! \brief partial specialisation of ComputeUnaryResult_ for scalars
  template <typename A, typename TagOp, typename Op>
  struct ComputeUnaryResult_<ScalarTag, TagOp, A, Op> {
    typedef std::decay_t<A> A_;
    //! \brief
    using Result = typename UnaryResultType<A_, Op>::type;
    //! \brief no specific handlers for scalars
    using Handle = typename UnaryResultType<A_, Op>::type;
  };  // end of struct ComputeUnaryResult_

  template <typename A, typename Op>
  class ComputeUnaryResult {
    //! \brief a simple alias
    typedef std::decay_t<A> A_;
    typedef typename ComputeObjectTag<A_>::type TagA;
    typedef typename ComputeObjectTag<Op>::type TagOp;

   public:
    typedef typename ComputeUnaryResult_<TagA, TagOp, A, Op>::Result Result;
    typedef typename ComputeUnaryResult_<TagA, TagOp, A, Op>::Handle Handle;
  };

  //! \brief an alias for the result of an unary operation
  template <typename T1, typename Op>
  using UnaryOperationResult = typename ComputeUnaryResult<T1, Op>::Result;
  //! \brief an alias of the handler of an unary operation
  template <typename T1, typename Op>
  using UnaryOperationHandler = typename ComputeUnaryResult<T1, Op>::Handle;
  //! \brief an alias
  template <typename T1, typename Op>
  TFEL_HOST_DEVICE constexpr bool isUnaryOperationResultTypeValid() {
    return !tfel::typetraits::IsInvalid<UnaryOperationResult<T1, Op>>::cond;
  }  // end of isUnaryOperationResultTypeValid

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_COMPUTEUNARYRESULT_HXX */
