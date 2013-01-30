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
  template<template<unsigned short,typename,bool> class Behaviour>
  struct UMATGenericPlaneStressHandler;

  /*!
   * forward declaration
   */
  template<unsigned short N,
	   template<unsigned short,typename,bool> class Behaviour>
  struct UMATIsotropicBehaviourHandler;

  /*!
   * forward declaration
   */
  template<unsigned short N,
	   template<unsigned short,typename,bool> class Behaviour>
  struct UMATOrthotropicBehaviourHandler;

  /*!
   * \class  UMATInterface
   * \brief  This class create an interface between Behaviour and UMAT
   * \author Helfer Thomas
   * \date   28 Jul 2006
   */
  template<template<unsigned short,typename,bool> class Behaviour>
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
      if(*NDI==-2){
	// plane strain
	UMATGenericPlaneStressHandler<Behaviour>::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
						      PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
						      STRESS);
      } else {
	if(*NTENS==3u){
	  DimensionDispatch<1u>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
				     TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
				     STATEV,NSTATV,STRESS,NDI,KINC);
	} else if(*NTENS==4){
	  DimensionDispatch<2u>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
				     TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
				     STATEV,NSTATV,STRESS,NDI,KINC);
	} else if(*NTENS==6){
	  DimensionDispatch<3u>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
				     TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
				     STATEV,NSTATV,STRESS,NDI,KINC);
	} else {
	  UMATInterfaceBase::displayInvalidNTENSValueErrorMessage();
	  *KINC = -2;
	}
      }
    } // end of UMATInterface::exe

  private:

    template<unsigned short N>
      struct DimensionDispatch
    {
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
	using namespace std;
	using namespace tfel::meta;
	using namespace tfel::utilities;
	typedef Behaviour<N,UMATReal,false> BV;
	try {
	  typedef UMATTraits<BV> Traits;
	  typedef typename IF<Traits::type==umat::ISOTROPIC,
			      UMATIsotropicBehaviourHandler<N,Behaviour>,
			      UMATOrthotropicBehaviourHandler<N,Behaviour> >::type Handler;
	  Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		       PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		       STRESS,NDI);
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

