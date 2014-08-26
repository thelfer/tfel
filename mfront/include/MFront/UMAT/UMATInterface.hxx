/*!
 * \file   UMATInterface.hxx
 * \brief  This file implements the UMATInterface class.
 * \author Helfer Thomas
 * \date   28 Jul 2006
 */

#ifndef _LIB_MFRONT_UMAT_CALL_H_
#define _LIB_MFRONT_UMAT_CALL_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Exception/TFELException.hxx"
#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Material/MaterialException.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATException.hxx"
#include"MFront/UMAT/UMATInterfaceBase.hxx"
#include"MFront/UMAT/UMATInterfaceDispatch.hxx"

namespace umat{

  /*!
   * \class UMATInterface
   * \brief This class create an interface * between an MFront
   * behaviour and the Cast3M finite element solver
   *
   * \note : most of branching is done at compile-time. to the very
   * exeception of the stress-free expansion which are handled through
   * two functions pointers which take into account the specificities
   * of finite strain strategy used. The choice of introducing those
   * runtime-functions comes from the fact that we did not want a code
   * duplication between two different finite strain strategies (to
   * reduce both compile-time and library size).
   *
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
	       const UMATReal *const DROT,  UMATReal *const DDSOE,
	       const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	       const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,const UMATReal *const DPRED,
	       UMATReal *const STATEV,const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,const UMATInt  *const NDI,
	       UMATInt  *const KINC,
	       const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::material;
      using namespace tfel::utilities;
      typedef ModellingHypothesis MH;
      if(*NDI==2){
	UMATInterface::template callBehaviour<MH::TRIDIMENSIONAL>(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
								  TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
								  STATEV,NSTATV,STRESS,KINC,sfeh);
      } else if(*NDI==0){
	UMATInterface::template callBehaviour<MH::AXISYMMETRICAL>(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
								  TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
								  STATEV,NSTATV,STRESS,KINC,sfeh);
      } else if(*NDI==-1){
	UMATInterface::template callBehaviour<MH::PLANESTRAIN>(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
							       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
							       STATEV,NSTATV,STRESS,KINC,sfeh);
      } else if(*NDI==-2){
	UMATInterface::template callBehaviour<MH::PLANESTRESS>(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
							       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
							       STATEV,NSTATV,STRESS,KINC,sfeh);
      } else if(*NDI==-3){
	UMATInterface::template callBehaviour<MH::GENERALISEDPLANESTRAIN>(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
									  TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
									  STATEV,NSTATV,STRESS,KINC,sfeh);
      } else if(*NDI==14){
	UMATInterface::template callBehaviour<MH::AXISYMMETRICALGENERALISEDPLANESTRAIN>(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
											TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
											STATEV,NSTATV,STRESS,KINC,sfeh);
      } else {
	UMATInterfaceBase::displayInvalidModellingHypothesisErrorMessage();
	*KINC = -7;
      }
    } // end of exe
    
    template<tfel::material::ModellingHypothesis::Hypothesis H>
    TFEL_UMAT_INLINE2 static void
    callBehaviour(const UMATInt  *const NTENS, const UMATReal *const DTIME,
		  const UMATReal *const DROT,  UMATReal *const DDSOE,
		  const UMATReal *const STRAN, const UMATReal *const DSTRAN,
		  const UMATReal *const TEMP,  const UMATReal *const DTEMP,
		  const UMATReal *const PROPS, const UMATInt  *const NPROPS,
		  const UMATReal *const PREDEF,const UMATReal *const DPRED,
		  UMATReal *const STATEV,const UMATInt  *const NSTATV,
		  UMATReal *const STRESS,UMATInt  *const KINC,
		  const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::utilities;
      typedef Behaviour<H,UMATReal,false> BV;
      typedef UMATTraits<BV> Traits;
      try {
	UMATInterfaceDispatch<Traits::btype,H,
			      Behaviour>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
					      TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
					      STATEV,NSTATV,STRESS,sfeh);
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
    } // end of UMATInterface::callBehaviour
    
  }; // end of struct UMATInterface
    
} // end of namespace umat

#include"MFront/UMAT/UMATBehaviourHandler.hxx"
#include"MFront/UMAT/UMATGenericPlaneStressHandler.hxx"

#endif /* _LIB_MFRONT_UMAT_CALL_H */

