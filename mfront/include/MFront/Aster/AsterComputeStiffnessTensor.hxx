/*!
 * \file   mfront/include/MFront/Aster/AsterComputeStiffnessTensor.hxx
 * \brief  This file declares the AsterComputeStiffnessTensor class
 * \author Thomas Helfer
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ASTERCOMPUTESTIFFNESSTENSOR_HXX
#define LIB_MFRONT_ASTERCOMPUTESTIFFNESSTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Aster/Aster.hxx"
#include "MFront/Aster/AsterConfig.hxx"
#include "MFront/Aster/AsterTraits.hxx"

namespace aster {

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ASTER_VISIBILITY_EXPORT void AsterComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, AsterReal, false>::StiffnessTensor&,
      const AsterReal* const);

  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ASTER_VISIBILITY_EXPORT void
  AsterComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, AsterReal, false>::StiffnessTensor&,
      const AsterReal* const);

  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ASTER_VISIBILITY_EXPORT void AsterComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, AsterReal, false>::StiffnessTensor&,
      const AsterReal* const);

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ASTER_VISIBILITY_EXPORT void AsterComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, AsterReal, false>::StiffnessTensor&,
      const AsterReal* const);

  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ASTER_VISIBILITY_EXPORT void
  AsterComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, AsterReal, false>::StiffnessTensor&,
      const AsterReal* const);

  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ASTER_VISIBILITY_EXPORT void AsterComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, AsterReal, false>::StiffnessTensor&,
      const AsterReal* const);

  /*!
   * This structure is in charge of computing the Stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template <AsterBehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis,
            AsterSymmetryType,
            bool>
  struct AsterComputeStiffnessTensor;

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC,
      true> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ISOTROPIC,
      false> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeIsotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ISOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<3u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeIsotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::AXISYMMETRICAL,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC,
      true> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRESS,
      ORTHOTROPIC,
      false> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeOrthotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::PLANESTRAIN,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<2u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AsterComputeStiffnessTensor<
      aster::STANDARDSTRAINBASEDBEHAVIOUR,
      tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
      ORTHOTROPIC,
      b> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<3u, AsterReal, false>::StiffnessTensor& C,
        const AsterReal* const props) {
      AsterComputeOrthotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

  /*!
   * Partial specialisation for finite strain behaviours
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H,
            AsterSymmetryType stype,
            bool b>
  struct TFEL_VISIBILITY_LOCAL
      AsterComputeStiffnessTensor<aster::STANDARDFINITESTRAINBEHAVIOUR,
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
                                     AsterReal,
                                     false>::StiffnessTensor& D,
        const AsterReal* const props) {
      AsterComputeStiffnessTensor<aster::STANDARDSTRAINBASEDBEHAVIOUR, H, stype,
                                  b>::exe(D, props);
    }
  };  // end of struct AsterComputeStiffnessTensor

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTERCOMPUTESTIFFNESSTENSOR_HXX */
