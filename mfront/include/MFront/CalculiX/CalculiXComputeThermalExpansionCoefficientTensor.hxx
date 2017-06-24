/*!
 * \file   mfront/include/MFront/CalculiX/CalculiXComputeThermalExpansionCoefficientTensor.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 avr 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_CALCULIXCOMPUTETHERMALEXPANSIONTENSOR_HXX
#define LIB_MFRONT_CALCULIXCOMPUTETHERMALEXPANSIONTENSOR_HXX 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/CalculiX/CalculiX.hxx"
#include"MFront/CalculiX/CalculiXConfig.hxx"
#include"MFront/CalculiX/CalculiXTraits.hxx"

namespace calculix
{

  template<CalculiXBehaviourType,
	   tfel::material::ModellingHypothesis::Hypothesis H,
	   CalculiXSymmetryType>
  struct CalculiXComputeThermalExpansionCoefficientTensor;

  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<1u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<1u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICAL,ISOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<2u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<2u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRESS,ISOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<2u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<2u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor


  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRAIN,ISOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<2u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<2u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<2u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<2u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ISOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<3u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<3u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<1u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<1u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICAL,ORTHOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<2u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<2u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRESS,ORTHOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<2u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<2u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor


  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRAIN,ORTHOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<2u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<2u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<2u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<2u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_CALCULIX_VISIBILITY_EXPORT
  CalculiXComputeThermalExpansionCoefficientTensor<calculix::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ORTHOTROPIC>
  {
    static void
    exe(const float* const,
	tfel::config::Types<3u,float,false>::ThermalExpansionCoefficientTensor&);
    static void
    exe(const double* const,
	tfel::config::Types<3u,double,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct CalculiXComputeThermalExpansionCoefficientTensor

} // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIXCOMPUTETHERMALEXPANSIONTENSOR_HXX */

