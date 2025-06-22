/*!
 * \file   mfront/include/MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx
 * \brief  This file declares the AbaqusComputeStiffnessTensor class
 * \author Helfer Thomas
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUSCOMPUTESTIFFNESSTENSOR_H_
#define LIB_MFRONT_ABAQUSCOMPUTESTIFFNESSTENSOR_H_

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Abaqus/Abaqus.hxx"
#include "MFront/Abaqus/AbaqusConfig.hxx"
#include "MFront/Abaqus/AbaqusTraits.hxx"

namespace abaqus {

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void AbaqusComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void AbaqusComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void AbaqusComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void AbaqusComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ABAQUS_VISIBILITY_EXPORT void
  AbaqusComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor&,
      const double* const);

  /*!
   * This structure is in charge of computing the Stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template <AbaqusBehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis,
            AbaqusSymmetryType,
            bool>
  struct AbaqusComputeStiffnessTensor;

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeIsotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeIsotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeOrthotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AbaqusComputeStiffnessTensor<
      abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeOrthotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

  /*!
   * Partial specialisation for finite strain behaviours
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H,
            AbaqusSymmetryType stype,
            bool b>
  struct TFEL_VISIBILITY_LOCAL
      AbaqusComputeStiffnessTensor<abaqus::FINITESTRAINSTANDARDBEHAVIOUR,
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
      AbaqusComputeStiffnessTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR, H,
                                   stype, b>::exe(D, props);
    }
  };  // end of struct AbaqusComputeStiffnessTensor

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUSCOMPUTESTIFFNESSTENSOR_H_ */
