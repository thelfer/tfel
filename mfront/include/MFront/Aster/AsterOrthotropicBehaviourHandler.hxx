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

namespace aster
{

  /*!
   * forward declaration
   */
  template<unsigned short N,
	   template<unsigned short,typename,bool> class Behaviour>
  struct AsterOrthotropicBehaviourHandler;

  // template<template<unsigned short,typename,bool> class Behaviour>
  // struct TFEL_VISIBILITY_LOCAL AsterOrthotropicBehaviourHandler<1u,Behaviour>
  //   : private AsterBehaviourHandler<1u,Behaviour>
  //   {
  // 	TFEL_ASTER_INLINE2 static 
  // 	  void exe(const AsterReal *const DTIME,
  // 		   const AsterReal *const,
  // 		   const AsterReal *const DDSOE,
  // 		   const AsterReal *const STRAN ,
  // 		   const AsterReal *const DSTRAN,
  // 		   const AsterReal *const TEMP  ,
  // 		   const AsterReal *const DTEMP,
  // 		   const AsterReal *const PROPS ,
  // 		   const AsterInt  *const NPROPS,
  // 		   const AsterReal *const PREDEF,
  // 		   const AsterReal *const DPRED,
  // 		   AsterReal *const STATEV,
  // 		   const AsterInt  *const NSTATV,
  // 		   AsterReal *const STRESS)
  // 	{
  // 	  using namespace tfel::meta;
  // 	  using namespace tfel::material;
  // 	  using namespace tfel::math;
  // 	  typedef MechanicalBehaviourTraits<Behaviour<1u,AsterReal,false> > MTraits;
  // 	  typedef AsterTraits<Behaviour<1u,AsterReal,false> > Traits;
  // 	  typedef AsterBehaviourHandler<1u,Behaviour> AsterBehaviourHandler;
  // 	  const bool is_defined_ = MTraits::is_defined;
  // 	  const bool bs = Traits::requiresStiffnessTensor;
  // 	  const bool ba = Traits::requiresThermalExpansionTensor;
  // 	  typedef typename IF<
  // 	    is_defined_,
  // 	    typename AsterBehaviourHandler::template Integrator<bs,ba>,
  // 	    typename AsterBehaviourHandler::Error>::type Handler;
  // 	  AsterBehaviourHandler::checkNPROPS(*NPROPS);
  // 	  AsterBehaviourHandler::checkNSTATV(*NSTATV);
  // 	  Handler handler(DTIME,DDSOE,STRAN,
  // 			  DSTRAN,TEMP,DTEMP,PROPS,
  // 			  PREDEF,DPRED,STATEV,
  // 			  STRESS);
  // 	  handler.exe(STRESS,STATEV);
  // 	} // end of AsterOrthotropicBehaviourHandler1D::exe
  //   }; // end of struct AsterOrthotropicBehaviourHandler1D

  // template<template<unsigned short,typename,bool> class Behaviour>
  // struct TFEL_VISIBILITY_LOCAL AsterOrthotropicBehaviourHandler<2u,Behaviour>
  //   : private AsterBehaviourHandler<2u,Behaviour>
  //   {
  // 	TFEL_ASTER_INLINE2 static 
  // 	  void exe(const AsterReal *const DTIME ,
  // 		   const AsterReal *const DROT  ,
  // 		   const AsterReal *const DDSOE,
  // 		   const AsterReal *const STRAN ,
  // 		   const AsterReal *const DSTRAN,
  // 		   const AsterReal *const TEMP  ,
  // 		   const AsterReal *const DTEMP,
  // 		   const AsterReal *const PROPS ,
  // 		   const AsterInt  *const NPROPS,
  // 		   const AsterReal *const PREDEF,
  // 		   const AsterReal *const DPRED,
  // 		   AsterReal *const STATEV,
  // 		   const AsterInt  *const NSTATV,
  // 		   AsterReal *const STRESS) 
  // 	{
  // 	  using namespace tfel::meta;
  // 	  using namespace tfel::material;
  // 	  using namespace tfel::math;
  // 	  typedef MechanicalBehaviourTraits<Behaviour<2u,AsterReal,false> > MTraits;
  // 	  typedef AsterTraits<Behaviour<2u,AsterReal,false> > Traits;
  // 	  typedef AsterBehaviourHandler<2u,Behaviour> AsterBehaviourHandler;
  // 	  const bool is_defined_ = MTraits::is_defined;
  // 	  const bool bs = Traits::requiresStiffnessTensor;
  // 	  const bool ba = Traits::requiresThermalExpansionTensor;
  // 	  typedef typename IF<
  // 	    is_defined_,
  // 	    typename AsterBehaviourHandler::template Integrator<bs,ba>,
  // 	    typename AsterBehaviourHandler::Error>::type Handler;
  // 	  AsterReal s[4];
  // 	  AsterReal e[4];
  // 	  AsterReal de[4];
  // 	  AsterRotationMatrix2D m(PROPS+7,DROT);
  // 	  m.rotateStressesForward(STRESS,s);
  // 	  m.rotateStrainsForward(STRAN,e);
  // 	  m.rotateStrainsForward(DSTRAN,de);
  // 	  AsterBehaviourHandler::checkNPROPS(*NPROPS);
  // 	  AsterBehaviourHandler::checkNSTATV(*NSTATV);
  // 	  Handler handler(DTIME,DDSOE,
  // 			  e,de,TEMP,DTEMP,PROPS,
  // 			  PREDEF,DPRED,STATEV,s);
  // 	  handler.exe(s,STATEV);
  // 	  m.rotateStressesBackward(s,STRESS);
  // 	} // end of AsterOrthotropicBehaviourHandler2D::exe
  //   }; // end of AsterOrthotropicBehaviourHandler2D

  // template<template<unsigned short,typename,bool> class Behaviour>
  // struct TFEL_VISIBILITY_LOCAL AsterOrthotropicBehaviourHandler<3u,Behaviour>
  //   : private AsterBehaviourHandler<3u,Behaviour>
  //   {
  // 	TFEL_ASTER_INLINE2 static 
  // 	  void exe(const AsterReal *const DTIME,
  // 		   const AsterReal *const DROT,
  // 		   const AsterReal *const DDSOE,
  // 		   const AsterReal *const STRAN,
  // 		   const AsterReal *const DSTRAN,
  // 		   const AsterReal *const TEMP  ,
  // 		   const AsterReal *const DTEMP,
  // 		   const AsterReal *const PROPS ,
  // 		   const AsterInt  *const NPROPS,
  // 		   const AsterReal *const PREDEF,
  // 		   const AsterReal *const DPRED,
  // 		   AsterReal *const STATEV,
  // 		   const AsterInt  *const NSTATV,
  // 		   AsterReal *const STRESS) 
  // 	{
  // 	  using namespace tfel::meta;
  // 	  using namespace tfel::material;
  // 	  using namespace tfel::math;
  // 	  typedef MechanicalBehaviourTraits<Behaviour<3u,AsterReal,false> > MTraits;
  // 	  typedef AsterTraits<Behaviour<3u,AsterReal,false> > Traits;
  // 	  typedef AsterBehaviourHandler<3u,Behaviour> AsterBehaviourHandler;
  // 	  const bool is_defined_ = MTraits::is_defined;
  // 	  const bool bs = Traits::requiresStiffnessTensor;
  // 	  const bool ba = Traits::requiresThermalExpansionTensor;
  // 	  typedef typename IF<
  // 	    is_defined_,
  // 	    typename AsterBehaviourHandler::template Integrator<bs,ba>,
  // 	    typename AsterBehaviourHandler::Error>::type Handler;
  // 	  AsterReal  s[6];
  // 	  AsterReal  e[6];
  // 	  AsterReal de[6];
  // 	  // Passage au repère matériau
  // 	  AsterRotationMatrix3D m(PROPS+9,DROT);
  // 	  m.rotateStressesForward(STRESS,s);
  // 	  m.rotateStrainsForward(STRAN,e);
  // 	  m.rotateStrainsForward(DSTRAN,de);
  // 	  AsterBehaviourHandler::checkNPROPS(*NPROPS);
  // 	  AsterBehaviourHandler::checkNSTATV(*NSTATV);
  // 	  Handler handler(DTIME,DDSOE,e,
  // 			  de,TEMP,DTEMP,PROPS,
  // 			  PREDEF,DPRED,STATEV,s);
  // 	  handler.exe(s,STATEV);
  // 	  m.rotateStressesBackward(s,STRESS);
  // 	} // end of AsterOrthotropicBehaviourHandler3D::exe
	
  //   }; // end of struct AsterOrthotropicBehaviourHandler3D
  
} // end of namespace aster

#endif /* _LIB_MFRONT_ASTER_ASTERORTHOTROPICBEHAVIOURHANDLER_H */

