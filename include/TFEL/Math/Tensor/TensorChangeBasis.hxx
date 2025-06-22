/*!
 * \file  include/TFEL/Math/Tensor/TensorChangeBasis.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 04 juil. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSORCHANGEBASIS_HXX
#define LIB_TFEL_MATH_TENSORCHANGEBASIS_HXX

namespace tfel {

  namespace math {

    /*!
     * \brief structure in charge of changing the basis of a tensor
     * \tparam N, the spatial dimension, see TensorDimeToSize for details.
     * \tparam T, numerical type used, by default, double
     * \note partial specialisation are provided in 1D, 2D and 3D
     */
    template <unsigned short N, typename T>
    struct TensorChangeBasis;
    /*!
     * \brief partial specialisation in 1D
     */
    template <typename T>
    struct TensorChangeBasis<1u, T> {
      //! a simple alias
      using real = tfel::typetraits::base_type<T>;
      //! a simple alias
      typedef const tmatrix<3u, 3u, real> RotationMatrix;
      /*!
       * do the change of basis
       * \param[in,out] v : on input, initial values of the tensor, on output
       * new values \param[out]    m : rotation matrix
       */
      static TFEL_MATH_INLINE2 void exe(T* const, const RotationMatrix&) {
      }  // end of exe
    };
    /*!
     * \brief partial specialisation in 2D
     */
    template <typename T>
    struct TensorChangeBasis<2u, T> {
      //! a simple alias
      using real = tfel::typetraits::base_type<T>;
      //! a simple alias
      typedef const tmatrix<3u, 3u, real> RotationMatrix;
      /*!
       * do the change of basis
       * \param[in,out] v : on input, initial values of the tensor, on output
       * new values \param[out]    m : rotation matrix
       */
      static TFEL_MATH_INLINE2 void exe(T* const v, const RotationMatrix& m) {
        using tfel::fsalgo::copy;
        T nv[5u];
        const T a = m(0, 0);
        const T b = m(0, 1);
        const T d = m(1, 0);
        const T e = m(1, 1);
        nv[0] = a * d * v[4] + a * d * v[3] + d * d * v[1] + a * a * v[0];
        nv[1] = b * e * v[4] + b * e * v[3] + e * e * v[1] + b * b * v[0];
        nv[3] = b * d * v[4] + a * e * v[3] + d * e * v[1] + a * b * v[0];
        nv[4] = a * e * v[4] + b * d * v[3] + d * e * v[1] + a * b * v[0];
        nv[2] = v[2];
        copy<5u>::exe(nv, v);
      }
    };
    /*!
     * \brief partial specialisation in 3D
     */
    template <typename T>
    struct TensorChangeBasis<3u, T> {
      //! a simple alias
      using real = tfel::typetraits::base_type<T>;
      //! a simple alias
      typedef const tmatrix<3u, 3u, real> RotationMatrix;
      /*!
       * do the change of basis
       * \param[in,out] v : on input, initial values of the tensor, on output
       * new values \param[out]    m : rotation matrix
       */
      static TFEL_MATH_INLINE2 void exe(T* const v, const RotationMatrix& m) {
        using tfel::fsalgo::copy;
        T nv[9u];
        const T a = m(0, 0);
        const T b = m(0, 1);
        const T c = m(0, 2);
        const T d = m(1, 0);
        const T e = m(1, 1);
        const T f = m(1, 2);
        const T g = m(2, 0);
        const T h = m(2, 1);
        const T i = m(2, 2);
        nv[0] = d * g * v[8] + d * g * v[7] + a * g * v[6] + a * g * v[5] +
                a * d * v[4] + a * d * v[3] + g * g * v[2] + d * d * v[1] +
                a * a * v[0];
        nv[1] = e * h * v[8] + e * h * v[7] + b * h * v[6] + b * h * v[5] +
                b * e * v[4] + b * e * v[3] + h * h * v[2] + e * e * v[1] +
                b * b * v[0];
        nv[2] = f * i * v[8] + f * i * v[7] + c * i * v[6] + c * i * v[5] +
                c * f * v[4] + c * f * v[3] + i * i * v[2] + f * f * v[1] +
                c * c * v[0];
        nv[3] = e * g * v[8] + d * h * v[7] + b * g * v[6] + a * h * v[5] +
                b * d * v[4] + a * e * v[3] + g * h * v[2] + d * e * v[1] +
                a * b * v[0];
        nv[4] = d * h * v[8] + e * g * v[7] + a * h * v[6] + b * g * v[5] +
                a * e * v[4] + b * d * v[3] + g * h * v[2] + d * e * v[1] +
                a * b * v[0];
        nv[5] = f * g * v[8] + d * i * v[7] + c * g * v[6] + a * i * v[5] +
                c * d * v[4] + a * f * v[3] + g * i * v[2] + d * f * v[1] +
                a * c * v[0];
        nv[6] = d * i * v[8] + f * g * v[7] + a * i * v[6] + c * g * v[5] +
                a * f * v[4] + c * d * v[3] + g * i * v[2] + d * f * v[1] +
                a * c * v[0];
        nv[7] = f * h * v[8] + e * i * v[7] + c * h * v[6] + b * i * v[5] +
                c * e * v[4] + b * f * v[3] + h * i * v[2] + e * f * v[1] +
                b * c * v[0];
        nv[8] = e * i * v[8] + f * h * v[7] + b * i * v[6] + c * h * v[5] +
                b * f * v[4] + c * e * v[3] + h * i * v[2] + e * f * v[1] +
                b * c * v[0];
        copy<9u>::exe(nv, v);
      }
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TENSORCHANGEBASIS_HXX */
