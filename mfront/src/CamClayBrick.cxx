/*!
 * \file  mfront/src/CamClayBrick.cxx
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
#include "MFront/CamClayBrick.hxx"

namespace mfront {

  CamClayBrick::CamClayBrick(AbstractBehaviourDSL& dsl_,
                             BehaviourDescription& mb_)
      : BehaviourBrickBase(dsl_, mb_) {
    auto& spf = mfront::bbrick::StressPotentialFactory::getFactory();
    this->ccsp = spf.generate("CamClay");
  }  // end of CamClayBrick

  std::string CamClayBrick::getName() const {
    return "CamClay";
  }  // end of getName

  BehaviourBrickDescription CamClayBrick::getDescription() const {
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

  std::vector<bbrick::OptionDescription> CamClayBrick::getOptions(
      const bool b) const {
    return this->ccsp->getOptions(this->bd, b);
  }  // end of getOptions

  void CamClayBrick::initialize(const Parameters&, const DataMap& d) {
    this->ccsp->initialize(this->bd, this->dsl, d);
  }  // end of initialize

  void CamClayBrick::completeVariableDeclaration() const {
    this->ccsp->completeVariableDeclaration(this->bd, this->dsl);
  }  // end of completeVariableDeclaration

  void CamClayBrick::endTreatment() const {
    this->ccsp->endTreatment(this->bd, this->dsl);
  }  // end of endTreatment

  std::vector<tfel::material::ModellingHypothesis::Hypothesis>
  CamClayBrick::getSupportedModellingHypotheses() const {
    return this->ccsp->getSupportedModellingHypotheses(this->bd, this->dsl);
  }  // end of getSupportedModellingHypothesis

  CamClayBrick::~CamClayBrick() = default;

}  // end of namespace mfront
