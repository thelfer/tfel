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

#ifndef LIB_TFEL_COMPUTEUNARYRESULT_HXX
#define LIB_TFEL_COMPUTEUNARYRESULT_HXX

#include <type_traits>

#include "TFEL/Math/General/UnaryResultType.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"

namespace tfel {

  namespace math {

    template <typename TagA, typename TagOp, typename A, typename Op>
    struct ComputeUnaryResult_ {
      using Result = tfel::meta::InvalidType;
      using Handle = tfel::meta::InvalidType;
    };

    //! Partial Specialisation of ComputeUnaryResult_ for scalars
    template <typename A, typename TagOp, typename Op>
    struct ComputeUnaryResult_<ScalarTag, TagOp, A, Op> {
      typedef typename std::decay<A>::type A_;
      typedef typename UnaryResultType<A_, Op>::type Result;
      typedef typename UnaryResultType<A_, Op>::type Handle;
    };

    template <typename A, typename Op>
    class ComputeUnaryResult {
      //! a simple alias
      typedef typename std::decay<A>::type A_;
      typedef typename ComputeObjectTag<A_>::type TagA;
      typedef typename ComputeObjectTag<Op>::type TagOp;

     public:
      typedef typename ComputeUnaryResult_<TagA, TagOp, A, Op>::Result Result;
      typedef typename ComputeUnaryResult_<TagA, TagOp, A, Op>::Handle Handle;
    };

    //! an alias for the result of an unary operation
    template <typename T1, typename Op>
    using UnaryOperationResult = typename ComputeUnaryResult<T1, Op>::Result;
    //! an alias of the handler of an unary operation
    template <typename T1, typename Op>
    using UnaryOperationHandler = typename ComputeUnaryResult<T1, Op>::Handle;
    //! an alias
    template <typename T1, typename Op>
    using isUnaryOperationResultTypeValid = std::integral_constant<
        bool,
        !tfel::typetraits::IsInvalid<UnaryOperationResult<T1, Op>>::cond>;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_COMPUTEUNARYRESULT_HXX */
