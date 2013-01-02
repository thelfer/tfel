/*!
 * \file   UMATInterface.hxx
 * \brief  This file implements the UMATInterface class.
 * \author Helfer Thomas
 * \date   28 Jul 2006
 */

#ifndef _LIB_MFRONT_UMAT_CALL_H_
#define _LIB_MFRONT_UMAT_CALL_H_ 

#include<string>
#include<algorithm>
#include<vector>
#include<limits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Config/TFELTypes.hxx"

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Exception/TFELException.hxx"
#include"TFEL/FSAlgorithm/copy.hxx"

#include"TFEL/Utilities/Info.hxx"
#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Material/MechanicalBehaviourTraits.hxx"
#include"TFEL/Material/MechanicalBehaviourData.hxx"
#include"TFEL/Material/MaterialException.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATTraits.hxx"
#include"MFront/UMAT/UMATException.hxx"
#include"MFront/UMAT/UMATOutOfBoundsPolicy.hxx"
#include"MFront/UMAT/UMATOrthotropicBehaviour.hxx"
#include"MFront/UMAT/UMATComputeStiffnessTensor.hxx"
#include"MFront/UMAT/UMATComputeThermalExpansionTensor.hxx"
#include"MFront/UMAT/UMATRotationMatrix.hxx"
#include"MFront/UMAT/UMATGetModellingHypothesis.hxx"

namespace umat{

  /*!
   * \class  UMATInterfaceBase
   * \brief  Base class for umat interfaces
   * \author Helfer Thomas
   * \date   12/12/2011
   */
  struct TFEL_VISIBILITY_EXPORT UMATInterfaceBase
  {

    /*!
     * \brief throw an UMATException. This method shall be called when
     * the number of materials properties declared by the beahviour and the
     * number of  materials properties declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of material properties declared by the behaviour
     * \param[in] n2 : number of material properties declared by the interface
     */
    static void
    throwUnMatchedNumberOfMaterialProperties(const std::string&,
					     const unsigned short n1,
					     const UMATInt n2);
    
    /*!
     * \brief throw an UMATException. This method shall be called when
     * the number of state variables declared by the beahviour and the
     * number of state variables declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of state variables declared by the behaviour
     * \param[in] n2 : number of state variables declared by the interface
     */
    static void
    throwUnMatchedNumberOfStateVariables(const std::string&,
					 const unsigned short n1,
					 const UMATInt n2);

    /*!
     * \brief display the error message out of an UMATException to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the UMATException to be treated
     */
    static void
    treatUmatException(const std::string&,
		       const UMATException&);

    /*!
     * \brief display the error message out of a material exception to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the material exception to be treated
     */
    static void
    treatMaterialException(const std::string&,
			   const tfel::material::MaterialException&);
    
    /*!
     * \brief display the error message out of a generic tfel
     * exception to the standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the exception to be treated
     */
    static void
    treatTFELException(const std::string&,
		       const tfel::exception::TFELException&);

    /*!
     * \brief display the error message out of a generic standard
     * exception to the standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the exception to be treated
     */
    static void
    treatStandardException(const std::string&,
			   const std::exception&);

    /*!
     * \brief display the error message when an unknown exception is caught
     * \param[in] b : behaviour name
     */
    static void
    treatUnknownException(const std::string&);

    /*!
     * \brief throw an UMATException if the time step is negative
     * \param[in] b : behaviour name
     */
    static void
    throwNegativeTimeStepException(const std::string&);

    /*!
     * \brief throw an UMATException if the behaviour integration
     * failed
     * \param[in] b : behaviour name
     */
    static void
    throwBehaviourIntegrationFailedException(const std::string&);

    /*!
     * \brief throw an UMATException if the maximum number of sub
     * stepping has been reached
     * \param[in] b : behaviour name
     */
    static void
    throwMaximumNumberOfSubSteppingReachedException(const std::string&);

    /*!
     * \brief throw an UMATException if the maximum number of sub
     * stepping has been reached
     * \param[in] b : behaviour name
     */
    static void
    throwPlaneStressMaximumNumberOfIterationsReachedException(const std::string&);

  }; // end of struct UMATInterfaceBase

  /*!
   * \class  UMATInterface
   * \brief  This class create an interface between Behaviour and UMAT
   * \author Helfer Thomas
   * \date   28 Jul 2006
   */
  template<template<unsigned short,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL UMATInterface
    : protected UMATInterfaceBase
  {

    struct TreatPlaneStress
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
	typedef UMATTraits<Behaviour<1u,UMATReal,false> > Traits;
	typedef typename IF<Traits::type==umat::ISOTROPIC,
			    TreatPlaneStressIsotropicBehaviour,
			    TreatPlaneStressOrthotropicBehaviour>::type Treat;
	Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		   PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		   STRESS);
      } // end of exe

    private:

      TFEL_UMAT_INLINE2 static void
      checkNSTATV(const UMATInt NSTATV)
      {
	using namespace tfel::utilities;
	typedef Behaviour<2u,UMATReal,false> BV;
	typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	const unsigned short NSTATV_  = Traits::internal_variables_nb+1u;
	const bool is_defined_        = Traits::is_defined;
	//Test if the nb of state variables matches Behaviour requirements
	if((NSTATV_!=NSTATV)&&is_defined_){
	  throwUnMatchedNumberOfStateVariables(Name<Behaviour<2u,UMATReal,false> >::getName(),
					       NSTATV_,NSTATV);
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
	using tfel::fsalgo::copy;
	typedef Behaviour<2u,UMATReal,false> BV;
	typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	const unsigned short NSTATV_  = Traits::internal_variables_nb+1u;
	TreatPlaneStress::checkNSTATV(*NSTATV);
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
	TreatPlaneStress::template iter<TreatPlaneStrain>(DTIME,DROT,DDSOE,
							  TEMP,DTEMP,PROPS,
							  NPROPS,PREDEF,DPRED,
							  STATEV,STRESS,
							  STRAN,DSTRAN,dez,
							  v,s,eto,deto);
	x[1] = dez;
	f[1] = s[2];
	
	if(abs(c3*s[2]) > 1.e-12){
	  dez -= c3*s[2];
	  TreatPlaneStress::template iter<TreatPlaneStrain>(DTIME,DROT,DDSOE,
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
	  TreatPlaneStress::template iter<TreatPlaneStrain>(DTIME,DROT,DDSOE,
							    TEMP,DTEMP,PROPS,
							    NPROPS,PREDEF,DPRED,
							    STATEV,STRESS,
							    STRAN,DSTRAN,dez,
							    v,s,eto,deto);
	  ++iter;
	}
	if(iter==iterMax){
	  throwPlaneStressMaximumNumberOfIterationsReachedException(Name<Behaviour<2u,UMATReal,false> >::getName());
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
	using tfel::fsalgo::copy;
	typedef Behaviour<2u,UMATReal,false> BV;
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
	  const UMATReal y = PROPS[0]; // Young Modulus
	  const UMATReal n = PROPS[1]; // Poisson ratio
	  const UMATReal c1 = -n/(1-n);
	  const UMATReal c3 = 1/y;
	  TreatPlaneStress::template exe<TreatIsotropicBehaviour<2u> >(c1,c1,c3,
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
	  using tfel::fsalgo::copy;
	  typedef Behaviour<2u,UMATReal,false> BV;
	  typedef MechanicalBehaviourTraits<BV> Traits;
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
	  TreatPlaneStress::template exe<TreatOrthotropicBehaviour2D>(c1,c2,S22,
								      DTIME,DROT,DDSOE,
								      STRAN,DSTRAN,TEMP,
								      DTEMP,nPROPS,NPROPS,
								      PREDEF,DPRED,STATEV,
								      NSTATV,STRESS);
	} // end of exe
      }; // end of struct TreatPlanStressOrthotropicBehaviour

    }; // end of struct TreatPlaneStrain

    TFEL_UMAT_INLINE2 static
      void exe(const UMATInt  *const NTENS, const UMATReal *const DTIME,
	       const UMATReal *const DROT,  const UMATReal *const DDSOE,
	       const UMATReal *const STRAN, const UMATReal *const DSTRAN,
	       const UMATReal *const TEMP,  const UMATReal *const DTEMP,
	       const UMATReal *const PROPS, const UMATInt  *const NPROPS,
	       const UMATReal *const PREDEF,const UMATReal *const DPRED,
	       UMATReal *const STATEV,const UMATInt  *const NSTATV,
	       UMATReal *const STRESS,const UMATInt  *const NDI,
	       UMATInt  *const KINC)
    {
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::utilities;
      typedef unsigned short ushort;
      try{
	if(*NTENS==3){
	  typedef UMATTraits<Behaviour<1u,UMATReal,false> > Traits;
	  typedef typename IF<Traits::type==umat::ISOTROPIC,
			      TreatIsotropicBehaviour<1u>,
			      TreatOrthotropicBehaviour1D>::type Treat;
	  Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		     PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		     STRESS,NDI);
      } else if(*NTENS==4){
	  if(*NDI==-2){
	    // plane strain
	    TreatPlaneStress::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
				  PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
				  STRESS);
	  } else {
	    typedef UMATTraits<Behaviour<2u,UMATReal,false> > Traits;
	    typedef typename IF<Traits::type==umat::ISOTROPIC,
				TreatIsotropicBehaviour<2u>,
				TreatOrthotropicBehaviour2D>::type Treat;
	    Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		       PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		       STRESS,NDI);
	  }
	} else if(*NTENS==6){
	  typedef UMATTraits<Behaviour<3u,UMATReal,false> > Traits;
	  typedef typename IF<Traits::type==umat::ISOTROPIC,
			      TreatIsotropicBehaviour<3u>,
			      TreatOrthotropicBehaviour3D>::type Treat;
	  Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		     PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,STRESS,
		     NDI);
	} else {
	  throw(UMATInvalidNTENSValue(ushort(*NTENS)));
	}
      }
      catch(const UMATException& e){
	if(*NTENS==3){
	  treatUmatException(Name<Behaviour<1u,UMATReal,false> >::getName(),e);
	} else if(*NTENS==4){
	  treatUmatException(Name<Behaviour<2u,UMATReal,false> >::getName(),e);
	} else if(*NTENS==6){
	  treatUmatException(Name<Behaviour<3u,UMATReal,false> >::getName(),e);
	}
	*KINC = -2;
      }
      catch(const tfel::material::MaterialException& e){
	if(*NTENS==3){
	  treatMaterialException(Name<Behaviour<1u,UMATReal,false> >::getName(),e);
	} else if(*NTENS==4){
	  treatMaterialException(Name<Behaviour<2u,UMATReal,false> >::getName(),e);
	} else if(*NTENS==6){
	  treatMaterialException(Name<Behaviour<3u,UMATReal,false> >::getName(),e);
	}
	*KINC = -3;
      }
      catch(const tfel::exception::TFELException& e){
	if(*NTENS==3){
	  treatTFELException(Name<Behaviour<1u,UMATReal,false> >::getName(),e);
	} else if(*NTENS==4){
	  treatTFELException(Name<Behaviour<2u,UMATReal,false> >::getName(),e);
	} else if(*NTENS==6){
	  treatTFELException(Name<Behaviour<3u,UMATReal,false> >::getName(),e);
	}
	*KINC = -4;
      }
      catch(const std::exception& e){
	if(*NTENS==3){
	  treatStandardException(Name<Behaviour<1u,UMATReal,false> >::getName(),e);
	} else if(*NTENS==4){
	  treatStandardException(Name<Behaviour<2u,UMATReal,false> >::getName(),e);
	} else if(*NTENS==6){
	  treatStandardException(Name<Behaviour<3u,UMATReal,false> >::getName(),e);
	}
	*KINC = -5;
      }
      catch(...){
	if(*NTENS==3){
	  treatUnknownException(Name<Behaviour<1u,UMATReal,false> >::getName());
	} else if(*NTENS==4){
	  treatUnknownException(Name<Behaviour<2u,UMATReal,false> >::getName());
	} else if(*NTENS==6){
	  treatUnknownException(Name<Behaviour<3u,UMATReal,false> >::getName());
	}
	*KINC = -6;
      }
} // end of UMATInterface::exe

  private:

    template<unsigned short N>
      struct TFEL_VISIBILITY_LOCAL TreatBehaviour
    {

      struct TFEL_VISIBILITY_LOCAL StiffnessTensorInitializer
      {
	typedef Behaviour<N,UMATReal,false> BV;
	typedef typename BV::BehaviourData  BData;
	TFEL_UMAT_INLINE static void
	  exe(BData& data,const UMATReal * const props){
	  UMATComputeStiffnessTensor<N,UMATTraits<BV>::type>::exe(props,
								  data.getStiffnessTensor());
	} // end of exe
      }; // end of struct StiffnessTensorInitializer

      struct TFEL_VISIBILITY_LOCAL ThermalExpansionTensorInitializer
      {
	typedef Behaviour<N,UMATReal,false> BV;
	typedef typename BV::BehaviourData  BData;
	TFEL_UMAT_INLINE static void
	  exe(BData& data,const UMATReal * const props){
	  UMATComputeThermalExpansionTensor<N,UMATTraits<BV>::type>::exe(props,
									 data.getThermalExpansionTensor());
	} // end of exe
      }; // end of struct ThermalExpansionTensorInitializer

      struct TFEL_VISIBILITY_LOCAL DoNothingInitializer
      {
	typedef Behaviour<N,UMATReal,false> BV;
	typedef typename BV::BehaviourData  BData;
	TFEL_UMAT_INLINE static void
	  exe(BData&,const UMATReal * const)
	{}
      }; // end of struct DoNothingInitializer

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
			       const UMATReal *const,
			       const UMATReal *const)
	{} // end of Error
	
	TFEL_UMAT_INLINE void exe(UMATReal *const,
				  UMATReal *const)
	  throw(UMATException)
	{
	  using namespace std;
	  using namespace tfel::utilities;
	  throw(UMATInvalidDimension(Name<Behaviour<N,UMATReal,false> >::getName()));
	  return;
	} // end of Error::exe
	
      }; // end of struct Error
      
      template<const bool bs,     // requires StiffnessTensor
	       const bool ba>     // requires ThermalExpansionTensor
	struct TFEL_VISIBILITY_LOCAL IntegratorWithTimeStepping
      {
	typedef typename tfel::meta::IF<bs,
					StiffnessTensorInitializer,
					DoNothingInitializer>::type SInitializer;

	typedef typename tfel::meta::IF<ba,
					ThermalExpansionTensorInitializer,
					DoNothingInitializer>::type AInitializer;

	TFEL_UMAT_INLINE IntegratorWithTimeStepping(const UMATReal *const DTIME ,
						    const UMATReal *const       ,
						    const UMATReal *const STRAN ,
						    const UMATReal *const DSTRAN,
						    const UMATReal *const TEMP  ,
						    const UMATReal *const DTEMP,
						    const UMATReal *const PROPS ,
						    const UMATReal *const PREDEF,
						    const UMATReal *const DPRED,
						    UMATReal *const STATEV,
						    UMATReal *const STRESS,
						    const UMATInt *const NDI)
	  : bData(STRESS,STRAN,TEMP,PROPS+UMATTraits<BV>::propertiesOffset,
		  STATEV,PREDEF),
	  iData(DTIME,DSTRAN,DTEMP,DPRED),
	  dt(*DTIME),
	  hypothesis(getModellingHypothesis(*NDI))
	    {
	      SInitializer::exe(this->bData,PROPS);
	      AInitializer::exe(this->bData,PROPS);
	    } // end of IntegratorWithTimeStepping

	TFEL_UMAT_INLINE2 void exe(UMATReal *const STRESS,
				   UMATReal *const STATEV)
	{
	  using namespace tfel::utilities;
	  typedef unsigned short ushort;
	  const UMATOutOfBoundsPolicy& up = UMATOutOfBoundsPolicy::getUMATOutOfBoundsPolicy();
	  unsigned short subSteps   = 0u;
	  unsigned short iterations = 1u;
	  bool convergence;
	  if(this->dt<0.){
	    throwNegativeTimeStepException(Name<Behaviour<N,UMATReal,false> >::getName());
	  }
	  while((iterations!=0)&&
		(subSteps!=UMATTraits<BV>::maximumSubStepping)){
	    convergence = true;
	    BV behaviour(this->bData,this->iData,
			 this->hypothesis);
	    behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	    behaviour.checkBounds();
	    try{
	      convergence = behaviour.integrate();
	    }
	    catch(const tfel::material::DivergenceException& e){
#ifdef MFRONT_UMAT_VERBOSE
	      std::cerr << "no convergence : " << e.what() << std::endl;
#endif
	      convergence = false;
	    }
	    if(convergence){
	      --(iterations);
	      behaviour.checkBounds();
	      behaviour.updateExternalStateVariables();
	      this->bData = static_cast<const BData&>(behaviour);
	    } else {
	      ++subSteps;
	      iterations = ushort(iterations*2u);
	      this->iData *= 0.5;
	    }
	  }
	  if((subSteps==UMATTraits<BV>::maximumSubStepping)&&(iterations!=0)){
	    throwMaximumNumberOfSubSteppingReachedException(Name<Behaviour<N,UMATReal,false> >::getName());
	  }
	  this->bData.UMATexportStateData(STRESS,STATEV);
	} // end of IntegratorWithTimeStepping::exe

      private:
	typedef Behaviour<N,UMATReal,false> BV;
	typedef typename BV::BehaviourData  BData;
	typedef typename BV::IntegrationData  IData;

	BData bData;
	IData iData;
	UMATReal dt;
	tfel::material::ModellingHypothesis::Hypothesis hypothesis;
	
      }; // end of struct IntegratorWithTimeStepping

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

	TFEL_UMAT_INLINE Integrator(const UMATReal *const DTIME ,
				    const UMATReal *const,
				    const UMATReal *const STRAN ,
				    const UMATReal *const DSTRAN,
				    const UMATReal *const TEMP,
				    const UMATReal *const DTEMP,
				    const UMATReal *const PROPS ,
				    const UMATReal *const PREDEF,
				    const UMATReal *const DPRED,
				    const UMATReal *const STATEV,
				    const UMATReal *const STRESS,
				    const UMATInt  *const NDI)
	  : behaviour(DTIME,STRESS,STRAN,DSTRAN,TEMP,DTEMP,
		      PROPS+UMATTraits<BV>::propertiesOffset,
		      STATEV,PREDEF,DPRED,getModellingHypothesis(*NDI)),
	  dt(*DTIME)
	    {
	      SInitializer::exe(this->behaviour,PROPS);
	      AInitializer::exe(this->behaviour,PROPS);
	      const UMATOutOfBoundsPolicy& up = UMATOutOfBoundsPolicy::getUMATOutOfBoundsPolicy();
	      this->behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	    } // end of Integrator::Integrator
	
	TFEL_UMAT_INLINE2
	  void exe(UMATReal *const STRESS,
		   UMATReal *const STATEV)
	{
	  using namespace tfel::utilities;
	  if(this->dt<0.){
	    throwNegativeTimeStepException(Name<Behaviour<N,UMATReal,false> >::getName());
	  }
	  behaviour.checkBounds();
	  if(!this->behaviour.integrate()){
	    throwBehaviourIntegrationFailedException(Name<Behaviour<N,UMATReal,false> >::getName());
	  }
	  behaviour.checkBounds();
	  this->behaviour.UMATexportStateData(STRESS,STATEV);
	} // end of Integrator::exe
	
      private:
	typedef Behaviour<N,UMATReal,false> BV;
	BV behaviour;
	UMATReal dt;
      }; // end of struct Integrator

      TFEL_UMAT_INLINE2 static void
	checkNPROPS(const UMATInt NPROPS)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::material;
	typedef Behaviour<N,UMATReal,false> BV;
	typedef MechanicalBehaviourTraits<BV> Traits;
	const unsigned short offset  = UMATTraits<BV>::propertiesOffset;
	const unsigned short nprops  = Traits::material_properties_nb;
	const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
	const bool is_defined_       = Traits::is_defined;
	//Test if the nb of properties matches Behaviour requirements
	if((NPROPS!=NPROPS_)&&is_defined_){
	  throwUnMatchedNumberOfMaterialProperties(Name<Behaviour<N,UMATReal,false> >::getName(),
						   NPROPS_,NPROPS);
	}
      } // end of checkNPROPS
      
      TFEL_UMAT_INLINE2 static void
	checkNSTATV(const UMATInt NSTATV)
      {
	using namespace tfel::utilities;
	typedef Behaviour<N,UMATReal,false> BV;
	typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	const unsigned short nstatv  = Traits::internal_variables_nb;
	const unsigned short NSTATV_ = nstatv == 0 ? 1u : nstatv;
	const bool is_defined_       = Traits::is_defined;
	//Test if the nb of state variables matches Behaviour requirements
	if((NSTATV_!=NSTATV)&&is_defined_){
	  throwUnMatchedNumberOfStateVariables(Name<Behaviour<N,UMATReal,false> >::getName(),
					       NSTATV_,NSTATV);
	}
      } // end of checkNSTATV
      
    }; // end of struct TreatBehaviour
    
    struct TFEL_VISIBILITY_LOCAL TreatOrthotropicBehaviour1D
      : private TreatBehaviour<1u>
      {
	TFEL_UMAT_INLINE2 static 
	  void exe(const UMATReal *const DTIME,
		   const UMATReal *const,
		   const UMATReal *const DDSOE,
		   const UMATReal *const STRAN ,
		   const UMATReal *const DSTRAN,
		   const UMATReal *const TEMP  ,
		   const UMATReal *const DTEMP,
		   const UMATReal *const PROPS ,
		   const UMATInt  *const NPROPS,
		   const UMATReal *const PREDEF,
		   const UMATReal *const DPRED,
		   UMATReal *const STATEV,
		   const UMATInt  *const NSTATV,
		   UMATReal *const STRESS,
		   const UMATInt  *const NDI)
	{
	  using namespace tfel::meta;
	  using namespace tfel::material;
	  using namespace tfel::math;
	  typedef MechanicalBehaviourTraits<Behaviour<1u,UMATReal,false> > MTraits;
	  typedef UMATTraits<Behaviour<1u,UMATReal,false> > Traits;
	  typedef TreatBehaviour<1u> TreatBehaviour;
	  const bool is_defined_ = MTraits::is_defined;
	  const bool bs = Traits::requiresStiffnessTensor;
	  const bool ba = Traits::requiresThermalExpansionTensor;
	  typedef typename IF<
	    is_defined_,
	    typename IF<
	      Traits::useTimeSubStepping,
	      typename TreatBehaviour::template IntegratorWithTimeStepping<bs,ba>,
	      typename TreatBehaviour::template Integrator<bs,ba>
	      >::type,
	    typename TreatBehaviour::Error>::type Handler;
	  TreatBehaviour::checkNPROPS(*NPROPS);
	  TreatBehaviour::checkNSTATV(*NSTATV);
	  Handler handler(DTIME,DDSOE,STRAN,
			  DSTRAN,TEMP,DTEMP,PROPS,
			  PREDEF,DPRED,STATEV,
			  STRESS,NDI);
	  handler.exe(STRESS,STATEV);
	} // end of TreatOrthotropicBehaviour1D::exe
      }; // end of struct TreatOrthotropicBehaviour1D

    struct TFEL_VISIBILITY_LOCAL TreatOrthotropicBehaviour2D
      : private TreatBehaviour<2u>
      {
	TFEL_UMAT_INLINE2 static 
	  void exe(const UMATReal *const DTIME ,
		   const UMATReal *const DROT  ,
		   const UMATReal *const DDSOE,
		   const UMATReal *const STRAN ,
		   const UMATReal *const DSTRAN,
		   const UMATReal *const TEMP  ,
		   const UMATReal *const DTEMP,
		   const UMATReal *const PROPS ,
		   const UMATInt  *const NPROPS,
		   const UMATReal *const PREDEF,
		   const UMATReal *const DPRED,
		   UMATReal *const STATEV,
		   const UMATInt  *const NSTATV,
		   UMATReal *const STRESS,
		   const UMATInt  *const NDI) 
	{
	  using namespace tfel::meta;
	  using namespace tfel::material;
	  using namespace tfel::math;
	  typedef MechanicalBehaviourTraits<Behaviour<2u,UMATReal,false> > MTraits;
	  typedef UMATTraits<Behaviour<2u,UMATReal,false> > Traits;
	  typedef TreatBehaviour<2u> TreatBehaviour;
	  const bool is_defined_ = MTraits::is_defined;
	  const bool bs = Traits::requiresStiffnessTensor;
	  const bool ba = Traits::requiresThermalExpansionTensor;
	  typedef typename IF<
	    is_defined_,
	    typename IF<
	      Traits::useTimeSubStepping,
	      typename TreatBehaviour::template IntegratorWithTimeStepping<bs,ba>,
	      typename TreatBehaviour::template Integrator<bs,ba>
	      >::type,
	    typename TreatBehaviour::Error>::type Handler;
	  UMATReal s[4];
	  UMATReal e[4];
	  UMATReal de[4];
	  UMATRotationMatrix2D m(PROPS+7,DROT);
	  m.rotateStressesForward(STRESS,s);
	  m.rotateStrainsForward(STRAN,e);
	  m.rotateStrainsForward(DSTRAN,de);
	  TreatBehaviour::checkNPROPS(*NPROPS);
	  TreatBehaviour::checkNSTATV(*NSTATV);
	  Handler handler(DTIME,DDSOE,
			  e,de,TEMP,DTEMP,PROPS,
			  PREDEF,DPRED,STATEV,s,NDI);
	  handler.exe(s,STATEV);
	  m.rotateStressesBackward(s,STRESS);
	} // end of TreatOrthotropicBehaviour2D::exe
      }; // end of TreatOrthotropicBehaviour2D

    struct TFEL_VISIBILITY_LOCAL TreatOrthotropicBehaviour3D
      : private TreatBehaviour<3u>
      {
	TFEL_UMAT_INLINE2 static 
	  void exe(const UMATReal *const DTIME,
		   const UMATReal *const DROT,
		   const UMATReal *const DDSOE,
		   const UMATReal *const STRAN,
		   const UMATReal *const DSTRAN,
		   const UMATReal *const TEMP  ,
		   const UMATReal *const DTEMP,
		   const UMATReal *const PROPS ,
		   const UMATInt  *const NPROPS,
		   const UMATReal *const PREDEF,
		   const UMATReal *const DPRED,
		   UMATReal *const STATEV,
		   const UMATInt  *const NSTATV,
		   UMATReal *const STRESS,
		   const UMATInt  *const NDI) 
	{
	  using namespace tfel::meta;
	  using namespace tfel::material;
	  using namespace tfel::math;
	  typedef MechanicalBehaviourTraits<Behaviour<3u,UMATReal,false> > MTraits;
	  typedef UMATTraits<Behaviour<3u,UMATReal,false> > Traits;
	  typedef TreatBehaviour<3u> TreatBehaviour;
	  const bool is_defined_ = MTraits::is_defined;
	  const bool bs = Traits::requiresStiffnessTensor;
	  const bool ba = Traits::requiresThermalExpansionTensor;
	  typedef typename IF<
	    is_defined_,
	    typename IF<
	    Traits::useTimeSubStepping,
	    typename TreatBehaviour::template IntegratorWithTimeStepping<bs,ba>,
	    typename TreatBehaviour::template Integrator<bs,ba>
	    >::type,
	    typename TreatBehaviour::Error>::type Handler;
	  UMATReal  s[6];
	  UMATReal  e[6];
	  UMATReal de[6];
	  // Passage au repère matériau
	  UMATRotationMatrix3D m(PROPS+9,DROT);
	  m.rotateStressesForward(STRESS,s);
	  m.rotateStrainsForward(STRAN,e);
	  m.rotateStrainsForward(DSTRAN,de);
	  TreatBehaviour::checkNPROPS(*NPROPS);
	  TreatBehaviour::checkNSTATV(*NSTATV);
	  Handler handler(DTIME,DDSOE,e,
			  de,TEMP,DTEMP,PROPS,
			  PREDEF,DPRED,STATEV,s,NDI);
	  handler.exe(s,STATEV);
	  m.rotateStressesBackward(s,STRESS);
	} // end of TreatOrthotropicBehaviour3D::exe
	
      }; // end of struct TreatOrthotropicBehaviour3D

    template<unsigned short N>
      struct TFEL_VISIBILITY_LOCAL TreatIsotropicBehaviour
	: public TreatBehaviour<N>
      {
	TFEL_UMAT_INLINE static
	  void exe(const UMATReal *const DTIME ,
		   const UMATReal *const,
		   const UMATReal *const DDSOE,
		   const UMATReal *const STRAN ,
		   const UMATReal *const DSTRAN,
		   const UMATReal *const TEMP  ,
		   const UMATReal *const DTEMP,
		   const UMATReal *const PROPS ,
		   const UMATInt  *const NPROPS,
		   const UMATReal *const PREDEF,
		   const UMATReal *const DPRED,
		   UMATReal *const STATEV,
		   const UMATInt  *const NSTATV,
		   UMATReal *const STRESS,
		   const UMATInt  *const NDI) 
	{
	  using namespace tfel::meta;
	  using namespace tfel::material;
	  typedef MechanicalBehaviourTraits<Behaviour<N,UMATReal,false> > MTraits;
	  typedef UMATTraits<Behaviour<N,UMATReal,false> > Traits;
	  typedef TreatBehaviour<N> TreatBehaviour;
	  const bool is_defined_ = MTraits::is_defined;
	  const bool bs = Traits::requiresStiffnessTensor;
	  const bool ba = Traits::requiresThermalExpansionTensor;
	  typedef typename IF<
	    is_defined_,
	    typename IF<
	      Traits::useTimeSubStepping,
	      typename TreatBehaviour::template IntegratorWithTimeStepping<bs,ba>,
	      typename TreatBehaviour::template Integrator<bs,ba>
	      >::type,
	    typename TreatBehaviour::Error>::type Handler;
	  TreatBehaviour::checkNPROPS(*NPROPS);
	  TreatBehaviour::checkNSTATV(*NSTATV);
	  Handler handler(DTIME,DDSOE,STRAN,
			  DSTRAN,TEMP,DTEMP,PROPS,
			  PREDEF,DPRED,STATEV,STRESS,
			  NDI);
	  handler.exe(STRESS,STATEV);
	} // end of TreatIsotropicBehaviour::exe
	
      }; // end of struct TreatIsotropicBehaviour
    
  }; // end of class UMATInterface
  
} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_CALL_H */

