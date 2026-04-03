/*!
 * \file   ConvertToTangentModuli.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CONVERTTOTANGENTMODULI_HXX
#define LIB_TFEL_MATH_CONVERTTOTANGENTMODULI_HXX

#include <type_traits>
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include "TFEL/Math/t2tost2.hxx"

namespace tfel::math {

  template <T2toST2Concept T2toST2Type, TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto convertToTangentModuli(
      const T2toST2Type& K, const TensorType& F) noexcept  //
      requires(getSpaceDimension<T2toST2Type>() ==
               getSpaceDimension<TensorType>()) {
    constexpr auto N = getSpaceDimension<T2toST2Type>();
    using value_type = result_type<numeric_type<T2toST2Type>,
                                   numeric_type<TensorType>, OpMult>;
    auto C = st2tost2<N, value_type>{};
    if constexpr (N == 1) {
      C(0, 0) = K(0, 0) * F(0);
      C(0, 1) = K(0, 1) * F(1);
      C(0, 2) = K(0, 2) * F(2);
      C(1, 0) = K(1, 0) * F(0);
      C(1, 1) = K(1, 1) * F(1);
      C(1, 2) = K(1, 2) * F(2);
      C(2, 0) = K(2, 0) * F(0);
      C(2, 1) = K(2, 1) * F(1);
      C(2, 2) = K(2, 2) * F(2);
    } else if constexpr (N == 2) {
      constexpr const auto icste2 = Cste<value_type>::isqrt2 / 2;
      C(0, 0) = K(0, 0) * F(0) + K(0, 3) * F(3);
      C(0, 3) =
          (K(0, 0) * F(4) + K(0, 3) * F(1) + K(0, 4) * F(0) + K(0, 1) * F(3)) *
          icste2;
      C(0, 1) = K(0, 4) * F(4) + K(0, 1) * F(1);
      C(0, 2) = K(0, 2) * F(2);
      C(3, 0) = K(3, 0) * F(0) + K(3, 3) * F(3);
      C(3, 3) =
          (K(3, 0) * F(4) + K(3, 3) * F(1) + K(3, 4) * F(0) + K(3, 1) * F(3)) *
          icste2;
      C(3, 1) = K(3, 4) * F(4) + K(3, 1) * F(1);
      C(3, 2) = K(3, 2) * F(2);
      C(1, 0) = K(1, 0) * F(0) + K(1, 3) * F(3);
      C(1, 3) =
          (K(1, 0) * F(4) + K(1, 3) * F(1) + K(1, 4) * F(0) + K(1, 1) * F(3)) *
          icste2;
      C(1, 1) = K(1, 4) * F(4) + K(1, 1) * F(1);
      C(1, 2) = K(1, 2) * F(2);
      C(2, 0) = K(2, 0) * F(0) + K(2, 3) * F(3);
      C(2, 3) =
          (K(2, 0) * F(4) + K(2, 3) * F(1) + K(2, 4) * F(0) + K(2, 1) * F(3)) *
          icste2;
      C(2, 1) = K(2, 4) * F(4) + K(2, 1) * F(1);
      C(2, 2) = K(2, 2) * F(2);
    } else {
      constexpr const auto icste2 = Cste<value_type>::isqrt2 / 2;
      C(0, 0) = K(0, 0) * F(0) + K(0, 3) * F(3) + K(0, 5) * F(5);
      C(0, 3) = (K(0, 0) * F(4) + K(0, 3) * F(1) + K(0, 5) * F(7) +
                 K(0, 4) * F(0) + K(0, 1) * F(3) + K(0, 7) * F(5)) *
                icste2;
      C(0, 4) = (K(0, 0) * F(6) + K(0, 3) * F(8) + K(0, 5) * F(2) +
                 K(0, 6) * F(0) + K(0, 8) * F(3) + K(0, 2) * F(5)) *
                icste2;
      C(0, 1) = K(0, 4) * F(4) + K(0, 1) * F(1) + K(0, 7) * F(7);
      C(0, 5) = (K(0, 4) * F(6) + K(0, 1) * F(8) + K(0, 7) * F(2) +
                 K(0, 6) * F(4) + K(0, 8) * F(1) + K(0, 2) * F(7)) *
                icste2;
      C(0, 2) = K(0, 6) * F(6) + K(0, 8) * F(8) + K(0, 2) * F(2);
      C(3, 0) = K(3, 0) * F(0) + K(3, 3) * F(3) + K(3, 5) * F(5);
      C(3, 3) = (K(3, 0) * F(4) + K(3, 3) * F(1) + K(3, 5) * F(7) +
                 K(3, 4) * F(0) + K(3, 1) * F(3) + K(3, 7) * F(5)) *
                icste2;
      C(3, 4) = (K(3, 0) * F(6) + K(3, 3) * F(8) + K(3, 5) * F(2) +
                 K(3, 6) * F(0) + K(3, 8) * F(3) + K(3, 2) * F(5)) *
                icste2;
      C(3, 1) = K(3, 4) * F(4) + K(3, 1) * F(1) + K(3, 7) * F(7);
      C(3, 5) = (K(3, 4) * F(6) + K(3, 1) * F(8) + K(3, 7) * F(2) +
                 K(3, 6) * F(4) + K(3, 8) * F(1) + K(3, 2) * F(7)) *
                icste2;
      C(3, 2) = K(3, 6) * F(6) + K(3, 8) * F(8) + K(3, 2) * F(2);
      C(4, 0) = K(4, 0) * F(0) + K(4, 3) * F(3) + K(4, 5) * F(5);
      C(4, 3) = (K(4, 0) * F(4) + K(4, 3) * F(1) + K(4, 5) * F(7) +
                 K(4, 4) * F(0) + K(4, 1) * F(3) + K(4, 7) * F(5)) *
                icste2;
      C(4, 4) = (K(4, 0) * F(6) + K(4, 3) * F(8) + K(4, 5) * F(2) +
                 K(4, 6) * F(0) + K(4, 8) * F(3) + K(4, 2) * F(5)) *
                icste2;
      C(4, 1) = K(4, 4) * F(4) + K(4, 1) * F(1) + K(4, 7) * F(7);
      C(4, 5) = (K(4, 4) * F(6) + K(4, 1) * F(8) + K(4, 7) * F(2) +
                 K(4, 6) * F(4) + K(4, 8) * F(1) + K(4, 2) * F(7)) *
                icste2;
      C(4, 2) = K(4, 6) * F(6) + K(4, 8) * F(8) + K(4, 2) * F(2);
      C(1, 0) = K(1, 0) * F(0) + K(1, 3) * F(3) + K(1, 5) * F(5);
      C(1, 3) = (K(1, 0) * F(4) + K(1, 3) * F(1) + K(1, 5) * F(7) +
                 K(1, 4) * F(0) + K(1, 1) * F(3) + K(1, 7) * F(5)) *
                icste2;
      C(1, 4) = (K(1, 0) * F(6) + K(1, 3) * F(8) + K(1, 5) * F(2) +
                 K(1, 6) * F(0) + K(1, 8) * F(3) + K(1, 2) * F(5)) *
                icste2;
      C(1, 1) = K(1, 4) * F(4) + K(1, 1) * F(1) + K(1, 7) * F(7);
      C(1, 5) = (K(1, 4) * F(6) + K(1, 1) * F(8) + K(1, 7) * F(2) +
                 K(1, 6) * F(4) + K(1, 8) * F(1) + K(1, 2) * F(7)) *
                icste2;
      C(1, 2) = K(1, 6) * F(6) + K(1, 8) * F(8) + K(1, 2) * F(2);
      C(5, 0) = K(5, 0) * F(0) + K(5, 3) * F(3) + K(5, 5) * F(5);
      C(5, 3) = (K(5, 0) * F(4) + K(5, 3) * F(1) + K(5, 5) * F(7) +
                 K(5, 4) * F(0) + K(5, 1) * F(3) + K(5, 7) * F(5)) *
                icste2;
      C(5, 4) = (K(5, 0) * F(6) + K(5, 3) * F(8) + K(5, 5) * F(2) +
                 K(5, 6) * F(0) + K(5, 8) * F(3) + K(5, 2) * F(5)) *
                icste2;
      C(5, 1) = K(5, 4) * F(4) + K(5, 1) * F(1) + K(5, 7) * F(7);
      C(5, 5) = (K(5, 4) * F(6) + K(5, 1) * F(8) + K(5, 7) * F(2) +
                 K(5, 6) * F(4) + K(5, 8) * F(1) + K(5, 2) * F(7)) *
                icste2;
      C(5, 2) = K(5, 6) * F(6) + K(5, 8) * F(8) + K(5, 2) * F(2);
      C(2, 0) = K(2, 0) * F(0) + K(2, 3) * F(3) + K(2, 5) * F(5);
      C(2, 3) = (K(2, 0) * F(4) + K(2, 3) * F(1) + K(2, 5) * F(7) +
                 K(2, 4) * F(0) + K(2, 1) * F(3) + K(2, 7) * F(5)) *
                icste2;
      C(2, 4) = (K(2, 0) * F(6) + K(2, 3) * F(8) + K(2, 5) * F(2) +
                 K(2, 6) * F(0) + K(2, 8) * F(3) + K(2, 2) * F(5)) *
                icste2;
      C(2, 1) = K(2, 4) * F(4) + K(2, 1) * F(1) + K(2, 7) * F(7);
      C(2, 5) = (K(2, 4) * F(6) + K(2, 1) * F(8) + K(2, 7) * F(2) +
                 K(2, 6) * F(4) + K(2, 8) * F(1) + K(2, 2) * F(7)) *
                icste2;
      C(2, 2) = K(2, 6) * F(6) + K(2, 8) * F(8) + K(2, 2) * F(2);
    }
    return C;
  }  // end of convertToTangentModuli

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_CONVERTTOTANGENTMODULI_HXX */
