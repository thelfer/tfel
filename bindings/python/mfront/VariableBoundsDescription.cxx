/*!
 * \file   VariableBoundsDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12 avril 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/VariableDescription.hxx"

void declareVariableBoundsDescription(pybind11::module_&);

void declareVariableBoundsDescription(pybind11::module_& m) {
  using namespace mfront;
  pybind11::enum_<VariableBoundsDescription::BoundsType>(m,
                                                         "VariableBoundsTypes")
      .value("LOWER", VariableBoundsDescription::LOWER)
      .value("UPPER", VariableBoundsDescription::UPPER)
      .value("LOWERANDUPPER", VariableBoundsDescription::LOWERANDUPPER);

  pybind11::class_<VariableBoundsDescription>(m, "VariableBoundsDescription")
      .def_readwrite("boundsType", &VariableBoundsDescription::boundsType)
      .def_readwrite("lineNumber", &VariableBoundsDescription::lineNumber)
      .def_readwrite("component", &VariableBoundsDescription::component)
      .def_readwrite("lowerBound", &VariableBoundsDescription::lowerBound)
      .def_readwrite("upperBound", &VariableBoundsDescription::upperBound);
}
