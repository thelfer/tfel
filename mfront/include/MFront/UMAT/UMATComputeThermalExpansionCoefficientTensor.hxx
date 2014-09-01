/*!
 * \file   mfront/include/MFront/UMAT/UMATComputeThermalExpansionCoefficientTensor.hxx
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

#ifndef _LIB_MFRONT_UMATCOMPUTETHERMALEXPANSIONTENSOR_HXX_
#define _LIB_MFRONT_UMATCOMPUTETHERMALEXPANSIONTENSOR_HXX_ 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMATTraits.hxx"

namespace umat
{

  template<UMATBehaviourType,
	   tfel::material::ModellingHypothesis::Hypothesis H,
	   UMATSymmetryType>
  struct UMATComputeThermalExpansionCoefficientTensor;

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<1u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICAL,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRESS,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor


  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<3u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<1u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::AXISYMMETRICAL,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRESS,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor


  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::PLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
					       tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<3u,UMATReal,false>::ThermalExpansionCoefficientTensor&);
  }; // end of struct UMATComputeThermalExpansionCoefficientTensor

} // end of namespace umat

#endif /* _LIB_MFRONT_UMATCOMPUTETHERMALEXPANSIONTENSOR_HXX */

