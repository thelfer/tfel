
/*!
 * \file   bindings/python/mfront/BehaviourData.cxx
 * \brief
 * \author Thomas Helfer
 * \date 04/05/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/BehaviourData.hxx"

static std::string getStringAttribute(const mfront::BehaviourData& d,
                                      const std::string& n) {
  return d.getAttribute<std::string>(n);
}

static unsigned short getUnsignedShortAttribute(const mfront::BehaviourData& d,
                                                const std::string& n) {
  return d.getAttribute<unsigned short>(n);
}

static bool getBooleanAttribute(const mfront::BehaviourData& d,
                                const std::string& n) {
  return d.getAttribute<bool>(n);
}

static std::vector<std::string> getRegistredMembersNames(
    const mfront::BehaviourData& d) {
  const auto& n = d.getRegistredMembersNames();
  return {n.begin(), n.end()};
}

static std::vector<std::string> getRegistredStaticMembersNames(
    const mfront::BehaviourData& d) {
  const auto& n = d.getRegistredStaticMembersNames();
  return {n.begin(), n.end()};
}

static std::vector<std::string> getVariablesNames(
    const mfront::BehaviourData& d) {
  const auto n = d.getVariablesNames();
  return {n.begin(), n.end()};
}

void declareBehaviourData(pybind11::module_&);

void declareBehaviourData(pybind11::module_& m) {
  using namespace mfront;
  const VariableDescription& (BehaviourData::*ptr)(const std::string&) const =
      &BehaviourData::getVariableDescription;

  pybind11::class_<BehaviourData>(
      m, "BehaviourData",
      "This structure gathers various behaviour characteristic\n"
      "for a given modelling hypothesis")
      .def(pybind11::init<>())
      .def("getMaterialProperties", &BehaviourData::getMaterialProperties,
           pybind11::return_value_policy::reference_internal)
      .def("getPersistentVariables", &BehaviourData::getPersistentVariables,
           pybind11::return_value_policy::reference_internal)
      .def("getIntegrationVariables", &BehaviourData::getIntegrationVariables,
           pybind11::return_value_policy::reference_internal)
      .def("getStateVariables", &BehaviourData::getStateVariables,
           pybind11::return_value_policy::reference_internal)
      .def("getAuxiliaryStateVariables",
           &BehaviourData::getAuxiliaryStateVariables,
           pybind11::return_value_policy::reference_internal)
      .def("getExternalStateVariables",
           &BehaviourData::getExternalStateVariables,
           pybind11::return_value_policy::reference_internal)
      .def("getLocalVariables", &BehaviourData::getLocalVariables,
           pybind11::return_value_policy::reference_internal)
      .def("getParameters", &BehaviourData::getParameters,
           pybind11::return_value_policy::reference_internal)
      .def("getAuxiliaryStateVariables",
           &BehaviourData::getAuxiliaryStateVariables,
           pybind11::return_value_policy::reference_internal)
      .def("getPersistentVariableDescription",
           &BehaviourData::getPersistentVariableDescription, pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getIntegrationVariableDescription",
           &BehaviourData::getIntegrationVariableDescription,
           pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getStateVariableDescription",
           &BehaviourData::getStateVariableDescription, pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("isMemberUsedInCodeBlocks", &BehaviourData::isMemberUsedInCodeBlocks,
           pybind11::arg("n"))
      .def("isMaterialPropertyName", &BehaviourData::isMaterialPropertyName,
           pybind11::arg("n"))
      .def("isLocalVariableName", &BehaviourData::isLocalVariableName,
           pybind11::arg("n"))
      .def("isPersistentVariableName", &BehaviourData::isPersistentVariableName,
           pybind11::arg("n"))
      .def("isIntegrationVariableName",
           &BehaviourData::isIntegrationVariableName, pybind11::arg("n"))
      .def("isIntegrationVariableIncrementName",
           &BehaviourData::isIntegrationVariableIncrementName,
           pybind11::arg("n"))
      .def("isStateVariableName", &BehaviourData::isStateVariableName,
           pybind11::arg("n"))
      .def("isStateVariableIncrementName",
           &BehaviourData::isStateVariableIncrementName, pybind11::arg("n"))
      .def("isAuxiliaryStateVariableName",
           &BehaviourData::isAuxiliaryStateVariableName, pybind11::arg("n"))
      .def("isExternalStateVariableName",
           &BehaviourData::isExternalStateVariableName, pybind11::arg("n"))
      .def("isExternalStateVariableIncrementName",
           &BehaviourData::isExternalStateVariableIncrementName,
           pybind11::arg("n"))
      .def("getVariableDescription", ptr, pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getVariableDescriptionByExternalName",
           &BehaviourData::getVariableDescriptionByExternalName,
           pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getPersistentVariableDescription",
           &BehaviourData::getPersistentVariableDescription, pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getPersistentVariableDescriptionByExternalName",
           &BehaviourData::getPersistentVariableDescriptionByExternalName,
           pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getIntegrationVariableDescription",
           &BehaviourData::getIntegrationVariableDescription,
           pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getIntegrationVariableDescriptionByExternalName",
           &BehaviourData::getIntegrationVariableDescriptionByExternalName,
           pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getStateVariableDescription",
           &BehaviourData::getStateVariableDescription, pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getStateVariableDescriptionByExternalName",
           &BehaviourData::getStateVariableDescriptionByExternalName,
           pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getAuxiliaryStateVariableDescription",
           &BehaviourData::getAuxiliaryStateVariableDescription,
           pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getAuxiliaryStateVariableDescriptionByExternalName",
           &BehaviourData::getAuxiliaryStateVariableDescriptionByExternalName,
           pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getExternalStateVariableDescription",
           &BehaviourData::getExternalStateVariableDescription,
           pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("getExternalStateVariableDescriptionByExternalName",
           &BehaviourData::getExternalStateVariableDescriptionByExternalName,
           pybind11::arg("n"),
           pybind11::return_value_policy::reference_internal)
      .def("isParameterName", &BehaviourData::isParameterName,
           pybind11::arg("n"))
      .def("isStaticVariableName", &BehaviourData::isStaticVariableName,
           pybind11::arg("n"))
      .def("hasGlossaryName", &BehaviourData::hasGlossaryName,
           pybind11::arg("n"))
      .def("hasEntryName", &BehaviourData::hasEntryName, pybind11::arg("n"))
      .def("getExternalName", &BehaviourData::getExternalName,
           pybind11::arg("n"))
      .def("getExternalNames",
           static_cast<std::vector<std::string> (BehaviourData::*)(
               const VariableDescriptionContainer&) const>(
               &BehaviourData::getExternalNames),
           pybind11::arg("v"))
      .def("setGlossaryName", &BehaviourData::setGlossaryName,
           pybind11::arg("n"), pybind11::arg("g"))
      .def("isGlossaryNameUsed", &BehaviourData::isGlossaryNameUsed,
           pybind11::arg("n"))
      .def("setEntryName", &BehaviourData::setEntryName, pybind11::arg("n"),
           pybind11::arg("e"))
      .def("isUsedAsEntryName", &BehaviourData::isUsedAsEntryName,
           pybind11::arg("n"))
      .def("getVariableNameFromGlossaryNameOrEntryName",
           &BehaviourData::getVariableNameFromGlossaryNameOrEntryName,
           pybind11::arg("n"))
      .def("hasAttribute", &BehaviourData::hasAttribute, pybind11::arg("n"))
      .def("setAttribute", &BehaviourData::setAttribute, pybind11::arg("n"),
           pybind11::arg("a"), pybind11::arg("b"))
      .def("getUnsignedShortAttribute", getUnsignedShortAttribute,
           pybind11::arg("n"))
      .def("getStringAttribute", getStringAttribute, pybind11::arg("n"))
      .def("getBooleanAttribute", getBooleanAttribute, pybind11::arg("n"))
      .def("getAttributes", &BehaviourData::getAttributes)
      .def("reserveName", &BehaviourData::reserveName, pybind11::arg("n"))
      .def("registerMemberName", &BehaviourData::registerMemberName,
           pybind11::arg("n"))
      .def("registerStaticMemberName", &BehaviourData::registerStaticMemberName,
           pybind11::arg("n"))
      .def("getRegistredMembersNames", getRegistredMembersNames)
      .def("getRegistredStaticMembersNames", getRegistredStaticMembersNames)
      .def("getVariablesNames", getVariablesNames)
      .def("hasParameter", &BehaviourData::hasParameter, pybind11::arg("n"))
      .def("hasParameters", &BehaviourData::hasParameters)
      .def("setFloatingPointParameterDefaultValue",
           static_cast<void (BehaviourData::*)(const std::string&,
                                               const double)>(
               &BehaviourData::setParameterDefaultValue),
           pybind11::arg("n"), pybind11::arg("v"))
      .def("setFloatingPointParameterDefaultValue",
           static_cast<void (BehaviourData::*)(
               const std::string&, const unsigned short, const double)>(
               &BehaviourData::setParameterDefaultValue),
           pybind11::arg("n"), pybind11::arg("i"), pybind11::arg("v"))
      .def("setFloattingPointParameterDefaultValue",
           static_cast<void (BehaviourData::*)(const std::string&,
                                               const double)>(
               &BehaviourData::setParameterDefaultValue),
           pybind11::arg("n"), pybind11::arg("v"))
      .def("setFloattingPointParameterDefaultValue",
           static_cast<void (BehaviourData::*)(
               const std::string&, const unsigned short, const double)>(
               &BehaviourData::setParameterDefaultValue),
           pybind11::arg("n"), pybind11::arg("i"), pybind11::arg("v"))
      .def("setUnsignedShortParameterDefaultValue",
           static_cast<void (BehaviourData::*)(const std::string&,
                                               const unsigned short)>(
               &BehaviourData::setParameterDefaultValue),
           pybind11::arg("n"), pybind11::arg("v"))
      .def("setIntegerParameterDefaultValue",
           static_cast<void (BehaviourData::*)(const std::string&, const int)>(
               &BehaviourData::setParameterDefaultValue),
           pybind11::arg("n"), pybind11::arg("v"))
      .def("getFloatingPointParameterDefaultValue",
           static_cast<double (BehaviourData::*)(const std::string&) const>(
               &BehaviourData::getFloattingPointParameterDefaultValue),
           pybind11::arg("n"))
      .def("getFloatingPointParameterDefaultValue",
           static_cast<double (BehaviourData::*)(const std::string&,
                                                 const unsigned short) const>(
               &BehaviourData::getFloattingPointParameterDefaultValue),
           pybind11::arg("n"), pybind11::arg("i"))
      .def("getFloattingPointParameterDefaultValue",
           static_cast<double (BehaviourData::*)(const std::string&) const>(
               &BehaviourData::getFloattingPointParameterDefaultValue),
           pybind11::arg("n"))
      .def("getFloattingPointParameterDefaultValue",
           static_cast<double (BehaviourData::*)(const std::string&,
                                                 const unsigned short) const>(
               &BehaviourData::getFloattingPointParameterDefaultValue),
           pybind11::arg("n"), pybind11::arg("i"))
      .def("getUnsignedShortParameterDefaultValue",
           static_cast<unsigned short (BehaviourData::*)(const std::string&)
                           const>(
               &BehaviourData::getUnsignedShortParameterDefaultValue),
           pybind11::arg("n"))
      .def("getIntegerParameterDefaultValue",
           static_cast<int (BehaviourData::*)(const std::string&) const>(
               &BehaviourData::getIntegerParameterDefaultValue),
           pybind11::arg("n"))
      .def("getCodeBlockNames", &BehaviourData::getCodeBlockNames)
      .def("getCode", &BehaviourData::getCode, pybind11::arg("n"),
           pybind11::arg("cn"), pybind11::arg("b"))
      .def("hasCode", &BehaviourData::hasCode, pybind11::arg("n"));
}
