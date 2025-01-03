/*!
 * \file   mfront/src/IsotropicHardeningRuleFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/BehaviourBrick/DataIsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/VoceIsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/SwiftIsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/PowerIsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/LinearIsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/UserDefinedIsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/StrainRateSensitiveIsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRuleFactory.hxx"

namespace mfront::bbrick {

  IsotropicHardeningRuleFactory& IsotropicHardeningRuleFactory::getFactory() {
    static IsotropicHardeningRuleFactory i;
    return i;
  }  // end of getFactory

  std::vector<std::string>
  IsotropicHardeningRuleFactory::getRegistredIsotropicHardeningRules() const {
    return getKeys(this->generators);
  }  // end of
     // IsotropicHardeningRuleFactory::getRegistredIsotropicHardeningRules

  void IsotropicHardeningRuleFactory::addGenerator(const std::string& n,
                                                   const Generator& g) {
    if (!this->generators.insert({n, g}).second) {
      tfel::raise(
          "IsotropicHardeningRuleFactory::addGenerator: "
          "generator '" +
          n + "' already registred");
    }
  }  // end of addGenerator

  std::shared_ptr<IsotropicHardeningRule>
  IsotropicHardeningRuleFactory::generate(const std::string& n) const {
    const auto p = this->generators.find(n);
    if (p == this->generators.end()) {
      tfel::raise(
          "IsotropicHardeningRuleFactory::generate: "
          "no generator named '" +
          n + "'");
    }
    return p->second();
  }  // end of generate

  IsotropicHardeningRuleFactory::IsotropicHardeningRuleFactory() {
    this->addGenerator("Linear", []() {
      return std::make_shared<bbrick::LinearIsotropicHardeningRule>();
    });
    this->addGenerator("Swift", []() {
      return std::make_shared<bbrick::SwiftIsotropicHardeningRule>();
    });
    this->addGenerator("Power", []() {
      return std::make_shared<bbrick::PowerIsotropicHardeningRule>();
    });
    this->addGenerator("Voce", []() {
      return std::make_shared<bbrick::VoceIsotropicHardeningRule>();
    });
    this->addGenerator("Data", []() {
      return std::make_shared<bbrick::DataIsotropicHardeningRule>();
    });
    this->addGenerator("UserDefined", []() {
      return std::make_shared<bbrick::UserDefinedIsotropicHardeningRule>();
    });
    this->addGenerator("StrainRateSensitive", []() {
      return std::make_shared<
          bbrick::StrainRateSensitiveIsotropicHardeningRule>();
    });
  }  // end of IsotropicHardeningRuleFactory

  IsotropicHardeningRuleFactory::~IsotropicHardeningRuleFactory() = default;

}  // end of namespace mfront::bbrick
