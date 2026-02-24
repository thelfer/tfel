/*!
 * \file   bindings/python/mfront/TargetsDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/LibraryDescription.hxx"
#include "MFront/TargetsDescription.hxx"

static std::vector<std::string> TargetsDescription_getHeaders(
    const mfront::TargetsDescription& t) {
  return t.headers;
}

static void TargetsDescription_setHeaders(mfront::TargetsDescription& t,
                                          const std::vector<std::string>& h) {
  t.headers = h;
}

static const std::vector<mfront::LibraryDescription>&
TargetsDescription_getLibraries(mfront::TargetsDescription& t) {
  return t.libraries;
}  // end of TargetsDescription_getLibraries

void declareTargetsDescription(pybind11::module_&);

void declareTargetsDescription(pybind11::module_& m) {
  using namespace mfront;
  using LibraryAccess =
      LibraryDescription& (TargetsDescription::*)(const std::string&);
  pybind11::class_<mfront::TargetsDescription>(m, "TargetsDescription")
      .def("getLibrary",
           static_cast<LibraryAccess>(&TargetsDescription::getLibrary),
           pybind11::arg("library"))
      .def_property("headers", TargetsDescription_getHeaders,
                    TargetsDescription_setHeaders)
      .def("getLibraries", TargetsDescription_getLibraries)
      .def_property_readonly("libraries", &TargetsDescription_getLibraries)
      .def_readwrite("specific_targets", &TargetsDescription::specific_targets)
      .def("__repr__", [](const TargetsDescription& l) {
        auto os = std::ostringstream{};
        os << l;
        return os.str();
      });
  m.def("describes", mfront::describes);
  m.def("mergeTargetsDescription", mfront::describes);
}
