/*!
 * \file   StensorComputeIsotropicFunctionDerivative.hxx
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

#ifndef LIB_TFEL_MATH_STENSORCOMPUTEISOTROPICFUNCTIONDERIVATIVE_HXX
#define LIB_TFEL_MATH_STENSORCOMPUTEISOTROPICFUNCTIONDERIVATIVE_HXX

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned short N>
      struct StensorComputeIsotropicFunctionDerivative;

      template <>
      struct StensorComputeIsotropicFunctionDerivative<1u> {
        /*!
         * compute the derivative of an isotropic function
         * \param[out] d:   result
         * \param[in]  f:   function
         * \param[in]  df:  derivative of the function
         * \param[in]  vp:  eigen values
         * \param[in]  m:   eigenvectors
         * \param[in]  eps: criterion value used to judge if two eigenvalues are
         * equals
         */
        template <typename ST2toST2Type,
                  typename Function,
                  typename FunctionDerivative,
                  typename T>
        static typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 1u),
            void>::type
        exe2(ST2toST2Type& d,
             const Function&,
             const FunctionDerivative& df,
             const tvector<3u, T>& vp,
             const rotation_matrix<T>&,
             const T) {
          using real = ST2toST2NumType<ST2toST2Type>;
          constexpr const auto zero = real(0);
          d(0, 0) = df(vp[0]);
          d(0, 1) = d(0, 2) = zero;
          d(1, 1) = df(vp[1]);
          d(1, 0) = d(1, 2) = zero;
          d(2, 2) = df(vp[2]);
          d(2, 0) = d(2, 1) = zero;
        }  // end of exe
           /*!
            * compute the derivative of an isotropic function
            * \param[out] d:   result
            * \param[in]  f:   function values
            * \param[in]  df:  values of the derivative of the function
            * \param[in]  vp:  eigen values
            * \param[in]  m:   eigenvectors
            * \param[in]  eps: criterion value used to judge if two eigenvalues are
            * equals
            */
        template <typename ST2toST2Type, typename T1, typename T2, typename T3>
        static typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 1u),
            void>::type
        exe(ST2toST2Type& d,
            const tvector<3u, T1>&,
            const tvector<3u, T2>& df,
            const tvector<3u, T3>&,
            const tmatrix<3u, 3u, tfel::typetraits::base_type<T3>>&,
            const T3) {
          using real = ST2toST2NumType<ST2toST2Type>;
          constexpr const auto zero = real(0);
          d(0, 0) = df[0];
          d(0, 1) = d(0, 2) = zero;
          d(1, 1) = df[1];
          d(1, 0) = d(1, 2) = zero;
          d(2, 2) = df[2];
          d(2, 0) = d(2, 1) = zero;
        }  // end of exe
      };

      template <>
      struct StensorComputeIsotropicFunctionDerivative<2u> {
        /*!
         * compute the derivative of an isotropic function
         * \param[out] d:   result
         * \param[in]  f:   function values
         * \param[in]  df:  values of the derivative of the function
         * \param[in]  vp:  eigen values
         * \param[in]  m:   eigenvectors
         * \param[in]  eps: criterion value used to judge if two eigenvalues are
         * equals
         */
        template <typename ST2toST2Type, typename T1, typename T2, typename T3>
        static typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 2u),
            void>::type
        exe(ST2toST2Type& d,
            const tvector<3u, T1>& f,
            const tvector<3u, T2>& df,
            const tvector<3u, T3>& vp,
            const tmatrix<3u, 3u, tfel::typetraits::base_type<T3>>& m,
            const T3 eps) {
          using real = ST2toST2NumType<ST2toST2Type>;
          using base = tfel::typetraits::base_type<real>;
          using tvector = tfel::math::tvector<3u, real>;
          using stensor = tfel::math::stensor<2u, real>;
          constexpr const base cste = Cste<base>::sqrt2;
          stensor n0, n1, n2;
          stensor::computeEigenTensors(n0, n1, n2, m);
          const tvector v0 = m.template column_view<0u>();
          const tvector v1 = m.template column_view<1u>();
          const auto n01 =
              stensor::buildFromVectorsSymmetricDiadicProduct(v0, v1) / cste;
          if (std::abs(vp(0) - vp(1)) > eps) {
            d = (n0 ^ n0) * df[0] + (n1 ^ n1) * df[1] + (n2 ^ n2) * df[2] +
                (f[0] - f[1]) / (vp(0) - vp(1)) * (n01 ^ n01);
          } else {
            const auto dfm = (df[0] + df[1]) / 2;
            d = ((n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01)) * dfm + (n2 ^ n2) * df[2];
          }
        }
        /*!
         * compute the derivative of an isotropic function
         * \param[out] d:   result
         * \param[in]  f:   function
         * \param[in]  df:  derivative of the function
         * \param[in]  vp:  eigen values
         * \param[in]  m:   eigenvectors
         * \param[in]  eps: criterion value used to judge if two eigenvalues are
         * equals
         */
        template <typename ST2toST2Type,
                  typename T,
                  typename Function,
                  typename FunctionDerivative>
        static typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 2u),
            void>::type
        exe2(ST2toST2Type& d,
             const Function& f,
             const FunctionDerivative& df,
             const tvector<3u, T>& vp,
             const rotation_matrix<T>& m,
             const T eps) {
          const auto fv = map(f, vp);
          const auto dfv = map(df, vp);
          StensorComputeIsotropicFunctionDerivative::exe(d, fv, dfv, vp, m,
                                                         eps);
        }  // end of exe
      };

      template <>
      struct StensorComputeIsotropicFunctionDerivative<3u> {
        /*!
         * compute the derivative of an isotropic function
         * \param[out] d:   result
         * \param[in]  f:   function values
         * \param[in]  df:  values of the derivative of the function
         * \param[in]  vp:  eigen values
         * \param[in]  m:   eigenvectors
         * \param[in]  eps: criterion value used to judge if two eigenvalues are
         * equals
         */
        template <typename ST2toST2Type, typename T1, typename T2, typename T3>
        static typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 3u),
            void>::type
        exe(ST2toST2Type& d,
            const tvector<3u, T1>& f,
            const tvector<3u, T2>& df,
            const tvector<3u, T3>& vp,
            const tmatrix<3u, 3u, tfel::typetraits::base_type<T3>>& m,
            const T3 eps) {
          using real = ST2toST2NumType<ST2toST2Type>;
          using base = tfel::typetraits::base_type<real>;
          using tvector = tfel::math::tvector<3u, real>;
          using stensor = tfel::math::stensor<3u, real>;
          using st2tost2 = tfel::math::st2tost2<3u, real>;
          constexpr const base cste = Cste<base>::sqrt2;
          if ((std::abs(vp(0) - vp(1)) < eps) &&
              (std::abs(vp(0) - vp(2)) < eps)) {
            const auto dfm = (df[0] + df[1] + df[2]) / 3;
            d = st2tost2::Id() * dfm;
          } else if (std::abs(vp(0) - vp(1)) < eps) {
            const tvector v0 = m.template column_view<0u>();
            const tvector v1 = m.template column_view<1u>();
            const tvector v2 = m.template column_view<2u>();
            stensor n0, n1, n2;
            stensor::computeEigenTensors(n0, n1, n2, m);
            const auto n01 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v1) / cste;
            const auto n02 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v2) / cste;
            const auto n12 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v1, v2) / cste;
            const auto vpm = (vp(0) + vp(1)) / 2;
            const auto dfm = (df[0] + df[1]) / 2;
            d = (((n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01)) * dfm +
                 (n2 ^ n2) * df[2] +
                 (f[0] - f[2]) / (vpm - vp(2)) * ((n02 ^ n02) + (n12 ^ n12)));
          } else if (std::abs(vp(0) - vp(2)) < eps) {
            const tvector v0 = m.template column_view<0u>();
            const tvector v1 = m.template column_view<1u>();
            const tvector v2 = m.template column_view<2u>();
            stensor n0, n1, n2;
            stensor::computeEigenTensors(n0, n1, n2, m);
            const auto n01 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v1) / cste;
            const auto n02 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v2) / cste;
            const auto n12 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v1, v2) / cste;
            const auto vpm = (vp(0) + vp(2)) / 2;
            const auto dfm = (df[0] + df[2]) / 2;
            d = (((n0 ^ n0) + (n2 ^ n2) + (n02 ^ n02)) * dfm +
                 (n1 ^ n1) * df[1] +
                 (f[0] - f[1]) / (vpm - vp(1)) * ((n01 ^ n01) + (n12 ^ n12)));
          } else if (std::abs(vp(1) - vp(2)) < eps) {
            const tvector v0 = m.template column_view<0u>();
            const tvector v1 = m.template column_view<1u>();
            const tvector v2 = m.template column_view<2u>();
            stensor n0, n1, n2;
            stensor::computeEigenTensors(n0, n1, n2, m);
            const auto n01 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v1) / cste;
            const auto n02 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v0, v2) / cste;
            const auto n12 =
                stensor::buildFromVectorsSymmetricDiadicProduct(v1, v2) / cste;
            const auto vpm = (vp(1) + vp(2)) / 2;
            const auto dfm = (df[1] + df[2]) / 2;
            d = ((n0 ^ n0) * df[0] +
                 ((n1 ^ n1) + (n2 ^ n2) + (n12 ^ n12)) * dfm +
                 ((f[0] - f[1]) / (vp(0) - vpm)) * ((n01 ^ n01) + (n02 ^ n02)));
          } else {
            st2tost2 dn0, dn1, dn2;
            stensor n0, n1, n2;
            stensor::computeEigenTensors(n0, n1, n2, m);
            stensor::computeEigenTensorsDerivatives(dn0, dn1, dn2, vp, m,
                                                    eps / 4);
            d = (n0 ^ n0) * df[0] + (n1 ^ n1) * df[1] + (n2 ^ n2) * df[2] +
                f[0] * dn0 + f[1] * dn1 + f[2] * dn2;
          }
        }  // end of exe
           /*!
            * compute the derivative of an isotropic function
            * \param[out] d:   result
            * \param[in]  f:   function
            * \param[in]  df:  derivative of the function
            * \param[in]  vp:  eigen values
            * \param[in]  m:   eigenvectors
            * \param[in]  eps: criterion value used to judge if two eigenvalues are
            * equals
            */
        template <typename ST2toST2Type,
                  typename T,
                  typename Function,
                  typename FunctionDerivative>
        static typename std::enable_if<
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
                (ST2toST2Traits<ST2toST2Type>::dime == 3u),
            void>::type
        exe2(ST2toST2Type& d,
             const Function& f,
             const FunctionDerivative& df,
             const tvector<3u, T>& vp,
             const rotation_matrix<T>& m,
             const T eps) {
          const auto fv = map(f, vp);
          const auto dfv = map(df, vp);
          StensorComputeIsotropicFunctionDerivative::exe(d, fv, dfv, vp, m,
                                                         eps);
        }  // end of exe
      };

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_STENSORCOMPUTEISOTROPICFUNCTIONDERIVATIVE_HXX */
