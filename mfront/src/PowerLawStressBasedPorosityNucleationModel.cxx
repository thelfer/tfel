/*!
 * \file   mfront/src/PowerLawStressBasedPorosityNucleationModel.cxx
 * \brief
 * \author Thomas Helfer
 * \date   05/04/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/PowerLawStressBasedPorosityNucleationModel.hxx"

namespace mfront {

  namespace bbrick {

    std::string PowerLawStressBasedPorosityNucleationModel::getModelName()
        const {
      return "PowerLawStressBasedNucleationModel";
    }  // end of
       // PowerLawStressBasedPorosityNucleationModel::getModelName

    std::vector<PorosityNucleationModelBase::MaterialCoefficientDescription>
    PowerLawStressBasedPorosityNucleationModel::
        getMaterialCoefficientDescriptions() const {
      auto mcs = StressBasedPorosityNucleationModelBase::
          getMaterialCoefficientDescriptions();
      mcs.push_back({"real", "fn", "law coefficient"});
      mcs.push_back(
          {"strain", "sn", "minimal value of equivalent pastic srain"});
      mcs.push_back({"real", "m", "exponent of the power law"});
      mcs.push_back({"real", "pmin", "minimal equivalent plastic strain"});
      return mcs;
    }  // end of getMaterialCoefficientDescriptions

    PowerLawStressBasedPorosityNucleationModel::
        ~PowerLawStressBasedPorosityNucleationModel() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
