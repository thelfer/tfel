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
   * \date   11/03/2016
   */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL EuroplexusInterface
    : protected EuroplexusInterfaceExceptions
  {

    TFEL_EPX_INLINE2 static void
    exe(const EPXData& d,
	const EuroplexusInt h)
    {
      using tfel::material::ModellingHypothesis;
      if(h==3){
	CallBehaviour<ModellingHypothesis::AXISYMMETRICAL>::exe(d);
      } else if(h==2){
	CallBehaviour<ModellingHypothesis::PLANESTRESS>::exe(d);
      } else if(h==1){
	CallBehaviour<ModellingHypothesis::PLANESTRAIN>::exe(d);
      } else if(h==0){
	CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(d);
      } else {
        EuroplexusInterfaceExceptions::displayUnsupportedHypothesisMessage();
	*(d.STATUS)=-2;
      }
    }

  private:

    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct UnsupportedHypothesisHandler
    {
      TFEL_EPX_INLINE2 static void
      exe(const EPXData&)
      {
	using BV = Behaviour<H,EuroplexusReal,false>;
	using MTraits = tfel::material::MechanicalBehaviourTraits<BV>;
	throw(EuroplexusInvalidModellingHypothesis(MTraits::getName()));
      }
    }; // end of struct UnsupportedHypothesisHandler
  
    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct CallBehaviour
    {
      TFEL_EPX_INLINE2 static void
      exe(const EPXData& d)
      {
	using BV = Behaviour<H,EuroplexusReal,false>;
	using MTraits  = tfel::material::MechanicalBehaviourTraits<BV>;
	const bool is_defined_ = MTraits::is_defined;
	using Handler = typename std::conditional<is_defined_,CallBehaviour2<H>,
						  UnsupportedHypothesisHandler<H>>::type;
	try{
	  Handler::exe(d);
	}
	catch(const EuroplexusException& e){
	  EuroplexusInterfaceExceptions::treatEuroplexusException(MTraits::getName(),e);
	  *(d.STATUS)= -2;
	}
	catch(const tfel::material::OutOfBoundsException& e){
	  EuroplexusInterfaceExceptions::treatMaterialException(MTraits::getName(),e);
	  *(d.STATUS)= -3;
	}
	catch(const tfel::material::DivergenceException& e){
	  EuroplexusInterfaceExceptions::treatMaterialException(MTraits::getName(),e);
	  *(d.STATUS)= -4;
	}
	catch(const tfel::material::MaterialException& e){
	  EuroplexusInterfaceExceptions::treatMaterialException(MTraits::getName(),e);
	  *(d.STATUS)= -5;
	}
	catch(const tfel::exception::TFELException& e){
	  EuroplexusInterfaceExceptions::treatTFELException(MTraits::getName(),e);
	  *(d.STATUS)= -6;
	}
	catch(const std::exception& e){
	  EuroplexusInterfaceExceptions::treatStandardException(MTraits::getName(),e);
	  *(d.STATUS)= -7;
	}
	catch(...){
	  EuroplexusInterfaceExceptions::treatUnknownException(MTraits::getName());
	  *(d.STATUS)= -8;
	}
      } // end of CallBehaviour::exe
    };
    
    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct CallBehaviour2
    {
      TFEL_EPX_INLINE2 static void
      exe(const EPXData& d)
      {
	typedef EuroplexusBehaviourHandler<H,Behaviour> AHandler;
	using BV = Behaviour<H,EuroplexusReal,false>;
	const bool bs = EuroplexusTraits<BV>::requiresStiffnessTensor;
	const bool ba = EuroplexusTraits<BV>::requiresThermalExpansionCoefficientTensor;
	using Integrator = typename AHandler::template Integrator<bs,ba>;
	AHandler::checkNPROPS(d.NPROPS);
	AHandler::checkNSTATV(d.NSTATV);
	AHandler::checkNPREDEF(d.NPREDEF);
	Integrator i(d);
	i.exe(d);
      }
    }; // end of struct CallBehaviour
  }; // end of struct EuroplexusInterface
    
} // end of namespace epx

#endif /* LIB_MFRONT_EPX_CALL_H_ */

