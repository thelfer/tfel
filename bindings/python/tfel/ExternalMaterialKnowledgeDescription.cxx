
/*!
 * \file  bindings/python/tfel/ExternalMaterialKnowledgeDescription.cxx
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
#include "TFEL/System/ExternalMaterialKnowledgeDescription.hxx"

void declareExternalMaterialKnowledgeDescription(pybind11::module_&);

void declareExternalMaterialKnowledgeDescription(pybind11::module_& m) {
  using namespace tfel::system;
  pybind11::class_<ExternalMaterialKnowledgeDescription>(
      m, "ExternalMaterialKnowledgeDescription",
      "A structure containing the information about a given material knowledge")
      .def(pybind11::init<>(), "Default constructor")
      .def(pybind11::init<std::string, std::string>(),
           pybind11::arg("l"), pybind11::arg("f"),
           "l: library\n"
           "f: entry point corresponding to the material knowledge")
      .def_readonly("library", &ExternalMaterialKnowledgeDescription::library,
                    "Name of the library")
      .def_readonly("entry_point",
                    &ExternalMaterialKnowledgeDescription::entry_point,
                    "Name of the entry point")
      .def_readonly("tfel_version",
                    &ExternalMaterialKnowledgeDescription::tfel_version,
                    "Version of TFEL used to generate the entry point")
      .def_readonly("unit_system",
                    &ExternalMaterialKnowledgeDescription::unit_system,
                    "Unit system used by the entry point (can be empty if no "
                    "unit\nsystem has been specified)")
      .def_readonly("build_id", &ExternalMaterialKnowledgeDescription::build_id,
                    "Build identifier")
      .def_readonly("source", &ExternalMaterialKnowledgeDescription::source,
                    "MFront source file")
      .def_readonly("mfront_interface",
                    &ExternalMaterialKnowledgeDescription::mfront_interface,
                    "Name of the interface used to generate the entry point")
      .def_readonly("material", &ExternalMaterialKnowledgeDescription::material,
                    "Name of the material")
      .def_readonly("author", &ExternalMaterialKnowledgeDescription::author,
                    "Author of the implementation")
      .def_readonly("date", &ExternalMaterialKnowledgeDescription::date,
                    "Date of the implementation")
      .def_readonly("description",
                    &ExternalMaterialKnowledgeDescription::description,
                    "Description of the material knowledge");

}  // end of declareExternalMaterialKnowledgeDescription
