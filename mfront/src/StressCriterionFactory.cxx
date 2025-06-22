/*!
 * \file   mfront/src/StressCriterionFactory.cxx
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
#include "MFront/BehaviourBrick/MisesStressCriterion.hxx"
#include "MFront/BehaviourBrick/Hill1948StressCriterion.hxx"
#include "MFront/BehaviourBrick/Barlat2004StressCriterion.hxx"
#include "MFront/BehaviourBrick/Hosford1972StressCriterion.hxx"
#include "MFront/BehaviourBrick/Drucker1949StressCriterion.hxx"
#include "MFront/BehaviourBrick/Cazacu2001StressCriterion.hxx"
#include "MFront/BehaviourBrick/Cazacu2004IsotropicStressCriterion.hxx"
#include "MFront/BehaviourBrick/Cazacu2004OrthotropicStressCriterion.hxx"
#include "MFront/BehaviourBrick/MohrCoulombStressCriterion.hxx"
#include "MFront/BehaviourBrick/MichelAndSuquet1992HollowSphereStressCriterion.hxx"
#include "MFront/BehaviourBrick/GursonTvergaardNeedleman1982StressCriterion.hxx"
#include "MFront/BehaviourBrick/RousselierTanguyBesson2002StressCriterion.hxx"
#include "MFront/BehaviourBrick/StressCriterionFactory.hxx"

namespace mfront {

  namespace bbrick {

    StressCriterionFactory& StressCriterionFactory::getFactory() {
      static StressCriterionFactory i;
      return i;
    }  // end of StressCriterionFactory::getFactory

    std::vector<std::string>
    StressCriterionFactory::getRegistredStressCriteria() const {
      return getKeys(this->generators);
    }  // end of StressCriterionFactory::getRegistredStressCriteria

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
        auto msg = std::string(
            "StressCriterionFactory::generate: "
            "no generator named '" +
            n + "'. Available criterion are:");
        for (const auto& g : this->generators) {
          msg += "\n- " + g.first;
        }
        tfel::raise(msg);
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
      this->addGenerator("Hill 1948", []() {
        return std::make_shared<bbrick::Hill1948StressCriterion>();
      });
      this->addGenerator("Drucker1949", []() {
        return std::make_shared<bbrick::Drucker1949StressCriterion>();
      });
      this->addGenerator("Drucker 1949", []() {
        return std::make_shared<bbrick::Drucker1949StressCriterion>();
      });
      this->addGenerator("Cazacu2001", []() {
        return std::make_shared<bbrick::Cazacu2001StressCriterion>();
      });
      this->addGenerator("Cazacu 2001", []() {
        return std::make_shared<bbrick::Cazacu2001StressCriterion>();
      });
      this->addGenerator("IsotropicCazacu2004", []() {
        return std::make_shared<bbrick::Cazacu2004IsotropicStressCriterion>();
      });
      this->addGenerator("Isotropic Cazacu 2004", []() {
        return std::make_shared<bbrick::Cazacu2004IsotropicStressCriterion>();
      });
      this->addGenerator("OrthotropicCazacu2004", []() {
        return std::make_shared<bbrick::Cazacu2004OrthotropicStressCriterion>();
      });
      this->addGenerator("Orthotropic Cazacu 2004", []() {
        return std::make_shared<bbrick::Cazacu2004OrthotropicStressCriterion>();
      });
      this->addGenerator("Hosford", []() {
        return std::make_shared<bbrick::Hosford1972StressCriterion>();
      });
      this->addGenerator("Hosford1972", []() {
        return std::make_shared<bbrick::Hosford1972StressCriterion>();
      });
      this->addGenerator("Hosford 1972", []() {
        return std::make_shared<bbrick::Hosford1972StressCriterion>();
      });
      this->addGenerator("Barlat", []() {
        return std::make_shared<bbrick::Barlat2004StressCriterion>();
      });
      this->addGenerator("Barlat2004", []() {
        return std::make_shared<bbrick::Barlat2004StressCriterion>();
      });
      this->addGenerator("Barlat 2004", []() {
        return std::make_shared<bbrick::Barlat2004StressCriterion>();
      });
      this->addGenerator("MohrCoulomb", []() {
        return std::make_shared<bbrick::MohrCoulombStressCriterion>();
      });
      this->addGenerator("MichelAndSuquet1992HollowSphere", []() {
        return std::make_shared<
            bbrick::MichelAndSuquet1992HollowSphereStressCriterion>();
      });
      this->addGenerator("GursonTvergaardNeedleman1982", []() {
        return std::make_shared<
            bbrick::GursonTvergaardNeedleman1982StressCriterion>();
      });
      this->addGenerator("GTN", []() {
        return std::make_shared<
            bbrick::GursonTvergaardNeedleman1982StressCriterion>();
      });
      this->addGenerator("GTN 1982", []() {
        return std::make_shared<
            bbrick::GursonTvergaardNeedleman1982StressCriterion>();
      });
      this->addGenerator("RousselierTanguyBesson2002", []() {
        return std::make_shared<
            bbrick::RousselierTanguyBesson2002StressCriterion>();
      });
      this->addGenerator("RousselierTanguyBesson 2002", []() {
        return std::make_shared<
            bbrick::RousselierTanguyBesson2002StressCriterion>();
      });
    }  // end of StressCriterionFactory::StressCriterionFactory

    StressCriterionFactory::~StressCriterionFactory() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
