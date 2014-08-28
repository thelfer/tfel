/*! 
 * \file  mfront/include/MFront/Cyrano/CyranoInterfaceDispatch.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANO_CYRANOINTERFACEDISPATCH_H_
#define _LIB_MFRONT_CYRANO_CYRANOINTERFACEDISPATCH_H_ 

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Material/MechanicalBehaviourTraits.hxx"

#include"MFront/Cyrano/Cyrano.hxx"
#include"MFront/Cyrano/CyranoTraits.hxx"
#include"MFront/Cyrano/CyranoInterfaceBase.hxx"

#include"MFront/Cyrano/CyranoOutOfBoundsPolicy.hxx"
#include"MFront/Cyrano/CyranoComputeThermalExpansionCoefficientTensor.hxx"
#include"MFront/Cyrano/CyranoGetModellingHypothesis.hxx"
#include"MFront/Cyrano/CyranoBehaviourHandler.hxx"
#include"MFront/Cyrano/CyranoIsotropicBehaviourHandler.hxx"
#include"MFront/Cyrano/CyranoOrthotropicBehaviourHandler.hxx"
// #include"MFront/Cyrano/CyranoGenericPlaneStressHandler.hxx"

namespace cyrano{

  /*!
   * \class  CyranoInterfaceDispatch
   * \brief  
   * \author Helfer Thomas
   * \date   24 Jul 2013
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct CyranoInterfaceDispatch
    : public CyranoInterfaceBase
  {
    TFEL_CYRANO_INLINE2 static
    void exe(const CyranoReal *const DTIME,
	     const CyranoReal *const DROT,  CyranoReal *const DDSOE,
	     const CyranoReal *const STRAN, const CyranoReal *const DSTRAN,
	     const CyranoReal *const TEMP,  const CyranoReal *const DTEMP,
	     const CyranoReal *const PROPS, const CyranoInt  *const NPROPS,
	     const CyranoReal *const PREDEF,const CyranoReal *const DPRED,
	     CyranoReal *const STATEV,const CyranoInt  *const NSTATV,
	     CyranoReal *const STRESS)
    {
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef Behaviour<H,CyranoReal,false> BV;
      //! a simple alias
      typedef CyranoTraits<BV> Traits;
      typedef typename IF<Traits::stype==cyrano::ISOTROPIC,
			  CyranoIsotropicBehaviourHandler<H,Behaviour>,
			  CyranoOrthotropicBehaviourHandler<H,Behaviour> >::type Handler;
      Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		   PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		   STRESS);
    } // end of exe
  }; // end of struct CyranoInterfaceDispatch

  // /*!
  //  * \class  CyranoInterfaceDispatch
  //  * \brief  partial specialisation for small strain behaviours
  //  * \author Helfer Thomas
  //  * \date   24 Jul 2013
  //  */
  // template<template<tfel::material::ModellingHypothesis::Hypothesis,
  // 		    typename,bool> class Behaviour>
  // struct CyranoInterfaceDispatch<tfel::material::ModellingHypothesis::PLANESTRESS,
  // 				 Behaviour>
  //   : public CyranoInterfaceBase
  // {
  //   TFEL_CYRANO_INLINE2 static
  //   void exe(const CyranoInt  *const NTENS, const CyranoReal *const DTIME,
  // 	     const CyranoReal *const DROT,  CyranoReal *const DDSOE,
  // 	     const CyranoReal *const STRAN, const CyranoReal *const DSTRAN,
  // 	     const CyranoReal *const TEMP,  const CyranoReal *const DTEMP,
  // 	     const CyranoReal *const PROPS, const CyranoInt  *const NPROPS,
  // 	     const CyranoReal *const PREDEF,const CyranoReal *const DPRED,
  // 	     CyranoReal *const STATEV,const CyranoInt  *const NSTATV,
  // 	     CyranoReal *const STRESS)
  //   {
  //     using namespace std;
  //     using namespace tfel::meta;
  //     typedef tfel::material::ModellingHypothesis MH;
  //     typedef Behaviour<MH::PLANESTRESS,CyranoReal,false> BV;
  //     typedef CyranoTraits<BV> Traits;
  //     if(tfel::material::MechanicalBehaviourTraits<BV>::is_defined){
  // 	typedef typename IF<Traits::stype==cyrano::ISOTROPIC,
  // 			    CyranoIsotropicBehaviourHandler<MH::PLANESTRESS,Behaviour>,
  // 			    CyranoOrthotropicBehaviourHandler<MH::PLANESTRESS,Behaviour> >::type Handler;
  // 	CyranoInterfaceBase::checkNTENSValue(*NTENS,Traits::ThermodynamicForceVariableSize);
  // 	Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
  // 		     PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
  // 		     STRESS);
  //     } else { 
  // 	CyranoGenericPlaneStressHandler<Behaviour>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
  // 							TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
  // 							STATEV,NSTATV,STRESS);
  //     }
  //   }
  // };

} // end of namespace cyrano

#endif /* _LIBMFRONT_CYRANO_CYRANOINTERFACEDISPATCH_H */
