/*! 
 * \file  CyranoOrthotropicBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANO_CYRANOORTHOTROPICBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_CYRANO_CYRANOORTHOTROPICBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_H_
#error "This header shall not be called directly"
#endif

namespace cyrano
{

  //! forward declaration
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct CyranoOrthotropicSmallStrainBehaviourHandler1D;
  
  /*!
   * An helper structure to make an appropriate dispatch based on the
   * spatial dimension
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CyranoOrthotropicSmallStrainBehaviourDispatcher
  {
    //! a simple alias
    typedef tfel::material::ModellingHypothesisToSpaceDimension<H> ModellingHypothesisToSpaceDimension;
    // spatial dimension
    static const unsigned short N = ModellingHypothesisToSpaceDimension::value;
    // the dispatch
    typedef CyranoOrthotropicSmallStrainBehaviourHandler1D<H,Behaviour> type;
  }; // end of struct CyranoOrthotropicSmallStrainBehaviourDispatcher;
  
  /*!
   * The handler for orthotropic behaviours
   * By default, this is unsupported (finite strain behaviour, cohesive zone models)
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CyranoOrthotropicBehaviourHandler
    : public CyranoOrthotropicSmallStrainBehaviourDispatcher<H,Behaviour>::type
  {
    typedef typename CyranoOrthotropicSmallStrainBehaviourDispatcher<H,Behaviour>::type Handler;
    using Handler::exe;
  };

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CyranoOrthotropicSmallStrainBehaviourHandler1D
    : private CyranoBehaviourHandler<H,Behaviour>
  {
    TFEL_CYRANO_INLINE2 static 
      void exe(const CyranoReal *const DTIME,
	       const CyranoReal *const,
	       CyranoReal *const DDSOE,
	       const CyranoReal *const STRAN ,
	       const CyranoReal *const DSTRAN,
	       const CyranoReal *const TEMP  ,
	       const CyranoReal *const DTEMP,
	       const CyranoReal *const PROPS ,
	       const CyranoInt  *const NPROPS,
	       const CyranoReal *const PREDEF,
	       const CyranoReal *const DPRED,
	       CyranoReal *const STATEV,
	       const CyranoInt  *const NSTATV,
	       CyranoReal *const STRESS)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      using namespace tfel::math;
      typedef MechanicalBehaviourTraits<Behaviour<H,CyranoReal,false> > MTraits;
      typedef CyranoTraits<Behaviour<H,CyranoReal,false> > Traits;
      typedef CyranoBehaviourHandler<H,Behaviour> CyranoBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessOperator;
      const bool ba = Traits::requiresThermalExpansionTensor;
      typedef typename IF<
	is_defined_,
	typename IF<
	  Traits::useTimeSubStepping,
	  typename CyranoBehaviourHandler::template IntegratorWithTimeStepping<bs,ba>,
	  typename CyranoBehaviourHandler::template Integrator<bs,ba>
	  >::type,
	typename CyranoBehaviourHandler::Error>::type Handler;
      CyranoBehaviourHandler::checkNPROPS(*NPROPS);
      CyranoBehaviourHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME,STRAN,
		      DSTRAN,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,STRESS);
      handler.exe(DDSOE,STRESS,STATEV);
    } // end of CyranoOrthotropicBehaviourHander<1u,Behaviour>::exe

  }; // end of struct CyranoOrthotropicBehaviourHander<1u,Behaviour>

} // end of namespace cyrano

#endif /* _LIB_MFRONT_CYRANO_CYRANOORTHOTROPICBEHAVIOURHANDLER_H */

