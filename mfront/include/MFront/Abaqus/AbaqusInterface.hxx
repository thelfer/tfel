/*!
 * \file   mfront/include/MFront/Abaqus/AbaqusInterface.hxx
 * \brief  This file implements the AbaqusInterface class.
 * \author Helfer Thomas
 * \date   28 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ABAQUS_CALL_H_
#define LIB_MFRONT_ABAQUS_CALL_H_ 

#include<string>
#include<vector>
#include<limits>
#include<algorithm>
#include<type_traits>

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Exception/TFELException.hxx"
#include"TFEL/FSAlgorithm/copy.hxx"

#include"TFEL/Material/MechanicalBehaviourTraits.hxx"
#include"TFEL/Material/MaterialException.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Abaqus/Abaqus.hxx"
#include"MFront/Abaqus/AbaqusConfig.hxx"
#include"MFront/Abaqus/AbaqusTraits.hxx"
#include"MFront/Abaqus/AbaqusException.hxx"
#include"MFront/Abaqus/AbaqusBehaviourHandler.hxx"
#include"MFront/Abaqus/AbaqusInterfaceExceptions.hxx"

namespace abaqus
{

  /*!
   * \class  AbaqusInterface
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
   *
   * \author Helfer Thomas
   * \date   28 Jul 2006
   */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AbaqusInterface
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
	     AbaqusReal *const STRESS,
	     const tfel::material::OutOfBoundsPolicy op,
	     const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::material;
      if( *NTENS == 4 ){
	return CallBehaviour<ModellingHypothesis::GENERALISEDPLANESTRAIN>::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,
									       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
									       STATEV,NSTATV,STRESS,op,sfeh);
      } else if( *NTENS == 3 ){
        return CallBehaviour<ModellingHypothesis::PLANESTRESS>::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,
								    TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
								    STATEV,NSTATV,STRESS,op,sfeh);
      } else if( *NTENS == 6 ){
        return CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,
								       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
								       STATEV,NSTATV,STRESS,op,sfeh);
      } else {
        AbaqusInterfaceExceptions::displayUnsupportedHypothesisMessage();
        return -2;
      }
    }

  private:

    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct UnsupportedHypothesisHandler
    {
      TFEL_ABAQUS_INLINE2 static void
      exe(const AbaqusInt  *const, const AbaqusReal *const,
	  const AbaqusReal *const,  AbaqusReal *const,
	  const AbaqusReal *const, const AbaqusReal *const,
	  const AbaqusReal *const,  const AbaqusReal *const,
	  const AbaqusReal *const, const AbaqusInt  *const,
	  const AbaqusReal *const,const AbaqusReal *const,
	  AbaqusReal *const,const AbaqusInt  *const,
	  AbaqusReal *const,
	  const tfel::material::OutOfBoundsPolicy,
	  const StressFreeExpansionHandler&)
      {
	using BV = Behaviour<H,AbaqusReal,false>;
	using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
	throw(AbaqusInvalidModellingHypothesis(MTraits::getName()));
      }
    }; // end of struct UnsupportedHypothesisHandler
  
    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct CallBehaviour
    {
      TFEL_ABAQUS_INLINE2 static
      int exe(const AbaqusInt  *const NTENS, const AbaqusReal *const DTIME,
	      const AbaqusReal *const DROT,  AbaqusReal *const DDSDDE,
	      const AbaqusReal *const STRAN, const AbaqusReal *const DSTRAN,
	      const AbaqusReal *const TEMP,  const AbaqusReal *const DTEMP,
	      const AbaqusReal *const PROPS, const AbaqusInt  *const NPROPS,
	      const AbaqusReal *const PREDEF,const AbaqusReal *const DPRED,
	      AbaqusReal *const STATEV,const AbaqusInt  *const NSTATV,
	      AbaqusReal *const STRESS,
	      const tfel::material::OutOfBoundsPolicy op,
	      const StressFreeExpansionHandler& sfeh)
      {
	using BV = Behaviour<H,AbaqusReal,false>;
	using MTraits  = tfel::material::MechanicalBehaviourTraits<BV>;
	const bool is_defined_ = MTraits::is_defined;
	using Handler = typename std::conditional<is_defined_,CallBehaviour2<H>,
						  UnsupportedHypothesisHandler<H>>::type;
	try{
	  Handler::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,
		       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
		       STATEV,NSTATV,STRESS,op,sfeh);
	}
	catch(const AbaqusException& e){
	  AbaqusInterfaceExceptions::treatAbaqusException(MTraits::getName(),e);
	  return -2;
	}
	catch(const tfel::material::OutOfBoundsException& e){
	  AbaqusInterfaceExceptions::treatMaterialException(MTraits::getName(),e);
	  return -3;
	}
	catch(const tfel::material::DivergenceException& e){
	  AbaqusInterfaceExceptions::treatTFELException(MTraits::getName(),e);
	  return -4;
	}
	catch(const tfel::material::MaterialException& e){
	  AbaqusInterfaceExceptions::treatStandardException(MTraits::getName(),e);
	  return -5;
	}
	catch(const tfel::exception::TFELException& e){
	  AbaqusInterfaceExceptions::treatTFELException(MTraits::getName(),e);
	  return -6;
	}
	catch(const std::exception& e){
	  AbaqusInterfaceExceptions::treatStandardException(MTraits::getName(),e);
	  return -7;
	}
	catch(...){
	  AbaqusInterfaceExceptions::treatUnknownException(MTraits::getName());
	  return -8;
	}
	return 0;
      } // end of CallBehaviour::exe
    };
    
    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct CallBehaviour2
    {
      TFEL_ABAQUS_INLINE2 static void
      exe(const AbaqusInt  *const, const AbaqusReal *const DTIME,
	  const AbaqusReal *const DROT,  AbaqusReal *const DDSDDE,
	  const AbaqusReal *const STRAN, const AbaqusReal *const DSTRAN,
	  const AbaqusReal *const TEMP,  const AbaqusReal *const DTEMP,
	  const AbaqusReal *const PROPS, const AbaqusInt  *const NPROPS,
	  const AbaqusReal *const PREDEF,const AbaqusReal *const DPRED,
	  AbaqusReal *const STATEV,const AbaqusInt  *const NSTATV,
	  AbaqusReal *const STRESS,
	  const tfel::material::OutOfBoundsPolicy op,
	  const StressFreeExpansionHandler& sfeh)
      {
	using namespace std;
	using BV = Behaviour<H,AbaqusReal,false>;
	using ATraits  =  AbaqusTraits<BV>;
	using AHandler = AbaqusBehaviourHandler<H,Behaviour>;
	const bool bs = ATraits::requiresStiffnessTensor;
	const bool ba = ATraits::requiresThermalExpansionCoefficientTensor;
	using Integrator = typename AHandler::template Integrator<bs,ba>;
	AHandler::checkNPROPS(*NPROPS);
	AHandler::checkNSTATV(*NSTATV);
	Integrator i(DTIME,STRAN,DSTRAN,
		     TEMP,DTEMP,PROPS,
		     PREDEF,DPRED,STATEV,
		     STRESS,DROT,op,sfeh);
	i.exe(DDSDDE,STRESS,STATEV);
      }
    }; // end of struct CallBehaviour
  }; // end of struct AbaqusInterface
    
} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_CALL_H_ */

