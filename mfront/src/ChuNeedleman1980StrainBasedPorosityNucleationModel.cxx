/*!
 * \file   mfront/src/ChuNeedleman1980StrainBasedPorosityNucleationModel.cxx
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
#include "MFront/BehaviourBrick/ChuNeedleman1980StrainBasedPorosityNucleationModel.hxx"

namespace mfront::bbrick {

  std::string ChuNeedleman1980StrainBasedPorosityNucleationModel::getModelName()
      const {
    return "ChuNeedleman1980StrainBasedNucleationModel";
  }  // end of getModelName

  bool ChuNeedleman1980StrainBasedPorosityNucleationModel::
      requiresLimitOnNucleationPorosity() const {
    return false;
  }  // end of requiresMaximumNucleationPorosity

  std::vector<PorosityNucleationModelBase::MaterialCoefficientDescription>
  ChuNeedleman1980StrainBasedPorosityNucleationModel::
      getMaterialCoefficientDescriptions() const {
    auto mcs = StrainBasedPorosityNucleationModelBase::
        getMaterialCoefficientDescriptions();
    mcs.push_back({"real", "fn", "porosity rate coefficient"});
    mcs.push_back({"strain", "en", "mean value of the gaussian distribution"});
    mcs.push_back(
        {"strain", "sn", "standard deviation of the gaussian distribution"});
    return mcs;
  }  // end of getMaterialCoefficientDescriptions

  ChuNeedleman1980StrainBasedPorosityNucleationModel::
      ~ChuNeedleman1980StrainBasedPorosityNucleationModel() = default;

}  // end of namespace mfront::bbrick
