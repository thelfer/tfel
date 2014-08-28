/*!
 * \file   mfront/include/MFront/Cyrano/CyranoInterface.hxx
 * \brief  This file implements the CyranoInterface class.
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANO_CALL_H_
#define _LIB_MFRONT_CYRANO_CALL_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Exception/TFELException.hxx"
#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Material/MaterialException.hxx"

#include"MFront/Cyrano/Cyrano.hxx"
#include"MFront/Cyrano/CyranoException.hxx"
#include"MFront/Cyrano/CyranoInterfaceBase.hxx"
#include"MFront/Cyrano/CyranoInterfaceDispatch.hxx"

namespace cyrano{

  /*!
   * \class  CyranoInterface
   * \brief  This class create an interface between Behaviour and Cyrano
   * \author Helfer Thomas
   * \date   28 Jul 2006
   */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CyranoInterface
    : protected CyranoInterfaceBase
  {

    /*!
     * Main entry point This barely make a dispatch based on the
     * spatial dimension and the modelling hypothesis.
     */
    TFEL_CYRANO_INLINE2 static
      void exe(const CyranoInt  *const NTENS, const CyranoReal *const DTIME,
	       const CyranoReal *const DROT,  CyranoReal *const DDSOE,
	       const CyranoReal *const STRAN, const CyranoReal *const DSTRAN,
	       const CyranoReal *const TEMP,  const CyranoReal *const DTEMP,
	       const CyranoReal *const PROPS, const CyranoInt  *const NPROPS,
	       const CyranoReal *const PREDEF,const CyranoReal *const DPRED,
	       CyranoReal *const STATEV,const CyranoInt  *const NSTATV,
	       CyranoReal *const STRESS,const CyranoInt  *const NDI,
	       CyranoInt  *const KINC)
    {
      using namespace tfel::material;
      using namespace tfel::utilities;
      typedef ModellingHypothesis MH;
      CyranoInterfaceBase::checkNTENSValue(*NTENS,3u);
      if(*NDI==1){
	CyranoInterface::template callBehaviour<MH::AXISYMMETRICALGENERALISEDPLANESTRAIN>(DTIME,DROT,DDSOE,STRAN,DSTRAN,
											  TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
											  STATEV,NSTATV,STRESS,KINC);
      } else if (*NDI==2){
	  CyranoInterface::template callBehaviour<MH::AXISYMMETRICALGENERALISEDPLANESTRESS>(DTIME,DROT,DDSOE,STRAN,DSTRAN,
											    TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
											    STATEV,NSTATV,STRESS,KINC);
      } else {
	CyranoInterfaceBase::displayInvalidModellingHypothesisErrorMessage();
	*KINC = -7;
      }
    } // end of exe
    
    template<tfel::material::ModellingHypothesis::Hypothesis H>
    TFEL_CYRANO_INLINE2 static void
    callBehaviour(const CyranoReal *const DTIME,
		  const CyranoReal *const DROT,  CyranoReal *const DDSOE,
		  const CyranoReal *const STRAN, const CyranoReal *const DSTRAN,
		  const CyranoReal *const TEMP,  const CyranoReal *const DTEMP,
		  const CyranoReal *const PROPS, const CyranoInt  *const NPROPS,
		  const CyranoReal *const PREDEF,const CyranoReal *const DPRED,
		  CyranoReal *const STATEV,const CyranoInt  *const NSTATV,
		  CyranoReal *const STRESS,CyranoInt  *const KINC)
    {
      using namespace tfel::utilities;
      typedef Behaviour<H,CyranoReal,false> BV;
      try {
	CyranoInterfaceDispatch<H,Behaviour>::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,
						  TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
						  STATEV,NSTATV,STRESS);
      }
      catch(const CyranoIntegrationFailed& e){
	CyranoInterfaceBase::treatCyranoException(Name<BV>::getName(),e);
	*KINC = -1;
      }
      catch(const CyranoException& e){
	CyranoInterfaceBase::treatCyranoException(Name<BV>::getName(),e);
	*KINC = -2;
      }
      catch(const tfel::material::MaterialException& e){
	CyranoInterfaceBase::treatMaterialException(Name<BV>::getName(),e);
	*KINC = -3;
      }
      catch(const tfel::exception::TFELException& e){
	CyranoInterfaceBase::treatTFELException(Name<BV>::getName(),e);
	*KINC = -4;
      }
      catch(const std::exception& e){
	CyranoInterfaceBase::treatStandardException(Name<BV>::getName(),e);
	*KINC = -5;
      }
      catch(...){
	CyranoInterfaceBase::treatUnknownException(Name<BV>::getName());
	*KINC = -6;
      }
    } // end of CyranoInterface::callBehaviour
    
  }; // end of struct CyranoInterface
    
} // end of namespace cyrano

#include"MFront/Cyrano/CyranoBehaviourHandler.hxx"
// #include"MFront/Cyrano/CyranoGenericPlaneStressHandler.hxx"

#endif /* _LIB_MFRONT_CYRANO_CALL_H */

