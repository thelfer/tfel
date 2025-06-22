/*!
 * \file   StructureCurrentState.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28 oct. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/StructureCurrentState.hxx"

namespace mtest {

  StructureCurrentState::StructureCurrentState() = default;

  StructureCurrentState::StructureCurrentState(StructureCurrentState&&) =
      default;

  StructureCurrentState::StructureCurrentState(const StructureCurrentState&) =
      default;

  StructureCurrentState& StructureCurrentState::operator=(
      const StructureCurrentState&) = default;

  StructureCurrentState& StructureCurrentState::operator=(
      StructureCurrentState&&) = default;

  StructureCurrentState::~StructureCurrentState() = default;

  void StructureCurrentState::setBehaviour(
      const std::shared_ptr<Behaviour>& p) {
    tfel::raise_if(this->b != nullptr,
                   "StructureCurrentState::setBehaviour: "
                   "behaviour already set");
    this->b = p;
  }

  void StructureCurrentState::setModellingHypothesis(const Hypothesis mh) {
    using tfel::material::ModellingHypothesis;
    tfel::raise_if(this->h != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "StructureCurrentState::setModellingHypothesis: "
                   "modelling hypothesis already set");
    this->h = mh;
  }

  BehaviourWorkSpace& StructureCurrentState::getBehaviourWorkSpace() const {
    using tfel::material::ModellingHypothesis;
    if (this->bwks.empty()) {
      tfel::raise_if(this->b == nullptr,
                     "StructureCurrentState::getBehaviourWorkSpace: "
                     "behaviour not set");
      tfel::raise_if(this->h == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                     "StructureCurrentState::getBehaviourWorkSpace: "
                     "modelling hypothesis not set");
      this->bwks.push_back(std::make_shared<BehaviourWorkSpace>());
      this->b->allocate(*(this->bwks[0]));
    }
    return *(this->bwks[0]);
  }  // end of StructureCurrentState::getBehaviourWorkSpace

  const Behaviour& StructureCurrentState::getBehaviour() const {
    tfel::raise_if(this->b == nullptr,
                   "StructureCurrentState::getBehaviour: "
                   "behaviour not set");
    return *(this->b);
  }  // end of StructureCurrentState::getBehaviour

  void update(StructureCurrentState& s) {
    for (auto& ls : s.istates) {
      mtest::update(ls);
    }
  }

  void revert(StructureCurrentState& s) {
    for (auto& ls : s.istates) {
      mtest::revert(ls);
    }
  }

}  // end of namespace mtest
