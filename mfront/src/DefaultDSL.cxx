/*!
 * \file   mfront/src/DefaultDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <fstream>
#include <stdexcept>

#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/DefaultDSL.hxx"

namespace mfront {

  DefaultDSL::DefaultDSL() {
    this->mb.setDSLName("Default");
    this->registerNewCallBack("@RequireStiffnessTensor",
                              &DefaultDSL::treatRequireStiffnessOperator);
    this->mb.declareAsASmallStrainStandardBehaviour();
  }

  std::string DefaultDSL::getDescription() {
    return "this parser is the most generic one as it does not make any "
           "restriction "
           "on the behaviour or the integration method that may be used";
  }  // end of DefaultDSL::getDescription

  std::string DefaultDSL::getName() { return "DefaultDSL"; }

  BehaviourDSLDescription DefaultDSL::getBehaviourDSLDescription() const {
    auto d = mfront::getDefaultStrainBasedBehaviourDSLDescription();
    d.integrationScheme = IntegrationScheme::USERDEFINEDSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputePredictionOperator,
                           BehaviourData::Integrator,
                           BehaviourData::ComputeTangentOperator};
    d.minimalMFrontFileBody = "@Integrator{}\n\n";
    return d;
  }  // end of DefaultDSL::getBehaviourDSLDescription

  DefaultDSL::~DefaultDSL() = default;

}  // end of namespace mfront
