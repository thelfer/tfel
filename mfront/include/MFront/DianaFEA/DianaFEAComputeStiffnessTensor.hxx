/*!
 * \file   mfront/include/MFront/DianaFEA/DianaFEAComputeStiffnessTensor.hxx
 * \brief  This file declares the DianaFEAComputeStiffnessTensor class
 * \author Thomas Helfer
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DIANAFEACOMPUTESTIFFNESSTENSOR_HXX
#define LIB_MFRONT_DIANAFEACOMPUTESTIFFNESSTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/DianaFEA/DianaFEA.hxx"
#include "MFront/DianaFEA/DianaFEAConfig.hxx"
#include "MFront/DianaFEA/DianaFEATraits.hxx"

namespace dianafea {

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_DIANAFEA_VISIBILITY_EXPORT void
  DianaFEAComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor&,
      const double* const);

  /*!
   * This structure is in charge of computing the Stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template <DianaFEABehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis,
            DianaFEASymmetryType,
            bool>
  struct DianaFEAComputeStiffnessTensor;

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeIsotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeIsotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeOrthotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL DianaFEAComputeStiffnessTensor<
      dianafea::STANDARDSTRAINBASEDBEHAVIOUR,
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
      DianaFEAComputeOrthotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

  /*!
   * Partial specialisation for finite strain behaviours
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H,
            DianaFEASymmetryType stype,
            bool b>
  struct TFEL_VISIBILITY_LOCAL
      DianaFEAComputeStiffnessTensor<dianafea::STANDARDFINITESTRAINBEHAVIOUR,
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
      DianaFEAComputeStiffnessTensor<dianafea::STANDARDSTRAINBASEDBEHAVIOUR, H,
                                     stype, b>::exe(D, props);
    }
  };  // end of struct DianaFEAComputeStiffnessTensor

}  // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEACOMPUTESTIFFNESSTENSOR_HXX */
