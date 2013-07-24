/*!
 * \file   AsterComputeThermalExpansionTensor.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 avr 2008
 */

#ifndef _LIB_MFRONT_ASTERCOMPUTETHERMALEXPANSIONTENSOR_HXX_
#define _LIB_MFRONT_ASTERCOMPUTETHERMALEXPANSIONTENSOR_HXX_ 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterConfig.hxx"
#include"MFront/Aster/AsterTraits.hxx"

namespace aster
{

  template<unsigned short N,
	   AsterSymmetryType>
  struct AsterComputeThermalExpansionTensor;

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionTensor<1u,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<1u,AsterReal,false>::ThermalExpansionTensor&);
  }; // end of struct AsterComputeThermalExpansionTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionTensor<2u,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionTensor&);
  }; // end of struct AsterComputeThermalExpansionTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionTensor<3u,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<3u,AsterReal,false>::ThermalExpansionTensor&);
  }; // end of struct AsterComputeThermalExpansionTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionTensor<1u,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<1u,AsterReal,false>::ThermalExpansionTensor&);
  }; // end of struct AsterComputeThermalExpansionTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionTensor<2u,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionTensor&);
  }; // end of struct AsterComputeThermalExpansionTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionTensor<3u,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<3u,AsterReal,false>::ThermalExpansionTensor&);
  }; // end of struct AsterComputeThermalExpansionTensor

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTERCOMPUTETHERMALEXPANSIONTENSOR_HXX */

