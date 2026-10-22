

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
      m, "ExternalMaterialPropertyDescription",
      "A structure containing the information about a given material property")
      .def(pybind11::init<>(), "Default constructor")
      .def(pybind11::init<std::string, std::string>(),
           pybind11::arg("l"), pybind11::arg("m"),
           "l: library\n"
           "m: material property")
      .def_readonly("law", &ExternalMaterialPropertyDescription::law,
                    "Name of the law defining the material property")
      .def_readonly("material_property",
                    &ExternalMaterialPropertyDescription::material_property,
                    "Name of the enty point associated with the material "
                    "property")
      .def_readonly("output", &ExternalMaterialPropertyDescription::output,
                    "External name of the output")
      .def_property_readonly("arguments", &get_args,
                             "External names of the arguments/inputs")
      .def_property_readonly("parameters", &get_parameters,
                             "List of parameters")
      .def("hasBounds", &ExternalMaterialPropertyDescription::hasBounds,
           pybind11::arg("v"),
           "Return true if the given variable has bounds\n"
           "v: variable name")
      .def("hasLowerBound", &ExternalMaterialPropertyDescription::hasLowerBound,
           pybind11::arg("v"),
           "Return true if the given variable has a lower bound\n"
           "v: variable name")
      .def("hasUpperBound", &ExternalMaterialPropertyDescription::hasUpperBound,
           pybind11::arg("v"),
           "Return true if the given variable has a upper bound\n"
           "v: variable name")
      .def("getLowerBound", &ExternalMaterialPropertyDescription::getLowerBound,
           pybind11::arg("v"),
           "Return the lower bound of the given variable\n"
           "v: variable name")
      .def("getUpperBound", &ExternalMaterialPropertyDescription::getUpperBound,
           pybind11::arg("v"),
           "Return the upper bound of the given variable\n"
           "v: variable name")
      .def("hasPhysicalBounds",
           &ExternalMaterialPropertyDescription::hasPhysicalBounds,
           pybind11::arg("v"),
           "Return true if the given variable has bounds\n"
           "v: variable name")
      .def("hasLowerPhysicalBound",
           &ExternalMaterialPropertyDescription::hasLowerPhysicalBound,
           pybind11::arg("v"),
           "Return true if the given variable has a lower physical bound\n"
           "v: variable name")
      .def("hasUpperPhysicalBound",
           &ExternalMaterialPropertyDescription::hasUpperPhysicalBound,
           pybind11::arg("v"),
           "Return true if the given variable has a upper physical bound\n"
           "v: variable name")
      .def("getLowerPhysicalBound",
           &ExternalMaterialPropertyDescription::getLowerPhysicalBound,
           pybind11::arg("v"),
           "Return the lower bound of the given variable\n"
           "v: variable name")
      .def("getUpperPhysicalBound",
           &ExternalMaterialPropertyDescription::getUpperPhysicalBound,
           pybind11::arg("v"),
           "Return the upper bound of the given variable\n"
           "v: variable name");

}  // end of declareExternalMaterialPropertyDescription
