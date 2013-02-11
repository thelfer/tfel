/*!
 * \file   UMATComputeThermalExpansionTensor.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 avr 2008
 */

#ifndef _LIB_MFRONT_UMATCOMPUTETHERMALEXPANSIONTENSOR_HXX_
#define _LIB_MFRONT_UMATCOMPUTETHERMALEXPANSIONTENSOR_HXX_ 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMATTraits.hxx"

namespace umat
{

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   UMATBehaviourType>
  struct UMATComputeThermalExpansionTensor;

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::AXISYMETRICALGENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<1u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::AXISYMETRICAL,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::PLANESTRESS,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor


  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<3u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::AXISYMETRICALGENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<1u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::AXISYMETRICAL,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::PLANESTRESS,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor


  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionTensor<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<3u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

} // end of namespace umat

#endif /* _LIB_MFRONT_UMATCOMPUTETHERMALEXPANSIONTENSOR_HXX */

