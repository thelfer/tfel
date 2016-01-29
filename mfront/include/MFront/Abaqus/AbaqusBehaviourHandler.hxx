/*! 
 * \file  mfront/include/MFront/Abaqus/AbaqusBehaviourHandler.hxx
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

#ifndef LIB_MFRONT_ABAQUS_ABAQUSBEHAVIOURHANDLER_H_
#define LIB_MFRONT_ABAQUS_ABAQUSBEHAVIOURHANDLER_H_ 

#ifndef LIB_MFRONT_ABAQUS_CALL_H_
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_ABAQUS_CALL_H_ */

#include"TFEL/Math/Matrix/TMatrixView.hxx"
#include"TFEL/Math/T2toST2/T2toST2View.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include"TFEL/Material/MechanicalBehaviour.hxx"

#include"MFront/Abaqus/AbaqusTangentOperator.hxx"
#include"MFront/Abaqus/AbaqusInterfaceExceptions.hxx"
#include"MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx"
#include"MFront/Abaqus/AbaqusComputeThermalExpansionCoefficientTensor.hxx"

namespace abaqus
{

  template<AbaqusBehaviourType btype>
  struct AbaqusTangentOperatorFlag;

  template<>
  struct AbaqusTangentOperatorFlag<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR>
    TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template<>
  struct AbaqusTangentOperatorFlag<abaqus::FINITESTRAINSTANDARDBEHAVIOUR>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR>
    TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value = TangentOperatorTraits::ABAQUS;
  };

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AbaqusBehaviourHandler
    : public AbaqusInterfaceExceptions
  {

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithStressFreeExpansion
      : public AbaqusInterfaceExceptions
    {
      //! a simple alias
      typedef Behaviour<H,AbaqusReal,false> BV;
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_ABAQUS_INLINE static 
	void exe(BV& b,
		 const AbaqusReal *const STRAN,
		 const AbaqusReal *const DSTRAN,
		 const StressFreeExpansionHandler& sfeh)
      {
	using std::pair;
	using tfel::fsalgo::copy;
	using namespace tfel::material;
	typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
	typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	const AbaqusInt N = ModellingHypothesisToSpaceDimension<H>::value;
	AbaqusReal dv0[AbaqusTraits<BV>::DrivingVariableSize];
	AbaqusReal dv1[AbaqusTraits<BV>::DrivingVariableSize];
	copy<AbaqusTraits<BV>::DrivingVariableSize>::exe(STRAN,dv0);
	copy<AbaqusTraits<BV>::DrivingVariableSize>::exe(DSTRAN,dv1);
	// check that the function pointer are not null
	if(sfeh==nullptr){
	  throwUnsupportedStressFreeExpansionException(Traits::getName());
	}
	pair<StressFreeExpansionType,StressFreeExpansionType> s;
	b.computeStressFreeExpansion(s);
	const auto& s0 = s.first;
	const auto& s1 = s.second;
	sfeh(dv0,dv1,&s0[0],&s1[0],N);
	b.setABAQUSBehaviourDataDrivingVariables(dv0);
	b.setABAQUSIntegrationDataDrivingVariables(dv1);
      } // end of exe

    }; // end of struct DrivingVariableInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithoutStressFreeExpansion
    {
      //! a simple alias
      typedef Behaviour<H,AbaqusReal,false> BV;
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_ABAQUS_INLINE static 
	void exe(BV& b,
		 const AbaqusReal *const STRAN,
		 const AbaqusReal *const DSTRAN,
		 const StressFreeExpansionHandler&)
      {
	b.setABAQUSBehaviourDataDrivingVariables(STRAN);
	b.setABAQUSIntegrationDataDrivingVariables(DSTRAN);
      } // end of exe
    }; // end of struct DrivingVariableInitialiserWithoutStressFreeExpansion

    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer
    {
      typedef Behaviour<H,AbaqusReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ABAQUS_INLINE static void
	exe(BData& data,const AbaqusReal * const props){
	typedef AbaqusTraits<BV> Traits;
	const bool buas = Traits::requiresUnAlteredStiffnessTensor;
	AbaqusComputeStiffnessTensor<AbaqusTraits<BV>::btype,H,
				     AbaqusTraits<BV>::etype,buas>::exe(data.getStiffnessTensor(),
									props);
      } // end of exe
    }; // end of struct StiffnessOperatorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer
    {
      typedef Behaviour<H,AbaqusReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ABAQUS_INLINE static void
	exe(BData& data,const AbaqusReal * const props){
	const unsigned short o =
	  AbaqusTraits<BV>::elasticPropertiesOffset;
	AbaqusComputeThermalExpansionCoefficientTensor<AbaqusTraits<BV>::btype,H,
						       AbaqusTraits<BV>::stype>::exe(props+o,
										     data.getThermalExpansionCoefficientTensor());
      } // end of exe
    }; // end of struct ThermalExpansionCoefficientTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer
    {
      typedef Behaviour<H,AbaqusReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ABAQUS_INLINE static void
	exe(BData&,const AbaqusReal * const)
      {}
    }; // end of struct DoNothingInitializer

    template<const bool bs,     // requires StiffnessOperator
	     const bool ba>     // requires ThermalExpansionCoefficientTensor
      struct TFEL_VISIBILITY_LOCAL Integrator
    {
      typedef typename std::conditional<bs,StiffnessOperatorInitializer,
					DoNothingInitializer>::type SInitializer;

      typedef typename std::conditional<ba,ThermalExpansionCoefficientTensorInitializer,
					DoNothingInitializer>::type AInitializer;

      TFEL_ABAQUS_INLINE Integrator(const AbaqusReal *const DTIME ,
				    const AbaqusReal *const STRAN ,
				    const AbaqusReal *const DSTRAN,
				    const AbaqusReal *const TEMP,
				    const AbaqusReal *const DTEMP,
				    const AbaqusReal *const PROPS ,
				    const AbaqusReal *const PREDEF,
				    const AbaqusReal *const DPRED,
				    const AbaqusReal *const STATEV,
				    const AbaqusReal *const STRESS,
				    const AbaqusReal *const DROT,
				    const tfel::material::OutOfBoundsPolicy op,
				    const StressFreeExpansionHandler& sfeh)
	: behaviour(DTIME,TEMP,DTEMP,
		    PROPS+AbaqusTraits<BV>::elasticPropertiesOffset+
		    AbaqusTraits<BV>::thermalExpansionPropertiesOffset,
		    STATEV,PREDEF,DPRED,DROT),
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
	    this->behaviour.setABAQUSBehaviourDataThermodynamicForces(STRESS,DROT);
	    this->behaviour.setOutOfBoundsPolicy(op);
	    this->behaviour.initialize();
	  } // end of Integrator::Integrator
	
      TFEL_ABAQUS_INLINE2
	void exe(AbaqusReal *const PNEWDT,
		 AbaqusReal *const DDSDDE,
		 AbaqusReal *const STRESS,
		 AbaqusReal *const STATEV)
      {
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename std::conditional<
	  Traits::hasConsistentTangentOperator,
	  ExtractAndConvertTangentOperator<H>,
	  ConsistentTangentOperatorIsNotAvalaible
	  >::type ConsistentTangentOperatorHandler;
	if(this->dt<0.){
	  throwNegativeTimeStepException(Traits::getName());
	}
	this->behaviour.checkBounds();
	typename BV::IntegrationResult r = BV::SUCCESS;
	const typename BV::SMFlag smflag = AbaqusTangentOperatorFlag<AbaqusTraits<BV>::btype>::value;
	auto tsf = behaviour.computeAPrioriTimeStepScalingFactor();
	*PNEWDT = tsf.second;
	if(!tsf.first){
	  r = BV::FAILURE;
	} else {
	  try{
	    r = this->behaviour.integrate(smflag,BV::CONSISTENTTANGENTOPERATOR);
	  } catch(DivergenceException&){
	    r==BV::FAILURE;
	  }
	  if(r==BV::FAILURE){
	    *PNEWDT = behaviour.getMinimalTimeStepScalingFactor();
	  } else {
	    tsf = behaviour.computeAPosterioriTimeStepScalingFactor();
	    if(!tsf.first){
	      r=BV::FAILURE;
	    }
	    *PNEWDT = std::min(tsf.second,*PNEWDT);
	  }
	}
	if(r==BV::FAILURE){
	  return;
	}
	this->behaviour.checkBounds();
	this->behaviour.ABAQUSexportStateData(STRESS,STATEV);
	ConsistentTangentOperatorHandler::exe(this->behaviour,DDSDDE);
      } // end of Integrator::exe
	
    private:
      
      struct ConsistentTangentOperatorIsNotAvalaible
      {
	typedef Behaviour<H,AbaqusReal,false> BV;
	static void exe(BV&,AbaqusReal *const)
	{
	  typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	  throwConsistentTangentOperatorIsNotAvalaible(Traits::getName());
	} // end of exe	  
      };

      typedef Behaviour<H,AbaqusReal,false> BV;
      BV behaviour;
      AbaqusReal dt;
    }; // end of struct Integrator

    TFEL_ABAQUS_INLINE2 static void
      checkNPROPS(const AbaqusInt NPROPS)
    {
      using namespace std;
      using namespace tfel::material;
      typedef Behaviour<H,AbaqusReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset  = (AbaqusTraits<BV>::elasticPropertiesOffset+
				      AbaqusTraits<BV>::thermalExpansionPropertiesOffset);
      const unsigned short nprops  = AbaqusTraits<BV>::material_properties_nb;
      const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of properties matches Behaviour requirements
      if((NPROPS!=NPROPS_)&&is_defined_){
	throwUnMatchedNumberOfMaterialProperties(Traits::getName(),
						 NPROPS_,NPROPS);
      }
    } // end of checkNPROPS
      
    TFEL_ABAQUS_INLINE2 static void
      checkNSTATV(const AbaqusInt NSTATV)
    {
      typedef Behaviour<H,AbaqusReal,false> BV;
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
      
  }; // end of struct AbaqusBehaviourHandler
  
} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSBEHAVIOURHANDLER_H_ */
