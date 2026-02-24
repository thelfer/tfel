/*!
 * \file   SolverWorkSpace.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MTest/SolverWorkSpace.hxx"

void declareSolverWorkSpace(pybind11::module_&);

void declareSolverWorkSpace(pybind11::module_& m) {
  pybind11::class_<mtest::SolverWorkSpace>(m, "SolverWorkSpace")
      .def(pybind11::init<>());
}
