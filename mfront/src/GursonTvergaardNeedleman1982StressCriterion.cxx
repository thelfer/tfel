/*!
 * \file   mfront/src/GursonTvergaardNeedleman1982StressCriterion.cxx
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
#include "MFront/StandardElastoViscoPlasticityBrick.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/GursonTvergaardNeedleman1982StressCriterion.hxx"

namespace mfront::bbrick {

  GursonTvergaardNeedleman1982StressCriterion::
      GursonTvergaardNeedleman1982StressCriterion()
      : StandardPorousStressCriterionBase("GursonTvergaardNeedleman1982") {
  }  // end of GursonTvergaardNeedleman1982StressCriterion

  std::vector<mfront::BehaviourSymmetryType>
  GursonTvergaardNeedleman1982StressCriterion::getSupportedBehaviourSymmetries()
      const {
    return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
  }  // end of getSupportedBehaviourSymmetries

  std::vector<OptionDescription>
  GursonTvergaardNeedleman1982StressCriterion::getOptions() const {
    auto opts = StressCriterionBase::getOptions();
    opts.emplace_back("f_c", "coalescence porosity",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("f_r", "fracture porosity",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("q_1", "first Tvergaard parameter",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("q_2", "second Tvergaard parameter",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("q_3", "third Tvergaard parameter",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  StressCriterion::PorosityEffectOnFlowRule
  GursonTvergaardNeedleman1982StressCriterion::
      getPorosityEffectOnEquivalentPlasticStrain() const {
    return StressCriterion::
        STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN;
  }  // end of getPorosityEffectOnEquivalentPlasticStrain

  std::string
  GursonTvergaardNeedleman1982StressCriterion::updatePorosityUpperBound(
      const BehaviourDescription&, const std::string& id, const Role r) const {
    if (!((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION))) {
      tfel::raise(
          "StressCriterionBase::updatePorosityUpperBound: invalid call for "
          "this stress criterion. This method is not valid for flow "
          "criteria (i.e. when the stress criterion is used to dertermine "
          "the flow direction in non associated plasticity).");
    }
    const auto bound = std::string(
        mfront::StandardElastoViscoPlasticityBrick::porosityUpperBound);
    const auto params =
        GursonTvergaardNeedleman1982StressCriterion::getVariableId(
            "sscb_parameters", id, r);
    return "this->" + bound + " = " +  //
           "std::min(this->" + bound + ", this->" + params + ".f_r);\n";
  }  // end of updatePorosityUpperBound

  GursonTvergaardNeedleman1982StressCriterion::
      ~GursonTvergaardNeedleman1982StressCriterion() = default;

}  // end of namespace mfront::bbrick
