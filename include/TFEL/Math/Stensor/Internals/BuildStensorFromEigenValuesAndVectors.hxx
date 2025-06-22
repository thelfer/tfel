/*!
 * \file   BuildStensorFromEigenValuesAndVectors.hxx
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

#ifndef LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMEIGENVALUESANDVECTORS_HXX
#define LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMEIGENVALUESANDVECTORS_HXX

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned short N>
      struct BuildStensorFromEigenValuesAndVectors;

      template <>
      struct BuildStensorFromEigenValuesAndVectors<1u> {
        template <typename T>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL void exe(
            stensor<1u, T>& s,
            const T& v1,
            const T& v2,
            const T& v3,
            const tfel::math::rotation_matrix<T>&) {
          s = {v1, v2, v3};
        }
      };

      template <>
      struct BuildStensorFromEigenValuesAndVectors<2u> {
        template <typename T>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL void exe(
            stensor<2u, T>& s,
            const T& v1,
            const T& v2,
            const T& v3,
            const tfel::math::rotation_matrix<T>& m) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s = {v1 * m(0, 0) * m(0, 0) + v2 * m(0, 1) * m(0, 1),
               v1 * m(1, 0) * m(1, 0) + v2 * m(1, 1) * m(1, 1), v3,
               (v1 * m(0, 0) * m(1, 0) + v2 * m(0, 1) * m(1, 1)) * cste};
        }
      };

      template <>
      struct BuildStensorFromEigenValuesAndVectors<3u> {
        template <typename T>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL void exe(
            stensor<3u, T>& s,
            const T& v1,
            const T& v2,
            const T& v3,
            const tfel::math::rotation_matrix<T>& m) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s = {v1 * m(0, 0) * m(0, 0) + v2 * m(0, 1) * m(0, 1) +
                   v3 * m(0, 2) * m(0, 2),
               v1 * m(1, 0) * m(1, 0) + v2 * m(1, 1) * m(1, 1) +
                   v3 * m(1, 2) * m(1, 2),
               v1 * m(2, 0) * m(2, 0) + v2 * m(2, 1) * m(2, 1) +
                   v3 * m(2, 2) * m(2, 2),
               (v1 * m(0, 0) * m(1, 0) + v2 * m(0, 1) * m(1, 1) +
                v3 * m(0, 2) * m(1, 2)) *
                   cste,
               (v1 * m(0, 0) * m(2, 0) + v2 * m(0, 1) * m(2, 1) +
                v3 * m(0, 2) * m(2, 2)) *
                   cste,
               (v1 * m(1, 0) * m(2, 0) + v2 * m(1, 1) * m(2, 1) +
                v3 * m(1, 2) * m(2, 2)) *
                   cste};
        }
      };

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMEIGENVALUESANDVECTORS_HXX */
