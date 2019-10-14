/*! 
 * \file  mfront/src/RungeKuttaFiniteStrainDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 17 févr. 2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/RungeKuttaFiniteStrainDSL.hxx"

namespace mfront{

  RungeKuttaFiniteStrainDSL::RungeKuttaFiniteStrainDSL() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("RungeKuttaFiniteStrain");
    this->mb.declareAsAFiniteStrainStandardBehaviour(true);
    // driving variables
    for(const auto& v: this->mb.getMainVariables()){
      const auto& dv = v.first;
      this->mb.addLocalVariable(h,VariableDescription(dv.type,dv.name+"_",1u,0u));
      this->mb.addLocalVariable(h,VariableDescription(SupportedTypes::getTimeDerivativeType(dv.type),
						      "d"+dv.name+"_",1u,0u));
    }
    this->registerNewCallBack(
        "@ComputeStress", &RungeKuttaFiniteStrainDSL::treatComputeThermodynamicForces);
    this->registerNewCallBack(
        "@ComputeFinalStress",
        &RungeKuttaFiniteStrainDSL::treatComputeFinalThermodynamicForces);
  } // end of RungeKuttaFiniteStrainDSL::RungeKuttaFiniteStrainDSL

  std::string RungeKuttaFiniteStrainDSL::getName() {
    return "RungeKuttaFiniteStrain";
  } // end of RungeKuttaFiniteStrainDSL::getName

  std::string RungeKuttaFiniteStrainDSL::getDescription() {
    return "this parser provides a generic integrator based on a theta method.";
  } // end of RungeKuttaFiniteStrainDSL::getDescription

  std::string RungeKuttaFiniteStrainDSL::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::ComputeThermodynamicForces) {
      return "@ComputeStress{}\n";
    }
    return RungeKuttaDSLBase::getCodeBlockTemplate(c, o);
  }  // end of RungeKuttaFiniteStrainDSL::getCodeBlockTemplate

  BehaviourDSLDescription RungeKuttaFiniteStrainDSL::getBehaviourDSLDescription() const {
    auto d = mfront::getDefaultFiniteStrainBehaviourDSLDescription();
    d.integrationScheme = IntegrationScheme::EXPLICITSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputePredictionOperator,
                           BehaviourData::ComputeThermodynamicForces,
                           BehaviourData::ComputeDerivative,
                           BehaviourData::ComputeTangentOperator};
    d.minimalMFrontFileBody = "@Derivative{}\n\n@ComputeFinalStress{};\n\n";
    return d;
  }  // end of RungeKuttaFiniteStrainDSL::getBehaviourDSLDescription

  RungeKuttaFiniteStrainDSL::~RungeKuttaFiniteStrainDSL() noexcept = default;

} // end of namespace mfront  
