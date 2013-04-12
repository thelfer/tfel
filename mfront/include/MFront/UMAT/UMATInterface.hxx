/*!
 * \file   UMATInterface.hxx
 * \brief  This file implements the UMATInterface class.
 * \author Helfer Thomas
 * \date   28 Jul 2006
 */

#ifndef _LIB_MFRONT_UMAT_CALL_H_
#define _LIB_MFRONT_UMAT_CALL_H_ 

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

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMATTraits.hxx"
#include"MFront/UMAT/UMATException.hxx"
#include"MFront/UMAT/UMATOutOfBoundsPolicy.hxx"
#include"MFront/UMAT/UMATOrthotropicBehaviour.hxx"
#include"MFront/UMAT/UMATComputeStiffnessTensor.hxx"
#include"MFront/UMAT/UMATComputeThermalExpansionTensor.hxx"
#include"MFront/UMAT/UMATRotationMatrix.hxx"
#include"MFront/UMAT/UMATGetModellingHypothesis.hxx"

#include"MFront/UMAT/UMATInterfaceBase.hxx"

namespace umat{

  /*!
   * forward declaration
   */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATGenericPlaneStressHandler;

  /*!
   * forward declaration
   */
  template<tfel::material::ModellingHypothesis::Hypothesis,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATIsotropicBehaviourHandler;

  /*!
   * forward declaration
   */
  template<tfel::material::ModellingHypothesis::Hypothesis,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATOrthotropicBehaviourHandler;

  /*!
   * \class  UMATInterface
   * \brief  This class create an interface between Behaviour and UMAT
   * \author Helfer Thomas
   * \date   28 Jul 2006
   */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATInterface
    : protected UMATInterfaceBase
  {

    /*!
     * Main entry point This barely make a dispatch based on the
     * spatial dimension and the modelling hypothesis.
     */
    TFEL_UMAT_INLINE2 static
      void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	       const UMATReal *const DROT,  const UMATReal *const DDSOE,
	       const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	       const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,const UMATReal *const DPRED,
	       UMATReal *const STATEV,const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,const UMATInt  *const NDI,
	       UMATInt  *const KINC)
    {
      typedef tfel::material::ModellingHypothesis MH;
      if(*NDI==2){
	MHDispatch<MH::TRIDIMENSIONAL>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
					    TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
					    STATEV,NSTATV,STRESS,KINC);
      } else if(*NDI==0){
	MHDispatch<MH::AXISYMMETRICAL>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
					    TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
					    STATEV,NSTATV,STRESS,KINC);
      } else if(*NDI==-1){
	MHDispatch<MH::PLANESTRAIN>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
					 TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
					 STATEV,NSTATV,STRESS,KINC);
      } else if(*NDI==-2){
	MHDispatch<MH::PLANESTRESS>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
					 TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
					 STATEV,NSTATV,STRESS,KINC);
      } else if(*NDI==-3){
	MHDispatch<MH::GENERALISEDPLANESTRAIN>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
						    TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
						    STATEV,NSTATV,STRESS,KINC);
      } else if(*NDI==14){
	MHDispatch<MH::AXISYMMETRICALGENERALISEDPLANESTRAIN>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
								  TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
								  STATEV,NSTATV,STRESS,KINC);
      } else {
	UMATInterfaceBase::displayInvalidModellingHypothesisErrorMessage();
	*KINC = -7;
      }
    } // end of UMATInterface::exe

  private:


    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct MHDispatch
    {
      TFEL_UMAT_INLINE2 static
      void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	       const UMATReal *const DROT,  const UMATReal *const DDSOE,
	       const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	       const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,const UMATReal *const DPRED,
	       UMATReal *const STATEV,const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,UMATInt  *const KINC)
      {
	using namespace std;
	using namespace tfel::meta;
	using namespace tfel::utilities;
	typedef Behaviour<H,UMATReal,false> BV;
	//! a simple alias
	typedef tfel::material::ModellingHypothesisToSpaceDimension<H> ModellingHypothesisToSpaceDimension;
	// spatial dimension
	static const unsigned short N = ModellingHypothesisToSpaceDimension::value;
	try {
	  typedef UMATTraits<BV> Traits;
	  typedef typename IF<Traits::type==umat::ISOTROPIC,
			      UMATIsotropicBehaviourHandler<H,Behaviour>,
			      UMATOrthotropicBehaviourHandler<H,Behaviour> >::type Handler;
	  UMATInterfaceBase::checkNTENSValue(*NTENS,N);
	  Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		       PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		       STRESS);
	}
	catch(const UMATIntegrationFailed& e){
	  UMATInterfaceBase::treatUmatException(Name<BV>::getName(),e);
	  *KINC = -1;
	}
	catch(const UMATException& e){
	  UMATInterfaceBase::treatUmatException(Name<BV>::getName(),e);
	  *KINC = -2;
	}
	catch(const tfel::material::MaterialException& e){
	  UMATInterfaceBase::treatMaterialException(Name<BV>::getName(),e);
	  *KINC = -3;
	}
	catch(const tfel::exception::TFELException& e){
	  UMATInterfaceBase::treatTFELException(Name<BV>::getName(),e);
	  *KINC = -4;
	}
	catch(const std::exception& e){
	  UMATInterfaceBase::treatStandardException(Name<BV>::getName(),e);
	  *KINC = -5;
	}
	catch(...){
	  UMATInterfaceBase::treatUnknownException(Name<BV>::getName());
	  *KINC = -6;
	}
    } // end of exe
  }; // end of DimensionDispatch
    
}; // end of class UMATInterface
  
} // end of namespace umat

#include"MFront/UMAT/UMATBehaviourHandler.hxx"
#include"MFront/UMAT/UMATGenericPlaneStressHandler.hxx"

#endif /* _LIB_MFRONT_UMAT_CALL_H */

