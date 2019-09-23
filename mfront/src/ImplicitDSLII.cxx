/*! 
 * \file  mfront/src/ImplicitDSLII.cxx
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

#include"MFront/ImplicitDSLII.hxx"

namespace mfront{

  ImplicitDSLII::ImplicitDSLII() {
    this->mb.declareAsASmallStrainStandardBehaviour();
  } // end of ImplicitDSLII::ImplicitDSLII

  std::string ImplicitDSLII::getName() {
    return "ImplicitII";
  } // end of ImplicitDSLII::getName

  std::string ImplicitDSLII::getDescription() {
    return "this parser provides a generic integrator based on a theta method.";
  } // end of ImplicitDSLII::getDescription

  BehaviourDSLDescription ImplicitDSLII::getBehaviourDSLDescription() const {
    auto d = mfront::getDefaultStrainBasedBehaviourDSLDescription();
    d.integrationScheme = IntegrationScheme::IMPLICITSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputePredictionOperator,
                           BehaviourData::ComputePredictor,
                           BehaviourData::ComputeStress,
                           BehaviourData::Integrator,
                           BehaviourData::ComputeTangentOperator};
    d.minimalMFrontFileBody = "@Integrator{}\n\n";
    return d;
  }  // end of ImplicitDSLII::getBehaviourDSLDescription

  ImplicitDSLII::~ImplicitDSLII() = default;

} // end of namespace mfront  
