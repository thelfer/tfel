/*!
 * \file   include/TFEL/Math/ST2toST2/BuildFromRotationMatrix.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13 oct. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2_BUILDFROMROTATIONMATRIX_HXX
#define LIB_TFEL_MATH_ST2TOST2_BUILDFROMROTATIONMATRIX_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace tfel::math::st2tost2_internals {

  /*!
   * \brief an auxiliary structure used to dispatch over the
   * dimension
   */
  template <unsigned short, typename T>
  struct BuildFromRotationMatrix;
  /*!
   * \brief partial specialisation in 1D
   */
  template <typename T>
  struct BuildFromRotationMatrix<1u, T> {
    /*!
     * \return the st2tost2 that has the same effect as applying
     * the rotation
     */
    static TFEL_MATH_INLINE2 tfel::math::st2tost2<1u, T> exe(
        const rotation_matrix<T>&) {
      return tfel::math::st2tost2<1u, T>::Id();
    }
  };  // end of struct BuildFromRotationMatrix<1u>
  /*!
   * \brief partial specialisation in 2D
   */
  template <typename T>
  struct BuildFromRotationMatrix<2u, T> {
    /*!
     * \return the st2tost2 that has the same effect as applying
     * the rotation
     * \param[in] r : rotation matrix
     */
    static TFEL_MATH_INLINE2 tfel::math::st2tost2<2u, T> exe(
        const rotation_matrix<T>& r) {
      tfel::math::st2tost2<2u, T> s;
      constexpr auto cste = Cste<T>::sqrt2;
      constexpr T zero = T{0};
      constexpr T one = T{1};
      // tmp[0]  =
      // cste*r(0,0)*r(1,0)*s[3]+r(1,0)*r(1,0)*s[1]+r(0,0)*r(0,0)*s[0];
      s(0, 0) = r(0, 0) * r(0, 0);
      s(0, 1) = r(1, 0) * r(1, 0);
      s(0, 2) = zero;
      s(0, 3) = cste * r(0, 0) * r(1, 0);
      // tmp[1]  =
      // cste*r(0,1)*r(1,1)*s[3]+r(1,1)*r(1,1)*s[1]+r(0,1)*r(0,1)*s[0];
      s(1, 0) = r(0, 1) * r(0, 1);
      s(1, 1) = r(1, 1) * r(1, 1);
      s(1, 2) = zero;
      s(1, 3) = cste * r(0, 1) * r(1, 1);
      // this component is invariant
      s(2, 0) = zero;
      s(2, 1) = zero;
      s(2, 2) = one;
      s(2, 3) = zero;
      // tmp[2]  = (r(0,0)*r(1,1)+r(0,1)*r(1,0))*s[3]+
      //           cste*(r(1,0)*r(1,1)*s[1]+r(0,0)*r(0,1)*s[0])
      s(3, 0) = cste * r(0, 0) * r(0, 1);
      s(3, 1) = cste * r(1, 0) * r(1, 1);
      s(3, 2) = zero;
      s(3, 3) = r(0, 0) * r(1, 1) + r(0, 1) * r(1, 0);
      return s;
    }  // end of BuildFromRotationMatrix<2u>::exe
  };   // end of struct BuildFromRotationMatrix<2u>
  /*!
   * \brief partial specialisation in 3D
   */
  template <typename T>
  struct BuildFromRotationMatrix<3u, T> {
    /*!
     * \return the st2tost2 that has the same effect as applying
     * the rotation
     * \param[in] r : rotation matrix
     */
    static TFEL_MATH_INLINE2 tfel::math::st2tost2<3u, T> exe(
        const rotation_matrix<T>& r) {
      constexpr auto cste = Cste<T>::sqrt2;
      tfel::math::st2tost2<3u, T> s;
      const T a = r(0, 0);
      const T b = r(0, 1);
      const T c = r(0, 2);
      const T d = r(1, 0);
      const T e = r(1, 1);
      const T f = r(1, 2);
      const T g = r(2, 0);
      const T h = r(2, 1);
      const T i = r(2, 2);
      // tmp[0] =
      // cste*(d*g*s[5]+a*g*s[4]+a*d*s[3])+g*g*s[2]+d*d*s[1]+a*a*s[0];
      s(0, 0) = a * a;
      s(0, 1) = d * d;
      s(0, 2) = g * g;
      s(0, 3) = cste * a * d;
      s(0, 4) = cste * a * g;
      s(0, 5) = cste * d * g;
      // tmp[1] =
      // cste*(e*h*s[5]+b*h*s[4]+b*e*s[3])+h*h*s[2]+e*e*s[1]+b*b*s[0];
      s(1, 0) = b * b;
      s(1, 1) = e * e;
      s(1, 2) = h * h;
      s(1, 3) = cste * b * e;
      s(1, 4) = cste * b * h;
      s(1, 5) = cste * e * h;
      // tmp[2] =
      // cste*(f*i*s[5]+c*i*s[4]+c*f*s[3])+i*i*s[2]+f*f*s[1]+c*c*s[0];
      s(2, 0) = c * c;
      s(2, 1) = f * f;
      s(2, 2) = i * i;
      s(2, 3) = cste * c * f;
      s(2, 4) = cste * c * i;
      s(2, 5) = cste * f * i;
      // tmp[3]  = (d*h+e*g)*s[5]+(a*h+b*g)*s[4]+(a*e+b*d)*s[3];
      // tmp[3] += cste*(g*h*s[2]+d*e*s[1]+a*b*s[0]);
      s(3, 0) = cste * a * b;
      s(3, 1) = cste * d * e;
      s(3, 2) = cste * g * h;
      s(3, 3) = a * e + b * d;
      s(3, 4) = a * h + b * g;
      s(3, 5) = d * h + e * g;
      // tmp[4]  = (d*i+f*g)*s[5]+(a*i+c*g)*s[4]+(a*f+c*d)*s[3];
      // tmp[4] += cste*(g*i*s[2]+d*f*s[1]+a*c*s[0]);
      s(4, 0) = cste * a * c;
      s(4, 1) = cste * d * f;
      s(4, 2) = cste * g * i;
      s(4, 3) = a * f + c * d;
      s(4, 4) = a * i + c * g;
      s(4, 5) = d * i + f * g;
      // tmp[5]  = (e*i+f*h)*s[5]+(b*i+c*h)*s[4]+(b*f+c*e)*s[3];
      // tmp[5] += cste*(h*i*s[2]+e*f*s[1]+b*c*s[0]);
      s(5, 0) = cste * b * c;
      s(5, 1) = cste * e * f;
      s(5, 2) = cste * h * i;
      s(5, 3) = b * f + c * e;
      s(5, 4) = b * i + c * h;
      s(5, 5) = e * i + f * h;
      return s;
    }  // end of BuildFromRotationMatrix<3u>::exe
  };   // end of struct BuildFromRotationMatrix<3u>

}  // end of namespace tfel::math::st2tost2_internals

#endif /* LIB_TFEL_MATH_ST2TOST2_BUILDFROMROTATIONMATRIX_HXX */
