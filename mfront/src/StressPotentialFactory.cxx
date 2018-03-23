/*!
 * \file   mfront/src/StressPotentialFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/HookeStressPotential.hxx"
#include "MFront/BehaviourBrick/DDIF2StressPotential.hxx"
#include "MFront/BehaviourBrick/StressPotentialFactory.hxx"

namespace mfront {

  namespace bbrick {

    StressPotentialFactory& StressPotentialFactory::getFactory() {
      static StressPotentialFactory i;
      return i;
    }  // end of StressPotentialFactory::getFactory

    void StressPotentialFactory::addGenerator(const std::string& n,
                                              const Generator& g) {
      if (!this->generators.insert({n, g}).second) {
        tfel::raise(
            "StressPotentialFactory::addGenerator: "
            "generator '" +
            n + "' already registred");
      }
    }  // end of StressPotentialFactory::addGenerator

    std::shared_ptr<StressPotential> StressPotentialFactory::generate(
        const std::string& n,
        AbstractBehaviourDSL& dsl,
        BehaviourDescription& bd,
        const DataMap& d) const {
      const auto p = this->generators.find(n);
      if (p == this->generators.end()) {
        tfel::raise(
            "StressPotentialFactory::generate: "
            "no generator named '" +
            n + "'");
      }
      return p->second(dsl, bd, d);
    }  // end of StressPotentialFactory::generate

    StressPotentialFactory::StressPotentialFactory() {
      this->addGenerator(
          "Hooke", [](AbstractBehaviourDSL& dsl, BehaviourDescription& bd,
                      const DataMap& d) {
            return std::make_shared<bbrick::HookeStressPotential>(dsl, bd, d);
          });
      this->addGenerator(
          "DDIF2", [](AbstractBehaviourDSL& dsl, BehaviourDescription& bd,
                      const DataMap& d) {
            return std::make_shared<bbrick::DDIF2StressPotential>(dsl, bd, d);
          });
    }  // end of StressPotentialFactory::StressPotentialFactory

    StressPotentialFactory::~StressPotentialFactory() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
