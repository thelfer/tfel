/*!
 * \file   StressCriterionBase.cxx
 * \brief
 * \author th202608
 * \date   10/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription> StressCriterionBase::getOptions() const {
      return {};
    }  // end of StressCriterionBase::getOptions

    void StressCriterionBase::initialize(BehaviourDescription& bd,
                                         AbstractBehaviourDSL&,
                                         const std::string& id,
                                         const DataMap& d,
                                         const Role r) {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      // checking options
      mfront::bbrick::check(d, this->getOptions());
      // reserve names
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
        bd.reserveName(uh, "seqel" + id);
        bd.reserveName(uh, "seq" + id);
        bd.reserveName(uh, "iseq" + id);
        bd.reserveName(uh, "dseq" + id + "_ds" + id);
        bd.reserveName(uh, "d2seq" + id + "_ds" + id + "_ds" + id);
      }
      if (r == FLOWCRITERION) {
        bd.reserveName(uh, "seqf" + id);
        bd.reserveName(uh, "iseqf" + id);
      }
      if ((r == FLOWCRITERION) || (r == STRESSANDFLOWCRITERION)) {
        bd.reserveName(uh, "n" + id);
        bd.reserveName(uh, "dn" + id + "_ds" + id);
      }
    }  // end of StressCriterionBase::initialize

    void StressCriterionBase::endTreatment(BehaviourDescription&,
                                           const AbstractBehaviourDSL&,
                                           const std::string&,
                                           const Role) {
    }  // end of StressCriterionBase::endTreatment

    StressCriterionBase::~StressCriterionBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
