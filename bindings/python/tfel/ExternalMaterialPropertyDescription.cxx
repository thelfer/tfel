

/*!
 * \file  bindings/python/tfel/ExternalMaterialPropertyDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 15 june 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/System/ExternalMaterialPropertyDescription.hxx"

static std::vector<std::string> get_args(
    const tfel::system::ExternalMaterialPropertyDescription& d) {
  return d.arguments;
}

static std::vector<std::string> get_parameters(
    const tfel::system::ExternalMaterialPropertyDescription& d) {
  return d.parameters;
}

void declareExternalMaterialPropertyDescription(pybind11::module_&);

void declareExternalMaterialPropertyDescription(pybind11::module_& m) {
  using namespace tfel::system;
  pybind11::class_<ExternalMaterialPropertyDescription,
                   ExternalMaterialKnowledgeDescription>(
      m, "ExternalMaterialPropertyDescription")
      .def(pybind11::init<>())
      .def(pybind11::init<std::string, std::string>())
      .def_readonly("law", &ExternalMaterialPropertyDescription::law)
      .def_readonly("material_property",
                    &ExternalMaterialPropertyDescription::material_property)
      .def_readonly("output", &ExternalMaterialPropertyDescription::output)
      .def_property_readonly("arguments", &get_args)
      .def_property_readonly("parameters", &get_parameters)
      .def("hasBounds", &ExternalMaterialPropertyDescription::hasBounds)
      .def("hasLowerBound", &ExternalMaterialPropertyDescription::hasLowerBound)
      .def("hasUpperBound", &ExternalMaterialPropertyDescription::hasUpperBound)
      .def("getLowerBound", &ExternalMaterialPropertyDescription::getLowerBound)
      .def("getUpperBound", &ExternalMaterialPropertyDescription::getUpperBound)
      .def("hasPhysicalBounds",
           &ExternalMaterialPropertyDescription::hasPhysicalBounds)
      .def("hasLowerPhysicalBound",
           &ExternalMaterialPropertyDescription::hasLowerPhysicalBound)
      .def("hasUpperPhysicalBound",
           &ExternalMaterialPropertyDescription::hasUpperPhysicalBound)
      .def("getLowerPhysicalBound",
           &ExternalMaterialPropertyDescription::getLowerPhysicalBound)
      .def("getUpperPhysicalBound",
           &ExternalMaterialPropertyDescription::getUpperPhysicalBound);

}  // end of declareExternalMaterialPropertyDescription
