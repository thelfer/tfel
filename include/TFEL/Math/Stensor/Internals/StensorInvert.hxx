/*!
 * \file   StensorInvert.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 janv. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_INTERNALS_STENSORINVERT_HXX
#define LIB_TFEL_MATH_INTERNALS_STENSORINVERT_HXX

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned short N>
      struct StensorInvert;

      template <>
      struct StensorInvert<1u> {
        template <typename T>
        static TFEL_MATH_INLINE
            stensor<1u,
                    typename ComputeBinaryResult<tfel::typetraits::base_type<T>,
                                                 T,
                                                 OpDiv>::Result>
            exe(const stensor<1u, T>& s) {
          return {1 / s(0), 1 / s(1), 1 / s(2)};
        }
      };

      template <>
      struct StensorInvert<2u> {
        template <typename T>
        static TFEL_MATH_INLINE
            stensor<2u,
                    typename ComputeBinaryResult<tfel::typetraits::base_type<T>,
                                                 T,
                                                 OpDiv>::Result>
            exe(const stensor<2u, T>& s) {
          const auto idet = 1 / det(s);
          return {s(1) * s(2) * idet, s(0) * s(2) * idet, 1 / s(2),
                  -s(2) * s(3) * idet};
        }
      };

      template <>
      struct StensorInvert<3u> {
        template <typename T>
        static TFEL_MATH_INLINE
            stensor<3u,
                    typename ComputeBinaryResult<tfel::typetraits::base_type<T>,
                                                 T,
                                                 OpDiv>::Result>
            exe(const stensor<3u, T>& s) {
          using real = tfel::typetraits::base_type<T>;
          TFEL_CONSTEXPR const auto two = real(2);
          TFEL_CONSTEXPR const auto one_half = real(1) / two;
          constexpr const auto icste = Cste<real>::isqrt2;
          const auto iJ = 1 / det(s);
          return {(s(1) * s(2) - s(5) * s(5) * one_half) * iJ,
                  (s(0) * s(2) - s(4) * s(4) * one_half) * iJ,
                  (s(0) * s(1) - s(3) * s(3) * one_half) * iJ,
                  (icste * s(4) * s(5) - s(2) * s(3)) * iJ,
                  (icste * s(3) * s(5) - s(1) * s(4)) * iJ,
                  (icste * s(3) * s(4) - s(0) * s(5)) * iJ};
        }
      };

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_STENSORINVERT_HXX */
