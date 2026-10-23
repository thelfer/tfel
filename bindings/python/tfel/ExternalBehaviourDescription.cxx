
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
      m, "ExternalBehaviourDescription",
      "A structure containing the information that could be retrieved\n"
      "by the ExternalLibraryManager class about a given behaviour for\n"
      "a given hypothesis")
      .def(pybind11::init<>(), "Default constructor")
      .def(pybind11::init<std::string, std::string, std::string>(),
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           "l: library\n"
           "f: function\n"
           "h: hypothesis")
      .def_property_readonly("elastic_material_properties_epts", &get_emps_epts,
                             "Names of entry points associated with elastic "
                             "material\n"
                             "properties exported by the behaviour, if any. "
                             "This is currently only\n"
                             "supported by a few interfaces (`Cyrano`, "
                             "`Cast3M`).\n"
                             "The size of this member is either:\n"
                             "- 0: which means that the behaviour does not "
                             "export its elastic\n"
                             "properties.\n"
                             "- 2, which means that the elastic behaviour is "
                             "isotropic.\n"
                             "- 9, which means that the elastic behaviour is "
                             "orthotropic.")
      .def_property_readonly("linear_thermal_expansion_coefficients_epts",
                             &get_themps_epts,
                             "Names of entry points associated with linear "
                             "thermal expansion\n"
                             "coefficients properties exported by the "
                             "behaviour, if any. This is\ncurrently only "
                             "supported by a few interfaces (`Cyrano`, "
                             "`Cast3M`).\n"
                             "The size of this member is either:\n"
                             "- 0: which means that the behaviour does not "
                             "export its elastic\n"
                             "properties.\n"
                             "- 1, which means that the thermal expansion is "
                             "isotropic.\n"
                             "- 3, which means that the thermal expansion is "
                             "orthotropic.")
      .def_property_readonly("mpnames", &get_mps,
                             "Names of the material properties that must be "
                             "given to the\n"
                             "behaviour by the calling solver")
      .def_property_readonly("ivnames", &get_ivs,
                             "Names of the internal state variables")
      .def_property_readonly("ivtypes", &get_ivs_types,
                             "Types of the internal state variables\n"
                             "- 0 means that the variable is a scalar\n"
                             "- 1 means that the variable is a symmetric "
                             "tensor\n"
                             "- 2 means that the variable is a vector\n"
                             "- 3 means that the variable is an unsymmetric "
                             "tensor")
      .def_property_readonly("evnames", &get_evs,
                             "Names of the external state variables")
      .def_property_readonly("evtypes", &get_evs_types,
                             "Types of the external state variables\n"
                             "- 0 means that the variable is a scalar\n"
                             "- 1 means that the variable is a symmetric "
                             "tensor\n"
                             "- 2 means that the variable is a vector\n"
                             "- 3 means that the variable is an unsymmetric "
                             "tensor")
      .def_readonly("btype", &ExternalBehaviourDescription::btype,
                    "Behaviour type\n"
                    "0 : general behaviour\n"
                    "1 : small strain behaviour\n"
                    "2 : finite strain behaviour\n"
                    "3 : cohesive zone model")
      .def_readonly("stype", &ExternalBehaviourDescription::stype,
                    "Symmetry type:\n"
                    "- If stype is equal to 0, the behaviour is isotropic.\n"
                    "- If stype is equal to 1, the behaviour is orthotropic.")
      .def_readonly("etype", &ExternalBehaviourDescription::etype,
                    "Elastic symmetry type:\n"
                    "- If etype is equal to 0, the behaviour elastic behaviour "
                    "is isotropic.\n"
                    "- If etype is equal to 1, the behaviour elastic behaviour "
                    "is\northotropic.")
      .def_readonly("isUPUIR", &ExternalBehaviourDescription::isUPUIR,
                    "If isUPUIR is true, the behaviour is usable in purely "
                    "implicit\nresolution.")
      .def_readonly("computesInternalEnergy",
                    &ExternalBehaviourDescription::computesInternalEnergy,
                    "If true, the behaviour may compute the internal energy")
      .def_readonly("computesDissipatedEnergy",
                    &ExternalBehaviourDescription::computesDissipatedEnergy,
                    "If true, the behaviour may compute the dissipated energy")
      .def_readonly("requiresStiffnessTensor",
                    &ExternalBehaviourDescription::requiresStiffnessTensor,
                    "If requiresStiffnessTensor is true, the caller must "
                    "provide the\nstiffness tensor.")
      .def_readonly("requiresThermalExpansionCoefficientTensor",
                    &ExternalBehaviourDescription::
                        requiresThermalExpansionCoefficientTensor,
                    "If requiresThermalExpansionCoefficientTensor is true, the "
                    "caller must provide the thermal expansion coefficients "
                    "tensor")
      .def("hasBounds", &ExternalBehaviourDescription::hasBounds,
           pybind11::arg("v"),
           "Return true if the given variable has bounds\n"
           "v: variable name")
      .def("hasLowerBound", &ExternalBehaviourDescription::hasLowerBound,
           pybind11::arg("v"),
           "Return true if the given variable has a lower bound\n"
           "v: variable name")
      .def("hasUpperBound", &ExternalBehaviourDescription::hasUpperBound,
           pybind11::arg("v"),
           "Return true if the given variable has a upper bound\n"
           "v: variable name")
      .def("getLowerBound", &ExternalBehaviourDescription::getLowerBound,
           pybind11::arg("v"),
           "Return the lower bound of the given variable\n"
           "v: variable name")
      .def("getUpperBound", &ExternalBehaviourDescription::getUpperBound,
           pybind11::arg("v"),
           "Return the upper bound of the given variable\n"
           "v: variable name")
      .def("hasPhysicalBounds",
           &ExternalBehaviourDescription::hasPhysicalBounds,
           pybind11::arg("v"),
           "Return true if the given variable has bounds\n"
           "v: variable name")
      .def("hasLowerPhysicalBound",
           &ExternalBehaviourDescription::hasLowerPhysicalBound,
           pybind11::arg("v"),
           "Return true if the given variable has a lower physical bound\n"
           "v: variable name")
      .def("hasUpperPhysicalBound",
           &ExternalBehaviourDescription::hasUpperPhysicalBound,
           pybind11::arg("v"),
           "Return true if the given variable has a upper physical bound\n"
           "v: variable name")
      .def("getLowerPhysicalBound",
           &ExternalBehaviourDescription::getLowerPhysicalBound,
           pybind11::arg("v"),
           "Return the lower bound of the given variable\n"
           "v: variable name")
      .def("getUpperPhysicalBound",
           &ExternalBehaviourDescription::getUpperPhysicalBound,
           pybind11::arg("v"),
           "Return the upper bound of the given variable\n"
           "v: variable name");
}
