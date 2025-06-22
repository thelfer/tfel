/*!
 * \file   mfront/src/InelasticFlowFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/BehaviourBrick/PlasticInelasticFlow.hxx"
#include "MFront/BehaviourBrick/NortonHoffViscoplasticFlow.hxx"
#include "MFront/BehaviourBrick/HyperbolicSineViscoplasticFlow.hxx"
#include "MFront/BehaviourBrick/HarmonicSumOfNortonHoffViscoplasticFlows.hxx"
#include "MFront/BehaviourBrick/UserDefinedViscoplasticFlow.hxx"
#include "MFront/BehaviourBrick/InelasticFlowFactory.hxx"

namespace mfront::bbrick {

  InelasticFlowFactory& InelasticFlowFactory::getFactory() {
    static InelasticFlowFactory i;
    return i;
  }  // end of getFactory

  void InelasticFlowFactory::addGenerator(const std::string& n,
                                          const Generator& g) {
    if (!this->generators.insert({n, g}).second) {
      tfel::raise(
          "InelasticFlowFactory::addGenerator: "
          "generator '" +
          n + "' already registred");
    }
  }  // end of addGenerator

  std::vector<std::string> InelasticFlowFactory::getRegistredInelasticFlows()
      const {
    return getKeys(this->generators);
  }  // end of getRegistredInelasticFlows

  std::shared_ptr<InelasticFlow> InelasticFlowFactory::generate(
      const std::string& n) const {
    const auto p = this->generators.find(n);
    if (p == this->generators.end()) {
      tfel::raise(
          "InelasticFlowFactory::generate: "
          "no generator named '" +
          n + "'");
    }
    return p->second();
  }  // end of generate

  InelasticFlowFactory::InelasticFlowFactory() {
    this->addGenerator("Norton", []() {
      return std::make_shared<NortonHoffViscoplasticFlow>();
    });
    this->addGenerator("Norton Hoff", []() {
      return std::make_shared<NortonHoffViscoplasticFlow>();
    });
    this->addGenerator("Norton-Hoff", []() {
      return std::make_shared<NortonHoffViscoplasticFlow>();
    });
    this->addGenerator("HyperbolicSine", []() {
      return std::make_shared<HyperbolicSineViscoplasticFlow>();
    });
    this->addGenerator("UserDefinedViscoplasticity", []() {
      return std::make_shared<UserDefinedViscoplasticFlow>();
    });
    this->addGenerator(
        "Plastic", []() { return std::make_shared<PlasticInelasticFlow>(); });
    this->addGenerator("HarmonicSumOfNortonHoffViscoplasticFlows", []() {
      return std::make_shared<HarmonicSumOfNortonHoffViscoplasticFlows>();
    });
  }  // end of InelasticFlowFactory

  InelasticFlowFactory::~InelasticFlowFactory() = default;

}  // end of namespace mfront::bbrick
