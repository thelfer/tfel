/*!
 * \file   include/TFEL/Math/Stensor/Internals/StensorExport.ixx
 *
 * \brief
 * \author Thomas Helfer
 * \date   07 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_STENSOREXPORT_I_
#define LIB_TFEL_STENSOREXPORT_I_

#include <cmath>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace tfel {

  namespace math {

    namespace internals {

      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToTab<1u> {
        template <typename T>
        TFEL_MATH_INLINE static void exe(const T* const src, T* const target) {
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
        }
      };

      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToTab<2u> {
        template <typename T>
        TFEL_MATH_INLINE static void exe(const T* const src, T* const target) {
          constexpr const auto icste = Cste<T>::isqrt2;
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
          target[3] = src[3] * icste;
        }
      };

      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToTab<3u> {
        template <typename T>
        TFEL_MATH_INLINE static void exe(const T* const src, T* const target) {
          constexpr const auto icste = Cste<T>::isqrt2;
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
          target[3] = src[3] * icste;
          target[4] = src[4] * icste;
          target[5] = src[5] * icste;
        }
      };

      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToVoigt<1u> {
        template <typename T>
        TFEL_MATH_INLINE static void exe(const T* const src, T* const target) {
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
        }
      };

      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToVoigt<2u> {
        template <typename T>
        TFEL_MATH_INLINE static void exe(const T* const src, T* const target) {
          constexpr const auto cste = Cste<T>::sqrt2;
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
          target[3] = src[3] * cste;
        }
      };

      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToVoigt<3u> {
        template <typename T>
        TFEL_MATH_INLINE static void exe(const T* const src, T* const target) {
          constexpr const auto cste = Cste<T>::sqrt2;
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
          target[3] = src[3] * cste;
          target[4] = src[4] * cste;
          target[5] = src[5] * cste;
        }
      };

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_STENSOREXPORT_I_ */
