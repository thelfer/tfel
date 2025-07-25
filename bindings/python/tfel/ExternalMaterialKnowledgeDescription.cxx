
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
      m, "ExternalMaterialKnowledgeDescription")
      .def(pybind11::init<>())
      .def(pybind11::init<std::string, std::string>())
      .def_readonly("library", &ExternalMaterialKnowledgeDescription::library)
      .def_readonly("entry_point",
                    &ExternalMaterialKnowledgeDescription::entry_point)
      .def_readonly("tfel_version",
                    &ExternalMaterialKnowledgeDescription::tfel_version)
      .def_readonly("unit_system",
                    &ExternalMaterialKnowledgeDescription::unit_system)
      .def_readonly("build_id", &ExternalMaterialKnowledgeDescription::build_id)
      .def_readonly("source", &ExternalMaterialKnowledgeDescription::source)
      .def_readonly("mfront_interface",
                    &ExternalMaterialKnowledgeDescription::mfront_interface)
      .def_readonly("material", &ExternalMaterialKnowledgeDescription::material)
      .def_readonly("author", &ExternalMaterialKnowledgeDescription::author)
      .def_readonly("date", &ExternalMaterialKnowledgeDescription::date)
      .def_readonly("description",
                    &ExternalMaterialKnowledgeDescription::description);

}  // end of declareExternalMaterialKnowledgeDescription
