/*!
 * \file   UMATInterface.hxx
 * \brief  This file implements the UMATInterface class.
 * \author Helfer Thomas
 * \date   28 Jul 2006
 */

#ifndef _LIB_MFRONT_UMAT_CALL_H_
#define _LIB_MFRONT_UMAT_CALL_H_ 

#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<limits>

#include"Config/TFELConfig.hxx"
#include"Config/TFELTypes.hxx"

#include"Metaprogramming/IF.hxx"

#include"Exception/TFELException.hxx"

#include"Utilities/Info.hxx"
#include"Utilities/Name.hxx"
#include"Utilities/ToString.hxx"

#include"Material/MechanicalBehaviourTraits.hxx"
#include"Material/MechanicalBehaviourData.hxx"
#include"Material/MaterialException.hxx"

#include"UMAT/UMAT.hxx"
#include"UMAT/UMATTraits.hxx"
#include"UMAT/UMATException.hxx"
#include"UMAT/UMATOutOfBoundsPolicy.hxx"
#include"UMAT/UMATOrthotropicBehaviour.hxx"
#include"UMAT/UMATComputeStiffnessTensor.hxx"
#include"UMAT/UMATComputeThermalExpansionTensor.hxx"
#include"UMAT/UMATRotationMatrix.hxx"

namespace umat{

  /*!
   * \class  UMATInterface
   * \brief  This class create an interface between Behaviour and UMAT
   * \author Helfer Thomas
   * \date   28 Jul 2006
   */
  template<template<unsigned short,typename,bool> class Behaviour>
  struct UMATInterface
  {
    
    TFEL_UMAT_INLINE2 static
    void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	     const UMATReal *const DROT,  const UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS,const UMATInt  *const,
	     UMATInt  *const KINC)
    {
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::utilities;
      try{
	if(*NTENS==3){
	  typedef UMATTraits<Behaviour<1u,UMATReal,false> > Traits;
	  typedef typename IF<Traits::type==umat::ISOTROPIC,
	    TreatIsotropicBehaviour<1u>,
	    TreatOrthotropicBehaviour1D>::type Treat;
	  Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		     PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,STRESS);
	} else if(*NTENS==4){
	  typedef UMATTraits<Behaviour<2u,UMATReal,false> > Traits;
	  typedef typename IF<Traits::type==umat::ISOTROPIC,
	    TreatIsotropicBehaviour<2u>,
	    TreatOrthotropicBehaviour2D>::type Treat;
	  Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		     PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,STRESS);
	} else if(*NTENS==6){
	  typedef UMATTraits<Behaviour<3u,UMATReal,false> > Traits;
	  typedef typename IF<Traits::type==umat::ISOTROPIC,
	    TreatIsotropicBehaviour<3u>,
	    TreatOrthotropicBehaviour3D>::type Treat;
	  Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		     PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,STRESS);
	} else{
	  throw(UMATInvalidNTENSValue(*NTENS));
	}
      }
      catch(const UMATException& e){
	cout << "The ";
	if(*NTENS==3){
	  cout << Name<Behaviour<1u,UMATReal,false> >::getName();
	} else if(*NTENS==4){
	  cout << Name<Behaviour<2u,UMATReal,false> >::getName();
	} else if(*NTENS==6){
	  cout << Name<Behaviour<3u,UMATReal,false> >::getName();
	}
	cout << " Behaviour has caused an " ;
	cout << "UMAT exception : " << e.getMsg() << endl;
	*KINC = -2;
      }
      catch(const tfel::material::MaterialException& e){
	cout << "The ";
	if(*NTENS==3){
	  cout << Name<Behaviour<1u,UMATReal,false> >::getName();
	} else if(*NTENS==4){
	  cout << Name<Behaviour<2u,UMATReal,false> >::getName();
	} else if(*NTENS==6){
	  cout << Name<Behaviour<3u,UMATReal,false> >::getName();
	}
	cout << " Behaviour has caused a " ;
	cout << "Material exception : " << e.getMsg() << endl;
	*KINC = -3;
      }
      catch(const tfel::exception::TFELException& e){
	cout << "The ";
	if(*NTENS==3){
	  cout << Name<Behaviour<1u,UMATReal,false> >::getName();
	} else if(*NTENS==4){
	  cout << Name<Behaviour<2u,UMATReal,false> >::getName();
	} else if(*NTENS==6){
	  cout << Name<Behaviour<3u,UMATReal,false> >::getName();
	}
	cout << " Behaviour has caused a " ;
	*KINC = -4;
      }
    
    } // end of UMATInterface::exe

  private:

    template<unsigned short N>
    struct TreatBehaviour
    {

      struct StiffnessTensorInitializer
      {
	typedef Behaviour<N,UMATReal,false> BV;
	typedef typename BV::BehaviourData  BData;
	TFEL_UMAT_INLINE static void
	exe(BData& data,const UMATReal * const props){
	  UMATComputeStiffnessTensor<N,UMATTraits<BV>::type>::exe(props,
								  data.getStiffnessTensor());
	} // end of exe
      }; // end of struct StiffnessTensorInitializer

      struct ThermalExpansionTensorInitializer
      {
	typedef Behaviour<N,UMATReal,false> BV;
	typedef typename BV::BehaviourData  BData;
	TFEL_UMAT_INLINE static void
	exe(BData& data,const UMATReal * const props){
	  UMATComputeThermalExpansionTensor<N,UMATTraits<BV>::type>::exe(props,
									 data.getThermalExpansionTensor());
	} // end of exe
      }; // end of struct ThermalExpansionTensorInitializer

      struct DoNothingInitializer
      {
	typedef Behaviour<N,UMATReal,false> BV;
	typedef typename BV::BehaviourData  BData;
	TFEL_UMAT_INLINE static void
	exe(BData&,const UMATReal * const)
	{}
      }; // end of struct DoNothingInitializer

      struct Error
      {
	
	TFEL_UMAT_INLINE Error(const UMATReal *const,
			       const UMATReal *const,
			       const UMATReal *const,
			       const UMATReal *const,
			       const UMATReal *const,
			       const UMATReal *const,
			       const UMATReal *const,
			       const UMATReal *const,
			       const UMATReal *const,
			       const UMATReal *const,
			       const UMATReal *const,
			       const UMATReal *const)
	{} // end of Error
	
	TFEL_UMAT_INLINE void exe(UMATReal *const,
				  UMATReal *const)
	  throw(UMATException)
	{
	  using namespace std;
	  using namespace tfel::utilities;
	  throw(UMATInvalidDimension(Name<Behaviour<N,UMATReal,false> >::getName()));
	  return;
	} // end of Error::exe
	
      }; // end of struct Error
      
      template<const bool bs,     // requires StiffnessTensor
	       const bool ba>     // requires ThermalExpansionTensor
      struct IntegratorWithTimeStepping
      {
	typedef typename tfel::meta::IF<bs,
					StiffnessTensorInitializer,
					DoNothingInitializer>::type SInitializer;

	typedef typename tfel::meta::IF<ba,
					ThermalExpansionTensorInitializer,
					DoNothingInitializer>::type AInitializer;

	TFEL_UMAT_INLINE IntegratorWithTimeStepping(const UMATReal *const DTIME ,
						    const UMATReal *const       ,
						    const UMATReal *const STRAN ,
						    const UMATReal *const DSTRAN,
						    const UMATReal *const TEMP  ,
						    const UMATReal *const DTEMP,
						    const UMATReal *const PROPS ,
						    const UMATReal *const PREDEF,
						    const UMATReal *const DPRED,
						    UMATReal *const STATEV,
						    UMATReal *const STRESS)
	  : bData(STRESS,STRAN,TEMP,PROPS+UMATTraits<BV>::propertiesOffset,
		  STATEV,PREDEF),
	    iData(DTIME,DSTRAN,DTEMP,DPRED),
	    dt(*DTIME)
	{
	  SInitializer::exe(this->bData,PROPS);
	  AInitializer::exe(this->bData,PROPS);
	} // end of IntegratorWithTimeStepping

	TFEL_UMAT_INLINE2 void exe(UMATReal *const STRESS,
				   UMATReal *const STATEV)
	{
	  const UMATOutOfBoundsPolicy& up = UMATOutOfBoundsPolicy::getUMATOutOfBoundsPolicy();
	  UMATReal dtMin = (this->dt)/(1 << UMATTraits<BV>::maximumSubStepping);
	  UMATReal tCurrent = 0.;
	  UMATReal dt_ = this->dt;
	  bool convergence;
	  if(dt_<0.){
	    throw(UMATException("negative time step"));
	  }
	  if(dt_<100.*std::numeric_limits<UMATReal>::min()){
	    throw(UMATException("time step too small"));
	  }
	  if(dtMin<100.*std::numeric_limits<UMATReal>::min()){
	    throw(UMATException("Minimum time step too small"));
	  }
	  while((tCurrent<dt_-dtMin*0.25)&&(dt_>dtMin)){
	    convergence = true;
	    BV behaviour(this->bData,this->iData);
	    behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	    behaviour.checkBounds();
	    try{
	      behaviour.integrate();
	    }
	    catch(const tfel::material::DivergenceException& e){
#ifdef MFRONT_UMAT_VERBOSE
	      std::cerr << "no convergence : " << e.what() << std::endl;
#endif
	      convergence = false;
	    }
	    if(convergence){
	      tCurrent += dt_;
	      behaviour.updateExternalStateVariables();
	      this->bData = static_cast<const BData&>(behaviour);
	    } else {
	      dt_ *= 0.5;
	      this->iData *= 0.5;
	    }
	  }
#ifdef MFRONT_UMAT_VERBOSE
	  std::cerr << "convergence : " 
		    << static_cast<unsigned long>((this->dt)/dt_)
		    << std::endl;
#endif

	  if(dt_<dtMin){
	    throw(UMATException("Maximum number of substepping reached"));
	  }
	  this->bData.UMATexportStateData(STRESS,STATEV);
	} // end of IntegratorWithTimeStepping::exe

      private:
	typedef Behaviour<N,UMATReal,false> BV;
	typedef typename BV::BehaviourData  BData;
	typedef typename BV::IntegrationData  IData;

	BData bData;
	IData iData;
	UMATReal dt;

      }; // end of struct IntegratorWithTimeStepping

      template<const bool bs,     // requires StiffnessTensor
	       const bool ba>     // requires ThermalExpansionTensor
      struct Integrator
      {
	typedef typename tfel::meta::IF<bs,
					StiffnessTensorInitializer,
					DoNothingInitializer>::type SInitializer;

	typedef typename tfel::meta::IF<ba,
					ThermalExpansionTensorInitializer,
					DoNothingInitializer>::type AInitializer;

	TFEL_UMAT_INLINE Integrator(const UMATReal *const DTIME ,
				    const UMATReal *const,
				    const UMATReal *const STRAN ,
				    const UMATReal *const DSTRAN,
				    const UMATReal *const TEMP,
				    const UMATReal *const DTEMP,
				    const UMATReal *const PROPS ,
				    const UMATReal *const PREDEF,
				    const UMATReal *const DPRED,
				    const UMATReal *const STATEV,
				    const UMATReal *const STRESS)
	  : behaviour(DTIME,STRESS,STRAN,DSTRAN,TEMP,DTEMP,
		      PROPS+UMATTraits<BV>::propertiesOffset,
		      STATEV,PREDEF,DPRED),
	    dt(*DTIME)
	{
	  SInitializer::exe(this->behaviour,PROPS);
	  AInitializer::exe(this->behaviour,PROPS);
	  const UMATOutOfBoundsPolicy& up = UMATOutOfBoundsPolicy::getUMATOutOfBoundsPolicy();
	  this->behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	  this->behaviour.checkBounds();
	} // end of Integrator::Integrator

	TFEL_UMAT_INLINE2
	void exe(UMATReal *const STRESS,
		 UMATReal *const STATEV)
	{
	  if(this->dt<0.){
	    throw(UMATException("negative time step"));
	  }
	  if(this->dt<100.*std::numeric_limits<UMATReal>::min()){
	    throw(UMATException("time step too small"));
	  }
	  this->behaviour.integrate();
	  this->behaviour.UMATexportStateData(STRESS,STATEV);
	} // end of Integrator::exe

      private:
	typedef Behaviour<N,UMATReal,false> BV;
	BV behaviour;
	UMATReal dt;
      }; // end of struct Integrator

      TFEL_UMAT_INLINE2 static void
      checkNPROPS(const UMATInt NPROPS)
	throw(UMATException)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::material;
	typedef Behaviour<N,UMATReal,false> BV;
	typedef MechanicalBehaviourTraits<BV> Traits;
	static const unsigned short offset  = UMATTraits<BV>::propertiesOffset;
	static const unsigned short nprops  = Traits::material_properties_nb;
	static const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
	static const bool is_defined_       = Traits::is_defined;
	//Test if the nb of properties matches Behaviour requirements
	if((NPROPS!=NPROPS_)&&is_defined_){
	  string msg = Name<Behaviour<N,UMATReal,false> >::getName();
	  msg += " : number of material properties does not match (";
	  msg += tfel::utilities::ToString(NPROPS_);
	  msg += " required and ";
	  msg += tfel::utilities::ToString(NPROPS);
	  msg += " given)";
	  throw(UMATException(msg));
	}
      } // end of checkNPROPS
      
      TFEL_UMAT_INLINE2 static void
      checkNSTATV(const UMATInt NSTATV)
	throw(UMATException)
      {
	typedef Behaviour<N,UMATReal,false> BV;
	typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	static const unsigned short nstatv  = Traits::internal_variables_nb;
	static const unsigned short NSTATV_ = nstatv == 0 ? 1u : nstatv;
	static const bool is_defined_       = Traits::is_defined;
	//Test if the nb of state variables matches Behaviour requirements
	if((NSTATV_!=NSTATV)&&is_defined_){
	  std::string msg = tfel::utilities::Name<Behaviour<N,UMATReal,false> >::getName();
	  msg += " : nb of internal state variables does not match (";
	  msg += tfel::utilities::ToString(NSTATV_);
	  msg += " required and ";
	  msg += tfel::utilities::ToString(NSTATV);
	  msg += " given)";
	  throw(UMATException(msg));
	}	  
      } // end of checkNSTATV

    }; // end of struct TreatBehaviour

    struct TreatOrthotropicBehaviour1D
      : private TreatBehaviour<1u>
    {
      TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME,
	       const UMATReal *const,
	       const UMATReal *const DDSOE,
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
	       UMATReal *const STRESS) 
      {
	using namespace tfel::meta;
	using namespace tfel::material;
	using namespace tfel::math;
	typedef MechanicalBehaviourTraits<Behaviour<1u,UMATReal,false> > MTraits;
	typedef UMATTraits<Behaviour<1u,UMATReal,false> > Traits;
	typedef TreatBehaviour<1u> TreatBehaviour;
	static const bool is_defined_ = MTraits::is_defined;
	static const bool bs = Traits::requiresStiffnessTensor;
	static const bool ba = Traits::requiresThermalExpansionTensor;
	typedef typename IF<
	  is_defined_,
	  typename IF<
	  Traits::useTimeSubStepping,
	  typename TreatBehaviour::template IntegratorWithTimeStepping<bs,ba>,
	  typename TreatBehaviour::template Integrator<bs,ba>
	  >::type,
	  typename TreatBehaviour::Error>::type Handler;
	TreatBehaviour::checkNPROPS(*NPROPS);
	TreatBehaviour::checkNSTATV(*NSTATV);
	Handler handler(DTIME,DDSOE,STRAN,
			DSTRAN,TEMP,DTEMP,PROPS,
			PREDEF,DPRED,STATEV,STRESS);
	handler.exe(STRESS,STATEV);
      } // end of TreatOrthotropicBehaviour1D::exe
    }; // end of struct TreatOrthotropicBehaviour1D

    struct TreatOrthotropicBehaviour2D
      : private TreatBehaviour<2u>
    {
      TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME ,
	       const UMATReal *const DROT  ,
	       const UMATReal *const DDSOE,
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
	       UMATReal *const STRESS) 
      {
	using namespace tfel::meta;
	using namespace tfel::material;
	using namespace tfel::math;
	typedef MechanicalBehaviourTraits<Behaviour<2u,UMATReal,false> > MTraits;
	typedef UMATTraits<Behaviour<2u,UMATReal,false> > Traits;
	typedef TreatBehaviour<2u> TreatBehaviour;
	static const bool is_defined_ = MTraits::is_defined;
	static const bool bs = Traits::requiresStiffnessTensor;
	static const bool ba = Traits::requiresThermalExpansionTensor;
	typedef typename IF<
	  is_defined_,
	  typename IF<
	  Traits::useTimeSubStepping,
	  typename TreatBehaviour::template IntegratorWithTimeStepping<bs,ba>,
	  typename TreatBehaviour::template Integrator<bs,ba>
	  >::type,
	  typename TreatBehaviour::Error>::type Handler;
	UMATReal s[4];
	UMATReal e[4];
	UMATReal de[4];
	UMATRotationMatrix2D m(PROPS+7,DROT);
	m.rotateStressesForward(STRESS,s);
	m.rotateStrainsForward(STRAN,e);
	m.rotateStrainsForward(DSTRAN,de);
	TreatBehaviour::checkNPROPS(*NPROPS);
	TreatBehaviour::checkNSTATV(*NSTATV);
	Handler handler(DTIME,DDSOE,
			e,de,TEMP,DTEMP,PROPS,
			PREDEF,DPRED,STATEV,s);
	handler.exe(STRESS,STATEV);
	m.rotateStressesBackward(s,STRESS);
      } // end of TreatOrthotropicBehaviour2D::exe
    }; // end of TreatOrthotropicBehaviour2D

    struct TreatOrthotropicBehaviour3D
      : private TreatBehaviour<3u>
    {
      TFEL_UMAT_INLINE2 static 
      void exe(const UMATReal *const DTIME,
	       const UMATReal *const DROT,
	       const UMATReal *const DDSOE,
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
	       UMATReal *const STRESS) 
      {
	using namespace tfel::meta;
	using namespace tfel::material;
	using namespace tfel::math;
	typedef MechanicalBehaviourTraits<Behaviour<3u,UMATReal,false> > MTraits;
	typedef UMATTraits<Behaviour<3u,UMATReal,false> > Traits;
	typedef TreatBehaviour<3u> TreatBehaviour;
	static const bool is_defined_ = MTraits::is_defined;
	static const bool bs = Traits::requiresStiffnessTensor;
	static const bool ba = Traits::requiresThermalExpansionTensor;
	typedef typename IF<
	  is_defined_,
	  typename IF<
	  Traits::useTimeSubStepping,
	  typename TreatBehaviour::template IntegratorWithTimeStepping<bs,ba>,
	  typename TreatBehaviour::template Integrator<bs,ba>
	  >::type,
	  typename TreatBehaviour::Error>::type Handler;
	UMATReal  s[6];
	UMATReal  e[6];
	UMATReal de[6];
	// Passage au repère matériau
	UMATRotationMatrix3D m(PROPS+9,DROT);
	m.rotateStressesForward(STRESS,s);
	m.rotateStrainsForward(STRAN,e);
	m.rotateStrainsForward(DSTRAN,de);
	using namespace std;
	TreatBehaviour::checkNPROPS(*NPROPS);
	TreatBehaviour::checkNSTATV(*NSTATV);
	Handler handler(DTIME,DDSOE,e,
			de,TEMP,DTEMP,PROPS,
			PREDEF,DPRED,STATEV,s);
	handler.exe(s,STATEV);
	m.rotateStressesBackward(s,STRESS);
      } // end of TreatOrthotropicBehaviour3D::exe

    }; // end of struct TreatOrthotropicBehaviour3D

    template<unsigned short N>
    struct TreatIsotropicBehaviour
      : public TreatBehaviour<N>
    {
      TFEL_UMAT_INLINE static
      void exe(const UMATReal *const DTIME ,
	       const UMATReal *const,
	       const UMATReal *const DDSOE,
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
	       UMATReal *const STRESS) 
      {
	using namespace tfel::meta;
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<Behaviour<N,UMATReal,false> > MTraits;
	typedef UMATTraits<Behaviour<N,UMATReal,false> > Traits;
	typedef TreatBehaviour<N> TreatBehaviour;
	static const bool is_defined_ = MTraits::is_defined;
	static const bool bs = Traits::requiresStiffnessTensor;
	static const bool ba = Traits::requiresThermalExpansionTensor;
	typedef typename IF<
	  is_defined_,
	  typename IF<
	  Traits::useTimeSubStepping,
	  typename TreatBehaviour::template IntegratorWithTimeStepping<bs,ba>,
	  typename TreatBehaviour::template Integrator<bs,ba>
	  >::type,
	  typename TreatBehaviour::Error>::type Handler;
	TreatBehaviour::checkNPROPS(*NPROPS);
	TreatBehaviour::checkNSTATV(*NSTATV);
	Handler handler(DTIME,DDSOE,STRAN,
			DSTRAN,TEMP,DTEMP,PROPS,
			PREDEF,DPRED,STATEV,STRESS);
	handler.exe(STRESS,STATEV);
      } // end of TreatIsotropicBehaviour::exe

    }; // end of struct TreatIsotropicBehaviour

  }; // end of class UMATInterface
  
} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_CALL_H */

