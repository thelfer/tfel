/*!
 * \file   mfront/src/StressPotentialFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include "TFEL/Raise.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/BehaviourBrick/HookeStressPotential.hxx"
#include "MFront/BehaviourBrick/DDIF2StressPotential.hxx"
#include "MFront/BehaviourBrick/CamClayStressPotential.hxx"
#include "MFront/BehaviourBrick/IsotropicDamageHookeStressPotential.hxx"
#include "MFront/BehaviourBrick/StressPotentialFactory.hxx"

namespace mfront::bbrick {

  StressPotentialFactory& StressPotentialFactory::getFactory() {
    static StressPotentialFactory i;
    return i;
  }  // end of getFactory

  void StressPotentialFactory::addGenerator(const std::string& n,
                                            const Generator& g) {
    if (!this->generators.insert({n, g}).second) {
      tfel::raise(
          "StressPotentialFactory::addGenerator: "
          "generator '" +
          n + "' already registred");
    }
  }  // end of addGenerator

  std::vector<std::string>
  StressPotentialFactory::getRegistredStressPotentials() const {
    return getKeys(this->generators);
  }  // end of getRegistredStressPotentials

  std::shared_ptr<StressPotential> StressPotentialFactory::generate(
      const std::string& n) const {
    const auto p = this->generators.find(n);
    if (p == this->generators.end()) {
      tfel::raise(
          "StressPotentialFactory::generate: "
          "no generator named '" +
          n + "'");
    }
    return p->second();
  }  // end of generate

  StressPotentialFactory::StressPotentialFactory() {
    this->addGenerator("Hooke", []() {
      return std::make_shared<bbrick::HookeStressPotential>();
    });
    this->addGenerator("DDIF2", [] {
      return std::make_shared<bbrick::DDIF2StressPotential>();
    });
    this->addGenerator("CamClay", [] {
      return std::make_shared<bbrick::CamClayStressPotential>();
    });
    this->addGenerator("IsotropicDamage", [] {
      return std::make_shared<bbrick::IsotropicDamageHookeStressPotential>();
    });
  }  // end of StressPotentialFactory

  StressPotentialFactory::~StressPotentialFactory() = default;

}  // end of namespace mfront::bbrick
