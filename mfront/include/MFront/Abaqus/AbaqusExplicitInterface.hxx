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

#include"TFEL/Material/MechanicalBehaviourTraits.hxx"
#include"TFEL/Material/MaterialException.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Abaqus/Abaqus.hxx"
#include"MFront/Abaqus/AbaqusConfig.hxx"
#include"MFront/Abaqus/AbaqusTraits.hxx"
#include"MFront/Abaqus/AbaqusException.hxx"
#include"MFront/Abaqus/AbaqusExplicitData.hxx"
// #include"MFront/Abaqus/AbaqusExplicitBehaviourHandler.hxx"
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
  template<tfel::material::ModellingHypothesis::Hypothesis,typename T,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AbaqusExplicitInterface
    : protected AbaqusInterfaceExceptions
  {

    TFEL_ABAQUS_INLINE2 static
    int computeElasticPrediction(const int i,const AbaqusExplicitData<T>& d)
    {
      
    };
    
  }; // end of struct AbaqusExplicitInterface
      
} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSEXPLICITINTERFACE_HXX_ */
