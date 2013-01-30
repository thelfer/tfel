/*! 
 * \file  AsterIsotropicBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 */

#ifndef _LIB_MFRONT_ASTER_ASTERISOTROPICBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_ASTER_ASTERISOTROPICBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H_
#error "This header shall not be called directly"
#endif

namespace aster
{

  template<unsigned short N,
	   template<unsigned short,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterIsotropicBehaviourHandler
    : public AsterBehaviourHandler<N,Behaviour>
  {
    TFEL_ASTER_INLINE static
      void exe(const AsterReal *const DTIME ,
	       const AsterReal *const,
	       AsterReal *const DDSOE,
	       const AsterReal *const STRAN ,
	       const AsterReal *const DSTRAN,
	       const AsterReal *const TEMP  ,
	       const AsterReal *const DTEMP,
	       const AsterReal *const PROPS ,
	       const AsterInt  *const NPROPS,
	       const AsterReal *const PREDEF,
	       const AsterReal *const DPRED,
	       AsterReal *const STATEV,
	       const AsterInt  *const NSTATV,
	       AsterReal *const STRESS) 
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef MechanicalBehaviourTraits<Behaviour<N,AsterReal,false> > MTraits;
      typedef AsterTraits<Behaviour<N,AsterReal,false> > Traits;
      typedef AsterBehaviourHandler<N,Behaviour> AsterBehaviourHandler;
      const bool is_defined_ = MTraits::is_defined;
      const bool bs = Traits::requiresStiffnessTensor;
      const bool ba = Traits::requiresThermalExpansionTensor;
      typedef typename IF<
	is_defined_,
	typename AsterBehaviourHandler::template Integrator<bs,ba>,
	typename AsterBehaviourHandler::Error>::type Handler;
      AsterBehaviourHandler::checkNPROPS(*NPROPS);
      AsterBehaviourHandler::checkNSTATV(*NSTATV);
      Handler handler(DTIME,STRAN,
		      DSTRAN,TEMP,DTEMP,PROPS,
		      PREDEF,DPRED,STATEV,STRESS);
      handler.exe(DDSOE,STRESS,STATEV);
    } // end of AsterIsotropicBehaviourHandler::exe
    
  }; // end of struct AsterIsotropicBehaviourHandler
  
} // end of namespace aster

#endif /* _LIB_MFRONT_ASTER_ASTERISOTROPICBEHAVIOURHANDLER_H */

