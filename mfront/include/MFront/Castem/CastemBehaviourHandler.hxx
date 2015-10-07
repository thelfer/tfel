/*! 
 * \file  mfront/include/MFront/Castem/CastemBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_CASTEM_CASTEMBEHAVIOURHANDLER_H_
#define LIB_MFRONT_CASTEM_CASTEMBEHAVIOURHANDLER_H_ 

#ifndef LIB_MFRONT_CASTEM_CALL_HXX
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_CASTEM_CALL_HXX */

#include"TFEL/Math/stensor.hxx"
#include"TFEL/Material/MechanicalBehaviour.hxx"
#include"MFront/Castem/CastemTangentOperator.hxx"
#include"MFront/Castem/CastemComputeStiffnessTensor.hxx"

namespace castem
{

  template<CastemBehaviourType btype>
  struct CastemTangentOperatorFlag;

  template<>
  struct CastemTangentOperatorFlag<castem::SMALLSTRAINSTANDARDBEHAVIOUR>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR>
    TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template<>
  struct CastemTangentOperatorFlag<castem::FINITESTRAINSTANDARDBEHAVIOUR>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR>
    TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value = TangentOperatorTraits::DSIG_DF;
  };

  template<>
  struct CastemTangentOperatorFlag<castem::COHESIVEZONEMODEL>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::COHESIVEZONEMODEL>
    TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template<CastemBehaviourType btype,
	   unsigned short N>
  struct CastemTangentOperatorType;

  template<unsigned short N>
  struct CastemTangentOperatorType<castem::SMALLSTRAINSTANDARDBEHAVIOUR,N>
  {
    typedef tfel::math::st2tost2<N,CastemReal> type;
  };

  template<unsigned short N>
  struct CastemTangentOperatorType<castem::FINITESTRAINSTANDARDBEHAVIOUR,N>
  {
    typedef tfel::math::t2tost2<N,CastemReal> type;
  };

  template<unsigned short N>
  struct CastemTangentOperatorType<castem::COHESIVEZONEMODEL,N>
  {
    typedef tfel::math::tmatrix<N,N,CastemReal> type;
  };
  
  /*!
   * structure in charge of calling the behaviour integrate method.
   * This structure handles two cases wether or not we shall handle
   * local substepping.
   */
  template<CastemBehaviourType type,
	   tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CastemBehaviourHandler
    :public CastemInterfaceExceptions
  {
    /*!
     * structure in charge of checking the thermal expansion
     * coefficient is zero
     */
    struct CheckThermalExpansionCoefficientIsNull
      : public CastemInterfaceExceptions
    {
      /*!
       * \param[in] a : thermal expansion coefficient
       */
      TFEL_CASTEM_INLINE static
      void exe(const CastemReal a)
      {
	using tfel::material::MechanicalBehaviourTraits;
	typedef Behaviour<H,CastemReal,false> BV;
	typedef MechanicalBehaviourTraits<BV> Traits;
	if(std::abs(a)>std::numeric_limits<CastemReal>::min()){
	  CastemInterfaceExceptions::throwThermalExpansionCoefficientShallBeNull(Traits::getName());
	}
      }
    };
    /*!
     * structure in charge of not checking the thermal expansion
     * coefficient is zero
     */
    struct DontCheckThermalExpansionCoefficientIsNull
      : public CastemInterfaceExceptions
    {
      /*!
       * \param[in] a : thermal expansion
       */
      TFEL_CASTEM_INLINE static
      void exe(const CastemReal)
      {} // end of exe
    };
    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithStressFreeExpansion
      : public CastemInterfaceExceptions
    {
      //! a simple alias
      typedef Behaviour<H,CastemReal,false> BV;
      //! a simple alias for the behaviour data
      typedef typename BV::BehaviourData  BData;
      //! a simple alias for the integration data
      typedef typename BV::IntegrationData  IData;
      //! a simple alias
      typedef tfel::material::ModellingHypothesisToSpaceDimension<H> ModellingHypothesisToSpaceDimension;
      // spatial dimension
      static constexpr unsigned short N = ModellingHypothesisToSpaceDimension::value;
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
      TFEL_CASTEM_INLINE static 
      void exe(BData& bData,
	       IData& iData,
	       const CastemReal *const STRAN,
	       const CastemReal *const DSTRAN,
	       const StressFreeExpansionHandler& sfeh)
      {
	using tfel::fsalgo::copy;
	using std::pair;
	using tfel::material::MechanicalBehaviourTraits;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
	CastemReal dv0[CastemTraits<BV>::DrivingVariableSize];
	CastemReal dv1[CastemTraits<BV>::DrivingVariableSize];
	copy<CastemTraits<BV>::DrivingVariableSize>::exe(STRAN,dv0);
	copy<CastemTraits<BV>::DrivingVariableSize>::exe(DSTRAN,dv1);
	// check that the function pointer are not null
	if(sfeh==0){
	  throwUnsupportedStressFreeExpansionException(Traits::getName());
	}
	// creating a fake behaviour to compoute the stress-free expansion
	// this is not really elegant by can't do better
	pair<StressFreeExpansionType,StressFreeExpansionType> s;
	BV b(bData,iData);
	b.computeStressFreeExpansion(s);
	const auto& s0 = s.first;
	const auto& s1 = s.second;
	sfeh(dv0,dv1,&s0[0],&s1[0],CastemInt(N));
	bData.setCASTEMBehaviourDataDrivingVariables(dv0);
	iData.setCASTEMIntegrationDataDrivingVariables(dv1);
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
      TFEL_CASTEM_INLINE static 
      void exe(BV& b,
	       const CastemReal *const STRAN,
	       const CastemReal *const DSTRAN,
	       const StressFreeExpansionHandler& sfeh)
      {
	using tfel::fsalgo::copy;
	using std::pair;
	using tfel::material::MechanicalBehaviourTraits;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
	CastemReal dv0[CastemTraits<BV>::DrivingVariableSize];
	CastemReal dv1[CastemTraits<BV>::DrivingVariableSize];
	copy<CastemTraits<BV>::DrivingVariableSize>::exe(STRAN,dv0);
	copy<CastemTraits<BV>::DrivingVariableSize>::exe(DSTRAN,dv1);
	// check that the function pointer are not null
	if(sfeh==nullptr){
	  throwUnsupportedStressFreeExpansionException(Traits::getName());
	}
	pair<StressFreeExpansionType,StressFreeExpansionType> s;
	b.computeStressFreeExpansion(s);
	const auto& s0 = s.first;
	const auto& s1 = s.second;
	sfeh(dv0,dv1,&s0[0],&s1[0],CastemInt(N));
	b.setCASTEMBehaviourDataDrivingVariables(dv0);
	b.setCASTEMIntegrationDataDrivingVariables(dv1);
      } // end of exe

    }; // end of struct DrivingVariableInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithoutStressFreeExpansion
    {
      //! a simple alias
      typedef Behaviour<H,CastemReal,false> BV;
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
      TFEL_CASTEM_INLINE static 
      void exe(BData& bData,
	       IData& iData,
	       const CastemReal *const STRAN,
	       const CastemReal *const DSTRAN,
	       const StressFreeExpansionHandler)
      {
	bData.setCASTEMBehaviourDataDrivingVariables(STRAN);
	iData.setCASTEMIntegrationDataDrivingVariables(DSTRAN);
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
      TFEL_CASTEM_INLINE static 
      void exe(BV& b,
	       const CastemReal *const STRAN,
	       const CastemReal *const DSTRAN,
	       const StressFreeExpansionHandler&)
      {
	b.setCASTEMBehaviourDataDrivingVariables(STRAN);
	b.setCASTEMIntegrationDataDrivingVariables(DSTRAN);
      } // end of exe
    }; // end of struct DrivingVariableInitialiserWithoutStressFreeExpansion

    /*!
     * An helper structure which is used to compute the stiffness
     * tensor for the behaviour that requires it.
     */
    struct TFEL_VISIBILITY_LOCAL StiffnessTensorInitializer
    {
      typedef Behaviour<H,CastemReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_CASTEM_INLINE static void
      exe(BData& data,const CastemReal * const props){
	typedef CastemTraits<BV> Traits;
	const bool buas = Traits::requiresUnAlteredStiffnessTensor;
	CastemComputeStiffnessTensor<type,H,Traits::stype,buas>::exe(data.getStiffnessTensor(),
								   props);
      } // end of exe
    }; // end of struct StiffnessTensorInitializer
    
    /*!
     * An helper structure which is used to compute the thermal
     * expansion tensor for the behaviour that requires it.
     */
    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer
    {
      typedef Behaviour<H,CastemReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_CASTEM_INLINE static void
	exe(BData& data,const CastemReal * const props){
	CastemComputeThermalExpansionCoefficientTensor<type,H,CastemTraits<BV>::stype>::exe(props,
									     data.getThermalExpansionCoefficientTensor());
      } // end of exe
    }; // end of struct ThermalExpansionCoefficientTensorInitializer
    
    /*!
     * an helper class which don't do any initialisation
     */
    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer
    {
      typedef Behaviour<H,CastemReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_CASTEM_INLINE static void
	exe(BData&,const CastemReal * const)
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
      
      TFEL_CASTEM_INLINE Error(const CastemReal *const,
			     const CastemReal *const,
			     const CastemReal *const,
			     const CastemReal *const,
			     const CastemReal *const,
			     const CastemReal *const,
			     const CastemReal *const,
			     const CastemReal *const,
			     const CastemReal *const,
			     const CastemReal *const,
			     const tfel::material::OutOfBoundsPolicy,
			     const StressFreeExpansionHandler&)
      {} // end of Error
      
      TFEL_CASTEM_INLINE void exe(CastemReal *const,
				CastemReal *const,
				CastemReal *const)
      {
	using namespace std;
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<Behaviour<H,CastemReal,false>> Traits;
	throw(CastemInvalidDimension(Traits::getName(),
				   ModellingHypothesisToSpaceDimension<H>::value));
	return;
      } // end of Error::exe
      
    }; // end of struct Error

    template<const bool bs,     // requires StiffnessTensor
	     const bool ba>     // requires ThermalExpansionCoefficientTensor
      struct TFEL_VISIBILITY_LOCAL IntegratorWithTimeStepping
    {
      //! A simple alias
      typedef typename std::conditional<bs,
				      StiffnessTensorInitializer,
				      DoNothingInitializer>::type SInitializer;
      //! A simple alias
      typedef typename std::conditional<ba,
				      ThermalExpansionCoefficientTensorInitializer,
				      DoNothingInitializer>::type AInitializer;
      
      TFEL_CASTEM_INLINE
      IntegratorWithTimeStepping(const CastemReal *const DTIME_,
				 const CastemReal *const STRAN_,
				 const CastemReal *const DSTRAN_,
				 const CastemReal *const TEMP_,
				 const CastemReal *const DTEMP_,
				 const CastemReal *const PROPS_,
				 const CastemReal *const PREDEF_,
				 const CastemReal *const DPRED_,
				 CastemReal *const STATEV_,
				 CastemReal *const STRESS_,
				 const tfel::material::OutOfBoundsPolicy op,
				 const StressFreeExpansionHandler& sfeh_)
	: DTIME(DTIME_),STRAN(STRAN_), DSTRAN(DSTRAN_),
	TEMP(TEMP_),DTEMP(DTEMP_),PROPS(PROPS_),
	PREDEF(PREDEF_),DPRED(DPRED_),
	STATEV(STATEV_),STRESS(STRESS_),
	policy(op),
	sfeh(sfeh_)
       {} // end of IntegratorWithTimeStepping
      
      TFEL_CASTEM_INLINE2 void
	exe(CastemReal *const ddsoe,
	    CastemReal *const stress,
	    CastemReal *const statev)
      {
	if(*ddsoe<-0.5){
	  this->computePredictionOperator(ddsoe);
	} else {
	  this->integrate(stress,statev,ddsoe);
	}
      } // end of IntegratorWithTimeStepping::exe

    private:

      void
      computePredictionOperator(CastemReal *const DDSOE)
      {
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename std::conditional<
	  Traits::hasPredictionOperator,
	  StandardPredictionOperatorComputer,
	  PredictionOperatorIsNotAvalaible
	  >::type PredictionOperatorComputer;
	typedef typename std::conditional<
	  Traits::hasStressFreeExpansion,
	  DrivingVariableInitialiserWithStressFreeExpansion,
	  DrivingVariableInitialiserWithoutStressFreeExpansion>::type DVInitializer;
	typedef typename std::conditional<
	  Traits::isConsistentTangentOperatorSymmetric,
	  SymmetricConsistentTangentOperatorComputer,
	  GeneralConsistentTangentOperatorComputer>::type
	  ConsistentTangentOperatorHandler;
	const typename BV::SMFlag smflag = CastemTangentOperatorFlag<CastemTraits<BV>::btype>::value;
	  typename BV::SMType smtype = BV::NOSTIFFNESSREQUESTED;
	if((-3.25<*DDSOE)&&(*DDSOE<-2.75)){
	  smtype = BV::TANGENTOPERATOR;
	} else if((-2.25<*DDSOE)&&(*DDSOE<-1.75)){
	  smtype = BV::SECANTOPERATOR;
	} else if((-1.25<*DDSOE)&&(*DDSOE<-0.75)){
	  smtype = BV::ELASTIC;
	} else {
	  throwInvalidDDSOEException(Traits::getName(),*DDSOE);
	}
	BV behaviour(this->DTIME,this->TEMP,this->DTEMP,
		     this->PROPS+CastemTraits<BV>::propertiesOffset,
		     this->STATEV,this->PREDEF,this->DPRED);
	SInitializer::exe(behaviour,PROPS);
	AInitializer::exe(behaviour,PROPS);
	DVInitializer::exe(behaviour,STRAN,DSTRAN,sfeh);
	behaviour.setCASTEMBehaviourDataThermodynamicForces(STRESS);
	behaviour.initialize();
	behaviour.setOutOfBoundsPolicy(this->policy);
	behaviour.checkBounds();
	const typename BV::IntegrationResult r =
	  PredictionOperatorComputer::exe(behaviour,smflag,smtype);
	if(r==BV::FAILURE){
	  throwPredictionComputationFailedException(Traits::getName());
	}
	ConsistentTangentOperatorHandler::exe(behaviour,DDSOE);
      }

      void
      integrate(CastemReal *const stress,
		CastemReal *const statev,
		CastemReal *const ddsoe)
      {
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename std::conditional<
	  Traits::hasStressFreeExpansion,
	  DrivingVariableInitialiserWithStressFreeExpansion,
	  DrivingVariableInitialiserWithoutStressFreeExpansion>::type DVInitializer;
	typedef typename std::conditional<
	  Traits::hasConsistentTangentOperator,
	  typename std::conditional<
	  Traits::isConsistentTangentOperatorSymmetric,
	  SymmetricConsistentTangentOperatorComputer,
	  GeneralConsistentTangentOperatorComputer>::type,
	  ConsistentTangentOperatorIsNotAvalaible
	  >::type ConsistentTangentOperatorHandler;
	const typename BV::SMFlag smflag = CastemTangentOperatorFlag<CastemTraits<BV>::btype>::value;
	typename BV::SMType smtype = BV::NOSTIFFNESSREQUESTED;
	if((-0.25<*ddsoe)&&(*ddsoe<0.25)){
	} else if((0.75<*ddsoe)&&(*ddsoe<1.25)){
	  smtype = BV::ELASTIC;
	} else if((1.75<*ddsoe)&&(*ddsoe<2.25)){
	  smtype = BV::SECANTOPERATOR;
	} else if((2.75<*ddsoe)&&(*ddsoe<3.25)){
	  smtype = BV::TANGENTOPERATOR;
	} else if((3.75<*ddsoe)&&(*ddsoe<4.25)){
	  smtype = BV::CONSISTENTTANGENTOPERATOR;
	} else {
	  throwInvalidDDSOEException(Traits::getName(),*ddsoe);
	}
	BV behaviour(this->DTIME,this->TEMP,this->DTEMP,
		     this->PROPS+CastemTraits<BV>::propertiesOffset,
		     this->STATEV,this->PREDEF,this->DPRED);
	typename BV::IntegrationResult r = BV::SUCCESS;
	try {
	  SInitializer::exe(behaviour,PROPS);
	  AInitializer::exe(behaviour,PROPS);
	  DVInitializer::exe(behaviour,STRAN,DSTRAN,sfeh);
	  behaviour.setCASTEMBehaviourDataThermodynamicForces(STRESS);
	  behaviour.initialize();
	  behaviour.setOutOfBoundsPolicy(this->policy);
	  behaviour.checkBounds();
	  r = behaviour.integrate(smflag,smtype);
	  behaviour.checkBounds();
	}
#ifdef MFRONT_CASTEM_VERBOSE
	catch(const tfel::material::DivergenceException& e){
	  std::cerr << "no convergence : " << e.what() << std::endl;
#else
	catch(const tfel::material::DivergenceException&){
#endif
	  r = BV::FAILURE;
	}
	if((r==BV::FAILURE)||((r==BV::UNRELIABLE_RESULTS)&&
			      (CastemTraits<BV>::doSubSteppingOnInvalidResults))){
	  this->integrate2(stress,statev,ddsoe,smtype);
	} else {
	  behaviour.CASTEMexportStateData(stress,statev);
	  if((*ddsoe>0.5)||(*ddsoe<-0.5)){
	    ConsistentTangentOperatorHandler::exe(behaviour,ddsoe);
	  }
	}
      }
	
      void
      integrate2(CastemReal *const stress,
		 CastemReal *const statev,
		 CastemReal *const ddsoe,
		 const typename Behaviour<H,CastemReal,false>::SMType smtype)
      {
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename std::conditional<
	  Traits::hasStressFreeExpansion,
	  DrivingVariableInitialiserWithStressFreeExpansion,
	  DrivingVariableInitialiserWithoutStressFreeExpansion>::type DVInitializer;
	typedef typename std::conditional<
	  Traits::hasConsistentTangentOperator,
	  typename std::conditional<
	  Traits::isConsistentTangentOperatorSymmetric,
	  SymmetricConsistentTangentOperatorComputer,
	  GeneralConsistentTangentOperatorComputer>::type,
	  ConsistentTangentOperatorIsNotAvalaible
	  >::type ConsistentTangentOperatorHandler;
	BData bData(this->TEMP,this->PROPS+CastemTraits<BV>::propertiesOffset,
		    this->STATEV,this->PREDEF);
	IData iData(this->DTIME,this->DTEMP,this->DPRED);
	SInitializer::exe(bData,this->PROPS);
	AInitializer::exe(bData,this->PROPS);
	DVInitializer::exe(bData,iData,this->STRAN,this->DSTRAN,sfeh);
	bData.setCASTEMBehaviourDataThermodynamicForces(this->STRESS);
	iData.scale(bData,0.5);
	unsigned short subSteps   = 1u;
	unsigned short iterations = 2u;
	const typename BV::SMFlag smflag = CastemTangentOperatorFlag<CastemTraits<BV>::btype>::value;
	while((iterations!=0)&&
	      (subSteps!=CastemTraits<BV>::maximumSubStepping)){
	  typename BV::IntegrationResult result;
	  BV behaviour(bData,iData);
	  try{
	    behaviour.initialize();
	    behaviour.setOutOfBoundsPolicy(this->policy);
	    behaviour.checkBounds();
	    if(iterations==1u){
	      result = behaviour.integrate(smflag,smtype);
	    } else {
	      result = behaviour.integrate(smflag,BV::NOSTIFFNESSREQUESTED);
	    }
	  }
#ifdef MFRONT_CASTEM_VERBOSE
	  catch(const tfel::material::DivergenceException& e){
	    std::cerr << "no convergence : " << e.what() << std::endl;
#else
	  catch(const tfel::material::DivergenceException&){
#endif
	    result = BV::FAILURE;
	  }
	  if((result==BV::SUCCESS)||
	     ((result==BV::UNRELIABLE_RESULTS)&&
	      (!CastemTraits<BV>::doSubSteppingOnInvalidResults))){
	    --(iterations);
	    behaviour.checkBounds();
	    behaviour.updateExternalStateVariables();
	    if(iterations==0){
	      behaviour.CASTEMexportStateData(stress,statev);
	      if(*ddsoe>0.5){
		ConsistentTangentOperatorHandler::exe(behaviour,ddsoe);
	      }
	    } else {
	      bData = static_cast<const BData&>(behaviour);
	    }
	  } else if ((result==BV::UNRELIABLE_RESULTS)&&
		     (CastemTraits<BV>::doSubSteppingOnInvalidResults)){
	    iterations = static_cast<unsigned short>(iterations*2u);
	    iData.scale(bData,0.5);
	  } else {
	    ++subSteps;
	    iterations = static_cast<unsigned short>(iterations*2u);
	    iData.scale(bData,0.5);
	  }
	}
	if((subSteps==CastemTraits<BV>::maximumSubStepping)&&(iterations!=0)){
	  throwMaximumNumberOfSubSteppingReachedException(Traits::getName());
	}
      } // end of integrate
    
      typedef Behaviour<H,CastemReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      typedef typename BV::IntegrationData  IData;

      const CastemReal *const DTIME;
      const CastemReal *const STRAN;
      const CastemReal *const DSTRAN;
      const CastemReal *const TEMP;
      const CastemReal *const DTEMP;
      const CastemReal *const PROPS;
      const CastemReal *const PREDEF;
      const CastemReal *const DPRED;
      const CastemReal *const STATEV;
      const CastemReal *const STRESS;
      const tfel::material::OutOfBoundsPolicy policy;
      const StressFreeExpansionHandler sfeh;
	
    }; // end of struct IntegratorWithTimeStepping

    template<const bool bs,     // requires StiffnessTensor
	     const bool ba>     // requires ThermalExpansionCoefficientTensor
      struct TFEL_VISIBILITY_LOCAL Integrator
    {
      typedef typename std::conditional<bs,
				      StiffnessTensorInitializer,
				      DoNothingInitializer>::type SInitializer;

      typedef typename std::conditional<ba,
				      ThermalExpansionCoefficientTensorInitializer,
				      DoNothingInitializer>::type AInitializer;

      TFEL_CASTEM_INLINE Integrator(const CastemReal *const DTIME ,
				  const CastemReal *const STRAN ,
				  const CastemReal *const DSTRAN,
				  const CastemReal *const TEMP,
				  const CastemReal *const DTEMP,
				  const CastemReal *const PROPS ,
				  const CastemReal *const PREDEF,
				  const CastemReal *const DPRED,
				  const CastemReal *const STATEV,
				  const CastemReal *const STRESS,
				  const tfel::material::OutOfBoundsPolicy op,
				  const StressFreeExpansionHandler& sfeh)
	: behaviour(DTIME,TEMP,DTEMP,
		    PROPS+CastemTraits<BV>::propertiesOffset,
		    STATEV,PREDEF,DPRED),
	dt(*DTIME)
	  {
	    using namespace tfel::material;
	    typedef MechanicalBehaviourTraits<BV> Traits;
	    typedef typename std::conditional<
	      Traits::hasStressFreeExpansion,
	      DrivingVariableInitialiserWithStressFreeExpansion,
	      DrivingVariableInitialiserWithoutStressFreeExpansion>::type DVInitializer;
	    SInitializer::exe(this->behaviour,PROPS);
	    AInitializer::exe(this->behaviour,PROPS);
	    DVInitializer::exe(this->behaviour,STRAN,DSTRAN,sfeh);
	    this->behaviour.setCASTEMBehaviourDataThermodynamicForces(STRESS);
	    this->behaviour.initialize();
	    this->behaviour.setOutOfBoundsPolicy(op);
	  } // end of Integrator::Integrator
	
      TFEL_CASTEM_INLINE2
	void exe(CastemReal *const DDSOE,
		 CastemReal *const STRESS,
		 CastemReal *const STATEV)
      {
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename std::conditional<
	  Traits::isConsistentTangentOperatorSymmetric,
	  SymmetricConsistentTangentOperatorComputer,
	  GeneralConsistentTangentOperatorComputer>::type
	  ConsistentTangentOperatorHandler;
	typedef typename std::conditional<
	  Traits::hasPredictionOperator,
	  StandardPredictionOperatorComputer,
	  PredictionOperatorIsNotAvalaible
	  >::type PredictionOperatorComputer;
	if(this->dt<0.){
	  throwNegativeTimeStepException(Traits::getName());
	}
	this->behaviour.checkBounds();
	typename BV::IntegrationResult r = BV::SUCCESS;
	const typename BV::SMFlag smflag = CastemTangentOperatorFlag<CastemTraits<BV>::btype>::value;
	if((-3.25<*DDSOE)&&(*DDSOE<-2.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,smflag,
					      BV::TANGENTOPERATOR);
	} else if((-2.25<*DDSOE)&&(*DDSOE<-1.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,smflag,
					      BV::SECANTOPERATOR);
	} else if((-1.25<*DDSOE)&&(*DDSOE<-0.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,smflag,
					      BV::ELASTIC);
	} else if((-0.25<*DDSOE)&&(*DDSOE<0.25)){
	  r = this->behaviour.integrate(smflag,BV::NOSTIFFNESSREQUESTED);
	} else if((0.75<*DDSOE)&&(*DDSOE<1.25)){
	  r = this->behaviour.integrate(smflag,BV::ELASTIC);
	} else if((1.75<*DDSOE)&&(*DDSOE<2.25)){
	  r = this->behaviour.integrate(smflag,BV::SECANTOPERATOR);
	} else if((2.75<*DDSOE)&&(*DDSOE<3.25)){
	  r = this->behaviour.integrate(smflag,BV::TANGENTOPERATOR);
	} else if((3.75<*DDSOE)&&(*DDSOE<4.25)){
	  r = this->behaviour.integrate(smflag,BV::CONSISTENTTANGENTOPERATOR);
	} else {
	  throwInvalidDDSOEException(Traits::getName(),*DDSOE);
	}
	if(r==BV::FAILURE){
	  // Il manque un vraie gestion locale de résultats imprécis
	  if(*DDSOE<-0.5){
	    throwPredictionComputationFailedException(Traits::getName());
	  } else {
	    throwBehaviourIntegrationFailedException(Traits::getName());
	  }
	}
	// if(this->behaviour.integrate(BV::NOSTIFFNESSREQUESTED)==BV::FAILURE){
	//   throwBehaviourIntegrationFailedException(Traits::getName());
	// }
	behaviour.checkBounds();
	this->behaviour.CASTEMexportStateData(STRESS,STATEV);
	if((*DDSOE>0.5)||(*DDSOE<-0.5)){
	  ConsistentTangentOperatorHandler::exe(this->behaviour,DDSOE);
	}
      } // end of Integrator::exe
	
    private:
      typedef Behaviour<H,CastemReal,false> BV;
      BV behaviour;
      CastemReal dt;
    }; // end of struct Integrator

    struct StandardPredictionOperatorComputer
    {
      typedef Behaviour<H,CastemReal,false> BV;
      static typename BV::IntegrationResult
      exe(BV& b,const typename BV::SMFlag smf,const typename BV::SMType smt)
      {
	return b.computePredictionOperator(smf,smt);
      } // end of exe	  
    };

    struct PredictionOperatorIsNotAvalaible
    {
      typedef Behaviour<H,CastemReal,false> BV;
      static typename BV::IntegrationResult
      exe(BV&,const typename BV::SMFlag,
	  const typename BV::SMType)
      {
	typedef Behaviour<H,CastemReal,false> BV;
	typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	throwPredictionOperatorIsNotAvalaible(Traits::getName());
	return BV::FAILURE;
      } // end of exe	  
    };
      
    struct ConsistentTangentOperatorIsNotAvalaible
    {
      typedef Behaviour<H,CastemReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(BV&,CastemReal *const)
      {
	throwConsistentTangentOperatorIsNotAvalaible(Traits::getName());
      } // end of exe	  
    };

    struct ConsistentTangentOperatorComputer
    {
      typedef Behaviour<H,CastemReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(const BV& bv,CastemReal *const DDSOE)
      {
	typedef typename CastemTangentOperatorType<CastemTraits<BV>::btype,N>::type TangentOperatorType;
	TangentOperatorType& Dt = *(reinterpret_cast<TangentOperatorType*>(DDSOE));
	Dt = bv.getTangentOperator();
	// l'opérateur tangent contient des sqrt(2) en petites et grandes transformations...
	CastemTangentOperator::normalize(Dt);
      } // end of exe	  
    };

    struct SymmetricConsistentTangentOperatorComputer
    {
      typedef Behaviour<H,CastemReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(const BV& bv,CastemReal *const DDSOE)
      {
	ConsistentTangentOperatorComputer::exe(bv,DDSOE);
      } // end of exe	  
    };

    struct GeneralConsistentTangentOperatorComputer
    {
      typedef Behaviour<H,CastemReal,false> BV;
      const static unsigned short N = 
	tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
      static void exe(const BV& bv,CastemReal *const DDSOE)
      {
	using namespace tfel::math;
	typedef typename CastemTangentOperatorType<CastemTraits<BV>::btype,N>::type TangentOperatorType;
	ConsistentTangentOperatorComputer::exe(bv,DDSOE);
	TangentOperatorType& Dt = *(reinterpret_cast<TangentOperatorType*>(DDSOE));
	// les conventions fortran....
	CastemTangentOperator::transpose(Dt);
      } // end of exe	  
    };

    TFEL_CASTEM_INLINE2 static void
      checkNPROPS(const CastemInt NPROPS)
    {
      using namespace std;
      using namespace tfel::material;
      typedef Behaviour<H,CastemReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset  = CastemTraits<BV>::propertiesOffset;
      const unsigned short nprops  = CastemTraits<BV>::material_properties_nb;
      const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
      const bool is_defined_       = Traits::is_defined;
      // Test if the nb of properties matches Behaviour requirements
      if((NPROPS!=NPROPS_)&&is_defined_){
      	throwUnMatchedNumberOfMaterialProperties(Traits::getName(),
      						 NPROPS_,NPROPS);
      }
    } // end of checkNPROPS
      
    TFEL_CASTEM_INLINE2 static void
      checkNSTATV(const CastemInt NSTATV)
    {
      typedef Behaviour<H,CastemReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short nstatv  = Traits::internal_variables_nb;
      const unsigned short NSTATV_ = nstatv == 0 ? 1u : nstatv;
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of state variables matches Behaviour requirements
      if((NSTATV_!=NSTATV)&&is_defined_){
	throwUnMatchedNumberOfStateVariables(Traits::getName(),
					     NSTATV_,NSTATV);
      }
    } // end of checkNSTATV
      
  }; // end of struct CastemBehaviourHandler
  
} // end of namespace castem  

#include"MFront/Castem/CastemIsotropicBehaviourHandler.hxx"
#include"MFront/Castem/CastemOrthotropicBehaviourHandler.hxx"

#endif /* LIB_MFRONT_CASTEM_CASTEMBEHAVIOURHANDLER_H_ */

