/*!
 * \file   StressCriterionBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription> StressCriterionBase::getOptions() const {
    return {};
  }  // end of StressCriterionBase::getOptions

  void StressCriterionBase::initialize(BehaviourDescription& bd,
                                       AbstractBehaviourDSL&,
                                       const std::string& id,
                                       const DataMap& d,
                                       const Role r) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    // checking options
    mfront::bbrick::check(d, this->getOptions());
    // declare the porosity if required
    if (this->isCoupledWithPorosityEvolution()) {
      addStateVariableIfNotDefined(
          bd, "real", "f", tfel::glossary::Glossary::Porosity, 1u, true);
    }
    // reserve names
    if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
      bd.reserveName(uh, "seqel" + id);
      bd.reserveName(uh, "seq" + id);
      bd.reserveName(uh, "iseq" + id);
      bd.reserveName(uh, "dseq" + id + "_ds" + id);
      bd.reserveName(uh, "d2seq" + id + "_ds" + id + "_ds" + id);
      if (this->isCoupledWithPorosityEvolution()) {
        const auto& f =
            bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
                tfel::glossary::Glossary::Porosity);
        bd.reserveName(uh, "dseq" + id + "_d" + f.name);
        bd.reserveName(uh, "d2seq" + id + "_ds" + id + "_d" + f.name);
      }
    }
    if (r == FLOWCRITERION) {
      bd.reserveName(uh, "seqf" + id);
      bd.reserveName(uh, "iseqf" + id);
      if (this->isCoupledWithPorosityEvolution()) {
        const auto& f =
            bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
                tfel::glossary::Glossary::Porosity);
        bd.reserveName(uh, "dseqf" + id + "_d" + f.name);
      }
    }
    if ((r == FLOWCRITERION) || (r == STRESSANDFLOWCRITERION)) {
      bd.reserveName(uh, "n" + id);
      bd.reserveName(uh, "dn" + id + "_ds" + id);
      if (this->isCoupledWithPorosityEvolution()) {
        const auto& f =
            bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
                tfel::glossary::Glossary::Porosity);
        bd.reserveName(uh, "dn" + id + "_d" + f.name);
      }
    }
  }  // end of StressCriterionBase::initialize

  void StressCriterionBase::endTreatment(BehaviourDescription&,
                                         const AbstractBehaviourDSL&,
                                         const std::string&,
                                         const Role) {
  }  // end of StressCriterionBase::endTreatment

  std::string StressCriterionBase::updatePorosityUpperBound(
      const BehaviourDescription&, const std::string&, const Role r) const {
    if (!((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION))) {
      tfel::raise(
          "StressCriterionBase::updatePorosityUpperBound: invalid call for "
          "this stress criterion. This method is not valid for flow "
          "criteria (i.e. when the stress criterion is used to dertermine "
          "the flow direction in non associated plasticity).");
    }
    return std::string{};
  }  // end of StressCriterionBase::updatePorosityUpperBound

  StressCriterionBase::~StressCriterionBase() = default;

}  // end of namespace mfront::bbrick
