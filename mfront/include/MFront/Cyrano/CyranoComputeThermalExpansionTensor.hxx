/*!
 * \file   CyranoComputeThermalExpansionTensor.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANOCOMPUTETHERMALEXPANSIONTENSOR_HXX_
#define _LIB_MFRONT_CYRANOCOMPUTETHERMALEXPANSIONTENSOR_HXX_ 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Cyrano/Cyrano.hxx"
#include"MFront/Cyrano/CyranoConfig.hxx"
#include"MFront/Cyrano/CyranoTraits.hxx"

namespace cyrano
{

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   CyranoSymmetryType>
  struct CyranoComputeThermalExpansionTensor;

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const CyranoReal* const,
	  tfel::config::Types<1u,CyranoReal,false>::ThermalExpansionTensor&);
  }; // end of struct CyranoComputeThermalExpansionTensor

  template<>
  struct MFRONT_CYRANO_VISIBILITY_EXPORT
  CyranoComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const CyranoReal* const,
	  tfel::config::Types<1u,CyranoReal,false>::ThermalExpansionTensor&);
  }; // end of struct CyranoComputeThermalExpansionTensor

} // end of namespace cyrano

#endif /* _LIB_MFRONT_CYRANOCOMPUTETHERMALEXPANSIONTENSOR_HXX */

