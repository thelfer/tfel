/*!
 * \file   bindings/python/mfront/MaterialKnowledgeDescription.cxx
 * \brief
 * \author Tristan Chenaille
 * \date   03/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/MaterialKnowledgeDescription.hxx"

static bool MaterialKnowledgeDescription_hasAttribute(
    const mfront::MaterialKnowledgeDescription& d, const std::string& n) {
  return d.hasAttribute(n);
}

template <typename T>
static T MaterialKnowledgeDescription_getAttribute(
    const mfront::MaterialKnowledgeDescription& d, const std::string& n) {
  return d.getAttribute<T>(n);
}

template <typename T>
static T MaterialKnowledgeDescription_getAttribute2(
    const mfront::MaterialKnowledgeDescription& d,
    const std::string& n,
    const T& v) {
  return d.getAttribute<T>(n, v);
}

void declareMaterialKnowledgeDescription(pybind11::module_&);

void declareMaterialKnowledgeDescription(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<MaterialKnowledgeDescription>(
      m, "MaterialKnowledgeDescription",
      "Base class for the description of material knowledge")
      .def("hasAttribute", MaterialKnowledgeDescription_hasAttribute,
           pybind11::arg("n"))
      .def("getAttributes", &MaterialKnowledgeDescription::getAttributes)
      .def("getUnsignedShortAttribute",
           MaterialKnowledgeDescription_getAttribute<unsigned short>,
           pybind11::arg("n"))
      .def("getStringAttribute",
           MaterialKnowledgeDescription_getAttribute<std::string>,
           pybind11::arg("n"))
      .def("getBooleanAttribute",
           MaterialKnowledgeDescription_getAttribute<bool>, pybind11::arg("n"))
      .def("getUnsignedShortAttribute",
           MaterialKnowledgeDescription_getAttribute2<unsigned short>,
           pybind11::arg("n"), pybind11::arg("v"))
      .def("getStringAttribute",
           MaterialKnowledgeDescription_getAttribute2<std::string>,
           pybind11::arg("n"), pybind11::arg("v"))
      .def("getBooleanAttribute",
           MaterialKnowledgeDescription_getAttribute2<bool>, pybind11::arg("n"),
           pybind11::arg("v"));
}
