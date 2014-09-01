/*!
 * \file   mfront/include/MFront/Cyrano/CyranoComputeStiffnessTensor.hxx
 * \brief  This file declares the CyranoComputeStiffnessTensor class
 * \author Helfer Thomas
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_CYRANOCOMPUTESTIFFNESSTENSOR_H_
#define _LIB_MFRONT_CYRANOCOMPUTESTIFFNESSTENSOR_H_ 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Cyrano/Cyrano.hxx"
#include"MFront/Cyrano/CyranoConfig.hxx"
#include"MFront/Cyrano/CyranoTraits.hxx"

namespace cyrano
{

  /*!
   * This structure is in charge of computing the Stiffness tensor
   * from the material properties given by Cyrano.  The resulting
   * tensor uses MFront representation of tensors and symmetric
   * tensors.
   * The first template parameter is the modelling hypothesis considered
   * The second template parameter is the behaviour symmetry type
   * (isotropy or orthotropy)
   * The third template parameter says if the altered or unaltered
   * stiffness tensor shall be computed. If true, the unaltered
   * stiffness tensor is computed
   */
  template<tfel::material::ModellingHypothesis::Hypothesis,
	   CyranoSymmetryType,bool>
  struct CyranoComputeStiffnessTensor;

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			       ISOTROPIC,true>
  {
    /*!
     * \brief compute the stiffness tensor
     * \param[out] D     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor&,
	const CyranoReal* const);
  }; // end of struct CyranoComputeStiffnessTensor

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			       ISOTROPIC,false>
  {
    /*!
     * \brief compute the stiffness tensor
     * \param[out] D     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor&,
	const CyranoReal* const);
  }; // end of struct CyranoComputeStiffnessTensor

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			       ORTHOTROPIC,true>
  {
    /*!
     * \brief compute the stiffness tensor
     * \param[out] D     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor&,
	const CyranoReal* const);
  }; // end of struct CyranoComputeStiffnessTensor

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			       ORTHOTROPIC,false>
  {
    /*!
     * \brief compute the stiffness tensor
     * \param[out] D     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor&,
	const CyranoReal* const);
  }; // end of struct CyranoComputeStiffnessTensor

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
			       ISOTROPIC,true>
  {
    /*!
     * \brief compute the stiffness tensor
     * \param[out] D     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor&,
	const CyranoReal* const);
  }; // end of struct CyranoComputeStiffnessTensor

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
			       ISOTROPIC,false>
  {
    /*!
     * \brief compute the stiffness tensor
     * \param[out] D     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor&,
	const CyranoReal* const);
  }; // end of struct CyranoComputeStiffnessTensor

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
			       ORTHOTROPIC,true>
  {
    /*!
     * \brief compute the stiffness tensor
     * \param[out] D     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor&,
	const CyranoReal* const);
  }; // end of struct CyranoComputeStiffnessTensor

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
			       ORTHOTROPIC,false>
  {
    /*!
     * \brief compute the stiffness tensor
     * \param[out] D     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor&,
	const CyranoReal* const);
  }; // end of struct CyranoComputeStiffnessTensor

} // end of namespace cyrano

#endif /* _LIB_MFRONT_CYRANOCOMPUTESTIFFNESSTENSOR_H */

