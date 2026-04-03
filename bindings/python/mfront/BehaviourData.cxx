
/*!
 * \file   bindings/python/mfront/BehaviourData.cxx
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

  pybind11::class_<BehaviourData>(m, "BehaviourData")
      .def("getMaterialProperties", &BehaviourData::getMaterialProperties,
           pybind11::return_value_policy::reference)
      .def("getPersistentVariables", &BehaviourData::getPersistentVariables,
           pybind11::return_value_policy::reference)
      .def("getIntegrationVariables", &BehaviourData::getIntegrationVariables,
           pybind11::return_value_policy::reference)
      .def("getStateVariables", &BehaviourData::getStateVariables,
           pybind11::return_value_policy::reference)
      .def("getAuxiliaryStateVariables",
           &BehaviourData::getAuxiliaryStateVariables,
           pybind11::return_value_policy::reference)
      .def("getExternalStateVariables",
           &BehaviourData::getExternalStateVariables,
           pybind11::return_value_policy::reference)
      .def("getLocalVariables", &BehaviourData::getLocalVariables,
           pybind11::return_value_policy::reference)
      .def("getParameters", &BehaviourData::getParameters,
           pybind11::return_value_policy::reference)
      .def("getAuxiliaryStateVariables",
           &BehaviourData::getAuxiliaryStateVariables,
           pybind11::return_value_policy::reference)
      .def("getPersistentVariableDescription",
           &BehaviourData::getPersistentVariableDescription,
           pybind11::return_value_policy::reference)
      .def("getIntegrationVariableDescription",
           &BehaviourData::getIntegrationVariableDescription,
           pybind11::return_value_policy::reference)
      .def("getStateVariableDescription",
           &BehaviourData::getStateVariableDescription,
           pybind11::return_value_policy::reference)
      .def("isMemberUsedInCodeBlocks", &BehaviourData::isMemberUsedInCodeBlocks)
      .def("isMaterialPropertyName", &BehaviourData::isMaterialPropertyName)
      .def("isLocalVariableName", &BehaviourData::isLocalVariableName)
      .def("isPersistentVariableName", &BehaviourData::isPersistentVariableName)
      .def("isIntegrationVariableName",
           &BehaviourData::isIntegrationVariableName)
      .def("isIntegrationVariableIncrementName",
           &BehaviourData::isIntegrationVariableIncrementName)
      .def("isStateVariableName", &BehaviourData::isStateVariableName)
      .def("isStateVariableIncrementName",
           &BehaviourData::isStateVariableIncrementName)
      .def("isAuxiliaryStateVariableName",
           &BehaviourData::isAuxiliaryStateVariableName)
      .def("isExternalStateVariableName",
           &BehaviourData::isExternalStateVariableName)
      .def("isExternalStateVariableIncrementName",
           &BehaviourData::isExternalStateVariableIncrementName)
      .def("getVariableDescription", ptr,
           pybind11::return_value_policy::reference)
      .def("getVariableDescriptionByExternalName",
           &BehaviourData::getVariableDescriptionByExternalName,
           pybind11::return_value_policy::reference)
      .def("getPersistentVariableDescription",
           &BehaviourData::getPersistentVariableDescription,
           pybind11::return_value_policy::reference)
      .def("getPersistentVariableDescriptionByExternalName",
           &BehaviourData::getPersistentVariableDescriptionByExternalName,
           pybind11::return_value_policy::reference)
      .def("getIntegrationVariableDescription",
           &BehaviourData::getIntegrationVariableDescription,
           pybind11::return_value_policy::reference)
      .def("getIntegrationVariableDescriptionByExternalName",
           &BehaviourData::getIntegrationVariableDescriptionByExternalName,
           pybind11::return_value_policy::reference)
      .def("getStateVariableDescription",
           &BehaviourData::getStateVariableDescription,
           pybind11::return_value_policy::reference)
      .def("getStateVariableDescriptionByExternalName",
           &BehaviourData::getStateVariableDescriptionByExternalName,
           pybind11::return_value_policy::reference)
      .def("getAuxiliaryStateVariableDescription",
           &BehaviourData::getAuxiliaryStateVariableDescription,
           pybind11::return_value_policy::reference)
      .def("getAuxiliaryStateVariableDescriptionByExternalName",
           &BehaviourData::getAuxiliaryStateVariableDescriptionByExternalName,
           pybind11::return_value_policy::reference)
      .def("getExternalStateVariableDescription",
           &BehaviourData::getExternalStateVariableDescription,
           pybind11::return_value_policy::reference)
      .def("getExternalStateVariableDescriptionByExternalName",
           &BehaviourData::getExternalStateVariableDescriptionByExternalName,
           pybind11::return_value_policy::reference)
      .def("isParameterName", &BehaviourData::isParameterName)
      .def("isStaticVariableName", &BehaviourData::isStaticVariableName)
      .def("hasGlossaryName", &BehaviourData::hasGlossaryName)
      .def("hasEntryName", &BehaviourData::hasEntryName)
      .def("getExternalName", &BehaviourData::getExternalName)
      .def("getExternalNames",
           static_cast<std::vector<std::string> (BehaviourData::*)(
               const VariableDescriptionContainer&) const>(
               &BehaviourData::getExternalNames))
      .def("setGlossaryName", &BehaviourData::setGlossaryName)
      .def("isGlossaryNameUsed", &BehaviourData::isGlossaryNameUsed)
      .def("setEntryName", &BehaviourData::setEntryName)
      .def("isUsedAsEntryName", &BehaviourData::isUsedAsEntryName)
      .def("getVariableNameFromGlossaryNameOrEntryName",
           &BehaviourData::getVariableNameFromGlossaryNameOrEntryName)
      .def("hasAttribute", &BehaviourData::hasAttribute)
      .def("setAttribute", &BehaviourData::setAttribute)
      .def("getUnsignedShortAttribute", getUnsignedShortAttribute)
      .def("getStringAttribute", getStringAttribute)
      .def("getBooleanAttribute", getBooleanAttribute)
      .def("getAttributes", &BehaviourData::getAttributes)
      .def("reserveName", &BehaviourData::reserveName)
      .def("registerMemberName", &BehaviourData::registerMemberName)
      .def("registerStaticMemberName", &BehaviourData::registerStaticMemberName)
      .def("getRegistredMembersNames", getRegistredMembersNames)
      .def("getRegistredStaticMembersNames", getRegistredStaticMembersNames)
      .def("getVariablesNames", getVariablesNames)
      .def("hasParameter", &BehaviourData::hasParameter)
      .def("hasParameters", &BehaviourData::hasParameters)
      .def("setFloatingPointParameterDefaultValue",
           static_cast<void (BehaviourData::*)(const std::string&,
                                               const double)>(
               &BehaviourData::setParameterDefaultValue))
      .def("setFloatingPointParameterDefaultValue",
           static_cast<void (BehaviourData::*)(
               const std::string&, const unsigned short, const double)>(
               &BehaviourData::setParameterDefaultValue))
      .def("setFloattingPointParameterDefaultValue",
           static_cast<void (BehaviourData::*)(const std::string&,
                                               const double)>(
               &BehaviourData::setParameterDefaultValue))
      .def("setFloattingPointParameterDefaultValue",
           static_cast<void (BehaviourData::*)(
               const std::string&, const unsigned short, const double)>(
               &BehaviourData::setParameterDefaultValue))
      .def("setUnsignedShortParameterDefaultValue",
           static_cast<void (BehaviourData::*)(const std::string&,
                                               const unsigned short)>(
               &BehaviourData::setParameterDefaultValue))
      .def("setIntegerParameterDefaultValue",
           static_cast<void (BehaviourData::*)(const std::string&, const int)>(
               &BehaviourData::setParameterDefaultValue))
      .def("getFloatingPointParameterDefaultValue",
           static_cast<double (BehaviourData::*)(const std::string&) const>(
               &BehaviourData::getFloattingPointParameterDefaultValue))
      .def("getFloatingPointParameterDefaultValue",
           static_cast<double (BehaviourData::*)(const std::string&,
                                                 const unsigned short) const>(
               &BehaviourData::getFloattingPointParameterDefaultValue))
      .def("getFloattingPointParameterDefaultValue",
           static_cast<double (BehaviourData::*)(const std::string&) const>(
               &BehaviourData::getFloattingPointParameterDefaultValue))
      .def("getFloattingPointParameterDefaultValue",
           static_cast<double (BehaviourData::*)(const std::string&,
                                                 const unsigned short) const>(
               &BehaviourData::getFloattingPointParameterDefaultValue))
      .def("getUnsignedShortParameterDefaultValue",
           static_cast<unsigned short (BehaviourData::*)(const std::string&)
                           const>(
               &BehaviourData::getUnsignedShortParameterDefaultValue))
      .def("getIntegerParameterDefaultValue",
           static_cast<int (BehaviourData::*)(const std::string&) const>(
               &BehaviourData::getIntegerParameterDefaultValue))
      .def("getCodeBlockNames", &BehaviourData::getCodeBlockNames)
      .def("getCode", &BehaviourData::getCode)
      .def("hasCode", &BehaviourData::hasCode);
}
