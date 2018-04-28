/*!
 * \file   mfront/src/StressCriterionFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/MisesStressCriterion.hxx"
#include "MFront/BehaviourBrick/Hill1948StressCriterion.hxx"
#include "MFront/BehaviourBrick/Barlat2004StressCriterion.hxx"
#include "MFront/BehaviourBrick/Hosford1972StressCriterion.hxx"
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

    StressCriterionFactory::StressCriterionFactory() {
      this->addGenerator("Mises", []() {
        return std::make_shared<bbrick::MisesStressCriterion>();
      });
      this->addGenerator("Hill", []() {
        return std::make_shared<bbrick::Hill1948StressCriterion>();
      });
      this->addGenerator("Hill1948", []() {
        return std::make_shared<bbrick::Hill1948StressCriterion>();
      });
      this->addGenerator("Hosford1972", []() {
        return std::make_shared<bbrick::Hosford1972StressCriterion>();
      });
      this->addGenerator("Hosford", []() {
        return std::make_shared<bbrick::Hosford1972StressCriterion>();
      });
      this->addGenerator("Barlat2004", []() {
        return std::make_shared<bbrick::Barlat2004StressCriterion>();
      });
      this->addGenerator("Barlat", []() {
        return std::make_shared<bbrick::Barlat2004StressCriterion>();
      });
    } // end of StressCriterionFactory::StressCriterionFactory

    StressCriterionFactory::~StressCriterionFactory() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
