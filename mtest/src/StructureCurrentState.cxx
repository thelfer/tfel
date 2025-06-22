/*!
 * \file   StructureCurrentState.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   28 oct. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

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
    if (this->b.get() != nullptr) {
      throw(
          std::runtime_error("StructureCurrentState::setBehaviour: "
                             "behaviour already set"));
    }
    this->b = p;
  }

  void StructureCurrentState::setModellingHypothesis(const Hypothesis mh) {
    using tfel::material::ModellingHypothesis;
    if (this->h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      throw(
          std::runtime_error("StructureCurrentState::setModellingHypothesis: "
                             "modelling hypothesis already set"));
    }
    this->h = mh;
  }

  BehaviourWorkSpace& StructureCurrentState::getBehaviourWorkSpace() const {
    using tfel::material::ModellingHypothesis;
    if (this->bwks.empty()) {
      if (this->b.get() == nullptr) {
        throw(
            std::runtime_error("StructureCurrentState::getBehaviourWorkSpace: "
                               "behaviour not set"));
      }
      if (this->h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        throw(
            std::runtime_error("StructureCurrentState::getBehaviourWorkSpace: "
                               "modelling hypothesis not set"));
      }
      this->bwks.push_back(std::make_shared<BehaviourWorkSpace>());
      this->b->allocate(*(this->bwks[0]));
    }
    return *(this->bwks[0]);
  }  // end of StructureCurrentState::getBehaviourWorkSpace

  const Behaviour& StructureCurrentState::getBehaviour() const {
    if (this->b.get() == nullptr) {
      throw(
          std::runtime_error("StructureCurrentState::getBehaviour: "
                             "behaviour not set"));
    }
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
