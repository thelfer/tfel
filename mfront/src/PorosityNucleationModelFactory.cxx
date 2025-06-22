/*!
 * \file   mfront/src/PorosityNucleationModelFactory.cxx
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
#include "MFront/BehaviourBrick/PowerLawStrainBasedPorosityNucleationModel.hxx"
#include "MFront/BehaviourBrick/PowerLawStressBasedPorosityNucleationModel.hxx"
#include "MFront/BehaviourBrick/ChuNeedleman1980StrainBasedPorosityNucleationModel.hxx"
#include "MFront/BehaviourBrick/ChuNeedleman1980StressBasedPorosityNucleationModel.hxx"
#include "MFront/BehaviourBrick/PorosityNucleationModelFactory.hxx"

namespace mfront {

  namespace bbrick {

    PorosityNucleationModelFactory&
    PorosityNucleationModelFactory::getFactory() {
      static PorosityNucleationModelFactory i;
      return i;
    }  // end of PorosityNucleationModelFactory::getFactory

    void PorosityNucleationModelFactory::addGenerator(const std::string& n,
                                                      const Generator& g) {
      if (!this->generators.insert({n, g}).second) {
        tfel::raise(
            "PorosityNucleationModelFactory::addGenerator: "
            "generator '" +
            n + "' already registred");
      }
    }  // end of PorosityNucleationModelFactory::addGenerator

    std::vector<std::string>
    PorosityNucleationModelFactory::getRegistredPorosityNucleationModels()
        const {
      return getKeys(this->generators);
    }  // end of
       // PorosityNucleationModelFactory::getRegistredPorosityNucleationModels

    std::shared_ptr<PorosityNucleationModel>
    PorosityNucleationModelFactory::generate(const std::string& n) const {
      const auto p = this->generators.find(n);
      if (p == this->generators.end()) {
        tfel::raise(
            "PorosityNucleationModelFactory::generate: "
            "no generator named '" +
            n + "'");
      }
      return p->second();
    }  // end of PorosityNucleationModelFactory::generate

    PorosityNucleationModelFactory::PorosityNucleationModelFactory() {
      this->addGenerator("Chu-Needleman 1980 (strain)", []() {
        return std::make_shared<
            ChuNeedleman1980StrainBasedPorosityNucleationModel>();
      });
      this->addGenerator("ChuNeedleman1980 (strain)", []() {
        return std::make_shared<
            ChuNeedleman1980StrainBasedPorosityNucleationModel>();
      });
      this->addGenerator("Chu-Needleman 1980 (stress)", []() {
        return std::make_shared<
            ChuNeedleman1980StressBasedPorosityNucleationModel>();
      });
      this->addGenerator("ChuNeedleman1980 (stress)", []() {
        return std::make_shared<
            ChuNeedleman1980StressBasedPorosityNucleationModel>();
      });
      this->addGenerator("PowerLaw (strain)", []() {
        return std::make_shared<PowerLawStrainBasedPorosityNucleationModel>();
      });
      this->addGenerator("PowerLaw (stress)", []() {
        return std::make_shared<PowerLawStressBasedPorosityNucleationModel>();
      });
    }  // end of PorosityNucleationModelFactory::PorosityNucleationModelFactory

    PorosityNucleationModelFactory::~PorosityNucleationModelFactory() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
