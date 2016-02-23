/*! 
 * \file  mfront/include/MFront/Europlexus/EuroplexusBehaviourHandler.hxx
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

#ifndef LIB_MFRONT_EPX_EUROPLEXUSBEHAVIOURHANDLER_H_
#define LIB_MFRONT_EPX_EUROPLEXUSBEHAVIOURHANDLER_H_ 

#ifndef LIB_MFRONT_EPX_CALL_H_
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_EPX_CALL_H_ */

#include"TFEL/Math/Matrix/TMatrixView.hxx"
#include"TFEL/Math/T2toST2/T2toST2View.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include"TFEL/Material/MechanicalBehaviour.hxx"

#include"MFront/Europlexus/EuroplexusInterfaceExceptions.hxx"
#include"MFront/Europlexus/EuroplexusComputeStiffnessTensor.hxx"
#include"MFront/Europlexus/EuroplexusComputeThermalExpansionCoefficientTensor.hxx"

namespace epx
{

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL EuroplexusBehaviourHandler
    : public EuroplexusInterfaceExceptions
  {

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithStressFreeExpansion
      : public EuroplexusInterfaceExceptions
    {
      //! a simple alias
      typedef Behaviour<H,EuroplexusReal,false> BV;
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_EPX_INLINE static 
	void exe(BV& b,
		 const EuroplexusReal *const STRAN,
		 const EuroplexusReal *const DSTRAN,
		 const StressFreeExpansionHandler& sfeh)
      {
	using std::pair;
	using tfel::fsalgo::copy;
	using namespace tfel::material;
	typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
	typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	const EuroplexusInt N = ModellingHypothesisToSpaceDimension<H>::value;
	EuroplexusReal dv0[EuroplexusTraits<BV>::DrivingVariableSize];
	EuroplexusReal dv1[EuroplexusTraits<BV>::DrivingVariableSize];
	copy<EuroplexusTraits<BV>::DrivingVariableSize>::exe(STRAN,dv0);
	copy<EuroplexusTraits<BV>::DrivingVariableSize>::exe(DSTRAN,dv1);
	// check that the function pointer are not null
	if(sfeh==nullptr){
	  throwUnsupportedStressFreeExpansionException(Traits::getName());
	}
	pair<StressFreeExpansionType,StressFreeExpansionType> s;
	b.computeStressFreeExpansion(s);
	const auto& s0 = s.first;
	const auto& s1 = s.second;
	sfeh(dv0,dv1,&s0[0],&s1[0],N);
	b.setEPXBehaviourDataDrivingVariables(dv0);
	b.setEPXIntegrationDataDrivingVariables(dv1);
      } // end of exe

    }; // end of struct DrivingVariableInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithoutStressFreeExpansion
    {
      //! a simple alias
      typedef Behaviour<H,EuroplexusReal,false> BV;
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_EPX_INLINE static 
	void exe(BV& b,
		 const EuroplexusReal *const STRAN,
		 const EuroplexusReal *const DSTRAN,
		 const StressFreeExpansionHandler&)
      {
	b.setEPXBehaviourDataDrivingVariables(STRAN);
	b.setEPXIntegrationDataDrivingVariables(DSTRAN);
      } // end of exe
    }; // end of struct DrivingVariableInitialiserWithoutStressFreeExpansion

    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer
    {
      typedef Behaviour<H,EuroplexusReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_EPX_INLINE static void
	exe(BData& data,const EuroplexusReal * const props){
	typedef EuroplexusTraits<BV> Traits;
	const bool buas = Traits::requiresUnAlteredStiffnessTensor;
	EuroplexusComputeStiffnessTensor<EuroplexusTraits<BV>::btype,H,
				     EuroplexusTraits<BV>::etype,buas>::exe(data.getStiffnessTensor(),
									props);
      } // end of exe
    }; // end of struct StiffnessOperatorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer
    {
      typedef Behaviour<H,EuroplexusReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_EPX_INLINE static void
	exe(BData& data,const EuroplexusReal * const props){
	const unsigned short o =
	  EuroplexusTraits<BV>::elasticPropertiesOffset;
	EuroplexusComputeThermalExpansionCoefficientTensor<EuroplexusTraits<BV>::btype,H,
						       EuroplexusTraits<BV>::stype>::exe(props+o,
										     data.getThermalExpansionCoefficientTensor());
      } // end of exe
    }; // end of struct ThermalExpansionCoefficientTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer
    {
      typedef Behaviour<H,EuroplexusReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_EPX_INLINE static void
	exe(BData&,const EuroplexusReal * const)
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

      TFEL_EPX_INLINE Integrator(const EuroplexusReal *const DTIME ,
				    const EuroplexusReal *const STRAN ,
				    const EuroplexusReal *const DSTRAN,
				    const EuroplexusReal *const TEMP,
				    const EuroplexusReal *const DTEMP,
				    const EuroplexusReal *const PROPS ,
				    const EuroplexusReal *const PREDEF,
				    const EuroplexusReal *const DPRED,
				    const EuroplexusReal *const STATEV,
				    const EuroplexusReal *const STRESS,
				    const tfel::material::OutOfBoundsPolicy op,
				    const StressFreeExpansionHandler& sfeh)
	: behaviour(DTIME,TEMP,DTEMP,
		    PROPS+EuroplexusTraits<BV>::elasticPropertiesOffset+
		    EuroplexusTraits<BV>::thermalExpansionPropertiesOffset,
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
	    this->behaviour.setEPXBehaviourDataThermodynamicForces(STRESS);
	    this->behaviour.setOutOfBoundsPolicy(op);
	    this->behaviour.initialize();
	  } // end of Integrator::Integrator
	
      TFEL_EPX_INLINE2
	void exe(EuroplexusReal *const PNEWDT,
		 EuroplexusReal *const STRESS,
		 EuroplexusReal *const STATEV)
      {
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	if(this->dt<0.){
	  throwNegativeTimeStepException(Traits::getName());
	}
	this->behaviour.checkBounds();
	typename BV::IntegrationResult r = BV::SUCCESS;
	auto tsf = behaviour.computeAPrioriTimeStepScalingFactor(*PNEWDT);
	*PNEWDT = tsf.second;
	if(!tsf.first){
	  r = BV::FAILURE;
	} else {
	  try{
	    r = this->behaviour.integrate(smflag,BV::NOSTIFFNESS);
	  } catch(DivergenceException&){
	    r=BV::FAILURE;
	  }
	  if(r==BV::FAILURE){
	    *PNEWDT = behaviour.getMinimalTimeStepScalingFactor();
	  } else {
	    tsf = behaviour.computeAPosterioriTimeStepScalingFactor(*PNEWDT);
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
	this->behaviour.EPXexportStateData(STRESS,STATEV);
      } // end of Integrator::exe
	
    private:
      
      typedef Behaviour<H,EuroplexusReal,false> BV;
      BV behaviour;
      EuroplexusReal dt;
    }; // end of struct Integrator

    TFEL_EPX_INLINE2 static void
      checkNPROPS(const EuroplexusInt NPROPS)
    {
      using namespace std;
      using namespace tfel::material;
      typedef Behaviour<H,EuroplexusReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset  = (EuroplexusTraits<BV>::elasticPropertiesOffset+
				      EuroplexusTraits<BV>::thermalExpansionPropertiesOffset);
      const unsigned short nprops  = EuroplexusTraits<BV>::material_properties_nb;
      const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of properties matches Behaviour requirements
      if((NPROPS!=NPROPS_)&&is_defined_){
	throwUnMatchedNumberOfMaterialProperties(Traits::getName(),
						 NPROPS_,NPROPS);
      }
    } // end of checkNPROPS
      
    TFEL_EPX_INLINE2 static void
      checkNSTATV(const EuroplexusInt NSTATV)
    {
      typedef Behaviour<H,EuroplexusReal,false> BV;
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
      
  }; // end of struct EuroplexusBehaviourHandler
  
} // end of namespace epx

#endif /* LIB_MFRONT_EPX_EUROPLEXUSBEHAVIOURHANDLER_H_ */
