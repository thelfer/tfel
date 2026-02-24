/*!
 * \file   include/TFEL/Math/Stensor/Internals/StensorComputeEigenValues.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORCOMPUTEEIGENVALUES_HXX
#define LIB_TFEL_MATH_STENSORCOMPUTEEIGENVALUES_HXX

#include <cmath>
#include "TFEL/ContractViolation.hxx"
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/MathException.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/CubicRoots.hxx"
#include "TFEL/Exception/TFELException.hxx"

namespace tfel::math::internals {

  // computeEigenValues
  template <unsigned short N>
  struct StensorComputeEigenValues;

  template <>
  struct StensorComputeEigenValues<1u> {
    template <typename T>
    static TFEL_MATH_INLINE2 void exe(
        const T* const v, T& vp1, T& vp2, T& vp3, const bool) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      vp1 = *v;
      vp2 = *(v + 1);
      vp3 = *(v + 2);
    }
  };

  template <>
  struct StensorComputeEigenValues<2u> {
    template <typename T>
    static TFEL_MATH_INLINE2 void exe(
        const T* const v, T& vp1, T& vp2, T& vp3, const bool) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      constexpr auto zero = T(0);
      constexpr const auto one_half = T(1) / T(2);
      const auto tr = v[0] + v[1];
      const auto tmp = v[0] - v[1];
      const auto delta =
          std::max((tmp * tmp * one_half + (v[3] * v[3])) * one_half, zero);
      // const auto r1 = one_half*tr+std::copysign(std::sqrt(delta),tr);
      // const auto r2 = c/r1;
      vp1 = one_half * tr + std::sqrt(delta);
      vp2 = one_half * tr - std::sqrt(delta);
      vp3 = *(v + 2);
    }
  };

  template <>
  struct StensorComputeEigenValues<3u> {
    template <typename T>
    static TFEL_MATH_INLINE2 void exe(
        const T* const v, T& vp1, T& vp2, T& vp3, const bool b) {
      static_assert(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      static_assert(tfel::typetraits::IsReal<T>::cond);
      constexpr auto icste = Cste<T>::isqrt2;
      constexpr const auto one_half = 1 / T{2};
      constexpr const auto one_third = 1 / T{3};
      stensor<3u, T> s(v);
      stensor<3u, T> s2(deviator(s));
      const auto vmax =
          *(fsalgo::max_element<6u>::exe(s2.begin(), [](const T x, const T y) {
            return tfel::math::abs(x) > tfel::math::abs(y);
          }));
      const bool n = tfel::math::abs(vmax) * std::numeric_limits<T>::epsilon() >
                     10 * std::numeric_limits<T>::min();
      if (n) {
        s2 *= T(1) / vmax;
      }
      constexpr auto p3 = T{-1};
      constexpr auto p2 = T{0};
      const T p1 = one_half * (s2[5] * s2[5] + s2[4] * s2[4] + s2[3] * s2[3]) -
                   (s2[0] * (s2[2] + s2[1]) + s2[1] * s2[2]);
      const T p0 = -one_half * (s2[0] * s2[5] * s2[5] + s2[1] * s2[4] * s2[4] +
                                s2[2] * s2[3] * s2[3]) +
                   icste * (s2[3] * s2[4] * s2[5]) + s2[0] * s2[1] * s2[2];
      const auto nb = CubicRoots::exe(vp1, vp2, vp3, p3, p2, p1, p0, b);
      if (nb == 0u) {
        reportContractViolation("invalid return value");
      } else if (nb == 1u) {
        if (tfel::math::abs(vp1 - vp2) < std::numeric_limits<T>::epsilon()) {
          const auto vm = (vp1 + vp2) * one_half;
          vp1 = vp2 = vm;
        }
        if (tfel::math::abs(vp1 - vp3) < std::numeric_limits<T>::epsilon()) {
          const auto vm = (vp1 + vp3) * one_half;
          vp1 = vp3 = vm;
        }
        if (tfel::math::abs(vp2 - vp3) < std::numeric_limits<T>::epsilon()) {
          const auto vm = (vp2 + vp3) * one_half;
          vp2 = vp3 = vm;
        }
      }
      if (n) {
        vp1 *= vmax;
        vp2 *= vmax;
        vp3 *= vmax;
      }
      const auto tr = trace(s) * one_third;
      vp1 += tr;
      vp2 += tr;
      vp3 += tr;
    }
  };

}  // end of namespace tfel::math::internals

#endif /* LIB_TFEL_MATH_STENSORCOMPUTEEIGENVALUES_HXX */
