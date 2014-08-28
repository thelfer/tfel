/*!
 * \file   mfront/include/MFront/Aster/AsterComputeThermalExpansionCoefficientTensor.hxx
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

  template<AsterBehaviourType,
	   tfel::material::ModellingHypothesis::Hypothesis H,
	   AsterSymmetryType>
  struct AsterComputeThermalExpansionCoefficientTensor;

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<1u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICAL,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRESS,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor


  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ISOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<3u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<1u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICAL,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRESS,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor


  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const AsterReal* const,
	  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
    exe(const AsterReal* const,
	tfel::config::Types<2u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_ASTER_VISIBILITY_EXPORT
  AsterComputeThermalExpansionCoefficientTensor<aster::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ORTHOTROPIC>
  {
    static void
    exe(const AsterReal* const,
	tfel::config::Types<3u,AsterReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct AsterComputeThermalExpansionCoefficientTensor

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTERCOMPUTETHERMALEXPANSIONTENSOR_HXX */

