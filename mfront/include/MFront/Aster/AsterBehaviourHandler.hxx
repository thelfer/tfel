/*! 
 * \file  AsterBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 */

#ifndef _LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_ASTER_CALL_H_
#error "This header shall not be called directly"
#endif

namespace aster
{

  template<unsigned short N,
	   template<unsigned short,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterBehaviourHandler
    : public AsterInterfaceBase
  {

    struct TFEL_VISIBILITY_LOCAL StiffnessTensorInitializer
    {
      typedef Behaviour<N,AsterReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ASTER_INLINE static void
	exe(BData& data,const AsterReal * const props){
#warning "stiffness"
	// AsterComputeStiffnessTensor<N,AsterTraits<BV>::type>::exe(props,
	// 							  data.getStiffnessTensor());
      } // end of exe
    }; // end of struct StiffnessTensorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionTensorInitializer
    {
      typedef Behaviour<N,AsterReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ASTER_INLINE static void
	exe(BData& data,const AsterReal * const props){
#warning "thermal expansion"
	// AsterComputeThermalExpansionTensor<N,AsterTraits<BV>::type>::exe(props,
	// 								 data.getThermalExpansionTensor());
      } // end of exe
    }; // end of struct ThermalExpansionTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer
    {
      typedef Behaviour<N,AsterReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ASTER_INLINE static void
	exe(BData&,const AsterReal * const)
      {}
    }; // end of struct DoNothingInitializer

    struct TFEL_VISIBILITY_LOCAL Error
    {
	
      TFEL_ASTER_INLINE Error(const AsterReal *const,
			      const AsterReal *const,
			      const AsterReal *const,
			      const AsterReal *const,
			      const AsterReal *const,
			      const AsterReal *const,
			      const AsterReal *const,
			      const AsterReal *const,
			      const AsterReal *const,
			      const AsterReal *const,
			      const AsterReal *const,
			      const AsterReal *const)
      {} // end of Error
	
      TFEL_ASTER_INLINE void exe(AsterReal *const,
				 AsterReal *const)
	throw(AsterException)
      {
	using namespace std;
	using namespace tfel::utilities;
	throw(AsterInvalidDimension(Name<Behaviour<N,AsterReal,false> >::getName()));
	return;
      } // end of Error::exe
	
    }; // end of struct Error

    template<const bool bs,     // requires StiffnessTensor
	     const bool ba>     // requires ThermalExpansionTensor
      struct TFEL_VISIBILITY_LOCAL Integrator
    {
      typedef typename tfel::meta::IF<bs,
				      StiffnessTensorInitializer,
				      DoNothingInitializer>::type SInitializer;

      typedef typename tfel::meta::IF<ba,
				      ThermalExpansionTensorInitializer,
				      DoNothingInitializer>::type AInitializer;

      TFEL_ASTER_INLINE Integrator(const AsterReal *const DTIME ,
				   const AsterReal *const STRAN ,
				   const AsterReal *const DSTRAN,
				   const AsterReal *const TEMP,
				   const AsterReal *const DTEMP,
				   const AsterReal *const PROPS ,
				   const AsterReal *const PREDEF,
				   const AsterReal *const DPRED,
				   const AsterReal *const STATEV,
				   const AsterReal *const STRESS)
	: behaviour(DTIME,STRESS,STRAN,DSTRAN,TEMP,DTEMP,
		    PROPS+AsterTraits<BV>::propertiesOffset,
		    STATEV,PREDEF,DPRED,
#warning "modelling hypothesis"
		    tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS),
	// getModellingHypothesis(*NDI)),
	dt(*DTIME)
	  {
	    SInitializer::exe(this->behaviour,PROPS);
	    AInitializer::exe(this->behaviour,PROPS);
#warning "out of bounds policy"
	    // const AsterOutOfBoundsPolicy& up = AsterOutOfBoundsPolicy::getAsterOutOfBoundsPolicy();
	    // this->behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	  } // end of Integrator::Integrator
	
      TFEL_ASTER_INLINE2
	void exe(AsterReal *const DDSOE,
		 AsterReal *const STRESS,
		 AsterReal *const STATEV)
      {
	using namespace tfel::utilities;
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename tfel::meta::IF<
	  Traits::hasConsistantTangentOperator,
	  ConsistantTangentOperatorComputer,
	  ConsistantTangentOperatorIsNotAvalaible
	  >::type ConsistantTangentOperatorHandler;
	if(this->dt<0.){
	  throwNegativeTimeStepException(Name<Behaviour<N,AsterReal,false> >::getName());
	}
	this->behaviour.checkBounds();
	if(!this->behaviour.integrate(*DDSOE>0.5)){
	  throwBehaviourIntegrationFailedException(Name<Behaviour<N,AsterReal,false> >::getName());
	}
	this->behaviour.checkBounds();
	this->behaviour.AsterExportStateData(STRESS,STATEV);
#warning "voir avec Jean-Michel : conventions racine de 2"
#warning "voir avec Jean-Michel : rotation matrice tangente cohérente (repère propre,...)"
#warning "voir avec Jean-Michel : grande déf et matrice tangente cohérente"
	if(*DDSOE>0.5){
	  ConsistantTangentOperatorHandler::exe(this->behaviour,DDSOE);
	}
      } // end of Integrator::exe
	
    private:

      struct ConsistantTangentOperatorIsNotAvalaible
      {
	typedef Behaviour<N,AsterReal,false> BV;
	static void exe(BV&,AsterReal *const)
	{
	  using namespace tfel::utilities;
	  throwConsistantTangentOperatorIsNotAvalaible(Name<Behaviour<N,AsterReal,false> >::getName());
	} // end of exe	  
      };

      struct ConsistantTangentOperatorComputer
      {
	typedef Behaviour<N,AsterReal,false> BV;
	static void exe(const BV& bv,AsterReal *const DDSOE)
	{
	  using namespace tfel::math;
#warning "réfléchir aux racines de 2"
	  *(reinterpret_cast<st2tost2<N,AsterReal>*>(DDSOE)) = bv.getTangentOperator();
	} // end of exe	  
      };

      typedef Behaviour<N,AsterReal,false> BV;
      BV behaviour;
      AsterReal dt;
    }; // end of struct Integrator

    TFEL_ASTER_INLINE2 static void
      checkNPROPS(const AsterInt NPROPS)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::material;
      typedef Behaviour<N,AsterReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset  = AsterTraits<BV>::propertiesOffset;
      const unsigned short nprops  = Traits::material_properties_nb;
      const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of properties matches Behaviour requirements
      if((NPROPS!=NPROPS_)&&is_defined_){
	throwUnMatchedNumberOfMaterialProperties(Name<Behaviour<N,AsterReal,false> >::getName(),
						 NPROPS_,NPROPS);
      }
    } // end of checkNPROPS
      
    TFEL_ASTER_INLINE2 static void
      checkNSTATV(const AsterInt NSTATV)
    {
      using namespace tfel::utilities;
      typedef Behaviour<N,AsterReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short nstatv  = Traits::internal_variables_nb;
      const unsigned short NSTATV_ = nstatv == 0 ? 1u : nstatv;
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of state variables matches Behaviour requirements
      if((NSTATV_!=NSTATV)&&is_defined_){
	throwUnMatchedNumberOfStateVariables(Name<Behaviour<N,AsterReal,false> >::getName(),
					     NSTATV_,NSTATV);
      }
    } // end of checkNSTATV
      
  }; // end of struct AsterBehaviourHandler
  
} // end of namespace aster

#include"MFront/Aster/AsterIsotropicBehaviourHandler.hxx"
#include"MFront/Aster/AsterOrthotropicBehaviourHandler.hxx"

#endif /* _LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H */
