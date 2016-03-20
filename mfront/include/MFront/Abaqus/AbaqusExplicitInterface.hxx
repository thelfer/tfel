/*!
 * \file   AbaqusExplicitInterface.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   19 mars 2016
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSEXPLICITINTERFACE_HXX
#define LIB_MFRONT_ABAQUS_ABAQUSEXPLICITINTERFACE_HXX

#include"MFront/Abaqus/Abaqus.hxx"

namespace abaqus{

  /*!
   * \class  AbaqusExplicitInterface
   * \brief This class create an interface between a behaviour class
   * and the abaqus finite element code
   *
   * \note : most of branching is done at compile-time. to the very
   * exeception of the stress-free expansion which are handled through
   * two functions pointers which take into account the specificities
   * of finite strain strategy used. The choice of introducing those
   * runtime-functions comes from the fact that we did not want a code
   * duplication between two different finite strain strategies (to
   * reduce both compile-time and library size).
   */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AbaqusExplicitInterface
    : protected AbaqusInterfaceExceptions
  {

    TFEL_ABAQUS_INLINE2 static
     int exe(const AbaqusInt  *const NTENS, const AbaqusReal *const DTIME,
	     const AbaqusReal *const DROT,  AbaqusReal *const DDSDDE,
	     const AbaqusReal *const STRAN, const AbaqusReal *const DSTRAN,
	     const AbaqusReal *const TEMP,  const AbaqusReal *const DTEMP,
	     const AbaqusReal *const PROPS, const AbaqusInt  *const NPROPS,
	     const AbaqusReal *const PREDEF,const AbaqusReal *const DPRED,
	     AbaqusReal *const STATEV,const AbaqusInt  *const NSTATV,
	     AbaqusReal *const STRESS,	    AbaqusReal *const PNEWDT,
	     const tfel::material::OutOfBoundsPolicy op,
	     const StressFreeExpansionHandler<AbaqusReal>& sfeh)
    {
      using namespace tfel::material;
      if( *NTENS == 4 ){
	return CallBehaviour<ModellingHypothesis::GENERALISEDPLANESTRAIN>::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,
									       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
									       STATEV,NSTATV,STRESS,PNEWDT,op,sfeh);
      } else if( *NTENS == 3 ){
        return CallBehaviour<ModellingHypothesis::PLANESTRESS>::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,
								    TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
								    STATEV,NSTATV,STRESS,PNEWDT,op,sfeh);
      } else if( *NTENS == 6 ){
        return CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,
								       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
								       STATEV,NSTATV,STRESS,PNEWDT,op,sfeh);
      } else {
        AbaqusInterfaceExceptions::displayUnsupportedHypothesisMessage();
        return -2;
      }
    }
  
} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSEXPLICITINTERFACE_HXX_ */
