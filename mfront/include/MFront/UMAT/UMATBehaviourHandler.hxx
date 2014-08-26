/*! 
 * \file  UMATBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 */

#ifndef _LIB_MFRONT_UMAT_UMATBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_UMAT_UMATBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_UMAT_CALL_H_
#error "This header shall not be called directly"
#endif

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Math/stensor.hxx"
#include"MFront/UMAT/UMATTangentOperator.hxx"
#include"MFront/UMAT/UMATComputeStiffnessTensor.hxx"

namespace umat
{

  template<UMATBehaviourType btype,
	   unsigned short N>
  struct UMATTangentOperatorType;

  template<unsigned short N>
  struct UMATTangentOperatorType<umat::SMALLSTRAINSTANDARDBEHAVIOUR,N>
  {
    typedef tfel::math::st2tost2<N,UMATReal> type;
  };

  template<unsigned short N>
  struct UMATTangentOperatorType<umat::FINITESTRAINSTANDARDBEHAVIOUR,N>
  {
    typedef tfel::math::t2tost2<N,UMATReal> type;
  };

  template<unsigned short N>
  struct UMATTangentOperatorType<umat::COHESIVEZONEMODEL,N>
  {
    typedef tfel::math::tmatrix<N,N,UMATReal> type;
  };
  
  /*!
   * structure in charge of calling the behaviour integrate method.
   * This structure handles two cases wether or not we shall handle
   * local substepping.
   */
  template<UMATBehaviourType type,
	   tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATBehaviourHandler
    :public UMATInterfaceBase
  {
    /*!
     * structure in charge of checking the thermal expansion
     * coefficient is zero
     */
    struct CheckThermalExpansionCoefficientIsNull
      : public UMATInterfaceBase
    {
      /*!
       * \param[in] a : thermal expansion coefficient
       */
      TFEL_UMAT_INLINE static
      void exe(const UMATReal a)
      {
	using std::abs;
	using std::numeric_limits;
	using tfel::utilities::Name;
	typedef Behaviour<H,UMATReal,false> BV;
	if(abs(a)>numeric_limits<UMATReal>::min()){
	  UMATInterfaceBase::throwThermalExpansionCoefficientShallBeNull(Name<BV>::getName());
	}
      }
    };
    /*!
     * structure in charge of not checking the thermal expansion
     * coefficient is zero
     */
    struct DontCheckThermalExpansionCoefficientIsNull
      : public UMATInterfaceBase
    {
      /*!
       * \param[in] a : thermal expansion
       */
      TFEL_UMAT_INLINE static
      void exe(const UMATReal)
      {} // end of exe
    };
    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithStressFreeExpansion
      : public UMATInterfaceBase
    {
      //! a simple alias
      typedef Behaviour<H,UMATReal,false> BV;
      //! a simple alias for the behaviour data
      typedef typename BV::BehaviourData  BData;
      //! a simple alias for the integration data
      typedef typename BV::IntegrationData  IData;
      //! a simple alias
      typedef tfel::material::ModellingHypothesisToSpaceDimension<H> ModellingHypothesisToSpaceDimension;
      // spatial dimension
      static const unsigned short N = ModellingHypothesisToSpaceDimension::value;
      /*!
       * \param[out] bData  : behaviour data
       * \param[out] iData  : integration data
       * \param[in]  b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_UMAT_INLINE static 
      void exe(BData& bData,
	       IData& iData,
	       const UMATReal *const STRAN,
	       const UMATReal *const DSTRAN,
	       const StressFreeExpansionHandler& sfeh)
      {
	using tfel::utilities::Name;
	using tfel::fsalgo::copy;
	using std::pair;
	typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
	UMATReal dv0[UMATTraits<BV>::DrivingVariableSize];
	UMATReal dv1[UMATTraits<BV>::DrivingVariableSize];
	copy<UMATTraits<BV>::DrivingVariableSize>::exe(STRAN,dv0);
	copy<UMATTraits<BV>::DrivingVariableSize>::exe(DSTRAN,dv1);
	// check that the function pointer are not null
	if(sfeh==0){
	  throwUnsupportedStressFreeExpansionException(Name<BV>::getName());
	}
	// creating a fake behaviour to compoute the stress-free expansion
	// this is not really elegant by can't do better
	pair<StressFreeExpansionType,StressFreeExpansionType> s;
	BV b(bData,iData);
	b.computeStressFreeExpansion(s);
	const StressFreeExpansionType& s0 = s.first;
	const StressFreeExpansionType& s1 = s.second;
	sfeh(dv0,dv1,&s0[0],&s1[0],UMATInt(N));
	bData.setUMATBehaviourDataDrivingVariables(dv0);
	iData.setUMATIntegrationDataDrivingVariables(dv1);
      } // end of exe
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_UMAT_INLINE static 
      void exe(BV& b,
	       const UMATReal *const STRAN,
	       const UMATReal *const DSTRAN,
	       const StressFreeExpansionHandler& sfeh)
      {
	using tfel::utilities::Name;
	using tfel::fsalgo::copy;
	using std::pair;
	typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
	UMATReal dv0[UMATTraits<BV>::DrivingVariableSize];
	UMATReal dv1[UMATTraits<BV>::DrivingVariableSize];
	copy<UMATTraits<BV>::DrivingVariableSize>::exe(STRAN,dv0);
	copy<UMATTraits<BV>::DrivingVariableSize>::exe(DSTRAN,dv1);
	// check that the function pointer are not null
	if(sfeh==0){
	  throwUnsupportedStressFreeExpansionException(Name<BV>::getName());
	}
	pair<StressFreeExpansionType,StressFreeExpansionType> s;
	b.computeStressFreeExpansion(s);
	const StressFreeExpansionType& s0 = s.first;
	const StressFreeExpansionType& s1 = s.second;
	sfeh(dv0,dv1,&s0[0],&s1[0],UMATInt(N));
	b.setUMATBehaviourDataDrivingVariables(dv0);
	b.setUMATIntegrationDataDrivingVariables(dv1);
      } // end of exe

    }; // end of struct DrivingVariableInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithoutStressFreeExpansion
    {
      //! a simple alias
      typedef Behaviour<H,UMATReal,false> BV;
      //! a simple alias for the behaviour data
      typedef typename BV::BehaviourData  BData;
      //! a simple alias for the integration data
      typedef typename BV::IntegrationData  IData;
      /*!
       * \param[out] bData  : behaviour data
       * \param[out] iData  : integration data
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_UMAT_INLINE static 
      void exe(BData& bData,
	       IData& iData,
	       const UMATReal *const STRAN,
	       const UMATReal *const DSTRAN,
	       const StressFreeExpansionHandler)
      {
	bData.setUMATBehaviourDataDrivingVariables(STRAN);
	iData.setUMATIntegrationDataDrivingVariables(DSTRAN);
      } // end of exe
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_UMAT_INLINE static 
      void exe(BV& b,
	       const UMATReal *const STRAN,
	       const UMATReal *const DSTRAN,
	       const StressFreeExpansionHandler&)
      {
	b.setUMATBehaviourDataDrivingVariables(STRAN);
	b.setUMATIntegrationDataDrivingVariables(DSTRAN);
      } // end of exe
    }; // end of struct DrivingVariableInitialiserWithoutStressFreeExpansion

    /*!
     * An helper structure which is used to compute the stiffness
     * tensor for the behaviour that requires it.
     */
    struct TFEL_VISIBILITY_LOCAL StiffnessTensorInitializer
    {
      typedef Behaviour<H,UMATReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_UMAT_INLINE static void
      exe(BData& data,const UMATReal * const props){
	typedef UMATTraits<BV> Traits;
	const bool buas = Traits::requiresUnAlteredStiffnessTensor;
	UMATComputeStiffnessTensor<type,H,Traits::stype,buas>::exe(data.getStiffnessTensor(),
								   props);
      } // end of exe
    }; // end of struct StiffnessTensorInitializer
    
    /*!
     * An helper structure which is used to compute the thermal
     * expansion tensor for the behaviour that requires it.
     */
    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer
    {
      typedef Behaviour<H,UMATReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_UMAT_INLINE static void
	exe(BData& data,const UMATReal * const props){
	UMATComputeThermalExpansionCoefficientTensor<type,H,UMATTraits<BV>::stype>::exe(props,
									     data.getThermalExpansionCoefficientTensor());
      } // end of exe
    }; // end of struct ThermalExpansionCoefficientTensorInitializer
    
    /*!
     * an helper class which don't do any initialisation
     */
    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer
    {
      typedef Behaviour<H,UMATReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_UMAT_INLINE static void
	exe(BData&,const UMATReal * const)
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
			     const StressFreeExpansionHandler&)
      {} // end of Error
      
      TFEL_UMAT_INLINE void exe(UMATReal *const,
				UMATReal *const,
				UMATReal *const)
      {
	using namespace std;
	using namespace tfel::material;
	using namespace tfel::utilities;
	throw(UMATInvalidDimension(Name<Behaviour<H,UMATReal,false> >::getName(),
				   ModellingHypothesisToSpaceDimension<H>::value));
	return;
      } // end of Error::exe
      
    }; // end of struct Error

    template<const bool bs,     // requires StiffnessTensor
	     const bool ba>     // requires ThermalExpansionCoefficientTensor
      struct TFEL_VISIBILITY_LOCAL IntegratorWithTimeStepping
    {
      //! A simple alias
      typedef typename tfel::meta::IF<bs,
				      StiffnessTensorInitializer,
				      DoNothingInitializer>::type SInitializer;
      //! A simple alias
      typedef typename tfel::meta::IF<ba,
				      ThermalExpansionCoefficientTensorInitializer,
				      DoNothingInitializer>::type AInitializer;
      
      TFEL_UMAT_INLINE
      IntegratorWithTimeStepping(const UMATReal *const DTIME ,
				 const UMATReal *const STRAN,
				 const UMATReal *const DSTRAN,
				 const UMATReal *const TEMP  ,
				 const UMATReal *const DTEMP,
				 const UMATReal *const PROPS ,
				 const UMATReal *const PREDEF,
				 const UMATReal *const DPRED,
				 UMATReal *const STATEV,
				 UMATReal *const STRESS,
				 const StressFreeExpansionHandler& sfeh)
	: bData(TEMP,PROPS+UMATTraits<BV>::propertiesOffset,
		STATEV,PREDEF),
	  iData(DTIME,DTEMP,DPRED),
	  dt(*DTIME)
       {
	 using namespace tfel::material;
	 typedef MechanicalBehaviourTraits<BV> Traits;
	 typedef typename tfel::meta::IF<
	   Traits::hasStressFreeExpansion,
	   DrivingVariableInitialiserWithStressFreeExpansion,
	   DrivingVariableInitialiserWithoutStressFreeExpansion>::type DVInitializer;
	 SInitializer::exe(this->bData,PROPS);
	 AInitializer::exe(this->bData,PROPS);
	 DVInitializer::exe(this->bData,this->iData,STRAN,DSTRAN,sfeh);
	 this->bData.setUMATBehaviourDataThermodynamicForces(STRESS);
       } // end of IntegratorWithTimeStepping
      
      TFEL_UMAT_INLINE2 void
	exe(UMATReal *const DDSOE,
	    UMATReal *const STRESS,
	    UMATReal *const STATEV)
      {
	using namespace tfel::utilities;
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename tfel::meta::IF<
	  Traits::hasConsistentTangentOperator,
	  typename tfel::meta::IF<
	  Traits::isConsistentTangentOperatorSymmetric,
	  SymmetricConsistentTangentOperatorComputer,
	  GeneralConsistentTangentOperatorComputer>::type,
	  ConsistentTangentOperatorIsNotAvalaible
	  >::type ConsistentTangentOperatorHandler;
	typedef typename tfel::meta::IF<
	  Traits::hasPredictionOperator,
	  StandardPredictionOperatorComputer,
	  PredictionOperatorIsNotAvalaible
	  >::type PredictionOperatorComputer;
	const UMATOutOfBoundsPolicy& up = UMATOutOfBoundsPolicy::getUMATOutOfBoundsPolicy();
	unsigned short subSteps   = 0u;
	unsigned short iterations = 1u;
	if(this->dt<0.){
	  throwNegativeTimeStepException(Name<Behaviour<H,UMATReal,false> >::getName());
	}
	while((iterations!=0)&&
	      (subSteps!=UMATTraits<BV>::maximumSubStepping)){
	  BV behaviour(this->bData,this->iData);
	  behaviour.initialize();
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
	      r = behaviour.integrate(BV::CONSISTENTTANGENTOPERATOR);
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
#ifdef MFRONT_UMAT_VERBOSE
	    std::cerr << "no convergence : " << e.what() << std::endl;
#endif
	    r = BV::FAILURE;
	  }
	  if((r==BV::SUCCESS)||((r==BV::UNRELIABLE_RESULTS)&&
	      (!UMATTraits<BV>::doSubSteppingOnInvalidResults))){
	    --(iterations);
	    behaviour.checkBounds();
	    behaviour.updateExternalStateVariables();
	    this->bData = static_cast<const BData&>(behaviour);
	    if(iterations==0){
	      if((*DDSOE>0.5)||(*DDSOE<-0.5)){
		ConsistentTangentOperatorHandler::exe(behaviour,DDSOE);
	      }
	    }
	  } else if ((r==BV::UNRELIABLE_RESULTS)&&
		     (UMATTraits<BV>::doSubSteppingOnInvalidResults)){
	    iterations = static_cast<unsigned short>(iterations*2u);
	    this->iData *= 0.5;
	  } else {
	    ++subSteps;
	    iterations = static_cast<unsigned short>(iterations*2u);
	    this->iData *= 0.5;
	  }
	}
	if((subSteps==UMATTraits<BV>::maximumSubStepping)&&(iterations!=0)){
	  throwMaximumNumberOfSubSteppingReachedException(Name<Behaviour<H,UMATReal,false> >::getName());
	}
	this->bData.UMATexportStateData(STRESS,STATEV);
      } // end of IntegratorWithTimeStepping::exe

    private:
      typedef Behaviour<H,UMATReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      typedef typename BV::IntegrationData  IData;

      BData bData;
      IData iData;
      UMATReal dt;
      tfel::material::ModellingHypothesis::Hypothesis hypothesis;
	
    }; // end of struct IntegratorWithTimeStepping

    template<const bool bs,     // requires StiffnessTensor
	     const bool ba>     // requires ThermalExpansionCoefficientTensor
      struct TFEL_VISIBILITY_LOCAL Integrator
    {
      typedef typename tfel::meta::IF<bs,
				      StiffnessTensorInitializer,
				      DoNothingInitializer>::type SInitializer;

      typedef typename tfel::meta::IF<ba,
				      ThermalExpansionCoefficientTensorInitializer,
				      DoNothingInitializer>::type AInitializer;

      TFEL_UMAT_INLINE Integrator(const UMATReal *const DTIME ,
				  const UMATReal *const STRAN ,
				  const UMATReal *const DSTRAN,
				  const UMATReal *const TEMP,
				  const UMATReal *const DTEMP,
				  const UMATReal *const PROPS ,
				  const UMATReal *const PREDEF,
				  const UMATReal *const DPRED,
				  const UMATReal *const STATEV,
				  const UMATReal *const STRESS,
				  const StressFreeExpansionHandler& sfeh)
	: behaviour(DTIME,TEMP,DTEMP,
		    PROPS+UMATTraits<BV>::propertiesOffset,
		    STATEV,PREDEF,DPRED),
	dt(*DTIME)
	  {
	    using namespace tfel::material;
	    typedef MechanicalBehaviourTraits<BV> Traits;
	    typedef typename tfel::meta::IF<
	      Traits::hasStressFreeExpansion,
	      DrivingVariableInitialiserWithStressFreeExpansion,
	      DrivingVariableInitialiserWithoutStressFreeExpansion>::type DVInitializer;
	    const UMATOutOfBoundsPolicy& up = UMATOutOfBoundsPolicy::getUMATOutOfBoundsPolicy();
	    SInitializer::exe(this->behaviour,PROPS);
	    AInitializer::exe(this->behaviour,PROPS);
	    DVInitializer::exe(this->behaviour,STRAN,DSTRAN,sfeh);
	    this->behaviour.setUMATBehaviourDataThermodynamicForces(STRESS);
	    this->behaviour.initialize();
	    this->behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	  } // end of Integrator::Integrator
	
      TFEL_UMAT_INLINE2
	void exe(UMATReal *const DDSOE,
		 UMATReal *const STRESS,
		 UMATReal *const STATEV)
      {
	using namespace tfel::utilities;
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename tfel::meta::IF<
	  Traits::hasConsistentTangentOperator,
	  typename tfel::meta::IF<
	  Traits::isConsistentTangentOperatorSymmetric,
	  SymmetricConsistentTangentOperatorComputer,
	  GeneralConsistentTangentOperatorComputer>::type,
	  ConsistentTangentOperatorIsNotAvalaible
	  >::type ConsistentTangentOperatorHandler;
	typedef typename tfel::meta::IF<
	  Traits::hasPredictionOperator,
	  StandardPredictionOperatorComputer,
	  PredictionOperatorIsNotAvalaible
	  >::type PredictionOperatorComputer;
	if(this->dt<0.){
	  throwNegativeTimeStepException(Name<BV>::getName());
	}
	this->behaviour.checkBounds();
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
	  r = this->behaviour.integrate(BV::CONSISTENTTANGENTOPERATOR);
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
	this->behaviour.UMATexportStateData(STRESS,STATEV);
	if((*DDSOE>0.5)||(*DDSOE<-0.5)){
	  ConsistentTangentOperatorHandler::exe(this->behaviour,DDSOE);
	}
      } // end of Integrator::exe
	
    private:
      typedef Behaviour<H,UMATReal,false> BV;
      BV behaviour;
      UMATReal dt;
    }; // end of struct Integrator

    struct StandardPredictionOperatorComputer
    {
      typedef Behaviour<H,UMATReal,false> BV;
      static typename BV::IntegrationResult
      exe(BV& b,const typename BV::SMType smt)
      {
	return b.computePredictionOperator(smt);
      } // end of exe	  
    };

    struct PredictionOperatorIsNotAvalaible
    {
      typedef Behaviour<H,UMATReal,false> BV;
      static typename BV::IntegrationResult
      exe(BV&,const typename BV::SMType)
      {
	using namespace tfel::utilities;
	throwPredictionOperatorIsNotAvalaible(Name<BV>::getName());
	return BV::FAILURE;
      } // end of exe	  
    };
      
    struct ConsistentTangentOperatorIsNotAvalaible
    {
      typedef Behaviour<H,UMATReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(BV&,UMATReal *const)
      {
	using namespace tfel::utilities;
	throwConsistentTangentOperatorIsNotAvalaible(Name<BV>::getName());
      } // end of exe	  
    };

    struct ConsistentTangentOperatorComputer
    {
      typedef Behaviour<H,UMATReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(const BV& bv,UMATReal *const DDSOE)
      {
	typedef typename UMATTangentOperatorType<UMATTraits<BV>::btype,N>::type TangentOperatorType;
	TangentOperatorType& Dt = *(reinterpret_cast<TangentOperatorType*>(DDSOE));
	Dt = bv.getTangentOperator();
	// l'opérateur tangent contient des sqrt(2)...
	UMATTangentOperator::normalize(Dt);
      } // end of exe	  
    };

    struct SymmetricConsistentTangentOperatorComputer
    {
      typedef Behaviour<H,UMATReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(const BV& bv,UMATReal *const DDSOE)
      {
	ConsistentTangentOperatorComputer::exe(bv,DDSOE);
      } // end of exe	  
    };

    struct GeneralConsistentTangentOperatorComputer
    {
      typedef Behaviour<H,UMATReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(const BV& bv,UMATReal *const DDSOE)
      {
	using namespace tfel::math;
	typedef typename UMATTangentOperatorType<UMATTraits<BV>::btype,N>::type TangentOperatorType;
	ConsistentTangentOperatorComputer::exe(bv,DDSOE);
	TangentOperatorType& Dt = *(reinterpret_cast<TangentOperatorType*>(DDSOE));
	// les conventions fortran....
	UMATTangentOperator::transpose(Dt);
      } // end of exe	  
    };

    TFEL_UMAT_INLINE2 static void
      checkNPROPS(const UMATInt NPROPS)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::material;
      typedef Behaviour<H,UMATReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset  = UMATTraits<BV>::propertiesOffset;
      const unsigned short nprops  = UMATTraits<BV>::material_properties_nb;
      const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
      const bool is_defined_       = Traits::is_defined;
      // Test if the nb of properties matches Behaviour requirements
      if((NPROPS!=NPROPS_)&&is_defined_){
      	throwUnMatchedNumberOfMaterialProperties(Name<Behaviour<H,UMATReal,false> >::getName(),
      						 NPROPS_,NPROPS);
      }
    } // end of checkNPROPS
      
    TFEL_UMAT_INLINE2 static void
      checkNSTATV(const UMATInt NSTATV)
    {
      using namespace tfel::utilities;
      typedef Behaviour<H,UMATReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short nstatv  = Traits::internal_variables_nb;
      const unsigned short NSTATV_ = nstatv == 0 ? 1u : nstatv;
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of state variables matches Behaviour requirements
      if((NSTATV_!=NSTATV)&&is_defined_){
	throwUnMatchedNumberOfStateVariables(Name<Behaviour<H,UMATReal,false> >::getName(),
					     NSTATV_,NSTATV);
      }
    } // end of checkNSTATV
      
  }; // end of struct UMATBehaviourHandler
  
} // end of namespace umat  

#include"MFront/UMAT/UMATIsotropicBehaviourHandler.hxx"
#include"MFront/UMAT/UMATOrthotropicBehaviourHandler.hxx"

#endif /* _LIB_MFRONT_UMAT_UMATBEHAVIOURHANDLER_H */

