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

void declareExternalMaterialPropertyDescription();

void declareExternalMaterialPropertyDescription() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;

  class_<ExternalMaterialPropertyDescription>(
      "ExternalMaterialPropertyDescription", init<>())
      .def(init<std::string, std::string>())
      .def_readonly("tfel_version",
                    &ExternalMaterialPropertyDescription::tfel_version)
      .def_readonly("build_id", &ExternalMaterialPropertyDescription::build_id)
      .def_readonly("source", &ExternalMaterialPropertyDescription::source)
      .def_readonly("mfront_interface",
                    &ExternalMaterialPropertyDescription::mfront_interface)
      .add_property("arguments", &get_args);

}  // end of declareExternalMaterialPropertyDescription
