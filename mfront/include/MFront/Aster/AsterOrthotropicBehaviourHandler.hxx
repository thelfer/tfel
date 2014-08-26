/*! 
 * \file  AsterOrthotropicBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 */

#ifndef _LIB_MFRONT_ASTER_ASTERORTHOTROPICBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_ASTER_ASTERORTHOTROPICBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H_
#error "This header shall not be called directly"
#endif

#include<cmath>
#include"MFront/Aster/AsterRotationMatrix.hxx"
#include"MFront/Aster/AsterFiniteStrainRotationMatrix.hxx"

namespace aster
{

  /*!
   * forward declaration
   */
  template<AsterBehaviourType btype,
	   unsigned short N,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct AsterOrthotropicBehaviourHandler;

  template<template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicBehaviourHandler<aster::SMALLSTRAINSTANDARDBEHAVIOUR,1u,Behaviour>
    : private AsterBehaviourHandler<1u,Behaviour>
  {
    TFEL_ASTER_INLINE2 static 
      void exe(const AsterReal *const DTIME,
	       const AsterReal *const,
	       AsterReal *const DDSOE,
	       const AsterReal *const STRAN ,
	       const AsterReal *const DSTRAN,
	       const AsterReal *const TEMP  ,
	       const AsterReal *const DTEMP,
	       const AsterReal *const PROPS ,
	       const AsterInt  *const NPROPS,
	       const AsterReal *const PREDEF,
	       const AsterReal *const DPRED,
	       AsterReal *const STATEV,
	       const AsterInt  *const NSTATV,
	       AsterReal *const STRESS,
	       const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef Behaviour<AsterModellingHypothesis<1u>::value,AsterReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> MTraits;
      typedef AsterTraits<BV> Traits;
      typedef AsterBehaviourHandler<1u,Behaviour> AsterBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename AsterBehaviourHandler::template Integrator<bs,ba>,
	typename AsterBehaviourHandler::Error>::type Handler;
      AsterBehaviourHandler::checkNPROPS(*NPROPS);
      AsterBehaviourHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME,STRAN,DSTRAN,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,STRESS,sfeh);
      handler.exe(DDSOE,STRESS,STATEV);
  } // end of AsterOrthotropicBehaviourHandler1D::exe
}; // end of struct AsterOrthotropicBehaviourHandler1D

template<template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
struct TFEL_VISIBILITY_LOCAL AsterOrthotropicBehaviourHandler<aster::SMALLSTRAINSTANDARDBEHAVIOUR,2u,Behaviour>
  : private AsterBehaviourHandler<2u,Behaviour>
{
  TFEL_ASTER_INLINE2 static 
    void exe(const AsterReal *const DTIME ,
	     const AsterReal *const DROT  ,
	     AsterReal *const DDSOE,
	     const AsterReal *const STRAN ,
	     const AsterReal *const DSTRAN,
	     const AsterReal *const TEMP  ,
	     const AsterReal *const DTEMP,
	     const AsterReal *const PROPS ,
	     const AsterInt  *const NPROPS,
	     const AsterReal *const PREDEF,
	     const AsterReal *const DPRED,
	     AsterReal *const STATEV,
	     const AsterInt  *const NSTATV,
	     AsterReal *const STRESS,
	     const StressFreeExpansionHandler& sfeh) 
  {
    using namespace tfel::meta;
    using namespace tfel::material;
    using namespace tfel::math;
    typedef Behaviour<AsterModellingHypothesis<2u>::value,AsterReal,false> BV;
    typedef MechanicalBehaviourTraits<BV> MTraits;
    typedef AsterTraits<BV> Traits;
    typedef AsterBehaviourHandler<2u,Behaviour> AsterBehaviourHandler;
    const bool is_defined_ = MTraits::is_defined;
    const bool bs = Traits::requiresStiffnessTensor;
    const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
    typedef typename IF<
      is_defined_,
      typename AsterBehaviourHandler::template Integrator<bs,ba>,
      typename AsterBehaviourHandler::Error>::type Handler;
    AsterReal s[4];
    AsterReal e[4];
    AsterReal de[4];
    AsterRotationMatrix2D m(DROT);
    m.rotateStressesForward(STRESS,s);
    m.rotateStrainsForward(STRAN,e);
    m.rotateStrainsForward(DSTRAN,de);
    AsterBehaviourHandler::checkNPROPS(*NPROPS);
    AsterBehaviourHandler::checkNSTATV(*NSTATV);
    const bool bDDSOE = std::abs(*DDSOE)>0.5; 
    Handler handler(DTIME,e,de,TEMP,DTEMP,PROPS,
		    PREDEF,DPRED,STATEV,s,sfeh);
    handler.exe(DDSOE,s,STATEV);
    m.rotateStressesBackward(s,STRESS);
    if(bDDSOE){
      m.rotateStiffnessMatrixBackward(DDSOE);
    }
  } // end of AsterOrthotropicBehaviourHandler2D::exe
  }; // end of AsterOrthotropicBehaviourHandler2D

  template<template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicBehaviourHandler<aster::SMALLSTRAINSTANDARDBEHAVIOUR,3u,Behaviour>
    : private AsterBehaviourHandler<3u,Behaviour>
  {
    TFEL_ASTER_INLINE2 static 
      void exe(const AsterReal *const DTIME,
	       const AsterReal *const DROT,
	       AsterReal *const DDSOE,
	       const AsterReal *const STRAN,
	       const AsterReal *const DSTRAN,
	       const AsterReal *const TEMP  ,
	       const AsterReal *const DTEMP,
	       const AsterReal *const PROPS ,
	       const AsterInt  *const NPROPS,
	       const AsterReal *const PREDEF,
	       const AsterReal *const DPRED,
	       AsterReal *const STATEV,
	       const AsterInt  *const NSTATV,
	       AsterReal *const STRESS,
	       const StressFreeExpansionHandler& sfeh) 
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef Behaviour<AsterModellingHypothesis<3u>::value,AsterReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> MTraits;
      typedef AsterTraits<BV> Traits;
      typedef AsterBehaviourHandler<3u,Behaviour> AsterBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename AsterBehaviourHandler::template Integrator<bs,ba>,
	typename AsterBehaviourHandler::Error>::type Handler;
      AsterReal  s[6];
      AsterReal  e[6];
      AsterReal de[6];
      // Passage au repère matériau
      AsterRotationMatrix3D m(DROT);
      m.rotateStressesForward(STRESS,s);
      m.rotateStrainsForward(STRAN,e);
      m.rotateStrainsForward(DSTRAN,de);
      AsterBehaviourHandler::checkNPROPS(*NPROPS);
      AsterBehaviourHandler::checkNSTATV(*NSTATV);
      const bool bDDSOE = std::abs(*DDSOE)>0.5; 
      Handler handler(DTIME,e,de,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,s,sfeh);
      handler.exe(DDSOE,s,STATEV);
      m.rotateStressesBackward(s,STRESS);
      if(bDDSOE){
	m.rotateStiffnessMatrixBackward(DDSOE);
      }
    } // end of AsterOrthotropicBehaviourHandler3D::exe
    
  }; // end of struct AsterOrthotropicBehaviourHandler3D
  
  template<template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicBehaviourHandler<aster::FINITESTRAINSTANDARDBEHAVIOUR,1u,Behaviour>
    : private AsterBehaviourHandler<1u,Behaviour>
  {
    TFEL_ASTER_INLINE2 static 
      void exe(const AsterReal *const DTIME,
	       const AsterReal *const,
	       AsterReal *const DDSOE,
	       const AsterReal *const F0 ,
	       const AsterReal *const F1,
	       const AsterReal *const TEMP  ,
	       const AsterReal *const DTEMP,
	       const AsterReal *const PROPS ,
	       const AsterInt  *const NPROPS,
	       const AsterReal *const PREDEF,
	       const AsterReal *const DPRED,
	       AsterReal *const STATEV,
	       const AsterInt  *const NSTATV,
	       AsterReal *const STRESS,
	       const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef Behaviour<AsterModellingHypothesis<1u>::value,AsterReal,false> BV;
      typedef AsterBehaviourHandler<1u,Behaviour> AsterBehaviourHandler;
      typedef MechanicalBehaviourTraits<BV> MTraits;
      typedef AsterTraits<BV> Traits;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
      	is_defined_,
      	typename AsterBehaviourHandler::template Integrator<bs,ba>,
      	typename AsterBehaviourHandler::Error>::type Handler;
      AsterBehaviourHandler::checkNPROPS(*NPROPS);
      AsterBehaviourHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME,F0,F1,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,STRESS,sfeh);
      handler.exe(DDSOE,STRESS,STATEV);
    } // end of AsterOrthotropicBehaviourHander<1u,Behaviour>::exe
  }; // end of struct AsterOrthotropicBehaviourHander<1u,Behaviour>

  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicBehaviourHandler<aster::FINITESTRAINSTANDARDBEHAVIOUR,2u,Behaviour>
    : private AsterBehaviourHandler<2u,Behaviour>
  {
    TFEL_ASTER_INLINE2 static 
      void exe(const AsterReal *const DTIME ,
	       const AsterReal *const DROT  ,
	       AsterReal *const DDSOE,
	       const AsterReal *const F0 ,
	       const AsterReal *const F1,
	       const AsterReal *const TEMP  ,
	       const AsterReal *const DTEMP,
	       const AsterReal *const PROPS ,
	       const AsterInt  *const NPROPS,
	       const AsterReal *const PREDEF,
	       const AsterReal *const DPRED,
	       AsterReal *const STATEV,
	       const AsterInt  *const NSTATV,
	       AsterReal *const STRESS,
	     const StressFreeExpansionHandler& sfeh) 
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef Behaviour<AsterModellingHypothesis<2u>::value,AsterReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> MTraits;
      typedef AsterTraits<BV> Traits;
      typedef AsterBehaviourHandler<2u,Behaviour> AsterBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename AsterBehaviourHandler::template Integrator<bs,ba>,
	typename AsterBehaviourHandler::Error>::type Handler;
      AsterReal s[4];
      AsterReal F0m[9];
      AsterReal F1m[9];
      AsterFiniteStrainRotationMatrix2D m(DROT);
      AsterBehaviourHandler::checkNPROPS(*NPROPS);
      AsterBehaviourHandler::checkNSTATV(*NSTATV);
      m.rotateStressesForward(STRESS,s);
      m.rotateDeformationGradientForward(F0,F0m);
      m.rotateDeformationGradientForward(F1,F1m);
      const bool bDDSOE = std::abs(*DDSOE)>0.5; 
      Handler handler(DTIME,F0m,F1m,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,s,sfeh);
      handler.exe(DDSOE,s,STATEV);
      m.rotateStressesBackward(s,STRESS);
      if(bDDSOE){
	m.rotateTangentOperatorBackward(DDSOE);
      }
    } // end of AsterOrthotropicBehaviourHander<2u,Behaviour>::exe
  }; // end of AsterOrthotropicBehaviourHander<2u,Behaviour>

  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterOrthotropicBehaviourHandler<aster::FINITESTRAINSTANDARDBEHAVIOUR,3u,Behaviour>
    : private AsterBehaviourHandler<3u,Behaviour>
  {
    TFEL_ASTER_INLINE2 static 
      void exe(const AsterReal *const DTIME,
	       const AsterReal *const DROT,
	       AsterReal *const DDSOE,
	       const AsterReal *const F0,
	       const AsterReal *const F1,
	       const AsterReal *const TEMP  ,
	       const AsterReal *const DTEMP,
	       const AsterReal *const PROPS ,
	       const AsterInt  *const NPROPS,
	       const AsterReal *const PREDEF,
	       const AsterReal *const DPRED,
	       AsterReal *const STATEV,
	       const AsterInt  *const NSTATV,
	       AsterReal *const STRESS,
	       const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef Behaviour<AsterModellingHypothesis<3u>::value,AsterReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> MTraits;
      typedef AsterBehaviourHandler<3u,Behaviour> AsterBehaviourHandler;
      typedef AsterTraits<BV> Traits;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
      typedef typename IF<
	is_defined_,
	typename AsterBehaviourHandler::template Integrator<bs,ba>,
	typename AsterBehaviourHandler::Error>::type Handler;
      AsterReal sm[6];
      AsterReal F0m[9];
      AsterReal F1m[9];
      // Passage au repère matériau
      AsterFiniteStrainRotationMatrix3D m(DROT);
      m.rotateStressesForward(STRESS,sm);
      m.rotateDeformationGradientForward(F0,F0m);
      m.rotateDeformationGradientForward(F1,F1m);
      AsterBehaviourHandler::checkNPROPS(*NPROPS);
      AsterBehaviourHandler::checkNSTATV(*NSTATV);
      const bool bDDSOE = std::abs(*DDSOE)>0.5; 
      Handler handler(DTIME,F0m,F1m,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,sm,sfeh);
      handler.exe(DDSOE,sm,STATEV);
      m.rotateStressesBackward(sm,STRESS);
      if(bDDSOE){
	m.rotateTangentOperatorBackward(DDSOE);
      }
    } // end of AsterOrthotropicBehaviourHandler<3u,Behaviour>::exe
  };

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTER_ASTERORTHOTROPICBEHAVIOURHANDLER_H */

