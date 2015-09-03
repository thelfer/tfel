/*! 
 * \file  mfront/include/MFront/Cyrano/CyranoInterfaceDispatch.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_CYRANO_CYRANOINTERFACEDISPATCH_H_
#define _LIB_MFRONT_CYRANO_CYRANOINTERFACEDISPATCH_H_ 

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Material/MechanicalBehaviourTraits.hxx"

#include"MFront/Cyrano/Cyrano.hxx"
#include"MFront/Cyrano/CyranoTraits.hxx"
#include"MFront/Cyrano/CyranoInterfaceBase.hxx"

#include"MFront/Cyrano/CyranoOutOfBoundsPolicy.hxx"
#include"MFront/Cyrano/CyranoComputeThermalExpansionCoefficientTensor.hxx"
#include"MFront/Cyrano/CyranoGetModellingHypothesis.hxx"
#include"MFront/Cyrano/CyranoBehaviourHandler.hxx"
#include"MFront/Cyrano/CyranoIsotropicBehaviourHandler.hxx"
#include"MFront/Cyrano/CyranoOrthotropicBehaviourHandler.hxx"
// #include"MFront/Cyrano/CyranoGenericPlaneStressHandler.hxx"

namespace cyrano{

  /*!
   * \class  CyranoInterfaceDispatch
   * \brief  
   * \author Helfer Thomas
   * \date   24 Jul 2013
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct CyranoInterfaceDispatch
    : public CyranoInterfaceBase
  {
    TFEL_CYRANO_INLINE2 static
    void exe(const CyranoReal *const DTIME,
	     const CyranoReal *const DROT,  CyranoReal *const DDSOE,
	     const CyranoReal *const STRAN, const CyranoReal *const DSTRAN,
	     const CyranoReal *const TEMP,  const CyranoReal *const DTEMP,
	     const CyranoReal *const PROPS, const CyranoInt  *const NPROPS,
	     const CyranoReal *const PREDEF,const CyranoReal *const DPRED,
	     CyranoReal *const STATEV,const CyranoInt  *const NSTATV,
	     CyranoReal *const STRESS,
	      const tfel::material::OutOfBoundsPolicy op)
    {
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::material;
      typedef Behaviour<H,CyranoReal,false> BV;
      //! a simple alias
      typedef CyranoTraits<BV> Traits;
      typedef typename IF<Traits::stype==cyrano::ISOTROPIC,
			  CyranoIsotropicBehaviourHandler<H,Behaviour>,
			  CyranoOrthotropicBehaviourHandler<H,Behaviour> >::type Handler;
      Handler::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		   PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		   STRESS,op);
    } // end of exe
  }; // end of struct CyranoInterfaceDispatch

} // end of namespace cyrano

#endif /* _LIBMFRONT_CYRANO_CYRANOINTERFACEDISPATCH_H */
