/*!
 * \file   bindings/python/mfront/BehaviourData.cxx
 * \brief
 * \author THOMAS HELFER
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

void declareBehaviourData() {
  using namespace boost::python;
  using namespace mfront;

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
      .def("setFloattingPointParameterDefaultVale",
           static_cast<void (BehaviourData::*)(const std::string&,
                                               const double)>(
               &BehaviourData::setParameterDefaultValue))
      .def("setUnsignedShortParameterDefaultVale",
           static_cast<void (BehaviourData::*)(const std::string&,
                                               const unsigned short)>(
               &BehaviourData::setParameterDefaultValue))
      .def("setIntegerParameterDefaultVale",
           static_cast<void (BehaviourData::*)(const std::string&, const int)>(
               &BehaviourData::setParameterDefaultValue))
      .def("getCodeBlockNames", &BehaviourData::getCodeBlockNames)
      .def("getCode", &BehaviourData::getCode)
      .def("hasCode", &BehaviourData::hasCode);
}
