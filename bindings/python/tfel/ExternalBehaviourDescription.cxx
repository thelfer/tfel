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

#include <boost/python.hpp>
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

void declareExternalBehaviourDescription();

void declareExternalBehaviourDescription() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;

  class_<ExternalBehaviourDescription,
         bases<ExternalMaterialKnowledgeDescription>>(
      "ExternalBehaviourDescription", init<>())
      .def(init<std::string, std::string, std::string>())
      .add_property("elastic_material_properties_epts", &get_emps_epts)
      .add_property("linear_thermal_expansion_coefficients_epts",
                    &get_themps_epts)
      .add_property("mpnames", &get_mps)
      .add_property("ivnames", &get_ivs)
      .add_property("ivtypes", &get_ivs_types)
      .add_property("evnames", &get_evs)
      .add_property("evtypes", &get_evs_types)
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
