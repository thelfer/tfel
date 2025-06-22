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

#ifndef LIB_TFEL_MATH_STENSOREXPORT_IXX
#define LIB_TFEL_MATH_STENSOREXPORT_IXX

#include <cmath>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace tfel {

  namespace math {

    namespace internals {

      /*!
       * \brief partial specialisation of the `ExportToTab` class in
       * 1D.
       */
      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToTab<1u> {
        /*!
         * \brief convert an array of values using the TFEL convention
         * to an array
         * \tparam T: numeric type
         * \param[in]  src: pointer to the original values.
         * \param[out] target: pointer to the new values.
         */
        template <typename T>
        TFEL_MATH_INLINE static void exe(const T* const src, T* const target) {
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
        }  // end of exe
      };   // end of ExportToTab<1u>
      /*!
       * \brief partial specialisation of the `ExportToTab` class in
       * 2D.
       */
      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToTab<2u> {
        /*!
         * \brief convert an array of values using the TFEL convention
         * to an array
         * \tparam T: numeric type
         * \param[in]  src: pointer to the original values.
         * \param[out] target: pointer to the new values.
         */
        template <typename T>
        TFEL_MATH_INLINE static void exe(const T* const src, T* const target) {
          constexpr const auto icste = Cste<T>::isqrt2;
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
          target[3] = src[3] * icste;
        }
      };
      /*!
       * \brief partial specialisation of the `ExportToTab` class in
       * 3D.
       */
      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToTab<3u> {
        /*!
         * \brief convert an array of values using the TFEL convention
         * to an array
         * \tparam T: numeric type
         * \param[in]  src: pointer to the original values.
         * \param[out] target: pointer to the new values.
         */
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
      /*!
       * \brief partial specialisation of the `ExportToVoigt` class in
       * 1D.
       */
      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToVoigt<1u> {
        /*!
         * \brief convert an array of values using the TFEL convention
         * to an array of values using the Voigt convention.
         * \tparam T: numeric type
         * \param[in]  src: pointer to the original values.
         * \param[out] target: pointer to the new values.
         */
        template <typename T>
        TFEL_MATH_INLINE static void exe(const T* const src, T* const target) {
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
        }
      };
      /*!
       * \brief partial specialisation of the `ExportToVoigt` class in
       * 2D.
       */
      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToVoigt<2u> {
        /*!
         * \brief convert an array of values using the TFEL convention
         * to an array of values using the Voigt convention.
         * \tparam T: numeric type
         * \param[in]  src: pointer to the original values.
         * \param[out] target: pointer to the new values.
         */
        template <typename T>
        TFEL_MATH_INLINE static void exe(const T* const src, T* const target) {
          constexpr const auto cste = Cste<T>::sqrt2;
          target[0] = src[0];
          target[1] = src[1];
          target[2] = src[2];
          target[3] = src[3] * cste;
        }
      };
      /*!
       * \brief partial specialisation of the `ExportToVoigt` class in
       * 3D.
       */
      template <>
      struct TFEL_VISIBILITY_LOCAL ExportToVoigt<3u> {
        /*!
         * \brief convert an array of values using the TFEL convention
         * to an array of values using the Voigt convention.
         * \tparam T: numeric type
         * \param[in]  src: pointer to the original values.
         * \param[out] target: pointer to the new values.
         */
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

#endif /* LIB_TFEL_MATH_STENSOREXPORT_IXX */
