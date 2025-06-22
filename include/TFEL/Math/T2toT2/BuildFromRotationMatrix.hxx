/*!
 * \file   include/TFEL/Math/T2toT2/BuildFromRotationMatrix.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   13 oct. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOT2_BUILDFROMROTATIONMATRIX_HXX_
#define LIB_TFEL_MATH_T2TOT2_BUILDFROMROTATIONMATRIX_HXX_

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    namespace t2tot2_internals {

      /*!
       * \brief an auxiliary structure used to dispatch over the
       * dimension
       */
      template <unsigned short>
      struct BuildFromRotationMatrix;
      /*!
       * \brief partial specialisation in 1D
       */
      template <>
      struct BuildFromRotationMatrix<1u> {
        /*!
         * \return the t2tot2 that has the same effect as applying
         * the rotation
         */
        template <typename T>
        static TFEL_MATH_INLINE2 tfel::math::t2tot2<1u, T> exe(
            const tfel::math::tmatrix<3, 3, T>&) {
          return tfel::math::t2tot2<1u, T>::Id();
        }
      };  // end of struct BuildFromRotationMatrix<1u>
      /*!
       * \brief partial specialisation in 2D
       */
      template <>
      struct BuildFromRotationMatrix<2u> {
        /*!
         * \return the t2tot2 that has the same effect as applying
         * the rotation
         * \param[in] r : rotation matrix
         */
        template <typename T>
        static TFEL_MATH_INLINE2 tfel::math::t2tot2<2u, T> exe(
            const tfel::math::tmatrix<3, 3, T>& r) {
          constexpr const T zero = T{0};
          constexpr const T one = T{1};
          tfel::math::t2tot2<2u, T> rt;
          const T a = r(0, 0);
          const T b = r(0, 1);
          const T d = r(1, 0);
          const T e = r(1, 1);
          // a*d*v[4]+a*d*v[3]+d*d*v[1]+a*a*v[0];
          rt(0, 4) = a * d;
          rt(0, 3) = a * d;
          rt(0, 2) = zero;
          rt(0, 1) = d * d;
          rt(0, 0) = a * a;
          // b*e*v[4]+b*e*v[3]+e*e*v[1]+b*b*v[0];
          rt(1, 4) = b * e;
          rt(1, 3) = b * e;
          rt(1, 2) = zero;
          rt(1, 1) = e * e;
          rt(1, 0) = b * b;
          // b*d*v[4]+a*e*v[3]+d*e*v[1]+a*b*v[0];
          rt(3, 4) = b * d;
          rt(3, 3) = a * e;
          rt(3, 2) = zero;
          rt(3, 1) = d * e;
          rt(3, 0) = a * b;
          // a*e*v[4]+b*d*v[3]+d*e*v[1]+a*b*v[0];
          rt(4, 4) = a * e;
          rt(4, 3) = b * d;
          rt(4, 2) = zero;
          rt(4, 1) = d * e;
          rt(4, 0) = a * b;
          // the last one
          rt(2, 4) = zero;
          rt(2, 3) = zero;
          rt(2, 2) = one;
          rt(2, 1) = zero;
          rt(2, 0) = zero;
          return rt;
        }  // end of BuildFromRotationMatrix<2u>::exe
      };   // end of struct BuildFromRotationMatrix<2u>
      /*!
       * \brief partial specialisation in 3D
       */
      template <>
      struct BuildFromRotationMatrix<3u> {
        /*!
         * \return the t2tot2 that has the same effect as applying
         * the rotation
         * \param[in] r : rotation matrix
         */
        template <typename T>
        static TFEL_MATH_INLINE2 tfel::math::t2tot2<3u, T> exe(
            const tfel::math::tmatrix<3, 3, T>& r) {
          const T a = r(0, 0);
          const T b = r(0, 1);
          const T c = r(0, 2);
          const T d = r(1, 0);
          const T e = r(1, 1);
          const T f = r(1, 2);
          const T g = r(2, 0);
          const T h = r(2, 1);
          const T i = r(2, 2);
          tfel::math::t2tot2<3u, T> rt;
          // d*g*v[8]+d*g*v[7]+a*g*v[6]+a*g*v[5]+a*d*v[4]+a*d*v[3]+g*g*v[2]+d*d*v[1]+a*a*v[0];
          rt(0, 0) = a * a;
          rt(0, 1) = d * d;
          rt(0, 2) = g * g;
          rt(0, 3) = a * d;
          rt(0, 4) = a * d;
          rt(0, 5) = a * g;
          rt(0, 6) = a * g;
          rt(0, 7) = d * g;
          rt(0, 8) = d * g;
          // e*h*v[8]+e*h*v[7]+b*h*v[6]+b*h*v[5]+b*e*v[4]+b*e*v[3]+h*h*v[2]+e*e*v[1]+b*b*v[0];
          rt(1, 0) = b * b;
          rt(1, 1) = e * e;
          rt(1, 2) = h * h;
          rt(1, 3) = b * e;
          rt(1, 4) = b * e;
          rt(1, 5) = b * h;
          rt(1, 6) = b * h;
          rt(1, 7) = e * h;
          rt(1, 8) = e * h;
          // f*i*v[8]+f*i*v[7]+c*i*v[6]+c*i*v[5]+c*f*v[4]+c*f*v[3]+i*i*v[2]+f*f*v[1]+c*c*v[0];
          rt(2, 0) = c * c;
          rt(2, 1) = f * f;
          rt(2, 2) = i * i;
          rt(2, 3) = c * f;
          rt(2, 4) = c * f;
          rt(2, 5) = c * i;
          rt(2, 6) = c * i;
          rt(2, 7) = f * i;
          rt(2, 8) = f * i;
          // e*g*v[8]+d*h*v[7]+b*g*v[6]+a*h*v[5]+b*d*v[4]+a*e*v[3]+g*h*v[2]+d*e*v[1]+a*b*v[0];
          rt(3, 8) = e * g;
          rt(3, 7) = d * h;
          rt(3, 6) = b * g;
          rt(3, 5) = a * h;
          rt(3, 4) = b * d;
          rt(3, 3) = a * e;
          rt(3, 2) = g * h;
          rt(3, 1) = d * e;
          rt(3, 0) = a * b;
          // d*h*v[8]+e*g*v[7]+a*h*v[6]+b*g*v[5]+a*e*v[4]+b*d*v[3]+g*h*v[2]+d*e*v[1]+a*b*v[0];
          rt(4, 8) = d * h;
          rt(4, 7) = e * g;
          rt(4, 6) = a * h;
          rt(4, 5) = b * g;
          rt(4, 4) = a * e;
          rt(4, 3) = b * d;
          rt(4, 2) = g * h;
          rt(4, 1) = d * e;
          rt(4, 0) = a * b;
          // f*g*v[8]+d*i*v[7]+c*g*v[6]+a*i*v[5]+c*d*v[4]+a*f*v[3]+g*i*v[2]+d*f*v[1]+a*c*v[0];
          rt(5, 8) = f * g;
          rt(5, 7) = d * i;
          rt(5, 6) = c * g;
          rt(5, 5) = a * i;
          rt(5, 4) = c * d;
          rt(5, 3) = a * f;
          rt(5, 2) = g * i;
          rt(5, 1) = d * f;
          rt(5, 0) = a * c;
          // d*i*v[8]+f*g*v[7]+a*i*v[6]+c*g*v[5]+a*f*v[4]+c*d*v[3]+g*i*v[2]+d*f*v[1]+a*c*v[0];
          rt(6, 8) = d * i;
          rt(6, 7) = f * g;
          rt(6, 6) = a * i;
          rt(6, 5) = c * g;
          rt(6, 4) = a * f;
          rt(6, 3) = c * d;
          rt(6, 2) = g * i;
          rt(6, 1) = d * f;
          rt(6, 0) = a * c;
          // f*h*v[8]+e*i*v[7]+c*h*v[6]+b*i*v[5]+c*e*v[4]+b*f*v[3]+h*i*v[2]+e*f*v[1]+b*c*v[0];
          rt(7, 8) = f * h;
          rt(7, 7) = e * i;
          rt(7, 6) = c * h;
          rt(7, 5) = b * i;
          rt(7, 4) = c * e;
          rt(7, 3) = b * f;
          rt(7, 2) = h * i;
          rt(7, 1) = e * f;
          rt(7, 0) = b * c;
          // e*i*v[8]+f*h*v[7]+b*i*v[6]+c*h*v[5]+b*f*v[4]+c*e*v[3]+h*i*v[2]+e*f*v[1]+b*c*v[0];
          rt(8, 8) = e * i;
          rt(8, 7) = f * h;
          rt(8, 6) = b * i;
          rt(8, 5) = c * h;
          rt(8, 4) = b * f;
          rt(8, 3) = c * e;
          rt(8, 2) = h * i;
          rt(8, 1) = e * f;
          rt(8, 0) = b * c;
          return rt;
        }  // end of BuildFromRotationMatrix<3u>::exe
      };   // end of struct BuildFromRotationMatrix<2u>

    }  // end of namespace t2tot2_internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOT2_BUILDFROMROTATIONMATRIX_HXX_ */
