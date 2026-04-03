/*!
 * \file   IEEE754.ixx
 * \brief
 * \author Thomas Helfer
 * \date   13 mars 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_IEEE754_IXX
#define LIB_TFEL_MATH_IEEE754_IXX

#include <bit>
#include <cmath>
#include <limits>
#include <cfloat>
#include <cstdint>
#include <stdexcept>
#include "TFEL/ContractViolation.hxx"

namespace tfel::math::ieee754 {

  constexpr int fpclassify(const float x) noexcept {
    const auto i = std::bit_cast<uint32_t>(x);
    const int e = i >> 23 & 0xff;
    if (!e) return i << 1 ? FP_SUBNORMAL : FP_ZERO;
    if (e == 0xff) return i << 9 ? FP_NAN : FP_INFINITE;
    return FP_NORMAL;
  }

  constexpr int fpclassify(const double x) noexcept {
    const auto i = std::bit_cast<uint64_t>(x);
    const int e = i >> 52 & 0x7ff;
    if (!e) return i << 1 ? FP_SUBNORMAL : FP_ZERO;
    if (e == 0x7ff) return i << 12 ? FP_NAN : FP_INFINITE;
    return FP_NORMAL;
  }

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
  constexpr int fpclassify(const long double x) noexcept {
    const auto i = std::bit_cast<uint64_t>(x);
    const int e = i >> 52 & 0x7ff;
    if (!e) return i << 1 ? FP_SUBNORMAL : FP_ZERO;
    if (e == 0x7ff) return i << 12 ? FP_NAN : FP_INFINITE;
    return FP_NORMAL;
  }
#elif LDBL_MANT_DIG == 64 && LDBL_MAX_EXP == 16384
  constexpr int fpclassify(const long double x) noexcept {
#ifdef __BYTE_ORDER
#if __BYTE_ORDER == __LITTLE_ENDIAN
#else
#error "Unsupported long double representation"
#endif
#endif /* __BYTE_ORDER	*/
    auto le = [] {
      struct char_array {
        char c[4];
      };
      const auto bint = std::bit_cast<char_array, std::uint32_t>(0x01020304);
      return bint.c[0] != 1;
    }();
    if (!le) {
      tfel::reportContractViolation(
          "tfel::math::ieee754::fpclassify: "
          "unsupported long double representation");
    }
    struct internal_representation {
      uint64_t m;
      uint16_t se;
    };
    const auto i = std::bit_cast<internal_representation, long double>(x);
    const int e = i.se & 0x7fff;
    const int msb = i.m >> 63;
    if (!e && !msb) return i.m ? FP_SUBNORMAL : FP_ZERO;
    if (!msb) return FP_NAN;
    if (e == 0x7fff) return i.m << 1 ? FP_NAN : FP_INFINITE;
    return FP_NORMAL;
  }
#elif LDBL_MANT_DIG == 113 && LDBL_MAX_EXP == 16384
  constexpr int fpclassify(const long double x) noexcept {
#ifdef __BYTE_ORDER
#if __BYTE_ORDER == __LITTLE_ENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#else
#error "Unsupported long double representation"
#endif
#endif /* __BYTE_ORDER */
    auto le = [] {
      struct char_array {
        char c[4];
      };
      const auto bint = std::bit_cast<char_array, std::uint32_t>(0x01020304);
      return bint.c[0] != 1;
    }();
    if (le) {
      struct internal_representation {
        uint64_t lo;
        uint32_t mid;
        uint16_t top;
        uint16_t se;
      };
      struct internal_representation2 {
        uint64_t lo;
        uint64_t hi;
      };
      auto i = std::bit_cast<internal_representation, long double>(x);
      const int e = i.se & 0x7fff;
      i.se = 0;
      const auto i2 = std::bit_cast<internal_representation2>(i);
      if (!e) return i2.lo | i2.hi ? FP_SUBNORMAL : FP_ZERO;
      if (e == 0x7fff) return i2.lo | i2.hi ? FP_NAN : FP_INFINITE;
    } else if (!le) {
      struct internal_representation {
        uint16_t se;
        uint16_t top;
        uint32_t mid;
        uint64_t lo;
      };
      auto i = std::bit_cast<internal_representation, long double>(x);
      const int e = i.se & 0x7fff;
      i.se = 0;
      struct internal_representation2 {
        uint64_t hi;
        uint64_t lo;
      };
      const auto i2 = std::bit_cast<internal_representation2>(i);
      if (!e) return i2.lo | i2.hi ? FP_SUBNORMAL : FP_ZERO;
      if (e == 0x7fff) return i2.lo | i2.hi ? FP_NAN : FP_INFINITE;
    }
    return FP_NORMAL;
  }
#endif

  constexpr bool isnan(const float x) noexcept {
    return fpclassify(x) == FP_NAN;
  }

  constexpr bool isnan(const double x) noexcept {
    return fpclassify(x) == FP_NAN;
  }

  constexpr bool isnan(const long double x) noexcept {
    return fpclassify(x) == FP_NAN;
  }

  constexpr bool isfinite(const float x) noexcept {
    const auto c = fpclassify(x);
    return (c == FP_NORMAL) || (c == FP_ZERO) || (c == FP_SUBNORMAL);
  }

  constexpr bool isfinite(const double x) noexcept {
    const auto c = fpclassify(x);
    return (c == FP_NORMAL) || (c == FP_ZERO) || (c == FP_SUBNORMAL);
  }

  constexpr bool isfinite(const long double x) noexcept {
    const auto c = fpclassify(x);
    return (c == FP_NORMAL) || (c == FP_ZERO) || (c == FP_SUBNORMAL);
  }

}  // end of namespace tfel::math::ieee754

#endif /* LIB_TFEL_MATH_IEEE754_IXX */
