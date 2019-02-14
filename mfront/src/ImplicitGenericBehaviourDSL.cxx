/*! 
 * \file  mfront/src/ImplicitGenericBehaviourDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 févr. 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/ImplicitGenericBehaviourDSL.hxx"

namespace mfront{

  ImplicitGenericBehaviourDSL::ImplicitGenericBehaviourDSL() {
    this->mb.setDSLName("ImplicitGenericBehaviour");
    this->mb.declareAsGenericBehaviour();
    this->registerNewCallBack("@Gradient",
                              &ImplicitGenericBehaviourDSL::treatGradient);
    this->registerNewCallBack(
        "@ThermodynamicForce",
        &ImplicitGenericBehaviourDSL::treatThermodynamicForce);
    this->registerNewCallBack(
        "@Flux", &ImplicitGenericBehaviourDSL::treatThermodynamicForce);
  }  // end of ImplicitGenericBehaviourDSL::ImplicitGenericBehaviourDSL

  std::string ImplicitGenericBehaviourDSL::getName() {
    return "ImplicitGenericBehaviourDSL";
  } // end of ImplicitGenericBehaviourDSL::getName

  std::string ImplicitGenericBehaviourDSL::getDescription() {
    return "this dsl provides a generic integrator based on a theta method.";
  } // end of ImplicitGenericBehaviourDSL::getDescription

  ImplicitGenericBehaviourDSL::~ImplicitGenericBehaviourDSL() noexcept =
          default;

} // end of namespace mfront  
