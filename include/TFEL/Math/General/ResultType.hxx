/*!
 * \file   include/TFEL/Math/General/ResultType.hxx
 * \brief  This file declares the ResultType class.
 * \author Helfer Thomas
 * \date   24 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_RESULT_TYPE_HXX_
#define LIB_TFEL_RESULT_TYPE_HXX_

#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"

namespace tfel {

  namespace math {

    /*
     * \class ResultType_
     * \brief A helper class for ResultType.
     * This default version returns InvalidType both for
     * Result and Handle.
     * \param typename TagA, tag of the type of the first
     * argument of the operation.
     * \param typename TagB, tag of the type of second
     * argument of the operation.
     * \param typename A, type of the first argument of the
     * operation.
     * \param typename B, type of the second argument of the
     * operation.
     * \param typename Op, operation.
     * \return type, type of the result of the operation.
     * \see ResultType.
     * \see ComputeObjectTag.
     */
    template <typename TagA, typename TagB, typename A, typename B, typename Op>
    struct ResultType_ {
      /*
       * Result of the binary operation.
       */
      typedef tfel::meta::InvalidType type;
    };  // end of ResultType_

    /*
     * \class ResultType
     * This metafunction gives the result of a binary operation.
     * This class is meant to be specialised on a per objet basis.
     * (in comparison, ComputeBinaryResult is meant to be specialised
     * on a per concept basis)
     * The helper class ResultType_ is only convenient to declare
     * scalar to objet operations.
     * \param typename A, type of the first argument of the
     * operation.
     * \param typename B, type of the second argument of the
     * operation.
     * \param typename Op, operation.
     * \return type, type of the result.
     */
    template <typename A, typename B, typename Op>
    struct ResultType {
      //! a simple alias
      using A_ = typename std::decay<A>::type;
      //! a simple alias
      using B_ = typename std::decay<B>::type;
      //! Tag of the object A
      using TagA = typename ComputeObjectTag<A_>::type;
      //! Tag of the object B
      using TagB = typename ComputeObjectTag<B_>::type;
      //! Result
      using type = typename ResultType_<TagA, TagB, A_, B_, Op>::type;
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_RESULT_TYPE_HXX_ */
