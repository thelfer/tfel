/*!
 * \file   mfront/include/MFront/Ansys/AnsysComputeStiffnessTensor.hxx
 * \brief  This file declares the AnsysComputeStiffnessTensor class
 * \author Thomas Helfer
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ANSYSCOMPUTESTIFFNESSTENSOR_HXX
#define LIB_MFRONT_ANSYSCOMPUTESTIFFNESSTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Ansys/Ansys.hxx"
#include "MFront/Ansys/AnsysConfig.hxx"
#include "MFront/Ansys/AnsysTraits.hxx"

namespace ansys {

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void AnsysComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void AnsysComputeIsotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void
  AnsysComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void
  AnsysComputeIsotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void AnsysComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void AnsysComputeIsotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void AnsysComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void AnsysComputeOrthotropicStiffnessTensor2D(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void
  AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void
  AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensor(
      tfel::config::Types<2u, double, false>::StiffnessTensor&,
      const double* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void AnsysComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, float, false>::StiffnessTensor&,
      const float* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_ANSYS_VISIBILITY_EXPORT void AnsysComputeOrthotropicStiffnessTensor3D(
      tfel::config::Types<3u, double, false>::StiffnessTensor&,
      const double* const);

  /*!
   * This structure is in charge of computing the Stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template <AnsysBehaviourType,
            tfel::material::ModellingHypothesis::Hypothesis,
            AnsysSymmetryType,
            bool>
  struct AnsysComputeStiffnessTensor;

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeIsotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeIsotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeIsotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeOrthotropicPlaneStressAlteredStiffnessTensor(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeOrthotropicStiffnessTensor2D(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  template <bool b>
  struct TFEL_VISIBILITY_LOCAL AnsysComputeStiffnessTensor<
      ansys::STANDARDSTRAINBASEDBEHAVIOUR,
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
      AnsysComputeOrthotropicStiffnessTensor3D(C, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

  /*!
   * Partial specialisation for finite strain behaviours
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H,
            AnsysSymmetryType stype,
            bool b>
  struct TFEL_VISIBILITY_LOCAL
      AnsysComputeStiffnessTensor<ansys::STANDARDFINITESTRAINBEHAVIOUR,
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
      AnsysComputeStiffnessTensor<ansys::STANDARDSTRAINBASEDBEHAVIOUR, H, stype,
                                  b>::exe(D, props);
    }
  };  // end of struct AnsysComputeStiffnessTensor

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYSCOMPUTESTIFFNESSTENSOR_HXX */
