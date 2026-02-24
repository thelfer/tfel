/*!
 * \file   mfront/include/MFront/Castem/CastemComputeStiffnessTensor.hxx
 * \brief  This file declares the CastemComputeStiffnessTensor class
 * \author Thomas Helfer
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEMCOMPUTESTIFFNESSTENSOR_HXX
#define LIB_MFRONT_CASTEMCOMPUTESTIFFNESSTENSOR_HXX

#include "MFront/MFrontConfig.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Castem/Castem.hxx"
#include "MFront/Castem/CastemConfig.hxx"
#include "MFront/Castem/CastemTraits.hxx"

namespace castem {

  /*!
   * \brief compute the stiffness tensor for axisymmetrical
   * generalised hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void CastemComputeIsotropicStiffnessTensor1D(
      tfel::config::Types<1u, CastemReal, false>::StiffnessTensor&,
      const CastemReal* const);

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void CastemComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, CastemReal, false>::StiffnessTensor&,
      const CastemReal* const);

  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void
  CastemComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, CastemReal, false>::StiffnessTensor&,
      const CastemReal* const);

  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void CastemComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, CastemReal, false>::StiffnessTensor&,
      const CastemReal* const);

  /*!
   * \brief compute the stiffness tensor for axisymmetrical
   * generalised hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void
  CastemComputeOrthotropicStiffnessTensor1D(
      tfel::config::Types<1u, CastemReal, false>::StiffnessTensor&,
      const CastemReal* const);

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void
  CastemComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, CastemReal, false>::StiffnessTensor&,
      const CastemReal* const);

  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void
  CastemComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, CastemReal, false>::StiffnessTensor&,
      const CastemReal* const);

  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void
  CastemComputeOrthotropicPlaneStressUnAlteredStiffnessTensor(
      tfel::config::Types<2u, CastemReal, false>::StiffnessTensor&,
      const CastemReal* const);

  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT void
  CastemComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, CastemReal, false>::StiffnessTensor&,
      const CastemReal* const);

  /*!
   * \brief This structure is in charge of computing the elastic
   * stiffness tensor from the material properties given by `Cast3M`.
   * The resulting operator uses `MFront` representation of tensors and
   * symmetric tensors.
   * \tparam btype: behaviour type
   * \tparam H: modelling hypothesis
   * \tparam s: symmetry type (isotropic or orthotropic)
   * \tparam b: boolean stating if the one computes the altered or
   * unaltered stiffness tensor. This parameter is only meaningful for
   * plane stress hypotheses.
   */
  template <CastemBehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis,
            CastemSymmetryType,
            bool>
  struct CastemComputeStiffnessTensor;
  /*!
   * \brief partial specialisation of the
   * `CastemComputeStiffnessTensor` for isotropic strain based
   * behaviour in axisymmetrical generalised plane strain.
   * \tparam b: unused boolean.
   */
  template <bool b>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<1u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeIsotropicStiffnessTensor1D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor
  /*!
   * \brief partial specialisation of the
   * `CastemComputeStiffnessTensor` for isotropic strain based
   * behaviour in axisymmetry.
   * \tparam b: unused boolean.
   */
  template <bool b>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor
  /*!
   * \brief partial specialisation of the
   * `CastemComputeStiffnessTensor` for isotropic strain based
   * behaviour in plane stress. The `exe` method will return the
   * unaltered stifness tensor.
   */
  template <>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC,
      true> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor
  /*!
   * \brief partial specialisation of the
   * `CastemComputeStiffnessTensor` for isotropic strain based
   * behaviour in plane stress. The `exe` method will return the
   * altered stifness tensor.
   */
  template <>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC,
      false> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeIsotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor
  /*!
   * \brief partial specialisation of the
   * `CastemComputeStiffnessTensor` for isotropic strain based
   * behaviour in plane strain.
   * \tparam b: unused boolean.
   */
  template <bool b>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor
  /*!
   * \brief partial specialisation of the
   * `CastemComputeStiffnessTensor` for isotropic strain based
   * behaviour in generalised plane strain.
   * \tparam b: unused boolean.
   */
  template <bool b>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor
  /*!
   * \brief partial specialisation of the
   * `CastemComputeStiffnessTensor` for isotropic strain based
   * behaviour in 3D.
   * \tparam b: unused boolean.
   */
  template <bool b>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<3u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeIsotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<1u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeOrthotropicStiffnessTensor1D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC,
      true> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeOrthotropicPlaneStressUnAlteredStiffnessTensor(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC,
      false> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeOrthotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor
  /*!
   * \brief
   */
  template <bool b>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL CastemComputeStiffnessTensor<
      castem::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<3u, CastemReal, false>::StiffnessTensor& C,
        const CastemReal* const props) {
      CastemComputeOrthotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct CastemComputeStiffnessTensor

  /*!
   * Partial specialisation for finite strain behaviours
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H,
            CastemSymmetryType stype,
            bool b>
  struct TFEL_VISIBILITY_LOCAL
      CastemComputeStiffnessTensor<castem::STANDARDFINITESTRAINBEHAVIOUR,
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
                                     CastemReal,
                                     false>::StiffnessTensor& D,
        const CastemReal* const props) {
      CastemComputeStiffnessTensor<castem::STANDARDSTRAINBASEDBEHAVIOUR, H,
                                   stype, b>::exe(D, props);
    }
  };  // end of struct CastemComputeStiffnessTensor

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEMCOMPUTESTIFFNESSTENSOR_HXX */
