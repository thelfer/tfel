/*!
 * \file   AsterInterface.hxx
 * \brief  This file implements the AsterInterface class.
 * \author Helfer Thomas
 * \date   28 Jul 2006
 */

#ifndef _LIB_MFRONT_ASTER_CALL_H_
#define _LIB_MFRONT_ASTER_CALL_H_ 

#include<string>
#include<algorithm>
#include<vector>
#include<limits>

#include"TFEL/Config/TFELTypes.hxx"

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Exception/TFELException.hxx"
#include"TFEL/FSAlgorithm/copy.hxx"

#include"TFEL/Utilities/Info.hxx"
#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Material/MechanicalBehaviourTraits.hxx"
#include"TFEL/Material/MechanicalBehaviourData.hxx"
#include"TFEL/Material/MaterialException.hxx"

#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterConfig.hxx"
#include"MFront/Aster/AsterTraits.hxx"
#include"MFront/Aster/AsterException.hxx"
// #include"MFront/ASTER/ASTEROutOfBoundsPolicy.hxx"
// #include"MFront/ASTER/ASTEROrthotropicBehaviour.hxx"
// #include"MFront/ASTER/ASTERComputeStiffnessTensor.hxx"
// #include"MFront/ASTER/ASTERComputeThermalExpansionTensor.hxx"
// #include"MFront/ASTER/ASTERRotationMatrix.hxx"
// #include"MFront/ASTER/ASTERGetModellingHypothesis.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

namespace aster{

  /*!
   * \class  AsterInterfaceBase
   * \brief  Base class for Aster interfaces
   * \author Helfer Thomas
   * \date   12/12/2011
   */
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterInterfaceBase
  {

    /*!
     * \brief throw an AsterException. This method shall be called when
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
					     const AsterInt n2);
    
    /*!
     * \brief throw an AsterException. This method shall be called when
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
					 const AsterInt n2);

    /*!
     * \brief display the error message out of an AsterException to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the AsterException to be treated
     */
    static void
    treatAsterException(const std::string&,
			const AsterException&);

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
     * \brief throw an AsterException if the time step is negative
     * \param[in] b : behaviour name
     */
    static void
    throwNegativeTimeStepException(const std::string&);

    /*!
     * \brief throw an AsterException if the behaviour integration
     * failed
     * \param[in] b : behaviour name
     */
    static void
    throwBehaviourIntegrationFailedException(const std::string&);

    /*!
     * \brief throw an AsterException if the a consistant tangent
     * operator has to be given.
     * \param[in] b : behaviour name
     */
    static void
    throwConsistantTangentOperatorIsNotAvalaible(const std::string&);

  }; // end of struct AsterInterfaceBase

  /*!
   * \class  AsterInterface
   * \brief  This class create an interface between Behaviour and Aster
   * \author Helfer Thomas
   * \date   28 Jul 2006
   */
  template<template<unsigned short,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterInterface
    : protected AsterInterfaceBase
  {


    TFEL_ASTER_INLINE2 static
      void exe(const AsterInt  *const NTENS, const AsterReal *const DTIME,
	       const AsterReal *const DROT,  AsterReal *const DDSOE,
	       const AsterReal *const STRAN, const AsterReal *const DSTRAN,
	       const AsterReal *const TEMP,  const AsterReal *const DTEMP,
	       const AsterReal *const PROPS, const AsterInt  *const NPROPS,
	       const AsterReal *const PREDEF,const AsterReal *const DPRED,
	       AsterReal *const STATEV,const AsterInt  *const NSTATV,
	       AsterReal *const STRESS,
	       AsterInt  *const KINC)
    {
      using namespace std;
      using namespace tfel::meta;
      using namespace tfel::utilities;
      typedef unsigned short ushort;
      try{
	if(*NTENS==3){
	  typedef AsterTraits<Behaviour<1u,AsterReal,false> > Traits;
#warning "orthotropic"
	  // typedef typename IF<Traits::type==Aster::ISOTROPIC,
	  // 		      TreatIsotropicBehaviour<1u>,
	  // 		      TreatOrthotropicBehaviour1D>::type Treat;
	  typedef TreatIsotropicBehaviour<1u> Treat;
	  Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		     PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		     STRESS);
      } else if(*NTENS==4){
	  typedef AsterTraits<Behaviour<2u,AsterReal,false> > Traits;
	  typedef TreatIsotropicBehaviour<2u> Treat;
#warning "orthotropic"
	  // typedef typename IF<Traits::type==Aster::ISOTROPIC,
	  // 			TreatIsotropicBehaviour<2u>,
	  // 			TreatOrthotropicBehaviour2D>::type Treat;
	    Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		       PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,
		       STRESS);
	    //	  }
	} else if(*NTENS==6){
	  typedef AsterTraits<Behaviour<3u,AsterReal,false> > Traits;
#warning "orthotropic"
	  // typedef typename IF<Traits::type==Aster::ISOTROPIC,
	  // 		      TreatIsotropicBehaviour<3u>,
	  // 		      TreatOrthotropicBehaviour3D>::type Treat;
	  typedef TreatIsotropicBehaviour<3u> Treat;
	  Treat::exe(DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,
		     PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,STRESS);
	} else {
	  throw(AsterInvalidNTENSValue(ushort(*NTENS)));
	}
      }
      catch(const AsterException& e){
	if(*NTENS==3){
	  treatAsterException(Name<Behaviour<1u,AsterReal,false> >::getName(),e);
	} else if(*NTENS==4){
	  treatAsterException(Name<Behaviour<2u,AsterReal,false> >::getName(),e);
	} else if(*NTENS==6){
	  treatAsterException(Name<Behaviour<3u,AsterReal,false> >::getName(),e);
	}
	*KINC = -2;
      }
      catch(const tfel::material::MaterialException& e){
	if(*NTENS==3){
	  treatMaterialException(Name<Behaviour<1u,AsterReal,false> >::getName(),e);
	} else if(*NTENS==4){
	  treatMaterialException(Name<Behaviour<2u,AsterReal,false> >::getName(),e);
	} else if(*NTENS==6){
	  treatMaterialException(Name<Behaviour<3u,AsterReal,false> >::getName(),e);
	}
	*KINC = -3;
      }
      catch(const tfel::exception::TFELException& e){
	if(*NTENS==3){
	  treatTFELException(Name<Behaviour<1u,AsterReal,false> >::getName(),e);
	} else if(*NTENS==4){
	  treatTFELException(Name<Behaviour<2u,AsterReal,false> >::getName(),e);
	} else if(*NTENS==6){
	  treatTFELException(Name<Behaviour<3u,AsterReal,false> >::getName(),e);
	}
	*KINC = -4;
      }
      catch(const std::exception& e){
	if(*NTENS==3){
	  treatStandardException(Name<Behaviour<1u,AsterReal,false> >::getName(),e);
	} else if(*NTENS==4){
	  treatStandardException(Name<Behaviour<2u,AsterReal,false> >::getName(),e);
	} else if(*NTENS==6){
	  treatStandardException(Name<Behaviour<3u,AsterReal,false> >::getName(),e);
	}
	*KINC = -5;
      }
      catch(...){
	if(*NTENS==3){
	  treatUnknownException(Name<Behaviour<1u,AsterReal,false> >::getName());
	} else if(*NTENS==4){
	  treatUnknownException(Name<Behaviour<2u,AsterReal,false> >::getName());
	} else if(*NTENS==6){
	  treatUnknownException(Name<Behaviour<3u,AsterReal,false> >::getName());
	}
	*KINC = -6;
      }
    } // end of AsterInterface::exe

  private:

    template<unsigned short N>
      struct TFEL_VISIBILITY_LOCAL TreatBehaviour
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
      
    }; // end of struct TreatBehaviour
    
    // struct TFEL_VISIBILITY_LOCAL TreatOrthotropicBehaviour1D
    //   : private TreatBehaviour<1u>
    //   {
    // 	TFEL_ASTER_INLINE2 static 
    // 	  void exe(const AsterReal *const DTIME,
    // 		   const AsterReal *const,
    // 		   const AsterReal *const DDSOE,
    // 		   const AsterReal *const STRAN ,
    // 		   const AsterReal *const DSTRAN,
    // 		   const AsterReal *const TEMP  ,
    // 		   const AsterReal *const DTEMP,
    // 		   const AsterReal *const PROPS ,
    // 		   const AsterInt  *const NPROPS,
    // 		   const AsterReal *const PREDEF,
    // 		   const AsterReal *const DPRED,
    // 		   AsterReal *const STATEV,
    // 		   const AsterInt  *const NSTATV,
    // 		   AsterReal *const STRESS)
    // 	{
    // 	  using namespace tfel::meta;
    // 	  using namespace tfel::material;
    // 	  using namespace tfel::math;
    // 	  typedef MechanicalBehaviourTraits<Behaviour<1u,AsterReal,false> > MTraits;
    // 	  typedef AsterTraits<Behaviour<1u,AsterReal,false> > Traits;
    // 	  typedef TreatBehaviour<1u> TreatBehaviour;
    // 	  const bool is_defined_ = MTraits::is_defined;
    // 	  const bool bs = Traits::requiresStiffnessTensor;
    // 	  const bool ba = Traits::requiresThermalExpansionTensor;
    // 	  typedef typename IF<
    // 	    is_defined_,
    // 	    typename TreatBehaviour::template Integrator<bs,ba>,
    // 	    typename TreatBehaviour::Error>::type Handler;
    // 	  TreatBehaviour::checkNPROPS(*NPROPS);
    // 	  TreatBehaviour::checkNSTATV(*NSTATV);
    // 	  Handler handler(DTIME,DDSOE,STRAN,
    // 			  DSTRAN,TEMP,DTEMP,PROPS,
    // 			  PREDEF,DPRED,STATEV,
    // 			  STRESS);
    // 	  handler.exe(STRESS,STATEV);
    // 	} // end of TreatOrthotropicBehaviour1D::exe
    //   }; // end of struct TreatOrthotropicBehaviour1D

    // struct TFEL_VISIBILITY_LOCAL TreatOrthotropicBehaviour2D
    //   : private TreatBehaviour<2u>
    //   {
    // 	TFEL_ASTER_INLINE2 static 
    // 	  void exe(const AsterReal *const DTIME ,
    // 		   const AsterReal *const DROT  ,
    // 		   const AsterReal *const DDSOE,
    // 		   const AsterReal *const STRAN ,
    // 		   const AsterReal *const DSTRAN,
    // 		   const AsterReal *const TEMP  ,
    // 		   const AsterReal *const DTEMP,
    // 		   const AsterReal *const PROPS ,
    // 		   const AsterInt  *const NPROPS,
    // 		   const AsterReal *const PREDEF,
    // 		   const AsterReal *const DPRED,
    // 		   AsterReal *const STATEV,
    // 		   const AsterInt  *const NSTATV,
    // 		   AsterReal *const STRESS) 
    // 	{
    // 	  using namespace tfel::meta;
    // 	  using namespace tfel::material;
    // 	  using namespace tfel::math;
    // 	  typedef MechanicalBehaviourTraits<Behaviour<2u,AsterReal,false> > MTraits;
    // 	  typedef AsterTraits<Behaviour<2u,AsterReal,false> > Traits;
    // 	  typedef TreatBehaviour<2u> TreatBehaviour;
    // 	  const bool is_defined_ = MTraits::is_defined;
    // 	  const bool bs = Traits::requiresStiffnessTensor;
    // 	  const bool ba = Traits::requiresThermalExpansionTensor;
    // 	  typedef typename IF<
    // 	    is_defined_,
    // 	    typename TreatBehaviour::template Integrator<bs,ba>,
    // 	    typename TreatBehaviour::Error>::type Handler;
    // 	  AsterReal s[4];
    // 	  AsterReal e[4];
    // 	  AsterReal de[4];
    // 	  AsterRotationMatrix2D m(PROPS+7,DROT);
    // 	  m.rotateStressesForward(STRESS,s);
    // 	  m.rotateStrainsForward(STRAN,e);
    // 	  m.rotateStrainsForward(DSTRAN,de);
    // 	  TreatBehaviour::checkNPROPS(*NPROPS);
    // 	  TreatBehaviour::checkNSTATV(*NSTATV);
    // 	  Handler handler(DTIME,DDSOE,
    // 			  e,de,TEMP,DTEMP,PROPS,
    // 			  PREDEF,DPRED,STATEV,s);
    // 	  handler.exe(s,STATEV);
    // 	  m.rotateStressesBackward(s,STRESS);
    // 	} // end of TreatOrthotropicBehaviour2D::exe
    //   }; // end of TreatOrthotropicBehaviour2D

    // struct TFEL_VISIBILITY_LOCAL TreatOrthotropicBehaviour3D
    //   : private TreatBehaviour<3u>
    //   {
    // 	TFEL_ASTER_INLINE2 static 
    // 	  void exe(const AsterReal *const DTIME,
    // 		   const AsterReal *const DROT,
    // 		   const AsterReal *const DDSOE,
    // 		   const AsterReal *const STRAN,
    // 		   const AsterReal *const DSTRAN,
    // 		   const AsterReal *const TEMP  ,
    // 		   const AsterReal *const DTEMP,
    // 		   const AsterReal *const PROPS ,
    // 		   const AsterInt  *const NPROPS,
    // 		   const AsterReal *const PREDEF,
    // 		   const AsterReal *const DPRED,
    // 		   AsterReal *const STATEV,
    // 		   const AsterInt  *const NSTATV,
    // 		   AsterReal *const STRESS) 
    // 	{
    // 	  using namespace tfel::meta;
    // 	  using namespace tfel::material;
    // 	  using namespace tfel::math;
    // 	  typedef MechanicalBehaviourTraits<Behaviour<3u,AsterReal,false> > MTraits;
    // 	  typedef AsterTraits<Behaviour<3u,AsterReal,false> > Traits;
    // 	  typedef TreatBehaviour<3u> TreatBehaviour;
    // 	  const bool is_defined_ = MTraits::is_defined;
    // 	  const bool bs = Traits::requiresStiffnessTensor;
    // 	  const bool ba = Traits::requiresThermalExpansionTensor;
    // 	  typedef typename IF<
    // 	    is_defined_,
    // 	    typename TreatBehaviour::template Integrator<bs,ba>,
    // 	    typename TreatBehaviour::Error>::type Handler;
    // 	  AsterReal  s[6];
    // 	  AsterReal  e[6];
    // 	  AsterReal de[6];
    // 	  // Passage au repère matériau
    // 	  AsterRotationMatrix3D m(PROPS+9,DROT);
    // 	  m.rotateStressesForward(STRESS,s);
    // 	  m.rotateStrainsForward(STRAN,e);
    // 	  m.rotateStrainsForward(DSTRAN,de);
    // 	  TreatBehaviour::checkNPROPS(*NPROPS);
    // 	  TreatBehaviour::checkNSTATV(*NSTATV);
    // 	  Handler handler(DTIME,DDSOE,e,
    // 			  de,TEMP,DTEMP,PROPS,
    // 			  PREDEF,DPRED,STATEV,s);
    // 	  handler.exe(s,STATEV);
    // 	  m.rotateStressesBackward(s,STRESS);
    // 	} // end of TreatOrthotropicBehaviour3D::exe
	
    //   }; // end of struct TreatOrthotropicBehaviour3D

    template<unsigned short N>
      struct TFEL_VISIBILITY_LOCAL TreatIsotropicBehaviour
	: public TreatBehaviour<N>
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
	  typedef TreatBehaviour<N> TreatBehaviour;
	  const bool is_defined_ = MTraits::is_defined;
	  const bool bs = Traits::requiresStiffnessTensor;
	  const bool ba = Traits::requiresThermalExpansionTensor;
	  typedef typename IF<
	    is_defined_,
	    typename TreatBehaviour::template Integrator<bs,ba>,
	    typename TreatBehaviour::Error>::type Handler;
	  TreatBehaviour::checkNPROPS(*NPROPS);
	  TreatBehaviour::checkNSTATV(*NSTATV);
	  Handler handler(DTIME,STRAN,
			  DSTRAN,TEMP,DTEMP,PROPS,
			  PREDEF,DPRED,STATEV,STRESS);
	  handler.exe(DDSOE,STRESS,STATEV);
	} // end of TreatIsotropicBehaviour::exe
	
      }; // end of struct TreatIsotropicBehaviour
    
  }; // end of class AsterInterface
  
} // end of namespace aster

#endif /* _LIB_MFRONT_ASTER_CALL_H */

