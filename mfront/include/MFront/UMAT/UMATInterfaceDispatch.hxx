/*! 
 * \file  UMATInterfaceDispatch.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 juil. 2013
 */

#ifndef _LIB_MFRONT_UMAT_UMATINTERFACEDISPATCH_H_
#define _LIB_MFRONT_UMAT_UMATINTERFACEDISPATCH_H_ 

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Material/MechanicalBehaviourTraits.hxx"
#include"TFEL/Material/MechanicalBehaviourData.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATTraits.hxx"
#include"MFront/UMAT/UMATInterfaceBase.hxx"

#include"MFront/UMAT/UMATOutOfBoundsPolicy.hxx"
#include"MFront/UMAT/UMATOrthotropicBehaviour.hxx"
#include"MFront/UMAT/UMATComputeStiffnessTensor.hxx"
#include"MFront/UMAT/UMATComputeThermalExpansionTensor.hxx"
#include"MFront/UMAT/UMATRotationMatrix.hxx"
#include"MFront/UMAT/UMATGetModellingHypothesis.hxx"
#include"MFront/UMAT/UMATBehaviourHandler.hxx"
#include"MFront/UMAT/UMATIsotropicBehaviourHandler.hxx"
#include"MFront/UMAT/UMATOrthotropicBehaviourHandler.hxx"
#include"MFront/UMAT/UMATGenericPlaneStressHandler.hxx"

namespace umat{

  /*!
   * \class  UMATInterfaceDispatch
   * \brief  
   * \author Helfer Thomas
   * \date   24 Jul 2013
   */
  template<UMATBehaviourType type,
	   tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATInterfaceDispatch
    : public UMATInterfaceBase
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const, const UMATReal *const,
	     const UMATReal *const, const UMATReal *const,
	     const UMATReal *const, const UMATReal *const,
	     const UMATReal *const, const UMATReal *const,
	     const UMATReal *const, const UMATInt  *const,
	     const UMATReal *const, const UMATReal *const,
	     UMATReal *const,const UMATInt  *const,
	     UMATReal *const,UMATInt  *const)
    {
      typedef tfel::material::ModellingHypothesis MH;
      UMATInterfaceBase::throwInvalidBehaviourTypeAndModellingHypothesis(type,MH::HypothesisToString(H));
    } // end of exe
  }; // end of struct UMATInterfaceDispatch

  /*!
   * \class  UMATInterfaceDispatch
   * \brief  partial specialisation for small strain behaviours
   * \author Helfer Thomas
   * \date   24 Jul 2013
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATInterfaceDispatch<SMALLSTRAINSTANDARDBEHAVIOUR,H,Behaviour>
    : public UMATInterfaceBase
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	     const UMATReal *const DROT,  const UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS)
    {
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef Behaviour<H,UMATReal,false> BV;
      //! a simple alias
      typedef UMATTraits<BV> Traits;
      typedef typename IF<Traits::stype==umat::ISOTROPIC,
			  UMATIsotropicBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,
							H,Behaviour>,
			  UMATOrthotropicBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,
							  H,Behaviour> >::type Handler;
      UMATInterfaceBase::checkNTENSValue(*NTENS,Traits::DrivingVariableSize);
      Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		   PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		   STRESS);
    } // end of exe
  }; // end of struct UMATInterfaceDispatch

  /*!
   * \class  UMATInterfaceDispatch
   * \brief  partial specialisation for small strain behaviours
   * \author Helfer Thomas
   * \date   24 Jul 2013
   */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATInterfaceDispatch<SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::PLANESTRESS,
			       Behaviour>
    : public UMATInterfaceBase
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	     const UMATReal *const DROT,  const UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS)
    {
      using namespace std;
      using namespace tfel::meta;
      typedef tfel::material::ModellingHypothesis MH;
      typedef Behaviour<MH::PLANESTRESS,UMATReal,false> BV;
      typedef UMATTraits<BV> Traits;
      if(tfel::material::MechanicalBehaviourTraits<BV>::is_defined){
	typedef typename IF<Traits::stype==umat::ISOTROPIC,
			    UMATIsotropicBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,
							  MH::PLANESTRESS,Behaviour>,
			    UMATOrthotropicBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,
							    MH::PLANESTRESS,Behaviour> >::type Handler;
	UMATInterfaceBase::checkNTENSValue(*NTENS,Traits::DrivingVariableSize);
	Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		     PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		     STRESS);
      } else { 
	UMATGenericPlaneStressHandler<Behaviour>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
						      TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
						      STATEV,NSTATV,STRESS);
      }
    }
  };

  /* cohesive zones models */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATInterfaceDispatch<COHESIVEZONEMODEL,tfel::material::ModellingHypothesis::PLANESTRAIN,Behaviour>
    : public UMATInterfaceBase
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	     const UMATReal *const DROT,  const UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS)
    {
      using namespace std;
      using namespace tfel::meta;
      typedef tfel::material::ModellingHypothesis MH;
      typedef Behaviour<MH::PLANESTRAIN,UMATReal,false> BV;
      //! a simple alias
      typedef UMATTraits<BV> Traits;
      typedef typename IF<Traits::stype==umat::ISOTROPIC,
			  UMATIsotropicBehaviourHandler<COHESIVEZONEMODEL,
							MH::PLANESTRAIN,Behaviour>,
			  UMATUnSupportedCaseHandler>::type Handler;
      UMATInterfaceBase::checkNTENSValue(*NTENS,Traits::DrivingVariableSize);
      Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		   PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		   STRESS);
    } // end of exe
  }; // end of struct UMATInterfaceDispatch

  /* cohesive zones models */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATInterfaceDispatch<COHESIVEZONEMODEL,tfel::material::ModellingHypothesis::TRIDIMENSIONAL,Behaviour>
    : public UMATInterfaceBase
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	     const UMATReal *const DROT,  const UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS)
    {
      using namespace std;
      using namespace tfel::meta;
      typedef tfel::material::ModellingHypothesis MH;
      typedef Behaviour<MH::TRIDIMENSIONAL,UMATReal,false> BV;
      //! a simple alias
      typedef UMATTraits<BV> Traits;
      typedef typename IF<Traits::stype==umat::ISOTROPIC,
			  UMATIsotropicBehaviourHandler<COHESIVEZONEMODEL,
							MH::TRIDIMENSIONAL,Behaviour>,
			  UMATUnSupportedCaseHandler>::type Handler;
      UMATInterfaceBase::checkNTENSValue(*NTENS,Traits::DrivingVariableSize);
      Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		   PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		   STRESS);
    } // end of exe
  }; // end of struct UMATInterfaceDispatch

} // end of namespace umat

#endif /* _LIBMFRONT_UMAT_UMATINTERFACEDISPATCH_H */
