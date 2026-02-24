/*!
 * \file   mfront/include/MFront/CalculiX/CalculiXComputeStiffnessTensor.hxx
 * \brief  This file declares the CalculiXComputeStiffnessTensor class
 * \author Thomas Helfer
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CALCULIXCOMPUTESTIFFNESSTENSOR_HXX
#define LIB_MFRONT_CALCULIXCOMPUTESTIFFNESSTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/CalculiX/CalculiX.hxx"
#include "MFront/CalculiX/CalculiXConfig.hxx"
#include "MFront/CalculiX/CalculiXTraits.hxx"

namespace calculix {

  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CALCULIX_VISIBILITY_EXPORT void computeIsotropicStiffnessTensor(
      tfel::config::Types<3u, CalculiXReal, false>::StiffnessTensor&,
      const CalculiXReal* const);
  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_CALCULIX_VISIBILITY_EXPORT void computeOrthotropicStiffnessTensor(
      tfel::config::Types<3u, CalculiXReal, false>::StiffnessTensor&,
      const CalculiXReal* const);

  /*!
   * This structure is in charge of computing the stiffness operator
   * from the material properties. The resulting operator uses MFront
   * representation of tensors and symmetric tensors.
   */
  template <CalculiXSymmetryType>
  struct CalculiXComputeStiffnessTensor;

  template <>
  struct TFEL_VISIBILITY_LOCAL CalculiXComputeStiffnessTensor<ISOTROPIC> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<3u, CalculiXReal, false>::StiffnessTensor& C,
        const CalculiXReal* const props) {
      computeIsotropicStiffnessTensor(C, props);
    }
  };  // end of struct CalculiXComputeStiffnessTensor

  template <>
  struct TFEL_VISIBILITY_LOCAL CalculiXComputeStiffnessTensor<ORTHOTROPIC> {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<3u, CalculiXReal, false>::StiffnessTensor& C,
        const CalculiXReal* const props) {
      computeOrthotropicStiffnessTensor(C, props);
    }
  };  // end of struct CalculiXComputeStiffnessTensor

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIXCOMPUTESTIFFNESSTENSOR_HXX */
