/*!
 * \file   mfront/include/MFront/Cyrano/CyranoComputeStiffnessTensor.hxx
 * \brief  This file declares the CyranoComputeStiffnessTensor class
 * \author Thomas Helfer
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANOCOMPUTESTIFFNESSTENSOR_HXX
#define LIB_MFRONT_CYRANOCOMPUTESTIFFNESSTENSOR_HXX

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Cyrano/Cyrano.hxx"
#include "MFront/Cyrano/CyranoConfig.hxx"
#include "MFront/Cyrano/CyranoTraits.hxx"

namespace cyrano {

  /*!
   * This structure is in charge of computing the Stiffness tensor
   * from the material properties given by Cyrano.  The resulting
   * tensor uses MFront representation of tensors and symmetric
   * tensors.
   * The template parameter is the behaviour symmetry type
   * (isotropy or orthotropy)
   */
  template <CyranoSymmetryType>
  struct CyranoComputeStiffnessTensor;

  template <>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
      CyranoComputeStiffnessTensor<ISOTROPIC> {
    /*!
     * \brief compute the stiffness tensor
     * \param[out] D     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<1u, CyranoReal, false>::StiffnessTensor&,
        const CyranoReal* const);
  };  // end of struct CyranoComputeStiffnessTensor

  template <>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
      CyranoComputeStiffnessTensor<ORTHOTROPIC> {
    /*!
     * \brief compute the stiffness tensor
     * \param[out] D     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void exe(
        tfel::config::Types<1u, CyranoReal, false>::StiffnessTensor&,
        const CyranoReal* const);
  };  // end of struct CyranoComputeStiffnessTensor

}  // end of namespace cyrano

#endif /* LIB_MFRONT_CYRANOCOMPUTESTIFFNESSTENSOR_HXX */
