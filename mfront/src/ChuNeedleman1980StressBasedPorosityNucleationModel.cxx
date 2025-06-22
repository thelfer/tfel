/*!
 * \file   mfront/src/ChuNeedleman1980StressBasedPorosityNucleationModel.cxx
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
#include "MFront/BehaviourBrick/ChuNeedleman1980StressBasedPorosityNucleationModel.hxx"

namespace mfront {

  namespace bbrick {

    std::string
    ChuNeedleman1980StressBasedPorosityNucleationModel::getModelName() const {
      return "ChuNeedleman1980StressBasedNucleationModel";
    }  // end of
       // ChuNeedleman1980StressBasedPorosityNucleationModel::getModelName

    std::vector<PorosityNucleationModelBase::MaterialCoefficientDescription>
    ChuNeedleman1980StressBasedPorosityNucleationModel::
        getMaterialCoefficientDescriptions() const {
      auto mcs = StressBasedPorosityNucleationModelBase::
          getMaterialCoefficientDescriptions();
      mcs.push_back({"real", "fn", "porosity rate coefficient"});
      mcs.push_back(
          {"stress", "sigm", "mean value of the gaussian distribution"});
      mcs.push_back(
          {"stress", "sn", "standard deviation of the gaussian distribution"});
      return mcs;
    }  // end of getMaterialCoefficientDescriptions

    ChuNeedleman1980StressBasedPorosityNucleationModel::
        ~ChuNeedleman1980StressBasedPorosityNucleationModel() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
