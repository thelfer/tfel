/*!
 * \file   StructureCurrentState.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/12/2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MTest/CurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"

static tfel::math::vector<mtest::CurrentState>& get_istates(
    mtest::StructureCurrentState& scs) {
  return scs.istates;
}

void declareStructureCurrentState(pybind11::module_&);

void declareStructureCurrentState(pybind11::module_& m) {
  pybind11::class_<mtest::StructureCurrentState>(m, "StructureCurrentState")
      .def(pybind11::init<>())
      .def_property_readonly("istates", get_istates,
                             pybind11::return_value_policy::reference);
}  // end of declareStructureCurrentState
