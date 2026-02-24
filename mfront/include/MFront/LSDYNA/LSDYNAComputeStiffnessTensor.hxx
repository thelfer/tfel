/*!
 * \file   mfront/include/MFront/LSDYNA/LSDYNAComputeStiffnessTensor.hxx
 * \brief  This file declares the LSDYNAComputeStiffnessTensor class
 * \author Thomas Helfer
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_LSDYNACOMPUTESTIFFNESSTENSOR_HXX
#define LIB_MFRONT_LSDYNACOMPUTESTIFFNESSTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/LSDYNA/LSDYNA.hxx"
#include "MFront/LSDYNA/LSDYNAConfig.hxx"
#include "MFront/LSDYNA/LSDYNATraits.hxx"

namespace lsdyna {

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void LSDYNAComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void LSDYNAComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void LSDYNAComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void LSDYNAComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNAComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNAComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNAComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_LSDYNA_VISIBILITY_EXPORT void
  LSDYNAComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor&,
      const double* const);

  /*!
   * This structure is in charge of computing the Stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template <LSDYNABehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis,
            LSDYNASymmetryType,
            bool>
  struct LSDYNAComputeStiffnessTensor;

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC,
      true> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC,
      false> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeIsotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<3u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeIsotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC,
      true> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC,
      false> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeOrthotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<2u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL LSDYNAComputeStiffnessTensor<
      lsdyna::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<3u, real, false>::StiffnessTensor& C,
        const real* const props) {
      LSDYNAComputeOrthotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

  /*!
   * Partial specialisation for finite strain behaviours
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H,
            LSDYNASymmetryType stype,
            bool b>
  struct TFEL_VISIBILITY_LOCAL
      LSDYNAComputeStiffnessTensor<lsdyna::STANDARDFINITESTRAINBEHAVIOUR,
                                   H,
                                   stype,
                                   b> {
    typedef tfel::material::ModellingHypothesisToSpaceDimension<H>
        ModellingHypothesisToSpaceDimension;
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    template <typename real>
    static void exe(
        typename tfel::config::Types<ModellingHypothesisToSpaceDimension::value,
                                     real,
                                     false>::StiffnessTensor& D,
        const real* const props) {
      LSDYNAComputeStiffnessTensor<lsdyna::STANDARDSTRAINBASEDBEHAVIOUR, H,
                                   stype, b>::exe(D, props);
    }
  };  // end of struct LSDYNAComputeStiffnessTensor

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNACOMPUTESTIFFNESSTENSOR_HXX */
