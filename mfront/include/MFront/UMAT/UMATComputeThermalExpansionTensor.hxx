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
#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATTraits.hxx"

namespace umat
{

  template<unsigned short N,
	   UMATBehaviourType>
  struct UMATComputeThermalExpansionTensor;

  template<>
  struct UMATComputeThermalExpansionTensor<1u,ISOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<1u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct UMATComputeThermalExpansionTensor<2u,ISOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct UMATComputeThermalExpansionTensor<3u,ISOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<3u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct UMATComputeThermalExpansionTensor<1u,ORTHOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<1u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct UMATComputeThermalExpansionTensor<2u,ORTHOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

  template<>
  struct UMATComputeThermalExpansionTensor<3u,ORTHOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<3u,UMATReal,false>::ThermalExpansionTensor&);
  }; // end of struct UMATComputeThermalExpansionTensor

} // end of namespace umat

#endif /* _LIB_MFRONT_UMATCOMPUTETHERMALEXPANSIONTENSOR_HXX */

