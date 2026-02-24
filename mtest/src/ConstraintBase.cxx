/*!
 * \file  mtest/src/ConstraintBase.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MTest/ConstraintBase.hxx"

namespace mtest {

  bool ConstraintBase::treatEvent(const std::string& e) {
    auto contains = [&e](const std::vector<std::string>& v) {
      return std::find(v.begin(), v.end(), e) != v.end();
    };
    const auto cs = this->active;
    this->active = contains(this->activating_events);
    this->active = !contains(this->desactivating_events);
    return cs != this->active;
  }  // end of ConstraintBase::treatEvent

  void ConstraintBase::setActivatingEvents(
      const std::vector<std::string>& evs) {
    tfel::raise_if(!this->activating_events.empty(),
                   "activating events already declared");
    this->activating_events = evs;
  }  // end of ConstraintBase::ConstraintBase::setActivatingEvents

  void ConstraintBase::setDesactivatingEvents(
      const std::vector<std::string>& evs) {
    tfel::raise_if(!this->desactivating_events.empty(),
                   "activating events already declared");
    this->desactivating_events = evs;
  }  // end of ConstraintBase::setDesactivatingEvents

  void ConstraintBase::setActive(const bool b) {
    this->active = b;
  }  // end of ConstraintBase::setActive

  bool ConstraintBase::isActive() const {
    return this->active;
  }  // end of ConstraintBase::isActive

  ConstraintBase::~ConstraintBase() = default;

}  // end of namespace mtest
