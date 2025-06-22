/*!
 * \file   include/TFEL/Math/Stensor/Internals/StensorChangeBasis.hxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   11 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_STENSORCHANGEBASIS_HXX
#define LIB_TFEL_STENSORCHANGEBASIS_HXX

#include <cmath>

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned short N>
      struct StensorChangeBasis;

      template <>
      struct StensorChangeBasis<1u> {
        template <typename T, typename T2>
        static void exe(T*, const tfel::math::tmatrix<3u, 3u, T2>&) {}
      };

      template <>
      struct StensorChangeBasis<2u> {
        template <typename T, typename T2>
        static void exe(T* s, const tfel::math::tmatrix<3u, 3u, T2>& m) {
          constexpr const auto cste = Cste<T2>::sqrt2;
          T tmp[3];
          // Works begin
          tmp[0] = cste * m(0, 0) * m(1, 0) * s[3] + m(1, 0) * m(1, 0) * s[1] +
                   m(0, 0) * m(0, 0) * s[0];
          tmp[1] = cste * m(0, 1) * m(1, 1) * s[3] + m(1, 1) * m(1, 1) * s[1] +
                   m(0, 1) * m(0, 1) * s[0];
          tmp[2] = (m(0, 0) * m(1, 1) + m(0, 1) * m(1, 0)) * s[3];
          tmp[2] +=
              cste * (m(1, 0) * m(1, 1) * s[1] + m(0, 0) * m(0, 1) * s[0]);
          s[0] = tmp[0];
          s[1] = tmp[1];
          s[3] = tmp[2];
        }
      };

      template <>
      struct StensorChangeBasis<3u> {
        template <typename T, typename T2>
        static void exe(T* s, const tfel::math::tmatrix<3u, 3u, T2>& m) {
          constexpr const auto cste = Cste<T2>::sqrt2;
          T tmp[6];
          const T2 a = m(0, 0);
          const T2 b = m(0, 1);
          const T2 c = m(0, 2);
          const T2 d = m(1, 0);
          const T2 e = m(1, 1);
          const T2 f = m(1, 2);
          const T2 g = m(2, 0);
          const T2 h = m(2, 1);
          const T2 i = m(2, 2);
          tmp[0] = cste * (d * g * s[5] + a * g * s[4] + a * d * s[3]) +
                   g * g * s[2] + d * d * s[1] + a * a * s[0];
          tmp[1] = cste * (e * h * s[5] + b * h * s[4] + b * e * s[3]) +
                   h * h * s[2] + e * e * s[1] + b * b * s[0];
          tmp[2] = cste * (f * i * s[5] + c * i * s[4] + c * f * s[3]) +
                   i * i * s[2] + f * f * s[1] + c * c * s[0];
          tmp[3] = (d * h + e * g) * s[5] + (a * h + b * g) * s[4] +
                   (a * e + b * d) * s[3];
          tmp[3] += cste * (g * h * s[2] + d * e * s[1] + a * b * s[0]);
          tmp[4] = (d * i + f * g) * s[5] + (a * i + c * g) * s[4] +
                   (a * f + c * d) * s[3];
          tmp[4] += cste * (g * i * s[2] + d * f * s[1] + a * c * s[0]);
          tmp[5] = (e * i + f * h) * s[5] + (b * i + c * h) * s[4] +
                   (b * f + c * e) * s[3];
          tmp[5] += cste * (h * i * s[2] + e * f * s[1] + b * c * s[0]);
          s[0] = tmp[0];
          s[1] = tmp[1];
          s[2] = tmp[2];
          s[3] = tmp[3];
          s[4] = tmp[4];
          s[5] = tmp[5];
        }
      };

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_STENSORCHANGEBASIS_HXX */
