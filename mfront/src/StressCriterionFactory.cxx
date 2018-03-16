/*!
 * \file   mfront/src/StressCriterionFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/StressCriterionFactory.hxx"

namespace mfront {

  namespace bbrick {

    StressCriterionFactory& StressCriterionFactory::getFactory() {
      static StressCriterionFactory i;
      return i;
    }  // end of StressCriterionFactory::getFactory

    void StressCriterionFactory::addGenerator(const std::string& n,
                                                 const Generator& g) {
      if (!this->generators.insert({n, g}).second) {
        tfel::raise(
            "StressCriterionFactory::addGenerator: "
            "generator '" +
            n + "' already registred");
      }
    }  // end of StressCriterionFactory::addGenerator

    std::shared_ptr<StressCriterion> StressCriterionFactory::generate(
        const std::string& n) const {
      const auto p = this->generators.find(n);
      if (p == this->generators.end()) {
        tfel::raise(
            "StressCriterionFactory::generate: "
            "no generator named '" +
            n + "'");
      }
      return p->second();
    }  // end of StressCriterionFactory::generate

    StressCriterionFactory::StressCriterionFactory() = default;

    StressCriterionFactory::~StressCriterionFactory() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
