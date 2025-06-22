/*!
 * \file   include/TFEL/Math/Stensor/Internals/StensorImport.hxx
 *
 * \brief
 * \author Helfer Thomas
 * \date   26 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_STENSORIMPORT_H_
#define LIB_TFEL_STENSORIMPORT_H_

#include <cmath>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned int N>
      struct ImportFromVoigt;

      template <unsigned int N>
      struct ImportFromTab;

      template <>
      struct TFEL_VISIBILITY_LOCAL ImportFromVoigt<1u> {
        template <typename T, typename T2>
        static TFEL_MATH_INLINE typename std::
            enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond, void>::type
            exe(T* const target, const T2* const src) {
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
        }
      };

      template <>
      struct TFEL_VISIBILITY_LOCAL ImportFromVoigt<2u> {
        template <typename T, typename T2>
        static TFEL_MATH_INLINE typename std::
            enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond, void>::type
            exe(T* const target, const T2* const src) {
          constexpr const auto icste = Cste<T>::isqrt2;
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
          target[3] = src[3] * icste;
        }
      };

      template <>
      struct TFEL_VISIBILITY_LOCAL ImportFromVoigt<3u> {
        template <typename T, typename T2>
        static TFEL_MATH_INLINE typename std::
            enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond, void>::type
            exe(T* const target, const T2* const src) {
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
      struct TFEL_VISIBILITY_LOCAL ImportFromTab<1u> {
        template <typename T, typename T2>
        static TFEL_MATH_INLINE typename std::
            enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond, void>::type
            exe(T* const target, const T2* const src) {
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
        }
      };

      template <>
      struct TFEL_VISIBILITY_LOCAL ImportFromTab<2u> {
        template <typename T, typename T2>
        static TFEL_MATH_INLINE typename std::
            enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond, void>::type
            exe(T* const target, const T2* const src) {
          constexpr const auto cste = Cste<T>::sqrt2;
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
          target[3] = src[3] * cste;
        }
      };

      template <>
      struct TFEL_VISIBILITY_LOCAL ImportFromTab<3u> {
        template <typename T, typename T2>
        static TFEL_MATH_INLINE typename std::
            enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond, void>::type
            exe(T* const target, const T2* const src) {
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

#endif /* LIB_TFEL_STENSORIMPORT_H_ */
