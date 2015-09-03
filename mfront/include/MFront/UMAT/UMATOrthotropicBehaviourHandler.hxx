/*! 
 * \file  mfront/include/MFront/UMAT/UMATOrthotropicBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_UMAT_UMATORTHOTROPICBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_UMAT_UMATORTHOTROPICBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_UMAT_UMATBEHAVIOURHANDLER_H_
#error "This header shall not be called directly"
#endif

#include"MFront/UMAT/UMATRotationMatrix.hxx"
#include"MFront/UMAT/UMATFiniteStrainRotationMatrix.hxx"

namespace umat
{

  //! forward declaration
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATOrthotropicSmallStrainBehaviourHandler1D;
  
  //! forward declaration
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATOrthotropicSmallStrainBehaviourHandler2D;

  //! forward declaration
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATOrthotropicSmallStrainBehaviourHandler3D;

  //! forward declaration
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATOrthotropicFiniteStrainBehaviourHandler1D;
  
  //! forward declaration
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATOrthotropicFiniteStrainBehaviourHandler2D;

  //! forward declaration
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATOrthotropicFiniteStrainBehaviourHandler3D;
  
  /*!
   * An helper structure to make an appropriate dispatch based on the
   * spatial dimension
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicSmallStrainBehaviourDispatcher
  {
    //! a simple alias
    typedef tfel::material::ModellingHypothesisToSpaceDimension<H> ModellingHypothesisToSpaceDimension;
    // spatial dimension
    static const unsigned short N = ModellingHypothesisToSpaceDimension::value;
    // the dispatch
    typedef typename 
      tfel::meta::IF<N==1,
		     UMATOrthotropicSmallStrainBehaviourHandler1D<H,Behaviour>,
		     typename tfel::meta::IF<N==2,
					     UMATOrthotropicSmallStrainBehaviourHandler2D<H,Behaviour>,
					     UMATOrthotropicSmallStrainBehaviourHandler3D<H,Behaviour> >::type
		     >::type type;
  }; // end of struct UMATOrthotropicSmallStrainBehaviourDispatcher;

  /*!
   * An helper structure to make an appropriate dispatch based on the
   * spatial dimension
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicFiniteStrainBehaviourDispatcher
  {
    //! a simple alias
    typedef tfel::material::ModellingHypothesisToSpaceDimension<H> ModellingHypothesisToSpaceDimension;
    // spatial dimension
    static const unsigned short N = ModellingHypothesisToSpaceDimension::value;
    // the dispatch
    typedef typename 
      tfel::meta::IF<N==1,
		     UMATOrthotropicFiniteStrainBehaviourHandler1D<H,Behaviour>,
		     typename tfel::meta::IF<N==2,
					     UMATOrthotropicFiniteStrainBehaviourHandler2D<H,Behaviour>,
					     UMATOrthotropicFiniteStrainBehaviourHandler3D<H,Behaviour> >::type
		     >::type type;
  }; // end of struct UMATOrthotropicFiniteStrainBehaviourDispatcher;
  
  /*!
   * The handler for orthotropic behaviours
   * By default, this is unsupported (cohesive zone models)
   */
  template<UMATBehaviourType type,
	   tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicBehaviourHandler
    : public UMATUnSupportedCaseHandler
  {
    using UMATUnSupportedCaseHandler::exe;
  };

  /*!
   * The handler for small strain orthotropic behaviours
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,H,Behaviour>
    : public UMATOrthotropicSmallStrainBehaviourDispatcher<H,Behaviour>::type
  {
    typedef typename UMATOrthotropicSmallStrainBehaviourDispatcher<H,Behaviour>::type Handler;
    using Handler::exe;
  };


  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicSmallStrainBehaviourHandler1D
    : private UMATBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,H,Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME,
	       const UMATReal *const,
	       UMATReal *const DDSOE,
	       const UMATReal *const STRAN ,
	       const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const tfel::material::OutOfBoundsPolicy op,
	       const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<H,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,H,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	  Traits::useTimeSubStepping,
	  typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	  typename UMATBehaviourHandler::template Integrator<bs,ba>
	  >::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      typedef typename IF<
	MTraits::handlesThermalExpansion,
	typename UMATBehaviourHandler::CheckThermalExpansionCoefficientIsNull,
	typename UMATBehaviourHandler::DontCheckThermalExpansionCoefficientIsNull
	>::type ThermalExpansionCoefficientCheck;
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[7]);
      ThermalExpansionCoefficientCheck::exe(PROPS[8]);
      ThermalExpansionCoefficientCheck::exe(PROPS[9]);
      Handler handler(DTIME,STRAN,DSTRAN,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,STRESS,op,sfeh);
      handler.exe(DDSOE,STRESS,STATEV);
    } // end of UMATOrthotropicBehaviourHander<1u,Behaviour>::exe
  }; // end of struct UMATOrthotropicBehaviourHander<1u,Behaviour>

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicSmallStrainBehaviourHandler2D
    : private UMATBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,H,Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME ,
	       const UMATReal *const DROT  ,
	       UMATReal *const DDSOE,
	       const UMATReal *const STRAN ,
	       const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const tfel::material::OutOfBoundsPolicy op,
	       const StressFreeExpansionHandler& sfeh) 
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<H,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,H,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	Traits::useTimeSubStepping,
	typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	typename UMATBehaviourHandler::template Integrator<bs,ba>
	>::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      typedef typename IF<
	MTraits::handlesThermalExpansion,
	typename UMATBehaviourHandler::CheckThermalExpansionCoefficientIsNull,
	typename UMATBehaviourHandler::DontCheckThermalExpansionCoefficientIsNull
	>::type ThermalExpansionCoefficientCheck;
      UMATReal s[4];
      UMATReal e[4];
      UMATReal de[4];
      UMATRotationMatrix2D m(PROPS+7,DROT);
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[10]);
      ThermalExpansionCoefficientCheck::exe(PROPS[11]);
      ThermalExpansionCoefficientCheck::exe(PROPS[12]);
      m.rotateStressesForward(STRESS,s);
      m.rotateStrainsForward(STRAN,e);
      m.rotateStrainsForward(DSTRAN,de);
      const bool bDDSOE = std::abs(*DDSOE)>0.5; 
      Handler handler(DTIME,e,de,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,s,op,sfeh);
      handler.exe(DDSOE,s,STATEV);
      m.rotateStressesBackward(s,STRESS);
      if(bDDSOE){
	m.rotateTangentOperatorBackward(DDSOE);
      }
    } // end of UMATOrthotropicBehaviourHander<2u,Behaviour>::exe
  }; // end of UMATOrthotropicBehaviourHander<2u,Behaviour>

  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL
  UMATOrthotropicSmallStrainBehaviourHandler2D<tfel::material::ModellingHypothesis::PLANESTRESS,
					       Behaviour>
    : private UMATBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,
				   tfel::material::ModellingHypothesis::PLANESTRESS,
				   Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME ,
	       const UMATReal *const DROT  ,
	       UMATReal *const DDSOE,
	       const UMATReal *const STRAN ,
	       const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const tfel::material::OutOfBoundsPolicy op,
	       const StressFreeExpansionHandler& sfeh) 
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      const ModellingHypothesis::Hypothesis H = ModellingHypothesis::PLANESTRESS;	   
      typedef MechanicalBehaviourTraits<Behaviour<H,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<H,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,H,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	Traits::useTimeSubStepping,
	typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	typename UMATBehaviourHandler::template Integrator<bs,ba>
	>::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      typedef typename IF<
	MTraits::handlesThermalExpansion,
	typename UMATBehaviourHandler::CheckThermalExpansionCoefficientIsNull,
	typename UMATBehaviourHandler::DontCheckThermalExpansionCoefficientIsNull
	>::type ThermalExpansionCoefficientCheck;
      UMATReal s[4];
      UMATReal e[4];
      UMATReal de[4];
      UMATRotationMatrix2D m(PROPS+4,DROT);
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[10]);
      ThermalExpansionCoefficientCheck::exe(PROPS[11]);
      ThermalExpansionCoefficientCheck::exe(PROPS[12]);
      m.rotateStressesForward(STRESS,s);
      m.rotateStrainsForward(STRAN,e);
      m.rotateStrainsForward(DSTRAN,de);
      const bool bDDSOE = std::abs(*DDSOE)>0.5; 
      Handler handler(DTIME,e,de,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,s,op,sfeh);
      handler.exe(DDSOE,s,STATEV);
      m.rotateStressesBackward(s,STRESS);
      if(bDDSOE){
	m.rotateTangentOperatorBackward(DDSOE);
      }
    } // end of UMATOrthotropicBehaviourHander<2u,Behaviour>::exe
  }; // end of UMATOrthotropicBehaviourHander<2u,Behaviour>

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicSmallStrainBehaviourHandler3D
    : private UMATBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,H,Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME,
	       const UMATReal *const DROT,
	       UMATReal *const DDSOE,
	       const UMATReal *const STRAN,
	       const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const tfel::material::OutOfBoundsPolicy op,
	       const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<H,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<SMALLSTRAINSTANDARDBEHAVIOUR,H,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	Traits::useTimeSubStepping,
	typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	typename UMATBehaviourHandler::template Integrator<bs,ba>
	>::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      typedef typename IF<
	MTraits::handlesThermalExpansion,
	typename UMATBehaviourHandler::CheckThermalExpansionCoefficientIsNull,
	typename UMATBehaviourHandler::DontCheckThermalExpansionCoefficientIsNull
	>::type ThermalExpansionCoefficientCheck;
      UMATReal  s[6];
      UMATReal  e[6];
      UMATReal de[6];
      // Passage au repère matériau
      UMATRotationMatrix3D m(PROPS+9,DROT);
      m.rotateStressesForward(STRESS,s);
      m.rotateStrainsForward(STRAN,e);
      m.rotateStrainsForward(DSTRAN,de);
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[16]);
      ThermalExpansionCoefficientCheck::exe(PROPS[17]);
      ThermalExpansionCoefficientCheck::exe(PROPS[18]);
      const bool bDDSOE = std::abs(*DDSOE)>0.5; 
      Handler handler(DTIME,e,de,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,s,op,sfeh);
      handler.exe(DDSOE,s,STATEV);
      m.rotateStressesBackward(s,STRESS);
      if(bDDSOE){
	m.rotateTangentOperatorBackward(DDSOE);
      }
    } // end of UMATOrthotropicBehaviourHandler<3u,Behaviour>::exe
  };

  /*!
   * The handler for finite strain orthotropic behaviours
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,H,Behaviour>
    : public UMATOrthotropicFiniteStrainBehaviourDispatcher<H,Behaviour>::type
  {
    typedef typename UMATOrthotropicFiniteStrainBehaviourDispatcher<H,Behaviour>::type Handler;
    using Handler::exe;
  };

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicFiniteStrainBehaviourHandler1D
    : private UMATBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,H,Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME,
	       const UMATReal *const,
	       UMATReal *const DDSOE,
	       const UMATReal *const F0 ,
	       const UMATReal *const F1,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const tfel::material::OutOfBoundsPolicy op,
	       const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<H,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,H,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	  Traits::useTimeSubStepping,
	  typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	  typename UMATBehaviourHandler::template Integrator<bs,ba>
	  >::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      typedef typename IF<
	MTraits::handlesThermalExpansion,
	typename UMATBehaviourHandler::CheckThermalExpansionCoefficientIsNull,
	typename UMATBehaviourHandler::DontCheckThermalExpansionCoefficientIsNull
	>::type ThermalExpansionCoefficientCheck;
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[7]);
      ThermalExpansionCoefficientCheck::exe(PROPS[8]);
      ThermalExpansionCoefficientCheck::exe(PROPS[9]);
      Handler handler(DTIME,F0,F1,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,STRESS,op,sfeh);
      handler.exe(DDSOE,STRESS,STATEV);
    } // end of UMATOrthotropicBehaviourHander<1u,Behaviour>::exe
  }; // end of struct UMATOrthotropicBehaviourHander<1u,Behaviour>

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicFiniteStrainBehaviourHandler2D
    : private UMATBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,H,Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME ,
	       const UMATReal *const DROT  ,
	       UMATReal *const DDSOE,
	       const UMATReal *const F0 ,
	       const UMATReal *const F1,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const tfel::material::OutOfBoundsPolicy op,
	       const StressFreeExpansionHandler& sfeh) 
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<H,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,H,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	Traits::useTimeSubStepping,
	typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	typename UMATBehaviourHandler::template Integrator<bs,ba>
	>::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      typedef typename IF<
	MTraits::handlesThermalExpansion,
	typename UMATBehaviourHandler::CheckThermalExpansionCoefficientIsNull,
	typename UMATBehaviourHandler::DontCheckThermalExpansionCoefficientIsNull
	>::type ThermalExpansionCoefficientCheck;
      UMATReal s[4];
      UMATReal F0m[9];
      UMATReal F1m[9];
      UMATFiniteStrainRotationMatrix2D m(PROPS+7,DROT);
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[10]);
      ThermalExpansionCoefficientCheck::exe(PROPS[11]);
      ThermalExpansionCoefficientCheck::exe(PROPS[12]);
      m.rotateStressesForward(STRESS,s);
      m.rotateDeformationGradientForward(F0,F0m);
      m.rotateDeformationGradientForward(F1,F1m);
      const bool bDDSOE = std::abs(*DDSOE)>0.5; 
      Handler handler(DTIME,F0m,F1m,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,s,op,sfeh);
      handler.exe(DDSOE,s,STATEV);
      m.rotateStressesBackward(s,STRESS);
      if(bDDSOE){
	m.rotateTangentOperatorBackward(DDSOE);
      }
    } // end of UMATOrthotropicBehaviourHander<2u,Behaviour>::exe
  }; // end of UMATOrthotropicBehaviourHander<2u,Behaviour>

  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL
  UMATOrthotropicFiniteStrainBehaviourHandler2D<tfel::material::ModellingHypothesis::PLANESTRESS,
						Behaviour>
    : private UMATBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,
				   tfel::material::ModellingHypothesis::PLANESTRESS,
				   Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME ,
	       const UMATReal *const DROT  ,
	       UMATReal *const DDSOE,
	       const UMATReal *const F0 ,
	       const UMATReal *const F1,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const tfel::material::OutOfBoundsPolicy op,
	       const StressFreeExpansionHandler& sfeh) 
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      const ModellingHypothesis::Hypothesis H = ModellingHypothesis::PLANESTRESS;	   
      typedef MechanicalBehaviourTraits<Behaviour<H,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<H,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,H,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	Traits::useTimeSubStepping,
	typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	typename UMATBehaviourHandler::template Integrator<bs,ba>
	>::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      typedef typename IF<
	MTraits::handlesThermalExpansion,
	typename UMATBehaviourHandler::CheckThermalExpansionCoefficientIsNull,
	typename UMATBehaviourHandler::DontCheckThermalExpansionCoefficientIsNull
	>::type ThermalExpansionCoefficientCheck;
      UMATReal s[4];
      UMATReal F0m[9];
      UMATReal F1m[9];
      UMATFiniteStrainRotationMatrix2D m(PROPS+4,DROT);
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[10]);
      ThermalExpansionCoefficientCheck::exe(PROPS[11]);
      ThermalExpansionCoefficientCheck::exe(PROPS[12]);
      m.rotateStressesForward(STRESS,s);
      m.rotateDeformationGradientForward(F0,F0m);
      m.rotateDeformationGradientForward(F1,F1m);
      const bool bDDSOE = std::abs(*DDSOE)>0.5; 
      Handler handler(DTIME,F0m,F1m,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,s,op,sfeh);
      handler.exe(DDSOE,s,STATEV);
      m.rotateStressesBackward(s,STRESS);
      if(bDDSOE){
	m.rotateTangentOperatorBackward(DDSOE);
      }
    } // end of UMATOrthotropicBehaviourHander<2u,Behaviour>::exe
  }; // end of UMATOrthotropicBehaviourHander<2u,Behaviour>

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATOrthotropicFiniteStrainBehaviourHandler3D
    : private UMATBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,H,Behaviour>
  {
    TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME,
	       const UMATReal *const DROT,
	       UMATReal *const DDSOE,
	       const UMATReal *const F0,
	       const UMATReal *const F1,
	       const UMATReal *const TEMP  ,
	       const UMATReal *const DTEMP,
	       const UMATReal *const PROPS ,
	       const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,
	       const UMATReal *const DPRED,
	       UMATReal *const STATEV,
	       const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,
	       const tfel::material::OutOfBoundsPolicy op,
	       const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H,UMATReal,false> > MTraits;
      typedef UMATTraits<Behaviour<H,UMATReal,false> > Traits;
      typedef UMATBehaviourHandler<FINITESTRAINSTANDARDBEHAVIOUR,H,Behaviour> UMATBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	Traits::useTimeSubStepping,
	typename UMATBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	typename UMATBehaviourHandler::template Integrator<bs,ba>
	>::type,
	typename UMATBehaviourHandler::Error>::type Handler;
      typedef typename IF<
	MTraits::handlesThermalExpansion,
	typename UMATBehaviourHandler::CheckThermalExpansionCoefficientIsNull,
	typename UMATBehaviourHandler::DontCheckThermalExpansionCoefficientIsNull
	>::type ThermalExpansionCoefficientCheck;
      UMATReal sm[6];
      UMATReal F0m[9];
      UMATReal F1m[9];
      // Passage au repère matériau
      UMATFiniteStrainRotationMatrix3D m(PROPS+9,DROT);
      m.rotateStressesForward(STRESS,sm);
      m.rotateDeformationGradientForward(F0,F0m);
      m.rotateDeformationGradientForward(F1,F1m);
      UMATBehaviourHandler::checkNPROPS(*NPROPS);
      UMATBehaviourHandler::checkNSTATV(*NSTATV);
      ThermalExpansionCoefficientCheck::exe(PROPS[16]);
      ThermalExpansionCoefficientCheck::exe(PROPS[17]);
      ThermalExpansionCoefficientCheck::exe(PROPS[18]);
      const bool bDDSOE = std::abs(*DDSOE)>0.5; 
      Handler handler(DTIME,F0m,F1m,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,sm,op,sfeh);
      handler.exe(DDSOE,sm,STATEV);
      m.rotateStressesBackward(sm,STRESS);
      if(bDDSOE){
	m.rotateTangentOperatorBackward(DDSOE);
      }
    } // end of UMATOrthotropicBehaviourHandler<3u,Behaviour>::exe
	
  }; // end of struct UMATOrthotropicBehaviourHandler<3u,Behaviour>

} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_UMATORTHOTROPICBEHAVIOURHANDLER_H */
