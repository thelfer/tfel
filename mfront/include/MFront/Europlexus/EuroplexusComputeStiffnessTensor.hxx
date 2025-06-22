/*!
 * \file   mfront/include/MFront/Europlexus/EuroplexusComputeStiffnessTensor.hxx
 * \brief  This file declares the EuroplexusComputeStiffnessTensor class
 * \author Thomas Helfer
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_EPXCOMPUTESTIFFNESSTENSOR_HXX
#define LIB_MFRONT_EPXCOMPUTESTIFFNESSTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Europlexus/Europlexus.hxx"
#include "MFront/Europlexus/EuroplexusConfig.hxx"
#include "MFront/Europlexus/EuroplexusTraits.hxx"

namespace epx {

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_EPX_VISIBILITY_EXPORT void EuroplexusComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor&,
      const EuroplexusReal* const);

  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  EuroplexusComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor&,
      const EuroplexusReal* const);

  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_EPX_VISIBILITY_EXPORT void EuroplexusComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, EuroplexusReal, false>::StiffnessTensor&,
      const EuroplexusReal* const);

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  EuroplexusComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor&,
      const EuroplexusReal* const);

  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  EuroplexusComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor&,
      const EuroplexusReal* const);

  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_EPX_VISIBILITY_EXPORT void
  EuroplexusComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, EuroplexusReal, false>::StiffnessTensor&,
      const EuroplexusReal* const);

  /*!
   * This structure is in charge of computing the Stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template <EuroplexusBehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis,
            EuroplexusSymmetryType,
            bool>
  struct EuroplexusComputeStiffnessTensor;

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC,
      true> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC,
      false> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeIsotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<3u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeIsotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC,
      true> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC,
      false> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeOrthotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL EuroplexusComputeStiffnessTensor<
      epx::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<3u, EuroplexusReal, false>::StiffnessTensor& C,
        const EuroplexusReal* const props) {
      EuroplexusComputeOrthotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

  /*!
   * Partial specialisation for finite strain behaviours
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H,
            EuroplexusSymmetryType stype,
            bool b>
  struct TFEL_VISIBILITY_LOCAL
      EuroplexusComputeStiffnessTensor<epx::STANDARDFINITESTRAINBEHAVIOUR,
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
    static void exe(
        typename tfel::config::Types<ModellingHypothesisToSpaceDimension::value,
                                     EuroplexusReal,
                                     false>::StiffnessTensor& D,
        const EuroplexusReal* const props) {
      EuroplexusComputeStiffnessTensor<epx::STANDARDSTRAINBASEDBEHAVIOUR, H,
                                       stype, b>::exe(D, props);
    }
  };  // end of struct EuroplexusComputeStiffnessTensor

}  // end of namespace epx

#endif /* LIB_MFRONT_EPXCOMPUTESTIFFNESSTENSOR_HXX */
