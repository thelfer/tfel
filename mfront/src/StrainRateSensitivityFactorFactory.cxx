/*!
 * \file   mfront/src/StrainRateSensitivityFactorFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/11/2024
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/BehaviourBrick/StrainRateSensitivityFactor.hxx"
#include "MFront/BehaviourBrick/StrainRateSensitivityFactorFactory.hxx"
#include "MFront/BehaviourBrick/CowperSymondsStrainRateSensitivityFactor.hxx"
#include "MFront/BehaviourBrick/JohnsonCookStrainRateSensitivityFactor.hxx"

namespace mfront::bbrick {

  StrainRateSensitivityFactorFactory&
  StrainRateSensitivityFactorFactory::getFactory() {
    static StrainRateSensitivityFactorFactory i;
    return i;
  }  // end of getFactory

  std::vector<std::string>
  StrainRateSensitivityFactorFactory::getRegistredStrainRateSensitivityFactors()
      const {
    return getKeys(this->generators);
  }  // end of getRegistredStrainRateSensitivityFactors

  void StrainRateSensitivityFactorFactory::addGenerator(const std::string& n,
                                                        const Generator& g) {
    if (!this->generators.insert({n, g}).second) {
      tfel::raise(
          "StrainRateSensitivityFactorFactory::addGenerator: "
          "generator '" +
          n + "' already registred");
    }
  }  // end of addGenerator

  std::shared_ptr<StrainRateSensitivityFactor>
  StrainRateSensitivityFactorFactory::generate(const std::string& n) const {
    const auto p = this->generators.find(n);
    if (p == this->generators.end()) {
      tfel::raise(
          "StrainRateSensitivityFactorFactory::generate: "
          "no generator named '" +
          n + "'");
    }
    return p->second();
  }  // end of generate

  StrainRateSensitivityFactorFactory::StrainRateSensitivityFactorFactory() {
    this->addGenerator("CowperSymonds", []() {
      return std::make_shared<
          bbrick::CowperSymondsStrainRateSensitivityFactor>();
    });
    this->addGenerator("JohnsonCook", []() {
      return std::make_shared<bbrick::JohnsonCookStrainRateSensitivityFactor>();
    });
  }  // end of StrainRateSensitivityFactorFactory

  StrainRateSensitivityFactorFactory::~StrainRateSensitivityFactorFactory() =
      default;

}  // end of namespace mfront::bbrick
