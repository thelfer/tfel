/*! 
 * \file  mfront/include/MFront/UMAT/UMATInterfaceDispatch.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 juil. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_UMAT_UMATINTERFACEDISPATCH_H_
#define _LIB_MFRONT_UMAT_UMATINTERFACEDISPATCH_H_ 

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Material/MechanicalBehaviourTraits.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATTraits.hxx"
#include"MFront/UMAT/UMATInterfaceExceptions.hxx"

#include"MFront/UMAT/UMATOutOfBoundsPolicy.hxx"
#include"MFront/UMAT/UMATOrthotropicBehaviour.hxx"
#include"MFront/UMAT/UMATComputeThermalExpansionCoefficientTensor.hxx"
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
    : public UMATInterfaceExceptions
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const, const UMATReal *const,
	     const UMATReal *const, const UMATReal *const,
	     const UMATReal *const, const UMATReal *const,
	     const UMATReal *const, const UMATReal *const,
	     const UMATReal *const, const UMATInt  *const,
	     const UMATReal *const, const UMATReal *const,
	     UMATReal *const,const UMATInt  *const,
	     UMATReal *const,
	     const StressFreeExpansionHandler&)
    {
      typedef tfel::material::ModellingHypothesis MH;
      UMATInterfaceExceptions::throwInvalidBehaviourTypeAndModellingHypothesis(type,MH::toString(H));
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
    : public UMATInterfaceExceptions
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	     const UMATReal *const DROT,  UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS,
	     const StressFreeExpansionHandler& sfeh)
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
      UMATInterfaceExceptions::checkNTENSValue(*NTENS,Traits::ThermodynamicForceVariableSize);
      Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		   PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		   STRESS,sfeh);
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
    : public UMATInterfaceExceptions
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	     const UMATReal *const DROT,  UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS,
	     const StressFreeExpansionHandler& sfeh)
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
	UMATInterfaceExceptions::checkNTENSValue(*NTENS,Traits::ThermodynamicForceVariableSize);
	Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		     PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		     STRESS,sfeh);
      } else { 
	UMATGenericPlaneStressHandler<Behaviour>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
						      TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
						      STATEV,NSTATV,STRESS,sfeh);
      }
    }
  };

  /*!
   * \class  UMATInterfaceDispatch
   * \brief  partial specialisation for small strain behaviours
   * \author Helfer Thomas
   * \date   24 Jul 2013
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATInterfaceDispatch<FINITESTRAINSTANDARDBEHAVIOUR,H,Behaviour>
    : public UMATInterfaceExceptions
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	     const UMATReal *const DROT,  UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS,
	     const StressFreeExpansionHandler& sfeh)
    {
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef Behaviour<H,UMATReal,false> BV;
      //! a simple alias
      typedef UMATTraits<BV> Traits;
      typedef typename IF<Traits::stype==umat::ISOTROPIC,
			  UMATIsotropicBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,
							H,Behaviour>,
			  UMATOrthotropicBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,
							  H,Behaviour> >::type Handler;
      UMATInterfaceExceptions::checkNTENSValue(*NTENS,Traits::ThermodynamicForceVariableSize);
      Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		   PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		   STRESS,sfeh);
    } // end of exe
  }; // end of struct UMATInterfaceDispatch

  /* cohesive zones models */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATInterfaceDispatch<COHESIVEZONEMODEL,tfel::material::ModellingHypothesis::PLANESTRAIN,Behaviour>
    : public UMATInterfaceExceptions
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	     const UMATReal *const DROT,  UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS,
	     const StressFreeExpansionHandler& sfeh)
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
			  UMATOrthotropicBehaviourHandler<COHESIVEZONEMODEL,
							  MH::PLANESTRAIN,Behaviour> >::type Handler;
      UMATInterfaceExceptions::checkNTENSValue(*NTENS,Traits::ThermodynamicForceVariableSize);
      UMATReal D[4];
      UMATReal u[2],du[2],t[2];
      u[0]  = STRAN[1];  u[1]  = STRAN[0];
      du[0] = DSTRAN[1]; du[1] = DSTRAN[0];
      t[0]  = STRESS[1]; t[1]  = STRESS[0];
      Handler::exe(DTIME,DROT,DDSOE,u,du,TEMP,DTEMP,
		   PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,t,sfeh);
      D[0] = DDSOE[0]; D[1] = DDSOE[1];
      D[2] = DDSOE[2]; D[3] = DDSOE[3];
      DDSOE[0] = D[3]; DDSOE[1] = D[2];
      DDSOE[2] = D[1]; DDSOE[3] = D[0];
      STRESS[0] = t[1]; STRESS[1] = t[0];
    } // end of exe
  }; // end of struct UMATInterfaceDispatch

  /* cohesive zones models */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATInterfaceDispatch<COHESIVEZONEMODEL,tfel::material::ModellingHypothesis::TRIDIMENSIONAL,Behaviour>
    : public UMATInterfaceExceptions
  {
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	     const UMATReal *const DROT,  UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS,
	     const StressFreeExpansionHandler& sfeh)
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
			  UMATOrthotropicBehaviourHandler<COHESIVEZONEMODEL,
							  MH::TRIDIMENSIONAL,Behaviour> >::type Handler;
      UMATInterfaceExceptions::checkNTENSValue(*NTENS,Traits::ThermodynamicForceVariableSize);
      UMATReal D[9];
      UMATReal u[3],du[3],t[3];
      u[0]  = STRAN[2];  u[1]  = STRAN[0];  u[2]  = STRAN[1]; 
      du[0] = DSTRAN[2]; du[1] = DSTRAN[0]; du[2] = DSTRAN[1];
      t[0]  = STRESS[2]; t[1]  = STRESS[0]; t[2]  = STRESS[1];
      Handler::exe(DTIME,DROT,DDSOE,u,du,TEMP,DTEMP,
		   PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,t,sfeh);
      D[0] = DDSOE[0]; D[1] = DDSOE[1]; D[2] = DDSOE[2];
      D[3] = DDSOE[3]; D[4] = DDSOE[4]; D[5] = DDSOE[5]; 
      D[6] = DDSOE[6]; D[7] = DDSOE[7]; D[8] = DDSOE[8]; 
      DDSOE[0] = D[4]; DDSOE[1] = D[5]; DDSOE[2] = D[3];
      DDSOE[3] = D[7]; DDSOE[4] = D[8]; DDSOE[5] = D[6]; 
      DDSOE[6] = D[1]; DDSOE[7] = D[2]; DDSOE[8] = D[0]; 
      STRESS[0] = t[1]; STRESS[1] = t[2]; STRESS[2] = t[0];
    } // end of exe
  }; // end of struct UMATInterfaceDispatch

} // end of namespace umat

#endif /* _LIBMFRONT_UMAT_UMATINTERFACEDISPATCH_H */
