/*!
 * \file   AsterInterface.hxx
 * \brief  This file implements the AsterInterface class.
 * \author Helfer Thomas
 * \date   28 Jul 2006
 */

#ifndef _LIB_MFRONT_ASTER_CALL_H_
#define _LIB_MFRONT_ASTER_CALL_H_ 

#include<string>
#include<algorithm>
#include<vector>
#include<limits>

#include"TFEL/Config/TFELTypes.hxx"

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Exception/TFELException.hxx"
#include"TFEL/FSAlgorithm/copy.hxx"

#include"TFEL/Utilities/Info.hxx"
#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Material/MechanicalBehaviourTraits.hxx"
#include"TFEL/Material/MechanicalBehaviourData.hxx"
#include"TFEL/Material/MaterialException.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterConfig.hxx"
#include"MFront/Aster/AsterTraits.hxx"
#include"MFront/Aster/AsterException.hxx"
#include"MFront/Aster/AsterInterfaceBase.hxx"

// #include"MFront/ASTER/ASTEROutOfBoundsPolicy.hxx"
// #include"MFront/ASTER/ASTEROrthotropicBehaviour.hxx"
// #include"MFront/ASTER/ASTERComputeStiffnessTensor.hxx"
// #include"MFront/ASTER/ASTERComputeThermalExpansionTensor.hxx"
// #include"MFront/ASTER/ASTERRotationMatrix.hxx"
// #include"MFront/ASTER/ASTERGetModellingHypothesis.hxx"

namespace aster
{

  template<unsigned short N>
  struct AsterModellingHypothesis;

  template<>
  struct AsterModellingHypothesis<1u>
  {
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    static const ModellingHypothesis::Hypothesis value = ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
  };

  template<>
  struct AsterModellingHypothesis<2u>
  {
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    static const ModellingHypothesis::Hypothesis value = ModellingHypothesis::GENERALISEDPLANESTRAIN;
  };

  template<>
  struct AsterModellingHypothesis<3u>
  {
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    static const ModellingHypothesis::Hypothesis value = ModellingHypothesis::TRIDIMENSIONAL;
  };

  /*!
   * forward declaration
   */
  template<unsigned short N,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct AsterIsotropicBehaviourHandler;

  /*!
   * forward declaration
   */
  template<unsigned short N,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct AsterOrthotropicBehaviourHandler;

  /*!
   * \class  AsterInterface
   * \brief  This class create an interface between Behaviour and Aster
   * \author Helfer Thomas
   * \date   28 Jul 2006
   */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterInterface
    : protected AsterInterfaceBase
  {

    TFEL_ASTER_INLINE2 static
     int exe(const AsterInt  *const NTENS, const AsterReal *const DTIME,
	       const AsterReal *const DROT,  AsterReal *const DDSOE,
	       const AsterReal *const STRAN, const AsterReal *const DSTRAN,
	       const AsterReal *const TEMP,  const AsterReal *const DTEMP,
	       const AsterReal *const PROPS, const AsterInt  *const NPROPS,
	       const AsterReal *const PREDEF,const AsterReal *const DPRED,
	       AsterReal *const STATEV,const AsterInt  *const NSTATV,
	       AsterReal *const STRESS)
    {
      if(*NTENS==3u){
	return DimensionDispatch<1u>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
					  TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
					  STATEV,NSTATV,STRESS);
      } else if(*NTENS==4){
	return DimensionDispatch<2u>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
					  TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
					  STATEV,NSTATV,STRESS);
      } else if(*NTENS==6){
	return DimensionDispatch<3u>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
					  TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
					  STATEV,NSTATV,STRESS);
      } else {
	AsterInterfaceBase::displayInvalidNTENSValueErrorMessage();
	return -2;
      }
    }

  private:

    template<unsigned short N>
    struct DimensionDispatch
    {
      TFEL_ASTER_INLINE2 static
      int exe(const AsterInt  *const, const AsterReal *const DTIME,
	      const AsterReal *const DROT,  AsterReal *const DDSOE,
	      const AsterReal *const STRAN, const AsterReal *const DSTRAN,
	      const AsterReal *const TEMP,  const AsterReal *const DTEMP,
	      const AsterReal *const PROPS, const AsterInt  *const NPROPS,
	      const AsterReal *const PREDEF,const AsterReal *const DPRED,
	      AsterReal *const STATEV,const AsterInt  *const NSTATV,
	      AsterReal *const STRESS)
      {
	using namespace std;
	using namespace tfel::meta;
	using namespace tfel::utilities;
	typedef Behaviour<AsterModellingHypothesis<N>::value,AsterReal,false> BV;
	try {
	  typedef AsterTraits<BV> Traits;
	  typedef typename IF<Traits::type==aster::ISOTROPIC,
			      AsterIsotropicBehaviourHandler<N,Behaviour>,
			      AsterOrthotropicBehaviourHandler<N,Behaviour> >::type Handler;
	  Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		       PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		       STRESS);
	} 
	catch(const AsterException& e){
	  AsterInterfaceBase::treatAsterException(Name<BV>::getName(),e);
	  return -2;
	}
	catch(const tfel::material::MaterialException& e){
	  AsterInterfaceBase::treatMaterialException(Name<BV>::getName(),e);
	  return -3;
	}
	catch(const tfel::exception::TFELException& e){
	  AsterInterfaceBase::treatTFELException(Name<BV>::getName(),e);
	  return -4;
	}
	catch(const std::exception& e){
	  AsterInterfaceBase::treatStandardException(Name<BV>::getName(),e);
	  return -5;
	}
	catch(...){
	  AsterInterfaceBase::treatUnknownException(Name<BV>::getName());
	  return -6;
	}
	return 0;
      } // end of DimensionDispatch::exe
    }; // end of struct DimensionDispatch
  }; // end of struct AsterInterface
  
} // end of namespace aster

#include"MFront/Aster/AsterBehaviourHandler.hxx"

#endif /* _LIB_MFRONT_ASTER_CALL_H */

