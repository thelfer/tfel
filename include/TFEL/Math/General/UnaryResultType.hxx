/*!
 * \file   include/TFEL/Math/General/UnaryResultType.hxx
 * \brief  This file declares the UnaryResultType class
 * \author Helfer Thomas
 * \date   13 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UNARYRESULTTYPE_H_
#define LIB_TFEL_UNARYRESULTTYPE_H_

#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"

namespace tfel {

  namespace math {

    /*!
     * \class UnaryResultType_
     * \brief A helper class for UnaryResultType.
     * This default version returns InvalidType both for
     * Result and Handle.
     * \tparam TagA, tag of the type of the first
     * argument of the operation.
     * \tparam A, type of the first argument of the
     * operation.
     * \tparam Op, operation.
     * \return type, type of the result of the operation.
     * \see UnaryResultType.
     * \see ComputeObjectTag.
     */
    template <typename TagA, typename TagOp, typename A, typename Op>
    struct UnaryResultType_ {
      //! result of the binary operation.
      typedef tfel::meta::InvalidType type;
    };  // end of UnaryResultType_
    /*!
     * \class UnaryResultType
     * This metafunction returns the type of the result of
     * an unary operation with a given type.
     * By default, returns InvalidType.
     * \tparam A, type on which the operation is performed.
     * \tparam Op, unary operation performed.
     * \return type, the type of the result
     * \see tfel::meta::InvalidType.
     * \note arguments A and Op should be reversed.
     */
    template <typename A, typename Op>
    class UnaryResultType {
      //! bare argument type
      using A_ = typename std::decay<A>::type;
      //! Tag of the object A
      typedef typename ComputeObjectTag<A_>::type TagA;
      typedef typename ComputeObjectTag<Op>::type TagOp;

     public:
      //! result
      typedef typename UnaryResultType_<TagA, TagOp, A_, Op>::type type;
    };  // end of UnaryResultType

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_UNARYRESULTTYPE_H_ */
