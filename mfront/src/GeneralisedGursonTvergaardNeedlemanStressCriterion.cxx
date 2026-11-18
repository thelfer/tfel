/*!
 * \file   mfront/src/GeneralisedGursonTvergaardNeedlemanStressCriterion.cxx
 * \brief  This file implements the
 * GeneralisedGursonTvergaardNeedlemanStressCriterion class
 * \author Tristan Chenaille
 * \date   14/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/GeneralisedGursonTvergaardNeedlemanStressCriterion.hxx"

namespace mfront::bbrick {

  GeneralisedGursonTvergaardNeedlemanStressCriterion::
      GeneralisedGursonTvergaardNeedlemanStressCriterion()
      : GursonTvergaardNeedleman1982StressCriterion(
            "GeneralisedGursonTvergaardNeedleman"),
        equivalent_stress("GeneralisedGursonTvergaardNeedleman") {
  }  // end of GeneralisedGursonTvergaardNeedlemanStressCriterion

  std::vector<OptionDescription>
  GeneralisedGursonTvergaardNeedlemanStressCriterion::getOptions() const {
    auto opts = GursonTvergaardNeedleman1982StressCriterion::getOptions();
    opts.push_back(PorousStressCriterionEquivalentStress::getOption());
    return opts;
  }  // end of getOptions

  void GeneralisedGursonTvergaardNeedlemanStressCriterion::initialize(
      BehaviourDescription& bd,
      AbstractBehaviourDSL& dsl,
      const std::string& id,
      const DataMap& d,
      const Role r) {
    GursonTvergaardNeedleman1982StressCriterion::initialize(bd, dsl, id, d, r);
    this->equivalent_stress.initialize(bd, dsl, id, d, r);
  }  // end of initialize

  void GeneralisedGursonTvergaardNeedlemanStressCriterion::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& id,
      const Role r) {
    GursonTvergaardNeedleman1982StressCriterion::endTreatment(bd, dsl, id, r);
    this->equivalent_stress.endTreatment(bd, dsl, id, r);
  }  // end of endTreatment

  std::string
  GeneralisedGursonTvergaardNeedlemanStressCriterion::computeElasticPrediction(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp) const {
    return this->equivalent_stress.computeElasticPrediction(id, bd, sp);
  }  // end of computeElasticPrediction

  std::string
  GeneralisedGursonTvergaardNeedlemanStressCriterion::computeCriterion(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp) const {
    return this->equivalent_stress.computeCriterion(id, bd, sp);
  }  // end of computeCriterion

  std::string GeneralisedGursonTvergaardNeedlemanStressCriterion::computeNormal(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    return this->equivalent_stress.computeNormal(id, bd, sp, r);
  }  // end of computeNormal

  std::string
  GeneralisedGursonTvergaardNeedlemanStressCriterion::computeNormalDerivative(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    return this->equivalent_stress.computeNormalDerivative(id, bd, sp, r);
  }  // end of computeNormalDerivative

  GeneralisedGursonTvergaardNeedlemanStressCriterion::
      ~GeneralisedGursonTvergaardNeedlemanStressCriterion() = default;

}  // end of namespace mfront::bbrick
