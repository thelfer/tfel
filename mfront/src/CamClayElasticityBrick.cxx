/*!
 * \file  mfront/src/CamClayElasticityBrick.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25/02/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressPotentialFactory.hxx"
#include "MFront/CamClayElasticityBrick.hxx"

namespace mfront {

  CamClayElasticityBrick::CamClayElasticityBrick(AbstractBehaviourDSL& dsl_,
                                                 BehaviourDescription& mb_)
      : BehaviourBrickBase(dsl_, mb_) {
    auto& spf = mfront::bbrick::StressPotentialFactory::getFactory();
    this->ccsp = spf.generate("CamClay");
  }  // end of CamClayElasticityBrick

  std::string CamClayElasticityBrick::getName() const {
    return "CamClayElasticity";
  }  // end of getName

  BehaviourBrickDescription CamClayElasticityBrick::getDescription() const {
    auto d = BehaviourBrickDescription{};
    d.behaviourType =
        tfel::material::MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR;
    d.integrationScheme = IntegrationScheme::IMPLICITSCHEME;
    d.supportedModellingHypotheses = {
        ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
        ModellingHypothesis::AXISYMMETRICAL, ModellingHypothesis::PLANESTRAIN,
        ModellingHypothesis::GENERALISEDPLANESTRAIN,
        ModellingHypothesis::TRIDIMENSIONAL};
    d.supportedBehaviourSymmetries = {mfront::ISOTROPIC};
    return d;
  }  // end of getDescription

  std::vector<bbrick::OptionDescription> CamClayElasticityBrick::getOptions(
      const bool b) const {
    return this->ccsp->getOptions(this->bd, b);
  }  // end of getOptions

  void CamClayElasticityBrick::initialize(const Parameters&, const DataMap& d) {
    this->ccsp->initialize(this->bd, this->dsl, d);
  }  // end of initialize

  void CamClayElasticityBrick::completeVariableDeclaration() const {
    this->ccsp->completeVariableDeclaration(this->bd, this->dsl);
  }  // end of completeVariableDeclaration

  void CamClayElasticityBrick::endTreatment() const {
    this->ccsp->endTreatment(this->bd, this->dsl);
  }  // end of endTreatment

  std::vector<tfel::material::ModellingHypothesis::Hypothesis>
  CamClayElasticityBrick::getSupportedModellingHypotheses() const {
    return this->ccsp->getSupportedModellingHypotheses(this->bd, this->dsl);
  }  // end of getSupportedModellingHypothesis

  CamClayElasticityBrick::~CamClayElasticityBrick() = default;

}  // end of namespace mfront
