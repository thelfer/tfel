/*!
 * \file   mfront/include/MFront/Europlexus/EuroplexusInterface.hxx
 * \brief  This file implements the EuroplexusInterface class.
 * \author Helfer Thomas
 * \date   28 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_EPX_CALL_H_
#define LIB_MFRONT_EPX_CALL_H_ 

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

#include"MFront/Europlexus/Europlexus.hxx"
#include"MFront/Europlexus/EuroplexusConfig.hxx"
#include"MFront/Europlexus/EuroplexusTraits.hxx"
#include"MFront/Europlexus/EuroplexusException.hxx"
#include"MFront/Europlexus/EuroplexusBehaviourHandler.hxx"
#include"MFront/Europlexus/EuroplexusInterfaceExceptions.hxx"

namespace epx
{

  /*!
   * \class  EuroplexusInterface
   * \brief This class create an interface between a behaviour class
   * and the europlexus finite element code
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
  struct TFEL_VISIBILITY_LOCAL EuroplexusInterface
    : protected EuroplexusInterfaceExceptions
  {

    TFEL_EPX_INLINE2 static
     int exe(const EuroplexusReal *const DTIME,EuroplexusReal *const DDSDDE,
	     const EuroplexusReal *const F0, const EuroplexusReal *const F1,
	     const EuroplexusReal *const TEMP,  const EuroplexusReal *const DTEMP,
	     const EuroplexusReal *const PROPS, const EuroplexusInt  *const NPROPS,
	     const EuroplexusReal *const PREDEF,const EuroplexusReal *const DPRED,
	     EuroplexusReal *const STATEV,const EuroplexusInt  *const NSTATV,
	     EuroplexusReal *const STRESS,	    EuroplexusReal *const PNEWDT,
	     const tfel::material::OutOfBoundsPolicy op,
	     const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::material;
      if( *NTENS == 4 ){
	return CallBehaviour<ModellingHypothesis::GENERALISEDPLANESTRAIN>::exe(NTENS,DTIME,F0,F1,
									       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
									       STATEV,NSTATV,STRESS,PNEWDT,op,sfeh);
      } else if( *NTENS == 3 ){
        return CallBehaviour<ModellingHypothesis::PLANESTRESS>::exe(NTENS,DTIME,F0,F1,
								    TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
								    STATEV,NSTATV,STRESS,PNEWDT,op,sfeh);
      } else if( *NTENS == 6 ){
        return CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(NTENS,DTIME,F0,F1,
								       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
								       STATEV,NSTATV,STRESS,PNEWDT,op,sfeh);
      } else {
        EuroplexusInterfaceExceptions::displayUnsupportedHypothesisMessage();
        return -2;
      }
    }

  private:

    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct UnsupportedHypothesisHandler
    {
      TFEL_EPX_INLINE2 static void
      exe(const EuroplexusInt  *const, const EuroplexusReal *const,
	  const EuroplexusReal *const,  EuroplexusReal *const,
	  const EuroplexusReal *const, const EuroplexusReal *const,
	  const EuroplexusReal *const,  const EuroplexusReal *const,
	  const EuroplexusReal *const, const EuroplexusInt  *const,
	  const EuroplexusReal *const,const EuroplexusReal *const,
	  EuroplexusReal *const,const EuroplexusInt  *const,
	  EuroplexusReal *const,      EuroplexusReal *const,
	  const tfel::material::OutOfBoundsPolicy,
	  const StressFreeExpansionHandler&)
      {
	using BV = Behaviour<H,EuroplexusReal,false>;
	using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
	throw(EuroplexusInvalidModellingHypothesis(MTraits::getName()));
      }
    }; // end of struct UnsupportedHypothesisHandler
  
    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct CallBehaviour
    {
      TFEL_EPX_INLINE2 static
      int exe(const EuroplexusInt  *const NTENS, const EuroplexusReal *const DTIME,
	      const EuroplexusReal *const F0, const EuroplexusReal *const F1,
	      const EuroplexusReal *const TEMP,  const EuroplexusReal *const DTEMP,
	      const EuroplexusReal *const PROPS, const EuroplexusInt  *const NPROPS,
	      const EuroplexusReal *const PREDEF,const EuroplexusReal *const DPRED,
	      EuroplexusReal *const STATEV,const EuroplexusInt  *const NSTATV,
	      EuroplexusReal *const STRESS,      EuroplexusReal *const PNEWDT,
	      const tfel::material::OutOfBoundsPolicy op,
	      const StressFreeExpansionHandler& sfeh)
      {
	using BV = Behaviour<H,EuroplexusReal,false>;
	using MTraits  = tfel::material::MechanicalBehaviourTraits<BV>;
	const bool is_defined_ = MTraits::is_defined;
	using Handler = typename std::conditional<is_defined_,CallBehaviour2<H>,
						  UnsupportedHypothesisHandler<H>>::type;
	try{
	  Handler::exe(NTENS,DTIME,F0,F1,
		       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
		       STATEV,NSTATV,STRESS,PNEWDT,op,sfeh);
	}
	catch(const EuroplexusException& e){
	  EuroplexusInterfaceExceptions::treatEuroplexusException(MTraits::getName(),e);
	  return -2;
	}
	catch(const tfel::material::OutOfBoundsException& e){
	  EuroplexusInterfaceExceptions::treatMaterialException(MTraits::getName(),e);
	  return -3;
	}
	catch(const tfel::material::DivergenceException& e){
	  EuroplexusInterfaceExceptions::treatMaterialException(MTraits::getName(),e);	  return -4;
	}
	catch(const tfel::material::MaterialException& e){
	  EuroplexusInterfaceExceptions::treatMaterialException(MTraits::getName(),e);
	  return -5;
	}
	catch(const tfel::exception::TFELException& e){
	  EuroplexusInterfaceExceptions::treatTFELException(MTraits::getName(),e);
	  return -6;
	}
	catch(const std::exception& e){
	  EuroplexusInterfaceExceptions::treatStandardException(MTraits::getName(),e);
	  return -7;
	}
	catch(...){
	  EuroplexusInterfaceExceptions::treatUnknownException(MTraits::getName());
	  return -8;
	}
	return 0;
      } // end of CallBehaviour::exe
    };
    
    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct CallBehaviour2
    {
      TFEL_EPX_INLINE2 static void
      exe(const EuroplexusInt  *const, const EuroplexusReal *const DTIME,
	  const EuroplexusReal *const F0, const EuroplexusReal *const F1,
	  const EuroplexusReal *const TEMP,  const EuroplexusReal *const DTEMP,
	  const EuroplexusReal *const PROPS, const EuroplexusInt  *const NPROPS,
	  const EuroplexusReal *const PREDEF,const EuroplexusReal *const DPRED,
	  EuroplexusReal *const STATEV,const EuroplexusInt  *const NSTATV,
	  EuroplexusReal *const STRESS,EuroplexusReal *const PNEWDT,
	  const tfel::material::OutOfBoundsPolicy op,
	  const StressFreeExpansionHandler& sfeh)
      {
	typedef EuroplexusBehaviourHandler<H,Behaviour> AHandler;
	using BV = Behaviour<H,EuroplexusReal,false>;
	using ATraits =  EuroplexusTraits<BV>;
	const bool bs = ATraits::requiresStiffnessTensor;
	const bool ba = ATraits::requiresThermalExpansionCoefficientTensor;
	using Integrator = typename AHandler::template Integrator<bs,ba>;
	AHandler::checkNPROPS(*NPROPS);
	AHandler::checkNSTATV(*NSTATV);
	Integrator i(DTIME,F0,F1,
		     TEMP,DTEMP,PROPS,
		     PREDEF,DPRED,STATEV,
		     STRESS,op,sfeh);
	i.exe(PNEWDT,STRESS,STATEV);
      }
    }; // end of struct CallBehaviour
  }; // end of struct EuroplexusInterface
    
} // end of namespace epx

#endif /* LIB_MFRONT_EPX_CALL_H_ */

