/*!
 * \file  mtest/src/Constraint.cxx
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

#include "MTest/Constraint.hxx"

namespace mtest {

  ConstraintOptions::ConstraintOptions() = default;
  ConstraintOptions::ConstraintOptions(ConstraintOptions&&) = default;
  ConstraintOptions::ConstraintOptions(const ConstraintOptions&) = default;
  ConstraintOptions& ConstraintOptions::operator=(ConstraintOptions&&) =
      default;
  ConstraintOptions& ConstraintOptions::operator=(const ConstraintOptions&) =
      default;

  Constraint::~Constraint() = default;

  void applyConstraintOptions(Constraint& c, const ConstraintOptions& o) {
    c.setActive(o.active);
    c.setActivatingEvents(o.activating_events);
    c.setDesactivatingEvents(o.desactivating_events);
  }  // end of applyConstraintOptions

}  // end of namespace mtest
