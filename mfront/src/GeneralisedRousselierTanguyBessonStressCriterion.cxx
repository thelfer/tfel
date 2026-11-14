/*!
 * \file   mfront/src/GeneralisedRousselierTanguyBessonStressCriterion.cxx
 * \brief  This file implements the
 * GeneralisedRousselierTanguyBessonStressCriterion class
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
#include "MFront/BehaviourBrick/GeneralisedRousselierTanguyBessonStressCriterion.hxx"

namespace mfront::bbrick {

  GeneralisedRousselierTanguyBessonStressCriterion::
      GeneralisedRousselierTanguyBessonStressCriterion()
      : RousselierTanguyBesson2002StressCriterion(
            "GeneralisedRousselierTanguyBesson"),
        equivalent_stress("GeneralisedRousselierTanguyBesson") {
  }  // end of GeneralisedRousselierTanguyBessonStressCriterion

  std::vector<OptionDescription>
  GeneralisedRousselierTanguyBessonStressCriterion::getOptions() const {
    auto opts = RousselierTanguyBesson2002StressCriterion::getOptions();
    opts.push_back(PorousStressCriterionEquivalentStress::getOption());
    return opts;
  }  // end of getOptions

  void GeneralisedRousselierTanguyBessonStressCriterion::initialize(
      BehaviourDescription& bd,
      AbstractBehaviourDSL& dsl,
      const std::string& id,
      const DataMap& d,
      const Role r) {
    RousselierTanguyBesson2002StressCriterion::initialize(bd, dsl, id, d, r);
    this->equivalent_stress.initialize(bd, dsl, id, d, r);
  }  // end of initialize

  void GeneralisedRousselierTanguyBessonStressCriterion::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& id,
      const Role r) {
    RousselierTanguyBesson2002StressCriterion::endTreatment(bd, dsl, id, r);
    this->equivalent_stress.endTreatment(bd, dsl, id, r);
  }  // end of endTreatment

  std::string
  GeneralisedRousselierTanguyBessonStressCriterion::computeElasticPrediction(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp) const {
    return this->equivalent_stress.computeElasticPrediction(id, bd, sp);
  }  // end of computeElasticPrediction

  std::string
  GeneralisedRousselierTanguyBessonStressCriterion::computeCriterion(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp) const {
    return this->equivalent_stress.computeCriterion(id, bd, sp);
  }  // end of computeCriterion

  std::string GeneralisedRousselierTanguyBessonStressCriterion::computeNormal(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    return this->equivalent_stress.computeNormal(id, bd, sp, r);
  }  // end of computeNormal

  std::string
  GeneralisedRousselierTanguyBessonStressCriterion::computeNormalDerivative(
      const std::string& id,
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const Role r) const {
    return this->equivalent_stress.computeNormalDerivative(id, bd, sp, r);
  }  // end of computeNormalDerivative

  GeneralisedRousselierTanguyBessonStressCriterion::
      ~GeneralisedRousselierTanguyBessonStressCriterion() = default;

}  // end of namespace mfront::bbrick
