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

#include <boost/python.hpp>
#include "TFEL/System/ExternalMaterialPropertyDescription.hxx"

static std::vector<std::string> get_args(
    const tfel::system::ExternalMaterialPropertyDescription& d) {
  return d.arguments;
}

static std::vector<std::string> get_parameters(
    const tfel::system::ExternalMaterialPropertyDescription& d) {
  return d.parameters;
}

void declareExternalMaterialPropertyDescription();

void declareExternalMaterialPropertyDescription() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;

  class_<ExternalMaterialPropertyDescription,
         bases<ExternalMaterialKnowledgeDescription>>(
      "ExternalMaterialPropertyDescription", init<>())
      .def(init<std::string, std::string>())
      .def_readonly("law", &ExternalMaterialPropertyDescription::law)
      .def_readonly("material_property",
                    &ExternalMaterialPropertyDescription::material_property)
      .def_readonly("output", &ExternalMaterialPropertyDescription::output)
      .add_property("arguments", &get_args)
      .add_property("parameters", &get_parameters)
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
