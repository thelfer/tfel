/*!
 * \file   mfront/include/MFront/Abaqus/AbaqusExplicitInterface.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   19 mars 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSEXPLICITINTERFACE_HXX
#define LIB_MFRONT_ABAQUS_ABAQUSEXPLICITINTERFACE_HXX

#include<string>
#include<vector>
#include<limits>
#include<algorithm>
#include<type_traits>

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Exception/TFELException.hxx"
#include"TFEL/FSAlgorithm/copy.hxx"

#include"TFEL/Material/MechanicalBehaviour.hxx"
#include"TFEL/Material/MechanicalBehaviourTraits.hxx"
#include"TFEL/Material/MaterialException.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Abaqus/Abaqus.hxx"
#include"MFront/Abaqus/AbaqusConfig.hxx"
#include"MFront/Abaqus/AbaqusTraits.hxx"
#include"MFront/Abaqus/AbaqusException.hxx"
#include"MFront/Abaqus/AbaqusExplicitData.hxx"
#include"MFront/Abaqus/AbaqusTangentOperator.hxx"
#include"MFront/Abaqus/AbaqusInterfaceExceptions.hxx"

namespace abaqus{

  /*!
   * \class  AbaqusExplicitInterface
   * \brief This class create an interface between a behaviour class
   * and the abaqus finite element code
   *
   * \note : most of branching is done at compile-time. to the very
   * exeception of the stress-free expansion which are handled through
   * two functions pointers which take into account the specificities
   * of finite strain strategy used. The choice of introducing those
   * runtime-functions comes from the fact that we did not want a code
   * duplication between two different finite strain strategies (to
   * reduce both compile-time and library size).
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,typename T,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AbaqusExplicitInterface
    : protected AbaqusInterfaceExceptions
  {
    //! space dimension
    static constexpr const unsigned short N =
      tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
    //! simple alias
    using MechanicalBehaviourBase = tfel::material::MechanicalBehaviourBase; 
    //! simple alias
    using TangentOperatorTraits =
      tfel::material::TangentOperatorTraits<
	MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR>;
    /*!
     * \param[out] D: elastic stiffness
     * \param[out] d: data
     * \param[out] i: block number
     */
    TFEL_ABAQUS_INLINE2 static
    int computeElasticPrediction(T *const D,
				 const AbaqusExplicitData<T>& d,
				 const int i)
    {
      constexpr const T zero = T{0};
      const tfel::math::stensor<N,T> e(zero);
      const tfel::math::stensor<N,T> de(zero);
      const tfel::math::stensor<N,T> s(zero);
      Behaviour<H,T,false> b(d,i);
      b.setBehaviourDataDrivingVariables(e);
      b.setBehaviourDataThermodynamicForces(s);
      b.setIntegrationDataDrivingVariables(de);
      //      b.setOutOfBoundsPolicy(d.op);
      b.initialize();
      //      b.checkBounds();
      const auto smf = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
      const auto r   = b.computePredictionOperator(smf,Behaviour<H,T,false>::ELASTIC);
      ExtractAndConvertTangentOperator<H>::exe(b,D);
      return r==Behaviour<H,T,false>::SUCCESS;
    };
    /*!
     * \brief integrate a behaviour written at small strain
     * \param[out/in] s: stress tensor
     * \param[out] d:  data
     * \param[out] e:  strain tensor
     * \param[out] de: strain tensor increment
     * \param[out] i:  block number
     */
    TFEL_ABAQUS_INLINE2 static
    int integrate(tfel::math::stensor<N,T>& s,
		  const AbaqusExplicitData<T>& d,
		  const tfel::math::stensor<N,T>& e,
		  const tfel::math::stensor<N,T>& de,
		  const int i)
    {
      Behaviour<H,T,false> b(d,i);
      b.setBehaviourDataDrivingVariables(e);
      b.setBehaviourDataThermodynamicForces(s);
      b.setIntegrationDataDrivingVariables(de);
      //      b.setOutOfBoundsPolicy(d.op);
      b.initialize();
      //      b.checkBounds();
      const auto smf = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
      T r_dt;
      auto tsf = b.computeAPrioriTimeStepScalingFactor(r_dt);
      if(!tsf.first){
	return -1;
      }
      if(b.integrate(smf,Behaviour<H,T,false>::NOSTIFFNESSREQUESTED)==
	 Behaviour<H,T,false>::FAILURE){
	return -1;
      }
      tsf = b.computeAPosterioriTimeStepScalingFactor(r_dt);
      if(!tsf.first){
	return -1;
      }
      b.checkBounds();
      b.exportStateData(s,d,i);
      return 0;
    };
  }; // end of struct AbaqusExplicitInterface
      
} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSEXPLICITINTERFACE_HXX_ */
