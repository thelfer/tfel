/*!
 * \file   include/TFEL/Math/Stensor/StensorConcept.ixx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATH_STENSOR_CONCEPT_IXX
#define TFEL_MATH_STENSOR_CONCEPT_IXX 1

#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel::math {

  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>(),
                   numeric_type<StensorType>>
  trace(const StensorType& s) {
    return s(0) + s(1) + s(2);
  }

  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      typename tfel::typetraits::AbsType<numeric_type<StensorType>>::type>
  abs(const StensorType& s) {
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1u) {
      return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
             tfel::math::abs(s(2));
    } else if constexpr (N == 2u) {
      return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
             tfel::math::abs(s(2)) + tfel::math::abs(s(3));
    } else if constexpr (N == 3u) {
      return tfel::math::abs(s(0)) + tfel::math::abs(s(1)) +
             tfel::math::abs(s(2)) + tfel::math::abs(s(3)) +
             tfel::math::abs(s(4)) + tfel::math::abs(s(5));
    }
  }  // end of abs

  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>(),
                   numeric_type<StensorType>>
  sigmaeq(const StensorType& s) {
    using real = base_type<numeric_type<StensorType>>;
    constexpr auto N = getSpaceDimension<StensorType>();
    constexpr const auto one_third = real(1) / real(3);
    constexpr const auto cste = real(3) / real(2);
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    auto square = [](const auto x) { return x * x; };
    const auto tr = one_third * trace(s);
    if constexpr (N == 1u) {
      return power<1, 2>(cste * (square(s(0) - tr) +  //
                                 square(s(1) - tr) +  //
                                 square(s(2) - tr)));
    } else if constexpr (N == 2u) {
      return power<1, 2>(cste * (square(s(0) - tr) +  //
                                 square(s(1) - tr) +  //
                                 square(s(2) - tr) + square(s(3))));
    } else if constexpr (N == 3u) {
      return power<1, 2>(cste * (square(s(0) - tr) +  //
                                 square(s(1) - tr) +  //
                                 square(s(2) - tr) +  //
                                 square(s(3)) + square(s(4)) + square(s(5))));
    }
  }  // end of sigmaeq

  template <typename StensorType>
  std::enable_if_t<implementsStensorConcept<StensorType>(),
                   EvaluationResult<StensorType>>
  deviator(const StensorType& s) {
    using Result = EvaluationResult<StensorType>;
    using real = base_type<numeric_type<StensorType>>;
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    constexpr const auto one_third = real{1} / real{3};
    const auto tr = one_third * trace(s);
    if constexpr (N == 1u) {
      return Result{s(0) - tr, s(1) - tr, s(2) - tr};
    } else if constexpr (N == 2u) {
      using NumType = numeric_type<StensorType>;
      return Result{s(0) - tr, s(1) - tr, s(2) - tr, NumType(s(3))};
    } else if constexpr (N == 3u) {
      using NumType = numeric_type<StensorType>;
      return Result{s(0) - tr,     s(1) - tr,     s(2) - tr,
                    NumType(s(3)), NumType(s(4)), NumType(s(5))};
    }
  }  // end of deviator

  template <typename StensorResultType, typename StensorType>
  std::enable_if_t<
      (implementsStensorConcept<StensorResultType>() &&
       implementsStensorConcept<StensorType>() &&
       isAssignableTo<typename ComputeUnaryResult<numeric_type<StensorType>,
                                                  Power<2>>::Result,
                      numeric_type<StensorResultType>>()),
      void>
  computeDeterminantDerivative(StensorResultType& dJ, const StensorType& s) {
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert(N == getSpaceDimension<StensorResultType>(),
                  "arguments must have the same space dimension");
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1u) {
      dJ[0] = s[1] * s[2];
      dJ[1] = s[0] * s[2];
      dJ[2] = s[0] * s[1];
    } else if constexpr (N == 2u) {
      dJ[0] = s[1] * s[2];
      dJ[1] = s[0] * s[2];
      dJ[2] = s[0] * s[1] - s[3] * s[3] / 2;
      dJ[3] = -s[3] * s[2];
    } else if constexpr (N == 3u) {
      using NumType = numeric_type<StensorType>;
      using real = base_type<NumType>;
      constexpr const auto one_half = real(1) / real(2);
      constexpr const auto icste = Cste<real>::isqrt2;
      dJ[0] = s[1] * s[2] - s[5] * s[5] * one_half;
      dJ[1] = s[0] * s[2] - s[4] * s[4] * one_half;
      dJ[2] = s[0] * s[1] - s[3] * s[3] * one_half;
      dJ[3] = icste * s[4] * s[5] - s[2] * s[3];
      dJ[4] = icste * s[3] * s[5] - s[1] * s[4];
      dJ[5] = icste * s[3] * s[4] - s[0] * s[5];
    }
  }  // end of ComputeDeterminantDerivative

  template <typename StensorResultType, typename StensorType>
  std::enable_if_t<
      (implementsStensorConcept<StensorResultType>() &&
       implementsStensorConcept<StensorType>() &&
       isAssignableTo<typename ComputeUnaryResult<numeric_type<StensorType>,
                                                  Power<2>>::Result,
                      numeric_type<StensorResultType>>()),
      void>
  computeDeviatorDeterminantDerivative(StensorResultType& dJ,
                                       const StensorType& s) {
    constexpr auto N = getSpaceDimension<StensorType>();
    static_assert(N == getSpaceDimension<StensorResultType>(),
                  "arguments must have the same space dimension");
    static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
    if constexpr (N == 1u) {
      dJ[0] = -(s[2] * s[2] + (2 * s[0] - 4 * s[1]) * s[2] + s[1] * s[1] +
                2 * s[0] * s[1] - 2 * s[0] * s[0]) /
              9.;
      dJ[1] = -(s[2] * s[2] + (2 * s[1] - 4 * s[0]) * s[2] - 2 * s[1] * s[1] +
                2 * s[0] * s[1] + s[0] * s[0]) /
              9.;
      dJ[2] = (2 * s[2] * s[2] + ((-2 * s[1]) - 2 * s[0]) * s[2] - s[1] * s[1] +
               4 * s[0] * s[1] - s[0] * s[0]) /
              9.;
    } else if constexpr (N == 2u) {
      dJ[0] =
          (3 * s[3] * s[3] - 2 * s[2] * s[2] + (8 * s[1] - 4 * s[0]) * s[2] -
           2 * s[1] * s[1] - 4 * s[0] * s[1] + 4 * s[0] * s[0]) /
          18;
      dJ[1] =
          (3 * s[3] * s[3] - 2 * s[2] * s[2] + (8 * s[0] - 4 * s[1]) * s[2] +
           4 * s[1] * s[1] - 4 * s[0] * s[1] - 2 * s[0] * s[0]) /
          18;
      dJ[2] =
          -(3 * s[3] * s[3] - 2 * s[2] * s[2] + (2 * s[1] + 2 * s[0]) * s[2] +
            s[1] * s[1] - 4 * s[0] * s[1] + s[0] * s[0]) /
          9;
      dJ[3] = -((2 * s[2] - s[1] - s[0]) * s[3]) / 3;
    } else if constexpr (N == 3u) {
      using NumType = numeric_type<StensorType>;
      using real = base_type<NumType>;
      constexpr const auto cste = Cste<real>::sqrt2;
      dJ[0] = -(6 * s[5] * s[5] - 3 * s[4] * s[4] - 3 * s[3] * s[3] +
                2 * s[2] * s[2] + (4 * s[0] - 8 * s[1]) * s[2] +
                2 * s[1] * s[1] + 4 * s[0] * s[1] - 4 * s[0] * s[0]) /
              18;
      dJ[1] = (3 * s[5] * s[5] - 6 * s[4] * s[4] + 3 * s[3] * s[3] -
               2 * s[2] * s[2] + (8 * s[0] - 4 * s[1]) * s[2] +
               4 * s[1] * s[1] - 4 * s[0] * s[1] - 2 * s[0] * s[0]) /
              18;
      dJ[2] = (3 * s[5] * s[5] + 3 * s[4] * s[4] - 6 * s[3] * s[3] +
               4 * s[2] * s[2] + ((-4 * s[1]) - 4 * s[0]) * s[2] -
               2 * s[1] * s[1] + 8 * s[0] * s[1] - 2 * s[0] * s[0]) /
              18;
      dJ[3] = (3 * cste * s[4] * s[5] +
               ((-4 * s[2]) + 2 * s[1] + 2 * s[0]) * s[3]) /
              6;
      dJ[4] =
          (3 * cste * s[3] * s[5] + (2 * s[2] - 4 * s[1] + 2 * s[0]) * s[4]) /
          6;
      dJ[5] =
          ((2 * s[2] + 2 * s[1] - 4 * s[0]) * s[5] + 3 * cste * s[3] * s[4]) /
          6;
    }
  }  // end of ComputeDeviatorDeterminantDerivative

}  // end of namespace tfel::math

#endif /* TFEL_MATH_STENSOR_CONCEPT_IXX */
