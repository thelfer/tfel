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
#include <boost/python.hpp>
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

static const std::vector<std::string> getRegistredMembersNames(
    const mfront::BehaviourData& d) {
  const auto& n = d.getRegistredMembersNames();
  return {n.begin(), n.end()};
}

static const std::vector<std::string> getRegistredStaticMembersNames(
    const mfront::BehaviourData& d) {
  const auto& n = d.getRegistredStaticMembersNames();
  return {n.begin(), n.end()};
}

static const std::vector<std::string> getVariablesNames(
    const mfront::BehaviourData& d) {
  const auto n = d.getVariablesNames();
  return {n.begin(), n.end()};
}

void declareBehaviourData();
void declareBehaviourData() {
  using namespace boost::python;
  using namespace mfront;

  const VariableDescription& (BehaviourData::*ptr)(const std::string&) const =
      &BehaviourData::getVariableDescription;

  class_<BehaviourData>("BehaviourData")
      .def("getMaterialProperties", &BehaviourData::getMaterialProperties,
           return_internal_reference<>())
      .def("getPersistentVariables", &BehaviourData::getPersistentVariables,
           return_internal_reference<>())
      .def("getIntegrationVariables", &BehaviourData::getIntegrationVariables,
           return_internal_reference<>())
      .def("getStateVariables", &BehaviourData::getStateVariables,
           return_internal_reference<>())
      .def("getAuxiliaryStateVariables",
           &BehaviourData::getAuxiliaryStateVariables,
           return_internal_reference<>())
      .def("getExternalStateVariables",
           &BehaviourData::getExternalStateVariables,
           return_internal_reference<>())
      .def("getLocalVariables", &BehaviourData::getLocalVariables,
           return_internal_reference<>())
      .def("getParameters", &BehaviourData::getParameters,
           return_internal_reference<>())
      .def("getAuxiliaryStateVariables",
           &BehaviourData::getAuxiliaryStateVariables,
           return_internal_reference<>())
      .def("getPersistentVariableDescription",
           &BehaviourData::getPersistentVariableDescription,
           return_internal_reference<>())
      .def("getIntegrationVariableDescription",
           &BehaviourData::getIntegrationVariableDescription,
           return_internal_reference<>())
      .def("getStateVariableDescription",
           &BehaviourData::getStateVariableDescription,
           return_internal_reference<>())
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
      .def("getVariableDescription", ptr, return_internal_reference<>())
      .def("getVariableDescriptionByExternalName",
           &BehaviourData::getVariableDescriptionByExternalName,
           return_internal_reference<>())
      .def("getPersistentVariableDescription",
           &BehaviourData::getPersistentVariableDescription,
           return_internal_reference<>())
      .def("getPersistentVariableDescriptionByExternalName",
           &BehaviourData::getPersistentVariableDescriptionByExternalName,
           return_internal_reference<>())
      .def("getIntegrationVariableDescription",
           &BehaviourData::getIntegrationVariableDescription,
           return_internal_reference<>())
      .def("getIntegrationVariableDescriptionByExternalName",
           &BehaviourData::getIntegrationVariableDescriptionByExternalName,
           return_internal_reference<>())
      .def("getStateVariableDescription",
           &BehaviourData::getStateVariableDescription,
           return_internal_reference<>())
      .def("getStateVariableDescriptionByExternalName",
           &BehaviourData::getStateVariableDescriptionByExternalName,
           return_internal_reference<>())
      .def("getAuxiliaryStateVariableDescription",
           &BehaviourData::getAuxiliaryStateVariableDescription,
           return_internal_reference<>())
      .def("getAuxiliaryStateVariableDescriptionByExternalName",
           &BehaviourData::getAuxiliaryStateVariableDescriptionByExternalName,
           return_internal_reference<>())
      .def("getExternalStateVariableDescription",
           &BehaviourData::getExternalStateVariableDescription,
           return_internal_reference<>())
      .def("getExternalStateVariableDescriptionByExternalName",
           &BehaviourData::getExternalStateVariableDescriptionByExternalName,
           return_internal_reference<>())
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
      .def("getAttributes", &BehaviourData::getAttributes,
           return_value_policy<copy_const_reference>())
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
