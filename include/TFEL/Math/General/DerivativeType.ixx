/*!
 * \file   include/TFEL/Math/General/DerivativeType.ixx
 * \brief
 * \author Thomas Helfer
 * \date   24/06/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_DERIVATIVETYPE_IXX
#define LIB_TFEL_MATH_DERIVATIVETYPE_IXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief an helper metafunction meant to be specialised
     */
    template <typename TagA, typename TagB, typename A, typename B>
    struct DerivativeTypeDispatcher {
      //! \brief result
      using type = tfel::meta::InvalidType;
    };  // end of struct DerivativeTypeDispatcher

    /*!
     * \brief a meta function returning the type of the derivative of
     * a variable of type T1 with respect to a variable of type T2
     * \tparam T1: type
     * \tparam T2: type
     */
    template <typename T1, typename T2>
    struct DerivativeType {
      //! boolean stating if both variables are scalar
      static constexpr const auto are_scalars =
          tfel::typetraits::IsScalar<T1>::cond &&
          tfel::typetraits::IsScalar<T2>::cond;
      //! the result
      using type = typename std::conditional<
          are_scalars,
          BinaryOperationResult<T1, T2, OpDiv>,
          typename tfel::math::DerivativeTypeDispatcher<
              typename ComputeObjectTag<T1>::type,
              typename ComputeObjectTag<T2>::type,
              T1,
              T2>::type>::type;
    };  // end of DerivativeType

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_DERIVATIVETYPE_IXX */
