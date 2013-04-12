/*!
 * \file   UMATComputeStiffnessTensor.hxx
 * \brief  This file declares the UMATComputeStiffnessTensor class
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#ifndef _LIB_MFRONT_UMATCOMPUTESTIFFNESSTENSOR_H_
#define _LIB_MFRONT_UMATCOMPUTESTIFFNESSTENSOR_H_ 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMATTraits.hxx"

namespace umat
{

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   UMATBehaviourType>
  struct UMATComputeStiffnessTensor;

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<1u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICAL,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRESS,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor


  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<3u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<1u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICAL,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRESS,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor


  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessTensor<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<3u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

} // end of namespace umat

#endif /* _LIB_MFRONT_UMATCOMPUTESTIFFNESSTENSOR_H */

