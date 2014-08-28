/*! 
 * \file  mfront/include/MFront/Cyrano/CyranoIsotropicBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANO_CYRANOISOTROPICBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_CYRANO_CYRANOISOTROPICBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_H_
#error "This header shall not be called directly"
#endif

#include"TFEL/FSAlgorithm/copy.hxx"

namespace cyrano
{

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CyranoIsotropicBehaviourHandler
  {

    TFEL_CYRANO_INLINE static
    void exe(const CyranoReal *const DTIME ,
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
      typedef MechanicalBehaviourTraits<Behaviour<H,CyranoReal,false> > MTraits;
      typedef CyranoTraits<Behaviour<H,CyranoReal,false> > Traits;
      typedef CyranoBehaviourHandler<H,Behaviour> CyranoBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionCoefficientTensor;
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
      Handler handler(DTIME,STRAN,DSTRAN,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,STRESS);
      handler.exe(DDSOE,STRESS,STATEV);
    } // end of CyranoIsotropicBehaviourHandlerBase::exe
  };

}

#endif /* _LIB_MFRONT_CYRANO_CYRANOISOTROPICBEHAVIOURHANDLER_H */

