/*! 
 * \file  UMATGenericPlaneStressHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 */

#ifndef _LIB_MFRONT_UMAT_UMATGENERICPLANESTRESSHANDLER_H_
#define _LIB_MFRONT_UMAT_UMATGENERICPLANESTRESSHANDLER_H_ 

#ifndef _LIB_MFRONT_UMAT_CALL_H_
#error "This header shall not be called directly"
#endif

namespace umat
{

  /*!
   * \brief Generic handler for the plane stress hypothesis
   * 
   * This allows behaviours written for the generalised plane stress
   * hypothesis to be used in plane stress computations.
   */
  template<template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct UMATGenericPlaneStressHandler
    : public UMATInterfaceBase
  {
    
    /*!
     * Main entry point This mainly choose between a specific
     * handler. The choice is made from the nature of elastic
     * behaviour (isotropic or not).
     */
    TFEL_UMAT_INLINE2 static
    void exe(const UMATReal *const DTIME,
	     const UMATReal *const DROT,  const UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS)
    {
      using namespace tfel::meta;
      using namespace tfel::material;
      const ModellingHypothesis::Hypothesis H = ModellingHypothesis::GENERALISEDPLANESTRAIN;
      typedef UMATTraits<Behaviour<H,UMATReal,false> > Traits;
      typedef typename IF<Traits::type==umat::ISOTROPIC,
			  UMATIsotropicBehaviourHandler<H,Behaviour>,
			  UMATOrthotropicBehaviourHandler<H,Behaviour> >::type Treat;
	// generalised plane stress hypothesis
      const UMATInt NDI = -3;
      Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		 PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		 STRESS,&NDI);
	} // end of exe
    
  private:
    
    TFEL_UMAT_INLINE2 static void
    checkNSTATV(const UMATInt NSTATV)
    {
      using namespace tfel::utilities;
      using namespace tfel::material;
      const ModellingHypothesis::Hypothesis H = ModellingHypothesis::GENERALISEDPLANESTRAIN;
      typedef Behaviour<H,UMATReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short NSTATV_  = Traits::internal_variables_nb+1u;
      const bool is_defined_        = Traits::is_defined;
      //Test if the nb of state variables matches Behaviour requirements
      if((NSTATV_!=NSTATV)&&is_defined_){
	throwUnMatchedNumberOfStateVariables(Name<BV>::getName(),NSTATV_,NSTATV);
      }
    } // end of checkNSTATV

    template<typename TreatPlaneStrain>
    TFEL_UMAT_INLINE2 static
    void exe(const UMATReal c1,
	     const UMATReal c2,
	     const UMATReal c3,
	     const UMATReal *const DTIME,
	     const UMATReal *const DROT,  const UMATReal *const DDSOE,
	     const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	     const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	     const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	     const UMATReal *const PREDEF,const UMATReal *const DPRED,
	     UMATReal *const STATEV,const UMATInt  *const NSTATV,
	     UMATReal *const STRESS)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::material;
      const ModellingHypothesis::Hypothesis H = ModellingHypothesis::GENERALISEDPLANESTRAIN;
      using tfel::fsalgo::copy;
      typedef Behaviour<H,UMATReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short NSTATV_  = Traits::internal_variables_nb+1u;
      UMATGenericPlaneStressHandler::checkNSTATV(*NSTATV);
      unsigned int iter;
      const unsigned int iterMax = 50;
	
      UMATReal eto[4];
      UMATReal deto[4];
      UMATReal s[4];
      UMATReal v[NSTATV_];
      UMATReal dez;
      UMATReal x[2];
      UMATReal f[2];

      dez = -c3*s[2]+c1*DSTRAN[0]+c2*DSTRAN[1];
      UMATGenericPlaneStressHandler::template iter<TreatPlaneStrain>(DTIME,DROT,DDSOE,
							TEMP,DTEMP,PROPS,
							NPROPS,PREDEF,DPRED,
							STATEV,STRESS,
							STRAN,DSTRAN,dez,
							v,s,eto,deto);
      x[1] = dez;
      f[1] = s[2];
	
      if(abs(c3*s[2]) > 1.e-12){
	dez -= c3*s[2];
	UMATGenericPlaneStressHandler::template iter<TreatPlaneStrain>(DTIME,DROT,DDSOE,
							  TEMP,DTEMP,PROPS,
							  NPROPS,PREDEF,DPRED,
							  STATEV,STRESS,
							  STRAN,DSTRAN,dez,
							  v,s,eto,deto);
      }	    
	
      iter = 2;
      while((abs(c3*s[2]) > 1.e-12)&&
	    (iter<iterMax)){
	x[0] = x[1];
	f[0] = f[1];
	x[1] = dez;
	f[1] = s[2];
	dez -= (x[1]-x[0])/(f[1]-f[0])*s[2];
	UMATGenericPlaneStressHandler::template iter<TreatPlaneStrain>(DTIME,DROT,DDSOE,
							  TEMP,DTEMP,PROPS,
							  NPROPS,PREDEF,DPRED,
							  STATEV,STRESS,
							  STRAN,DSTRAN,dez,
							  v,s,eto,deto);
	++iter;
      }
      if(iter==iterMax){
	throwPlaneStressMaximumNumberOfIterationsReachedException(Name<BV>::getName());
      }
      copy<4>::exe(s,STRESS);
      copy<NSTATV_>::exe(v,STATEV);
      STATEV[*NSTATV-1] += dez;
    } // end of exe
      
    template<typename TreatPlaneStrain>
    TFEL_UMAT_INLINE2 static
    void iter(const UMATReal *const DTIME,
	      const UMATReal *const DROT,
	      const UMATReal *const DDSOE,
	      const UMATReal *const TEMP,
	      const UMATReal *const DTEMP,
	      const UMATReal *const PROPS,
	      const UMATInt  *const NPROPS,
	      const UMATReal *const PREDEF,
	      const UMATReal *const DPRED,
	      const UMATReal *const STATEV,
	      const UMATReal *const STRESS,
	      const UMATReal *const STRAN,
	      const UMATReal *const DSTRAN,
	      const UMATReal dez,
	      UMATReal *const v,
	      UMATReal *const s,
	      UMATReal *const eto,
	      UMATReal *const deto)
    {
      using namespace tfel::material;
      using tfel::fsalgo::copy;
      const ModellingHypothesis::Hypothesis H = ModellingHypothesis::GENERALISEDPLANESTRAIN;
      typedef Behaviour<H,UMATReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const UMATInt nNSTATV = Traits::internal_variables_nb;
      const UMATInt NDI = -1;
      const unsigned short NSTATV_  = Traits::internal_variables_nb+1u;
	
      copy<4>::exe(STRESS,s);
      copy<4>::exe(STRAN,eto);
      copy<4>::exe(DSTRAN,deto);
      eto[2]  = STATEV[NSTATV_-1];
      deto[2] = dez;
      copy<NSTATV_>::exe(STATEV,v);
	
      TreatPlaneStrain::exe(DTIME,DROT,DDSOE,eto,deto,TEMP,DTEMP,
			    PROPS,NPROPS,PREDEF,DPRED,v,
			    &nNSTATV,s,&NDI);
    }
    
    struct TreatPlaneStressIsotropicBehaviour
    {
	
      TFEL_UMAT_INLINE2 static
      void exe(const UMATReal *const DTIME,
	       const UMATReal *const DROT,  const UMATReal *const DDSOE,
	       const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	       const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,const UMATReal *const DPRED,
	       UMATReal *const STATEV,const UMATInt  *const NSTATV,
	       UMATReal *const STRESS)
      {
	using namespace tfel::material;
	const ModellingHypothesis::Hypothesis H = ModellingHypothesis::GENERALISEDPLANESTRAIN;
	typedef UMATIsotropicBehaviourHandler<H,Behaviour> BehaviourHandler;
	const UMATReal y = PROPS[0]; // Young Modulus
	const UMATReal n = PROPS[1]; // Poisson ratio
	const UMATReal c1 = -n/(1-n);
	const UMATReal c3 = 1/y;
	UMATGenericPlaneStressHandler::template exe<BehaviourHandler>(c1,c1,c3,
									DTIME,DROT,DDSOE,
									STRAN,DSTRAN,TEMP,
									DTEMP,PROPS,NPROPS,
									PREDEF,DPRED,STATEV,
									NSTATV,STRESS);
      } // end of exe
    }; // end of struct TreatPlanStressIsotropicBehaviour

    struct TreatPlaneStressOrthotropicBehaviour
    {
      TFEL_UMAT_INLINE2 static
      void exe(const UMATReal *const DTIME,
	       const UMATReal *const DROT,  const UMATReal *const DDSOE,
	       const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	       const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,const UMATReal *const DPRED,
	       UMATReal *const STATEV,const UMATInt  *const NSTATV,
	       UMATReal *const STRESS)
      {
	using namespace tfel::meta;
	using namespace tfel::material;
	const ModellingHypothesis::Hypothesis H = ModellingHypothesis::GENERALISEDPLANESTRAIN;
	using tfel::fsalgo::copy;
	typedef Behaviour<H,UMATReal,false> BV;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef UMATOrthotropicBehaviourHandler<H,Behaviour> BehaviourHandler;
	const unsigned short offset  = UMATTraits<BV>::propertiesOffset;
	const unsigned short nprops  = Traits::material_properties_nb;
	const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
	UMATReal nPROPS[NPROPS_];
	nPROPS[0]  = PROPS[0];
	nPROPS[1]  = PROPS[1];
	nPROPS[2]  = PROPS[6];
	nPROPS[3]  = PROPS[2];
	nPROPS[4]  = PROPS[7];
	nPROPS[5]  = PROPS[8];
	nPROPS[6]  = PROPS[3];
	nPROPS[7]  = PROPS[4];
	nPROPS[8]  = PROPS[5];
	nPROPS[9]  = PROPS[9];
	nPROPS[10]  = PROPS[10];
	nPROPS[11]  = PROPS[11];
	nPROPS[12] = UMATReal(0);
	copy<nprops>::exe(PROPS+13,nPROPS+13);

	// S11 = 1/E1
	const UMATReal S11=1/nPROPS[0];
	// S22 = 1/E2
	const UMATReal S22=1/nPROPS[1];
	// S22 = 1/E3
	const UMATReal S33=1/nPROPS[2];
	// S12 = -n12/E1
	const UMATReal S12=-nPROPS[3]/nPROPS[0];
	// S13 = -n13/E1
	const UMATReal S13=-nPROPS[5]/nPROPS[0];
	// S23 = -n23/E2
	const UMATReal S23=-nPROPS[4]/nPROPS[1];
	const UMATReal detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
	const UMATReal C22=(S11*S22-S12*S12)/detS;
	const UMATReal C20=(S12*S23-S13*S22)/detS;
	const UMATReal C21=(S12*S13-S11*S23)/detS;
	// Plane stress condition leads to :
	// Dezz = - (1/C22)*(C20*Dexx+C21*Deyy)
	const UMATReal c1 = -C20/C22;
	const UMATReal c2 = -C21/C22;
	// calling the resolution
	UMATGenericPlaneStressHandler::template exe<BehaviourHandler>(c1,c2,S22,
									DTIME,DROT,DDSOE,
									STRAN,DSTRAN,TEMP,
									DTEMP,nPROPS,NPROPS,
									PREDEF,DPRED,STATEV,
									NSTATV,STRESS);
      } // end of exe
    }; // end of struct TreatPlanStressOrthotropicBehaviour

  }; // end of struct TreatPlaneStrain

} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_UMATGENERICPLANESTRESSHANDLER_H */

