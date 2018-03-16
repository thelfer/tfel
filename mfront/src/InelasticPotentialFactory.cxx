/*!
 * \file   mfront/src/InelasticPotentialFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/InelasticPotentialFactory.hxx"

namespace mfront {

  namespace bbrick {

    InelasticPotentialFactory& InelasticPotentialFactory::getFactory() {
      static InelasticPotentialFactory i;
      return i;
    }  // end of InelasticPotentialFactory::getFactory

    void InelasticPotentialFactory::addGenerator(const std::string& n,
                                                 const Generator& g) {
      if (!this->generators.insert({n, g}).second) {
        tfel::raise(
            "InelasticPotentialFactory::addGenerator: "
            "generator '" +
            n + "' already registred");
      }
    }  // end of InelasticPotentialFactory::addGenerator

    std::shared_ptr<InelasticPotential> InelasticPotentialFactory::generate(
        const std::string& n) const {
      const auto p = this->generators.find(n);
      if (p == this->generators.end()) {
        tfel::raise(
            "InelasticPotentialFactory::generate: "
            "no generator named '" +
            n + "'");
      }
      return p->second();
    }  // end of InelasticPotentialFactory::generate

    InelasticPotentialFactory::InelasticPotentialFactory() = default;

    InelasticPotentialFactory::~InelasticPotentialFactory() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
