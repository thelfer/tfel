/*!
 * \file   mfront/src/StressPotentialFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date 15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/BehaviourBrick/HookeStressPotential.hxx"
#include "MFront/BehaviourBrick/DDIF2StressPotential.hxx"
#include "MFront/BehaviourBrick/ScalarDamageHookeStressPotential.hxx"
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
    this->addGenerator("IsotropicDamage", [] {
      return std::make_shared<bbrick::ScalarDamageHookeStressPotential>();
    });
    this->addGenerator("ScalarDamage", [] {
      return std::make_shared<bbrick::ScalarDamageHookeStressPotential>();
    });
  }  // end of StressPotentialFactory

  StressPotentialFactory::~StressPotentialFactory() = default;

}  // end of namespace mfront::bbrick
