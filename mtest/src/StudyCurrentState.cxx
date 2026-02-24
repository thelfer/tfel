/*!
 * \file   StudyCurrentState.cxx
 * \brief
 * \author Thomas Helfer
 * \date   11 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MTest/StudyCurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"

namespace mtest {

  StudyCurrentState::StudyCurrentState() = default;

  StudyCurrentState::StudyCurrentState(StudyCurrentState&&) = default;

  StudyCurrentState::StudyCurrentState(const StudyCurrentState&) = default;

  StudyCurrentState& StudyCurrentState::operator=(const StudyCurrentState&) =
      default;

  StudyCurrentState& StudyCurrentState::operator=(StudyCurrentState&&) =
      default;

  StudyCurrentState StudyCurrentState::makeDeepCopy() const {
    StudyCurrentState copy(*this);
    for (auto& [n, state] : copy.s) {
      state = std::make_shared<StructureCurrentState>(state->makeDeepCopy());
    }
    return copy;
  }  // end of makeDeepCopy

  bool StudyCurrentState::getFailureStatus() const {
    for (const auto failure_status : this->failure_criterion_status) {
      if (failure_status) {
        return true;
      }
    }
    return false;
  }  // end of getFailureStatus

  bool StudyCurrentState::getFailureCriterionStatus(const std::size_t i) const {
    if (i >= this->failure_criterion_status.size()) {
      tfel::raise(
          "StudyCurrentState::getFailureCriterionStatus: "
          "invalid index");
    }
    return this->failure_criterion_status[i];
  }  // end of getFailureCriterionStatus

  void StudyCurrentState::setFailureCriterionStatus(const std::size_t i,
                                                    const bool status) {
    if (i >= this->failure_criterion_status.size()) {
      tfel::raise(
          "StudyCurrentState::getFailureCriterionStatus: "
          "invalid index");
    }
    this->failure_criterion_status[i] = status;
  }  // end of setFailureCriterionStatus

  void StudyCurrentState::setNumberOfFailureCriterionStatus(
      const std::size_t n) {
    this->failure_criterion_status.resize(n, false);
  }  // end of setNumberOfFailureCriterionStatus

  std::size_t StudyCurrentState::getNumberOfFailureCriterionStatus()
      const noexcept {
    return this->failure_criterion_status.size();
  }  // end of getNumberOfFailureCriterionStatus

  void StudyCurrentState::initialize(const StudyCurrentState::size_type psz) {
    this->u_1.clear();
    this->u0.clear();
    this->u1.clear();
    this->u10.clear();
    this->u_1.resize(psz, real(0));
    this->u0.resize(psz, real(0));
    this->u1.resize(psz, real(0));
    this->u10.resize(psz, real(0));
    this->s.clear();
  }  // end of StudyCurrentState::initialize

  void StudyCurrentState::update(const real dt) {
    this->dt_1 = dt;
    this->u_1 = this->u0;
    this->u0 = this->u1;
    this->u10 = this->u1;
    for (auto& p : this->s) {
      auto& ss = *(p.second);
      mtest::update(ss);
    }
  }  // end of StudyCurrentState::update

  void StudyCurrentState::revert() {
    this->u1 = this->u0;
    this->u10 = this->u0;
    for (auto& p : this->s) {
      auto& ss = *(p.second);
      mtest::revert(ss);
    }
  }  // end of StudyCurrentState::revert

  StructureCurrentState& StudyCurrentState::getStructureCurrentState(
      const std::string& n) {
    auto p = this->s.find(n);
    if (p == this->s.end()) {
      const auto ps = std::make_shared<StructureCurrentState>();
      p = this->s.insert({n, ps}).first;
    }
    return *(p->second);
  }  // end of StudyCurrentState::getStructureCurrentState

  const StructureCurrentState& StudyCurrentState::getStructureCurrentState(
      const std::string& n) const {
    const auto p = this->s.find(n);
    tfel::raise_if(p == this->s.end(),
                   "StudyCurrentState::getStructureCurrentState: "
                   "no state associated to structure '" +
                       n + "'");
    return *(p->second);
  }

  bool StudyCurrentState::containsParameter(const std::string& n) const {
    return this->parameters.count(n) != 0;
  }  // end of StudyCurrentState::containsParameter

  void StudyCurrentState::throwUnknownParameterException(const std::string& n) {
    tfel::raise(
        "StudyCurrentState::throwUnknownParameterException:"
        "no parameter '" +
        n + "' declared");
  }

  void StudyCurrentState::throwAlreadyDeclaredParameterException(
      const std::string& n) {
    tfel::raise(
        "StudyCurrentState::throwAlreadyDeclaredParameterException:"
        "parameter '" +
        n + "' already declared");
  }

  bool StudyCurrentState::containsEvolution(const std::string& n) const {
    return this->evs.find(n) != this->evs.end();
  }

  void StudyCurrentState::addEvolution(const std::string& n,
                                       std::shared_ptr<Evolution> e) {
    auto p = this->evs.find(n);
    if (p != this->evs.end()) {
      tfel::raise(
          "StudyCurrentState::getEvolution: "
          "evolution '" +
          n + "' already defined");
    }
    this->evs[n] = e;
  }

  Evolution& StudyCurrentState::getEvolution(const std::string& n) {
    auto p = this->evs.find(n);
    if (p == this->evs.end()) {
      tfel::raise(
          "StudyCurrentState::getEvolution: "
          "no evolution named '" +
          n + "'");
    }
    return *(p->second);
  }

  const Evolution& StudyCurrentState::getEvolution(const std::string& n) const {
    auto p = this->evs.find(n);
    if (p == this->evs.end()) {
      tfel::raise(
          "StudyCurrentState::getEvolution: "
          "no evolution named '" +
          n + "'");
    }
    return *(p->second);
  }

  StudyCurrentState::~StudyCurrentState() = default;

}  // end of namespace mtest
