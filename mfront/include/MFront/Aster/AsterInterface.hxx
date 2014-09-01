/*!
 * \file   mfront/include/MFront/Aster/AsterInterface.hxx
 * \brief  This file implements the AsterInterface class.
 * \author Helfer Thomas
 * \date   28 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_ASTER_CALL_H_
#define _LIB_MFRONT_ASTER_CALL_H_ 

#include<string>
#include<algorithm>
#include<vector>
#include<limits>

#include"TFEL/Config/TFELTypes.hxx"

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Exception/TFELException.hxx"
#include"TFEL/FSAlgorithm/copy.hxx"

#include"TFEL/Utilities/Info.hxx"
#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Math/stensor.hxx"

#include"TFEL/Material/MechanicalBehaviourTraits.hxx"
#include"TFEL/Material/MaterialException.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterConfig.hxx"
#include"MFront/Aster/AsterTraits.hxx"
#include"MFront/Aster/AsterException.hxx"
#include"MFront/Aster/AsterInterfaceBase.hxx"

namespace aster
{

  /*!
   * forward declaration
   */
  template<AsterBehaviourType,
	   tfel::material::ModellingHypothesis::Hypothesis,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class>
  struct AsterIsotropicBehaviourHandler;

  /*!
   * forward declaration
   */
  template<AsterBehaviourType,
	   tfel::material::ModellingHypothesis::Hypothesis,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class>
  struct AsterOrthotropicBehaviourHandler;

  /*!
   * \class  AsterInterface
   * \brief This class create an interface between a behaviour class
   * and the aster finite element code
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
  struct TFEL_VISIBILITY_LOCAL AsterInterface
    : protected AsterInterfaceBase
  {

    TFEL_ASTER_INLINE2 static
     int exe(const AsterInt  *const NTENS, const AsterReal *const DTIME,
	     const AsterReal *const DROT,  AsterReal *const DDSOE,
	     const AsterReal *const STRAN, const AsterReal *const DSTRAN,
	     const AsterReal *const TEMP,  const AsterReal *const DTEMP,
	     const AsterReal *const PROPS, const AsterInt  *const NPROPS,
	     const AsterReal *const PREDEF,const AsterReal *const DPRED,
	     AsterReal *const STATEV,const AsterInt  *const NSTATV,
	     AsterReal *const STRESS, const AsterInt  *const NUMMOD,
	     const StressFreeExpansionHandler& sfeh)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef ModellingHypothesis ModellingHypothesis;
      if( *NUMMOD == 2u ){
	/*!
	 * L'hypothèse de déformations planes généralisées n'est pas
	 * supportée dans Aster (version 12.x) Cette hypothèse doit
	 * être appelée quand les contraintes planes sont traitées par
	 * l'algorithme de De Borst.
	 */
        typedef Behaviour<ModellingHypothesis::GENERALISEDPLANESTRAIN,AsterReal,false> BV;
        typedef MechanicalBehaviourTraits<BV> MTraits;
        typedef AsterTraits<BV> Traits;
        const bool is_defined_ = MTraits::is_defined;
        typedef typename IF<is_defined_,
                            CallBehaviour<ModellingHypothesis::GENERALISEDPLANESTRAIN>,
                            Behaviour2DWrapper<Traits::btype,ModellingHypothesis::GENERALISEDPLANESTRAIN> >::type Handler;
        return Handler::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
                            TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
                            STATEV,NSTATV,STRESS,sfeh);
      } else if( *NUMMOD == 4u ){
        typedef Behaviour<ModellingHypothesis::AXISYMMETRICAL,AsterReal,false> BV;
        typedef MechanicalBehaviourTraits<BV> MTraits;
        typedef AsterTraits<BV> Traits;
        const bool is_defined_ = MTraits::is_defined;
        typedef typename IF<is_defined_,
                            CallBehaviour<ModellingHypothesis::AXISYMMETRICAL>,
                            Behaviour2DWrapper<Traits::btype,ModellingHypothesis::AXISYMMETRICAL> >::type Handler;
        return Handler::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
                            TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
                            STATEV,NSTATV,STRESS,sfeh);
      } else if( *NUMMOD == 5u ){
        typedef Behaviour<ModellingHypothesis::PLANESTRESS,AsterReal,false> BV;
        typedef MechanicalBehaviourTraits<BV> MTraits;
        typedef AsterTraits<BV> Traits;
        const bool is_defined_ = MTraits::is_defined;
        typedef typename IF<is_defined_,
                            CallBehaviour<ModellingHypothesis::PLANESTRESS>,
                            Behaviour2DWrapper<Traits::btype,ModellingHypothesis::PLANESTRESS> >::type Handler;
        return Handler::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
                            TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
                            STATEV,NSTATV,STRESS,sfeh);
      } else if( *NUMMOD == 6u ){
        typedef Behaviour<ModellingHypothesis::PLANESTRAIN,AsterReal,false> BV;
        typedef MechanicalBehaviourTraits<BV> MTraits;
        typedef AsterTraits<BV> Traits;
        const bool is_defined_ = MTraits::is_defined;
        typedef typename IF<is_defined_,
                            CallBehaviour<ModellingHypothesis::PLANESTRAIN>,
                            Behaviour2DWrapper<Traits::btype,ModellingHypothesis::PLANESTRAIN> >::type Handler;
        return Handler::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
                            TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
                            STATEV,NSTATV,STRESS,sfeh);
      } else if( *NUMMOD == 3u ){
        return CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,
								       TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
								       STATEV,NSTATV,STRESS,sfeh);
      } else {
        AsterInterfaceBase::displayUnsupportedHypothesisMessage();
        return -2;
      }
    }

  private:

    template<AsterBehaviourType,
	     tfel::material::ModellingHypothesis::Hypothesis>
    struct Behaviour2DWrapper;

    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct Behaviour2DWrapper<aster::SMALLSTRAINSTANDARDBEHAVIOUR,H>
    {
      TFEL_ASTER_INLINE2 static
      int exe(const AsterInt  *const, const AsterReal *const DTIME,
	      const AsterReal *const DROT,  AsterReal *const DDSOE,
	      const AsterReal *const STRAN, const AsterReal *const DSTRAN,
	      const AsterReal *const TEMP,  const AsterReal *const DTEMP,
	      const AsterReal *const PROPS, const AsterInt  *const NPROPS,
	      const AsterReal *const PREDEF,const AsterReal *const DPRED,
	      AsterReal *const STATEV,const AsterInt  *const NSTATV,
	      AsterReal *const STRESS,
	      const StressFreeExpansionHandler& sfeh)
      {
	using namespace tfel::meta;
	using namespace tfel::math;
	using namespace tfel::material;
	using tfel::fsalgo::copy;
	const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
	AsterInt NTENS   = 6u;
	AsterReal s[6u]  = {0.,0.,0.,0.,0.,0.};
	AsterReal e[6u]  = {0.,0.,0.,0.,0.,0.};
	AsterReal de[6u] = {0.,0.,0.,0.,0.,0.};
	AsterReal K[36u] = {0.,0.,0.,0.,0.,0.,
			    0.,0.,0.,0.,0.,0.,
			    0.,0.,0.,0.,0.,0.,
			    0.,0.,0.,0.,0.,0.,
			    0.,0.,0.,0.,0.,0.,
			    0.,0.,0.,0.,0.,0.};
	K[0u] = DDSOE[0];
	copy<StensorDimeToSize<N>::value>::exe(STRESS,s);
	copy<StensorDimeToSize<N>::value>::exe(STRAN,e);
	copy<StensorDimeToSize<N>::value>::exe(DSTRAN,de);
	int r = CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(&NTENS,DTIME,DROT,K,e,de,
									TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
									STATEV,NSTATV,s,sfeh);
	if(r==0){
	  AsterReduceTangentOperator<N>::exe(DDSOE,K);
	  copy<StensorDimeToSize<N>::value>::exe(s,STRESS);
	}
	return r;
      }
    };

    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct Behaviour2DWrapper<aster::FINITESTRAINSTANDARDBEHAVIOUR,H>
    {
      TFEL_ASTER_INLINE2 static
      int exe(const AsterInt  *const, const AsterReal *const DTIME,
	      const AsterReal *const DROT,  AsterReal *const DDSOE,
	      const AsterReal *const STRAN, const AsterReal *const DSTRAN,
	      const AsterReal *const TEMP,  const AsterReal *const DTEMP,
	      const AsterReal *const PROPS, const AsterInt  *const NPROPS,
	      const AsterReal *const PREDEF,const AsterReal *const DPRED,
	      AsterReal *const STATEV,const AsterInt  *const NSTATV,
	      AsterReal *const STRESS,
	      const StressFreeExpansionHandler& sfeh)
      {
	using namespace std;
	using namespace tfel::meta;
	using namespace tfel::math;
	using namespace tfel::material;
	using tfel::fsalgo::copy;
	const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
	AsterInt NTENS   = 6u;
	AsterReal s[6u]  = {0.,0.,0.,0.,0.,0.};
	AsterReal F0[9u] = {0.,0.,0.,0.,0.,0.,0.,0.,0.};
	AsterReal F1[9u] = {0.,0.,0.,0.,0.,0.,0.,0.,0.};
	copy<StensorDimeToSize<N>::value>::exe(STRESS,s);
	copy<9u>::exe(STRAN,F0);
	copy<9u>::exe(DSTRAN,F1);
	int r = CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(&NTENS,DTIME,DROT,DDSOE,F0,F1,
									TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
									STATEV,NSTATV,s,sfeh);
	if(r==0){
	  copy<StensorDimeToSize<N>::value>::exe(s,STRESS);
	}
	return r;
      }
    };

    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct Behaviour2DWrapper<aster::COHESIVEZONEMODEL,H>
    {
      TFEL_ASTER_INLINE2 static
      int exe(const AsterInt  *const,       const AsterReal *const DTIME,
	      const AsterReal *const DROT,  AsterReal *const DDSOE,
	      const AsterReal *const STRAN, const AsterReal *const DSTRAN,
	      const AsterReal *const TEMP,  const AsterReal *const DTEMP,
	      const AsterReal *const PROPS, const AsterInt  *const NPROPS,
	      const AsterReal *const PREDEF,const AsterReal *const DPRED,
	      AsterReal *const STATEV,const AsterInt  *const NSTATV,
	      AsterReal *const STRESS,
	      const StressFreeExpansionHandler& sfeh)
      {
	using tfel::material::ModellingHypothesis;
	using tfel::fsalgo::copy;
	AsterInt NTENS   = 6u;
	AsterReal s[3u]  = {0.,0.,0.};
	AsterReal u0[3u] = {0.,0.,0.};
	AsterReal du[3u] = {0.,0.,0.};
	AsterReal K[9u]  = {0.,0.,0.,
			    0.,0.,0.,
			    0.,0.,0.};
	K[0u] = DDSOE[0];
	copy<2u>::exe(STRESS,s);
	copy<2u>::exe(STRAN,u0);
	copy<2u>::exe(DSTRAN,du);
	int r = CallBehaviour<ModellingHypothesis::TRIDIMENSIONAL>::exe(&NTENS,DTIME,DROT,DDSOE,u0,du,
									TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,
									STATEV,NSTATV,s,sfeh);
	if(r==0){
	  copy<2u>::exe(s,STRESS);
	  DDSOE[0]=K[0];
	  DDSOE[1]=K[1];
	  DDSOE[2]=K[4];
	  DDSOE[3]=K[5];
	}
	return r;
      }
    }; // end of Behaviour2DWrapper<aster::COHESIVEZONEMODEL,N>

    template<tfel::material::ModellingHypothesis::Hypothesis H>
    struct CallBehaviour
    {
      TFEL_ASTER_INLINE2 static
      int exe(const AsterInt  *const, const AsterReal *const DTIME,
	      const AsterReal *const DROT,  AsterReal *const DDSOE,
	      const AsterReal *const STRAN, const AsterReal *const DSTRAN,
	      const AsterReal *const TEMP,  const AsterReal *const DTEMP,
	      const AsterReal *const PROPS, const AsterInt  *const NPROPS,
	      const AsterReal *const PREDEF,const AsterReal *const DPRED,
	      AsterReal *const STATEV,const AsterInt  *const NSTATV,
	      AsterReal *const STRESS,
	      const StressFreeExpansionHandler& sfeh)
      {
	using namespace std;
	using namespace tfel::meta;
	using namespace tfel::utilities;
	typedef Behaviour<H,AsterReal,false> BV;
	typedef AsterTraits<BV> Traits;
	try {
	  typedef typename IF<Traits::type==aster::ISOTROPIC,
	    AsterIsotropicBehaviourHandler<Traits::btype,H,Behaviour>,
	    AsterOrthotropicBehaviourHandler<Traits::btype,H,Behaviour> >::type Handler;
	  Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		       PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		       STRESS,sfeh);
	} 
	catch(const AsterException& e){
	  if(Traits::errorReportPolicy!=ASTER_NOERRORREPORT){
	    AsterInterfaceBase::treatAsterException(Name<BV>::getName(),e);
	  }
	  return -2;
	}
	catch(const tfel::material::MaterialException& e){
	  if(Traits::errorReportPolicy!=ASTER_NOERRORREPORT){
	    AsterInterfaceBase::treatMaterialException(Name<BV>::getName(),e);
	  }
	  return -3;
	}
	catch(const tfel::exception::TFELException& e){
	  if(Traits::errorReportPolicy!=ASTER_NOERRORREPORT){
	    AsterInterfaceBase::treatTFELException(Name<BV>::getName(),e);
	  }
	  return -4;
	}
	catch(const std::exception& e){
	  if(Traits::errorReportPolicy!=ASTER_NOERRORREPORT){
	    AsterInterfaceBase::treatStandardException(Name<BV>::getName(),e);
	  }
	  return -5;
	}
	catch(...){
	  if(Traits::errorReportPolicy!=ASTER_NOERRORREPORT){
	    AsterInterfaceBase::treatUnknownException(Name<BV>::getName());
	  }
	  return -6;
	}
	return 0;
      } // end of CallBehaviour::exe
    }; // end of struct CallBehaviour
  }; // end of struct AsterInterface
  
} // end of namespace aster

#include"MFront/Aster/AsterBehaviourHandler.hxx"

#endif /* _LIB_MFRONT_ASTER_CALL_H */

