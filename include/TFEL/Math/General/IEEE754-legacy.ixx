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

#include <cmath>
#include <limits>
#include <cfloat>
#include <cstdint>
#include <stdexcept>
#include "TFEL/ContractViolation.hxx"

namespace tfel::math::ieee754 {

  constexpr int fpclassify(const float x) noexcept {
    union {
      float f;
      uint32_t i;
    } u = {x};
    int e = u.i >> 23 & 0xff;
    if (!e) return u.i << 1 ? FP_SUBNORMAL : FP_ZERO;
    if (e == 0xff) return u.i << 9 ? FP_NAN : FP_INFINITE;
    return FP_NORMAL;
  }

  constexpr int fpclassify(const double x) noexcept {
    union {
      double f;
      uint64_t i;
    } u = {x};
    int e = u.i >> 52 & 0x7ff;
    if (!e) return u.i << 1 ? FP_SUBNORMAL : FP_ZERO;
    if (e == 0x7ff) return u.i << 12 ? FP_NAN : FP_INFINITE;
    return FP_NORMAL;
  }

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
  constexpr int fpclassify(const long double x) noexcept {
    union {
      long double f;
      uint64_t i;
    } u = {x};
    int e = u.i >> 52 & 0x7ff;
    if (!e) return u.i << 1 ? FP_SUBNORMAL : FP_ZERO;
    if (e == 0x7ff) return u.i << 12 ? FP_NAN : FP_INFINITE;
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
      union {
        uint32_t i;
        char c[4];
      } bint = {0x01020304};
      return bint.c[0] != 1;
    }();
    if (!le) {
      tfel::reportContractViolation(
          "tfel::math::ieee754::fpclassify: "
          "unsupported long double representation");
    }
    union {
      long double f;
      struct {
        uint64_t m;
        uint16_t se;
      } i;
    } u = {x};
    int e = u.i.se & 0x7fff;
    int msb = u.i.m >> 63;
    if (!e && !msb) return u.i.m ? FP_SUBNORMAL : FP_ZERO;
    if (!msb) return FP_NAN;
    if (e == 0x7fff) return u.i.m << 1 ? FP_NAN : FP_INFINITE;
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
      union {
        uint32_t i;
        char c[4];
      } bint = {0x01020304};
      return bint.c[0] != 1;
    }();
    if (le) {
      union {
        long double f;
        struct {
          uint64_t lo;
          uint32_t mid;
          uint16_t top;
          uint16_t se;
        } i;
        struct {
          uint64_t lo;
          uint64_t hi;
        } i2;
      } u = {x};
      int e = u.i.se & 0x7fff;
      u.i.se = 0;
      if (!e) return u.i2.lo | u.i2.hi ? FP_SUBNORMAL : FP_ZERO;
      if (e == 0x7fff) return u.i2.lo | u.i2.hi ? FP_NAN : FP_INFINITE;
    } else if (!le) {
      union {
        long double f;
        struct {
          uint16_t se;
          uint16_t top;
          uint32_t mid;
          uint64_t lo;
        } i;
        struct {
          uint64_t hi;
          uint64_t lo;
        } i2;
      } u = {x};
      int e = u.i.se & 0x7fff;
      u.i.se = 0;
      if (!e) return u.i2.lo | u.i2.hi ? FP_SUBNORMAL : FP_ZERO;
      if (e == 0x7fff) return u.i2.lo | u.i2.hi ? FP_NAN : FP_INFINITE;
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
