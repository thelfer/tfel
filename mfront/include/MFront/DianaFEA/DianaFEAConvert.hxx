/*!
 * \file   DianaFEAConvert.hxx
 * \brief
 * \author Thomas Helfer
 * \date   22 mars 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DIANAFEA_DIANAFEACONVERT_HXX
#define LIB_MFRONT_DIANAFEA_DIANAFEACONVERT_HXX

#include "TFEL/Math/General/MathConstants.hxx"

namespace dianafea {

  /*!
   * \brief class defining the convertion from dianafea to mfront for
   * symmetric tensors. This class is specialised in 1D, 2D and 3D.
   * \tparam N: space dimension
   */
  template <unsigned short N>
  struct ImportSymmetricTensor;
  /*!
   * \brief class defining the convertion from mfront to dianafea for
   * symmetric tensors. This class is specialised in 1D, 2D and 3D.
   * \tparam N: space dimension
   */
  template <unsigned short N>
  struct ExportSymmetricTensor;

  /*!
   * \brief class defining the convertion from dianafea to mfront for
   * driving variables
   * \tparam H: modelling hypothesis
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct ImportGradients {
    //! space dimension
    static constexpr unsigned short N =
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::stensor<N, T>& e,
                           const DianaFEAReal* const v) {
      ImportSymmetricTensor<N>::exe(e, v);
    }  // end of exe
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::tensor<N, T>& F,
                           const DianaFEAReal* const v) {
      tfel::math::tensor<N, T>::buildFromFortranMatrix(F, v);
    }  // end of exe
  };   // end of struct ImportGradients

  /*!
   * \brief class defining the convertion from dianafea to mfront for
   * thermodynamic forces
   * \tparam H: modelling hypothesis
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct ImportThermodynamicForces {
    //! space dimension
    static constexpr unsigned short N =
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::stensor<N, T>& s,
                           const DianaFEAReal* const v) {
      ImportSymmetricTensor<N>::exe(s, v);
    }  // end of exe
  };   // end of struct ImportThermodynamicForces

  /*!
   * \brief class defining the convertion from mfront to dianafea for
   * thermodynamic forces
   * \tparam H: modelling hypothesis
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct ExportThermodynamicForces {
    //! space dimension
    static constexpr unsigned short N =
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] v: values
     * \param[in]  s: symmetric tensor to be exported
     */
    template <typename T>
    static inline void exe(DianaFEAReal* const v,
                           const tfel::math::stensor<N, T>& s) {
      ExportSymmetricTensor<N>::exe(v, s);
    }  // end of exe
  };   // end of struct ExportThermodynamicForces

  /*!
   * \brief partial specialisation of the ImportSymmetricTensor in 1D.
   */
  template <>
  struct ImportSymmetricTensor<1u> {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::stensor<1u, T>& e,
                           const DianaFEAReal* const v) {
      e[0] = v[0];
      e[1] = v[1];
      e[2] = v[2];
    }  // end of exe
  };
  /*!
   * \brief partial specialisation of the ImportSymmetricTensor in 2D.
   */
  template <>
  struct ImportSymmetricTensor<2u> {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::stensor<2u, T>& e,
                           const DianaFEAReal* const v) {
      constexpr auto icste = tfel::math::Cste<T>::isqrt2;
      e[0] = v[0];
      e[1] = v[1];
      e[2] = v[2];
      e[3] = v[3] * icste;
    }  // end of exe
  };
  /*!
   * \brief partial specialisation of the ImportSymmetricTensor in 3D.
   */
  template <>
  struct ImportSymmetricTensor<3u> {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::stensor<3u, T>& e,
                           const DianaFEAReal* const v) {
      constexpr auto icste = tfel::math::Cste<T>::isqrt2;
      e[0] = v[0];
      e[1] = v[1];
      e[2] = v[2];
      e[3] = v[3] * icste;
      e[4] = v[5] * icste;
      e[5] = v[4] * icste;
    }  // end of exe
  };
  /*!
   * \brief partial specialisation of the ExportSymmetricTensor in 1D.
   */
  template <>
  struct ExportSymmetricTensor<1u> {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(DianaFEAReal* const v,
                           const tfel::math::stensor<1u, T>& e) {
      v[0] = e[0];
      v[1] = e[1];
      v[2] = e[2];
    }  // end of exe
  };
  /*!
   * \brief partial specialisation of the ExportSymmetricTensor in 2D.
   */
  template <>
  struct ExportSymmetricTensor<2u> {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(DianaFEAReal* const v,
                           const tfel::math::stensor<2u, T>& e) {
      constexpr auto icste = tfel::math::Cste<T>::isqrt2;
      v[0] = e[0];
      v[1] = e[1];
      v[2] = e[2];
      v[3] = e[3] * icste;
    }  // end of exe
  };
  /*!
   * \brief partial specialisation of the ExportSymmetricTensor in 3D.
   */
  template <>
  struct ExportSymmetricTensor<3u> {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(DianaFEAReal* const v,
                           const tfel::math::stensor<3u, T>& e) {
      constexpr auto icste = tfel::math::Cste<T>::isqrt2;
      v[0] = e[0];
      v[1] = e[1];
      v[2] = e[2];
      v[3] = e[3] * icste;
      v[5] = e[4] * icste;
      v[4] = e[5] * icste;
    }  // end of exe
  };

}  // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEA_DIANAFEACONVERT_HXX */
