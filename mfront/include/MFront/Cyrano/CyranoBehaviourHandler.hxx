/*! 
 * \file  CyranoBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_CYRANO_CALL_H_
#error "This header shall not be called directly"
#endif

#include"TFEL/Utilities/Name.hxx"
#include"MFront/Cyrano/CyranoTangentOperator.hxx"

namespace cyrano
{
  
  /*!
   * structure in charge of calling the behaviour integrate method.
   * This structure handles two cases wether or not we shall handle
   * local substepping.
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CyranoBehaviourHandler
    :public CyranoInterfaceBase
  {
    
    /*!
     * An helper structure which is used to compute the stiffness
     * tensor for the behaviour that requires it.
     */
    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_CYRANO_INLINE static void
      exe(BData& data,const CyranoReal * const props){
	CyranoComputeStiffnessOperator<H,CyranoTraits<BV>::stype>::exe(props,
								       data.getStiffnessOperator());
      } // end of exe
    }; // end of struct StiffnessOperatorInitializer
    
    /*!
     * An helper structure which is used to compute the thermal
     * expansion tensor for the behaviour that requires it.
     */
    struct TFEL_VISIBILITY_LOCAL ThermalExpansionTensorInitializer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_CYRANO_INLINE static void
	exe(BData& data,const CyranoReal * const props){
	CyranoComputeThermalExpansionTensor<H,CyranoTraits<BV>::stype>::exe(props,
									    data.getThermalExpansionTensor());
      } // end of exe
    }; // end of struct ThermalExpansionTensorInitializer
    
    /*!
     * an helper class which don't do any initialisation
     */
    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_CYRANO_INLINE static void
	exe(BData&,const CyranoReal * const)
      {}
    }; // end of struct DoNothingInitializer
    
    /*!
     * A helper structure used to handle the case where the behaviour
     * is not defined.
     *
     * For example, if the behaviour is only defined in 1D, this
     * structure will be used if the user tries to use it in 2D or 3D.
     */
    struct TFEL_VISIBILITY_LOCAL Error
    {
      
      TFEL_CYRANO_INLINE Error(const CyranoReal *const,
			     const CyranoReal *const,
			     const CyranoReal *const,
			     const CyranoReal *const,
			     const CyranoReal *const,
			     const CyranoReal *const,
			     const CyranoReal *const,
			     const CyranoReal *const,
			     const CyranoReal *const,
			     const CyranoReal *const)
      {} // end of Error
      
      TFEL_CYRANO_INLINE void exe(CyranoReal *const,
				CyranoReal *const,
				CyranoReal *const)
	throw(CyranoException)
      {
	using namespace std;
	using namespace tfel::material;
	using namespace tfel::utilities;
	throw(CyranoInvalidDimension(Name<Behaviour<H,CyranoReal,false> >::getName(),
				   ModellingHypothesisToSpaceDimension<H>::value));
	return;
      } // end of Error::exe
      
    }; // end of struct Error

    template<const bool bs,     // requires StiffnessOperator
	     const bool ba>     // requires ThermalExpansionTensor
      struct TFEL_VISIBILITY_LOCAL IntegratorWithTimeStepping
    {
      //! A simple alias
      typedef typename tfel::meta::IF<bs,
				      StiffnessOperatorInitializer,
				      DoNothingInitializer>::type SInitializer;
      //! A simple alias
      typedef typename tfel::meta::IF<ba,
				      ThermalExpansionTensorInitializer,
				      DoNothingInitializer>::type AInitializer;
      
      TFEL_CYRANO_INLINE
      IntegratorWithTimeStepping(const CyranoReal *const DTIME ,
				 const CyranoReal *const STRAN ,
				 const CyranoReal *const DSTRAN,
				 const CyranoReal *const TEMP  ,
				 const CyranoReal *const DTEMP,
				 const CyranoReal *const PROPS ,
				 const CyranoReal *const PREDEF,
				 const CyranoReal *const DPRED,
				 CyranoReal *const STATEV,
				 CyranoReal *const STRESS)
	: bData(STRESS,STRAN,TEMP,PROPS+CyranoTraits<BV>::propertiesOffset,
		STATEV,PREDEF),
	  iData(DTIME,DSTRAN,DTEMP,DPRED),
	  dt(*DTIME)
       {
	 SInitializer::exe(this->bData,PROPS);
	 AInitializer::exe(this->bData,PROPS);
       } // end of IntegratorWithTimeStepping
      
      TFEL_CYRANO_INLINE2 void
	exe(CyranoReal *const DDSOE,
	    CyranoReal *const STRESS,
	    CyranoReal *const STATEV)
      {
	using namespace tfel::utilities;
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename tfel::meta::IF<
	  Traits::hasConsistantTangentOperator,
	  typename tfel::meta::IF<
	  Traits::isConsistantTangentOperatorSymmetric,
	  SymmetricConsistantTangentOperatorComputer,
	  GeneralConsistantTangentOperatorComputer>::type,
	  ConsistantTangentOperatorIsNotAvalaible
	  >::type ConsistantTangentOperatorHandler;
	typedef typename tfel::meta::IF<
	  Traits::hasPredictionOperator,
	  StandardPredictionOperatorComputer,
	  PredictionOperatorIsNotAvalaible
	  >::type PredictionOperatorComputer;
	const CyranoOutOfBoundsPolicy& up = CyranoOutOfBoundsPolicy::getCyranoOutOfBoundsPolicy();
	unsigned short subSteps   = 0u;
	unsigned short iterations = 1u;
	if(this->dt<0.){
	  throwNegativeTimeStepException(Name<Behaviour<H,CyranoReal,false> >::getName());
	}
	while((iterations!=0)&&
	      (subSteps!=CyranoTraits<BV>::maximumSubStepping)){
	  BV behaviour(this->bData,this->iData);
	  behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	  behaviour.checkBounds();
	  typename BV::IntegrationResult r = BV::SUCCESS;
	  try{
	    if((-3.25<*DDSOE)&&(*DDSOE<-2.75)){
	      r = PredictionOperatorComputer::exe(behaviour,BV::TANGENTOPERATOR);
	    } else if((-2.25<*DDSOE)&&(*DDSOE<-1.75)){
	      r = PredictionOperatorComputer::exe(behaviour,BV::SECANTOPERATOR);
	    } else if((-1.25<*DDSOE)&&(*DDSOE<-0.75)){
	      r = PredictionOperatorComputer::exe(behaviour,BV::ELASTIC);
	    } else if((-0.25<*DDSOE)&&(*DDSOE<0.25)){
	      r = behaviour.integrate(BV::NOSTIFFNESSREQUESTED);
	    } else if((0.75<*DDSOE)&&(*DDSOE<1.25)){
	      r = behaviour.integrate(BV::ELASTIC);
	    } else if((1.75<*DDSOE)&&(*DDSOE<2.25)){
	      r = behaviour.integrate(BV::SECANTOPERATOR);
	    } else if((2.75<*DDSOE)&&(*DDSOE<3.25)){
	      r = behaviour.integrate(BV::TANGENTOPERATOR);
	    } else if((3.75<*DDSOE)&&(*DDSOE<4.25)){
	      r = behaviour.integrate(BV::CONSISTANTTANGENTOPERATOR);
	    } else {
	      throwInvalidDDSOEException(Name<BV>::getName(),*DDSOE);
	    }
	    if(r==BV::FAILURE){
	      if(*DDSOE<-0.5){
		throwPredictionComputationFailedException(Name<BV>::getName());
	      }
	    }
	  }
	  catch(const tfel::material::DivergenceException& e){
#ifdef MFRONT_CYRANO_VERBOSE
	    std::cerr << "no convergence : " << e.what() << std::endl;
#endif
	    r = BV::FAILURE;
	  }
	  if((r==BV::SUCCESS)||((r==BV::UNRELIABLE_RESULTS)&&
	      (!CyranoTraits<BV>::doSubSteppingOnInvalidResults))){
	    --(iterations);
	    behaviour.checkBounds();
	    behaviour.updateExternalStateVariables();
	    this->bData = static_cast<const BData&>(behaviour);
	    if(iterations==0){
	      if((*DDSOE>0.5)||(*DDSOE<-0.5)){
		ConsistantTangentOperatorHandler::exe(behaviour,DDSOE);
	      }
	    }
	  } else if ((r==BV::UNRELIABLE_RESULTS)&&
		     (CyranoTraits<BV>::doSubSteppingOnInvalidResults)){
	    iterations = static_cast<unsigned short>(iterations*2u);
	    this->iData *= 0.5;
	  } else {
	    ++subSteps;
	    iterations = static_cast<unsigned short>(iterations*2u);
	    this->iData *= 0.5;
	  }
	}
	if((subSteps==CyranoTraits<BV>::maximumSubStepping)&&(iterations!=0)){
	  throwMaximumNumberOfSubSteppingReachedException(Name<Behaviour<H,CyranoReal,false> >::getName());
	}
	this->bData.CYRANOexportStateData(STRESS,STATEV);
      } // end of IntegratorWithTimeStepping::exe

    private:
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      typedef typename BV::IntegrationData  IData;

      BData bData;
      IData iData;
      CyranoReal dt;
      tfel::material::ModellingHypothesis::Hypothesis hypothesis;
	
    }; // end of struct IntegratorWithTimeStepping

    template<const bool bs,     // requires StiffnessOperator
	     const bool ba>     // requires ThermalExpansionTensor
      struct TFEL_VISIBILITY_LOCAL Integrator
    {
      typedef typename tfel::meta::IF<bs,
				      StiffnessOperatorInitializer,
				      DoNothingInitializer>::type SInitializer;

      typedef typename tfel::meta::IF<ba,
				      ThermalExpansionTensorInitializer,
				      DoNothingInitializer>::type AInitializer;

      TFEL_CYRANO_INLINE Integrator(const CyranoReal *const DTIME ,
				  const CyranoReal *const STRAN ,
				  const CyranoReal *const DSTRAN,
				  const CyranoReal *const TEMP,
				  const CyranoReal *const DTEMP,
				  const CyranoReal *const PROPS ,
				  const CyranoReal *const PREDEF,
				  const CyranoReal *const DPRED,
				  const CyranoReal *const STATEV,
				  const CyranoReal *const STRESS)
	: behaviour(DTIME,STRESS,STRAN,DSTRAN,TEMP,DTEMP,
		    PROPS+CyranoTraits<BV>::propertiesOffset,
		    STATEV,PREDEF,DPRED),
	dt(*DTIME)
	  {
	    SInitializer::exe(this->behaviour,PROPS);
	    AInitializer::exe(this->behaviour,PROPS);
	    const CyranoOutOfBoundsPolicy& up = CyranoOutOfBoundsPolicy::getCyranoOutOfBoundsPolicy();
	    this->behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	  } // end of Integrator::Integrator
	
      TFEL_CYRANO_INLINE2
	void exe(CyranoReal *const DDSOE,
		 CyranoReal *const STRESS,
		 CyranoReal *const STATEV)
      {
	using namespace tfel::utilities;
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename tfel::meta::IF<
	  Traits::hasConsistantTangentOperator,
	  typename tfel::meta::IF<
	  Traits::isConsistantTangentOperatorSymmetric,
	  SymmetricConsistantTangentOperatorComputer,
	  GeneralConsistantTangentOperatorComputer>::type,
	  ConsistantTangentOperatorIsNotAvalaible
	  >::type ConsistantTangentOperatorHandler;
	typedef typename tfel::meta::IF<
	  Traits::hasPredictionOperator,
	  StandardPredictionOperatorComputer,
	  PredictionOperatorIsNotAvalaible
	  >::type PredictionOperatorComputer;
	if(this->dt<0.){
	  throwNegativeTimeStepException(Name<BV>::getName());
	}
	behaviour.checkBounds();
	typename BV::IntegrationResult r = BV::SUCCESS;
	if((-3.25<*DDSOE)&&(*DDSOE<-2.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,BV::TANGENTOPERATOR);
	} else if((-2.25<*DDSOE)&&(*DDSOE<-1.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,BV::SECANTOPERATOR);
	} else if((-1.25<*DDSOE)&&(*DDSOE<-0.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,BV::ELASTIC);
	} else if((-0.25<*DDSOE)&&(*DDSOE<0.25)){
	  r = this->behaviour.integrate(BV::NOSTIFFNESSREQUESTED);
	} else if((0.75<*DDSOE)&&(*DDSOE<1.25)){
	  r = this->behaviour.integrate(BV::ELASTIC);
	} else if((1.75<*DDSOE)&&(*DDSOE<2.25)){
	  r = this->behaviour.integrate(BV::SECANTOPERATOR);
	} else if((2.75<*DDSOE)&&(*DDSOE<3.25)){
	  r = this->behaviour.integrate(BV::TANGENTOPERATOR);
	} else if((3.75<*DDSOE)&&(*DDSOE<4.25)){
	  r = this->behaviour.integrate(BV::CONSISTANTTANGENTOPERATOR);
	} else {
	  throwInvalidDDSOEException(Name<BV>::getName(),*DDSOE);
	}
	if(r==BV::FAILURE){
	  // Il manque un vraie gestion locale de résultats imprécis
	  if(*DDSOE<-0.5){
	    throwPredictionComputationFailedException(Name<BV>::getName());
	  } else {
	    throwBehaviourIntegrationFailedException(Name<BV>::getName());
	  }
	}
	// if(this->behaviour.integrate(BV::NOSTIFFNESSREQUESTED)==BV::FAILURE){
	//   throwBehaviourIntegrationFailedException(Name<BV>::getName());
	// }
	behaviour.checkBounds();
	this->behaviour.CYRANOexportStateData(STRESS,STATEV);
	if((*DDSOE>0.5)||(*DDSOE<-0.5)){
	  ConsistantTangentOperatorHandler::exe(this->behaviour,DDSOE);
	}
      } // end of Integrator::exe
	
    private:
      typedef Behaviour<H,CyranoReal,false> BV;
      BV behaviour;
      CyranoReal dt;
    }; // end of struct Integrator

    struct StandardPredictionOperatorComputer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      static typename BV::IntegrationResult
      exe(BV& b,const typename BV::SMType smt)
      {
	return b.computePredictionOperator(smt);
      } // end of exe	  
    };

    struct PredictionOperatorIsNotAvalaible
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      static typename BV::IntegrationResult
      exe(BV&,const typename BV::SMType)
      {
	using namespace tfel::utilities;
	throwPredictionOperatorIsNotAvalaible(Name<BV>::getName());
	return BV::FAILURE;
      } // end of exe	  
    };
      
    struct ConsistantTangentOperatorIsNotAvalaible
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(BV&,CyranoReal *const)
      {
	using namespace tfel::utilities;
	throwConsistantTangentOperatorIsNotAvalaible(Name<BV>::getName());
      } // end of exe	  
    };

    struct ConsistantTangentOperatorComputer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(const BV& bv,CyranoReal *const DDSOE)
      {
	using namespace tfel::math;
	st2tost2<N,CyranoReal>& Dt = *(reinterpret_cast<st2tost2<N,CyranoReal>*>(DDSOE));
	Dt = bv.getTangentOperator();
      } // end of exe	  
    };

    struct SymmetricConsistantTangentOperatorComputer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(const BV& bv,CyranoReal *const DDSOE)
      {
	ConsistantTangentOperatorComputer::exe(bv,DDSOE);
      } // end of exe	  
    };

    struct GeneralConsistantTangentOperatorComputer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(const BV& bv,CyranoReal *const DDSOE)
      {
	using namespace tfel::math;
	ConsistantTangentOperatorComputer::exe(bv,DDSOE);
	st2tost2<N,CyranoReal>& Dt = *(reinterpret_cast<st2tost2<N,CyranoReal>*>(DDSOE));
	// les conventions fortran....
	CyranoTangentOperator::transpose(Dt);
      } // end of exe	  
    };

    TFEL_CYRANO_INLINE2 static void
      checkNPROPS(const CyranoInt NPROPS)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::material;
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset  = CyranoTraits<BV>::propertiesOffset;
      const unsigned short nprops  = Traits::material_properties_nb;
      const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of properties matches Behaviour requirements
      if((NPROPS!=NPROPS_)&&is_defined_){
	throwUnMatchedNumberOfMaterialProperties(Name<Behaviour<H,CyranoReal,false> >::getName(),
						 NPROPS_,NPROPS);
      }
    } // end of checkNPROPS
      
    TFEL_CYRANO_INLINE2 static void
      checkNSTATV(const CyranoInt NSTATV)
    {
      using namespace tfel::utilities;
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short nstatv  = Traits::internal_variables_nb;
      const unsigned short NSTATV_ = nstatv == 0 ? 1u : nstatv;
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of state variables matches Behaviour requirements
      if((NSTATV_!=NSTATV)&&is_defined_){
	throwUnMatchedNumberOfStateVariables(Name<Behaviour<H,CyranoReal,false> >::getName(),
					     NSTATV_,NSTATV);
      }
    } // end of checkNSTATV
      
  }; // end of struct CyranoBehaviourHandler
  
} // end of namespace cyrano  

#include"MFront/Cyrano/CyranoIsotropicBehaviourHandler.hxx"
#include"MFront/Cyrano/CyranoOrthotropicBehaviourHandler.hxx"

#endif /* _LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_H */

