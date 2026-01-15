/*!
 * \file   Constraint.cxx
 * \brief
 * \author th202608
 * \date   13/02/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Raise.hxx"
#include "MTest/Constraint.hxx"

static mtest::ConstraintOptions makeConstraintOptions(
    const pybind11::kwargs d) {
  mtest::ConstraintOptions opts;
  for (const auto& [key, value] : d) {
    const auto k = pybind11::cast<std::string>(key);
    if (k == "active") {
      opts.active = pybind11::cast<bool>(value);
    } else if (k == "activating_events") {
      opts.activating_events = pybind11::cast<std::vector<std::string>>(value);
    } else if (k == "desactivating_events") {
      opts.desactivating_events =
          pybind11::cast<std::vector<std::string>>(value);
    } else {
      tfel::raise("unexpected key '" + k + "'");
    }
  }
  return opts;
}  // end of makeConstraintOptions

void declareConstraint(pybind11::module_&);
void declareConstraint(pybind11::module_& m) {
  pybind11::class_<mtest::ConstraintOptions>(m, "ConstraintOptions")
      .def_readonly("active", &mtest::ConstraintOptions::active)
      .def_readonly("activating_events",
                    &mtest::ConstraintOptions::activating_events)
      .def_readonly("desactivating_events",
                    &mtest::ConstraintOptions::desactivating_events);
  m.def("makeConstraintOptions", makeConstraintOptions);
}  // end of declareConstraints
