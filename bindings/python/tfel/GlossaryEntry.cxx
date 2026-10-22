
/*!
 * \file  bindings/python/tfel/GlossaryEntry.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 31 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Glossary/GlossaryEntry.hxx"

void declareGlossaryEntry(pybind11::module_&);

void declareGlossaryEntry(pybind11::module_& m) {
  using namespace tfel::glossary;
  pybind11::class_<GlossaryEntry>(
      m, "GlossaryEntry",
      "Structure describing an entry of the glossary")
      .def("__repr__",
           [](const GlossaryEntry& e) { return e.getShortDescription(); },
           "Return the short description")
      .def_property_readonly("key", &GlossaryEntry::getKey,
                             "Return the key")
      .def_property_readonly("names", &GlossaryEntry::getNames,
                             "Return names associated with this key")
      .def_property_readonly("units", &GlossaryEntry::getUnits,
                             "Return the units")
      .def_property_readonly("type", &GlossaryEntry::getType,
                             "Return the type")
      .def_property_readonly("short_description",
                             &GlossaryEntry::getShortDescription,
                             "Return the short description")
      .def_property_readonly("description", &GlossaryEntry::getDescription,
                             "Return the description")
      .def_property_readonly("notes", &GlossaryEntry::getNotes,
                             "Return the notes");

}  // end of declareGlossaryEntry
