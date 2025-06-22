/*!
 * \file   mfront/src/MichelAndSuquet1992HollowSphereStressCriterion.cxx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   20/07/2020
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
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/MichelAndSuquet1992HollowSphereStressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    MichelAndSuquet1992HollowSphereStressCriterion::
        MichelAndSuquet1992HollowSphereStressCriterion()
        : StandardPorousStressCriterionBase("MichelAndSuquet1992HollowSphere") {
    }  // end of
    // MichelAndSuquet1992HollowSphereStressCriterion::MichelAndSuquet1992HollowSphereStressCriterion

    std::vector<mfront::BehaviourSymmetryType>
    MichelAndSuquet1992HollowSphereStressCriterion::
        getSupportedBehaviourSymmetries() const {
      return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
    }  // end of
    // MichelAndSuquet1992HollowSphereStressCriterion::getSupportedBehaviourSymmetries()

    std::vector<OptionDescription>
    MichelAndSuquet1992HollowSphereStressCriterion::getOptions() const {
      auto opts = StressCriterionBase::getOptions();
      opts.emplace_back("n", "Norton exponent of the matrix",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of MichelAndSuquet1992HollowSphereStressCriterion::getOptions()

    StressCriterion::PorosityEffectOnFlowRule
    MichelAndSuquet1992HollowSphereStressCriterion::
        getPorosityEffectOnEquivalentPlasticStrain() const {
      return StressCriterion::NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN;
    }  // end of
       // MichelAndSuquet1992HollowSphereStressCriterion::getPorosityEffectOnEquivalentPlasticStrain()

    MichelAndSuquet1992HollowSphereStressCriterion::
        ~MichelAndSuquet1992HollowSphereStressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
