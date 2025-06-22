/*!
 * \file   mfront/src/PowerLawStrainBasedPorosityNucleationModel.cxx
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
#include "MFront/BehaviourBrick/PowerLawStrainBasedPorosityNucleationModel.hxx"

namespace mfront {

  namespace bbrick {

    std::string PowerLawStrainBasedPorosityNucleationModel::getModelName()
        const {
      return "PowerLawStrainBasedNucleationModel";
    }  // end of getModelName

    bool PowerLawStrainBasedPorosityNucleationModel::
        requiresLimitOnNucleationPorosity() const {
      return true;
    }  // end of requiresMaximumNucleationPorosity

    std::vector<PorosityNucleationModelBase::MaterialCoefficientDescription>
    PowerLawStrainBasedPorosityNucleationModel::
        getMaterialCoefficientDescriptions() const {
      auto mcs = StrainBasedPorosityNucleationModelBase::
          getMaterialCoefficientDescriptions();
      mcs.push_back({"real", "fn", "law coefficient"});
      mcs.push_back(
          {"strain", "en", "minimal value of equivalent pastic srain"});
      mcs.push_back({"real", "m", "exponent of the power law"});
      return mcs;
    }  // end of getMaterialCoefficientDescriptions

    PowerLawStrainBasedPorosityNucleationModel::
        ~PowerLawStrainBasedPorosityNucleationModel() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
