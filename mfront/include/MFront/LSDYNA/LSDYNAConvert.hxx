/*!
 * \file   LSDYNAConvert.hxx
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

#ifndef LIB_MFRONT_LSDYNA_LSDYNACONVERT_HXX
#define LIB_MFRONT_LSDYNA_LSDYNACONVERT_HXX

#include "TFEL/Math/General/MathConstants.hxx"

namespace lsdyna {

  /*!
   * \brief class defining the convertion from lsdyna to mfront for
   * driving variables
   * \tparam H: modelling hypothesis
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct UMATImportGradients {
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
                           const LSDYNAReal* const v) {
      e.importVoigt(v);
    }  // end of exe
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::tensor<N, T>& F,
                           const LSDYNAReal* const v) {
      tfel::math::tensor<N, T>::buildFromFortranMatrix(F, v);
    }  // end of exe
  };   // end of struct UMATImportGradients
  /*!
   * \brief partial specialisation of the UMATImportGradients
   * for the plane stress modelling hypothesis.
   */
  template <>
  struct UMATImportGradients<tfel::material::ModellingHypothesis::PLANESTRESS> {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::stensor<2u, T>& e,
                           const LSDYNAReal* const v) {
      constexpr auto icste = tfel::math::Cste<LSDYNAReal>::isqrt2;
      e[0] = v[0];
      e[1] = v[1];
      e[2] = LSDYNAReal{0};
      e[3] = v[2] * icste;
    }  // end of exe
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::tensor<2u, T>& F,
                           const LSDYNAReal* const v) {
      tfel::math::tensor<2u, T>::buildFromFortranMatrix(F, v);
    }  // end of exe
  };   // end of struct UMATImportGradients
  /*!
   * \brief class defining the convertion from lsdyna to mfront for
   * thermodynamic forces
   * \tparam H: modelling hypothesis
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct UMATImportThermodynamicForces {
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
                           const LSDYNAReal* const v) {
      s.importTab(v);
    }  // end of exe
  };   // end of struct UMATImportThermodynamicForces
  /*!
   * \brief partial specialisation of the UMATImportThermodynamicForces
   * for the plane stress modelling hypothesis.
   */
  template <>
  struct UMATImportThermodynamicForces<
      tfel::material::ModellingHypothesis::PLANESTRESS> {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] s: symmetric tensor to be filled
     * \param[in]  v: values
     */
    template <typename T>
    static inline void exe(tfel::math::stensor<2u, T>& s,
                           const LSDYNAReal* const v) {
      constexpr auto cste = tfel::math::Cste<LSDYNAReal>::sqrt2;
      s[0] = v[0];
      s[1] = v[1];
      s[2] = LSDYNAReal{0};
      s[3] = v[2] * cste;
    }  // end of exe
  };   // end of struct UMATImportThermodynamicForces
  /*!
   * \brief class defining the convertion from mfront to lsdyna for
   * thermodynamic forces
   * \tparam H: modelling hypothesis
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct UMATExportThermodynamicForces {
    //! space dimension
    static constexpr unsigned short N =
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] v: values
     * \param[in]  s: symmetric tensor to be exported
     */
    template <typename T>
    static inline void exe(LSDYNAReal* const v,
                           const tfel::math::stensor<N, T>& s) {
      s.exportTab(v);
    }  // end of exe
  };   // end of struct UMATExportThermodynamicForces
  /*!
   * \brief partial specialisation of the UMATExportThermodynamicForces
   * for the plane stress modelling hypothesis.
   */
  template <>
  struct UMATExportThermodynamicForces<
      tfel::material::ModellingHypothesis::PLANESTRESS> {
    /*!
     * \tparam T: type of the thermodynamique forces
     * \param[out] v: values
     * \param[in]  s: symmetric tensor to be exported
     */
    template <typename T>
    static inline void exe(LSDYNAReal* const v,
                           const tfel::math::stensor<2u, T>& s) {
      constexpr auto icste = tfel::math::Cste<LSDYNAReal>::isqrt2;
      v[0] = s[0];
      v[1] = s[1];
      v[2] = s[3] * icste;
    }  // end of exe
  };   // end of struct UMATExportThermodynamicForces

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNA_LSDYNACONVERT_HXX */
