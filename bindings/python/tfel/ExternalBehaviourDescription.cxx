
/*!
 * \file  bindings/python/tfel/ExternalBehaviourDescription.cxx
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
#include "TFEL/System/ExternalBehaviourDescription.hxx"

static std::vector<std::string> get_emps_epts(
    const tfel::system::ExternalBehaviourDescription& d) {
  return d.elastic_material_properties_epts;
}

static std::vector<std::string> get_themps_epts(
    const tfel::system::ExternalBehaviourDescription& d) {
  return d.linear_thermal_expansion_coefficients_epts;
}

static std::vector<std::string> get_mps(
    const tfel::system::ExternalBehaviourDescription& d) {
  return d.mpnames;
}

static std::vector<std::string> get_ivs(
    const tfel::system::ExternalBehaviourDescription& d) {
  return d.ivnames;
}

static std::vector<int> get_ivs_types(
    const tfel::system::ExternalBehaviourDescription& d) {
  return d.ivtypes;
}

static std::vector<int> get_evs_types(
    const tfel::system::ExternalBehaviourDescription& d) {
  return d.ivtypes;
}

static std::vector<std::string> get_evs(
    const tfel::system::ExternalBehaviourDescription& d) {
  return d.evnames;
}

void declareExternalBehaviourDescription(pybind11::module_&);

void declareExternalBehaviourDescription(pybind11::module_& m) {
  using namespace tfel::system;

  pybind11::class_<ExternalBehaviourDescription,
                   ExternalMaterialKnowledgeDescription>(
      m, "ExternalBehaviourDescription")
      .def(pybind11::init<>())
      .def(pybind11::init<std::string, std::string, std::string>())
      .def_property_readonly("elastic_material_properties_epts", &get_emps_epts)
      .def_property_readonly("linear_thermal_expansion_coefficients_epts",
                             &get_themps_epts)
      .def_property_readonly("mpnames", &get_mps)
      .def_property_readonly("ivnames", &get_ivs)
      .def_property_readonly("ivtypes", &get_ivs_types)
      .def_property_readonly("evnames", &get_evs)
      .def_property_readonly("evtypes", &get_evs_types)
      .def_readonly("btype", &ExternalBehaviourDescription::btype)
      .def_readonly("stype", &ExternalBehaviourDescription::stype)
      .def_readonly("etype", &ExternalBehaviourDescription::etype)
      .def_readonly("isUPUIR", &ExternalBehaviourDescription::isUPUIR)
      .def_readonly("computesInternalEnergy",
                    &ExternalBehaviourDescription::computesInternalEnergy)
      .def_readonly("computesDissipatedEnergy",
                    &ExternalBehaviourDescription::computesDissipatedEnergy)
      .def_readonly("requiresStiffnessTensor",
                    &ExternalBehaviourDescription::requiresStiffnessTensor)
      .def_readonly("requiresThermalExpansionCoefficientTensor",
                    &ExternalBehaviourDescription::
                        requiresThermalExpansionCoefficientTensor)
      .def("hasBounds", &ExternalBehaviourDescription::hasBounds)
      .def("hasLowerBound", &ExternalBehaviourDescription::hasLowerBound)
      .def("hasUpperBound", &ExternalBehaviourDescription::hasUpperBound)
      .def("getLowerBound", &ExternalBehaviourDescription::getLowerBound)
      .def("getUpperBound", &ExternalBehaviourDescription::getUpperBound)
      .def("hasPhysicalBounds",
           &ExternalBehaviourDescription::hasPhysicalBounds)
      .def("hasLowerPhysicalBound",
           &ExternalBehaviourDescription::hasLowerPhysicalBound)
      .def("hasUpperPhysicalBound",
           &ExternalBehaviourDescription::hasUpperPhysicalBound)
      .def("getLowerPhysicalBound",
           &ExternalBehaviourDescription::getLowerPhysicalBound)
      .def("getUpperPhysicalBound",
           &ExternalBehaviourDescription::getUpperPhysicalBound);
}
