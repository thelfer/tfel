/*!
 * \file   mfront/src/RousselierTanguyBesson2002StressCriterion.cxx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   14/04/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/StandardElastoViscoPlasticityBrick.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/RousselierTanguyBesson2002StressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    RousselierTanguyBesson2002StressCriterion::
        RousselierTanguyBesson2002StressCriterion()
        : StandardPorousStressCriterionBase("RousselierTanguyBesson2002") {
    }  // end of RousselierTanguyBesson2002StressCriterion

    std::vector<mfront::BehaviourSymmetryType>
    RousselierTanguyBesson2002StressCriterion::getSupportedBehaviourSymmetries()
        const {
      return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
    }  // end of getSupportedBehaviourSymmetries

    std::vector<OptionDescription>
    RousselierTanguyBesson2002StressCriterion::getOptions() const {
      auto opts = StressCriterionBase::getOptions();
      opts.emplace_back("DR", "first coefficient of the Roussielier model",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("qR", "second coefficient of the Roussielier model",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of getOptions

    StressCriterion::PorosityEffectOnFlowRule
    RousselierTanguyBesson2002StressCriterion::
        getPorosityEffectOnEquivalentPlasticStrain() const {
      return StressCriterion::
          STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN;
    }  // end of getPorosityEffectOnEquivalentPlasticStrain

    std::string
    RousselierTanguyBesson2002StressCriterion::updatePorosityUpperBound(
        const BehaviourDescription&,
        const std::string& id,
        const Role r) const {
      if (!((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION))) {
        tfel::raise(
            "RousselierTanguyBesson2002StressCriterion::"
            "updatePorosityUpperBound: invalid call for this stress criterion. "
            "This method is not valid for flow criteria (i.e. when the stress "
            "criterion is used to dertermine the flow direction in non "
            "associated plasticity).");
      }
      const auto bound = std::string(
          mfront::StandardElastoViscoPlasticityBrick::porosityUpperBound);
      const auto params =
          StressCriterion::getVariableId("sscb_parameters", id, r);
      return "this->" + bound + " = " +   //
             "std::min(this->" + bound +  //
             ", 3 / (2 * this->" + params + ".DR));\n";
    }  // end of
       // RousselierTanguyBesson2002StressCriterion::updatePorosityUpperBound()

    RousselierTanguyBesson2002StressCriterion::
        ~RousselierTanguyBesson2002StressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
