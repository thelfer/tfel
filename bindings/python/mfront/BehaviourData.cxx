/*!
 * \file   bindings/python/mfront/BehaviourData.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   04 mai 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<set>
#include<memory>
#include<boost/python.hpp>
#include"MFront/BehaviourData.hxx"

static std::string
getStringAttribute(const mfront::BehaviourData& d,
		   const std::string& n){
  return d.getAttribute<std::string>(n);
}

static unsigned short
getUnsignedShortAttribute(const mfront::BehaviourData& d,
			  const std::string& n){
  return d.getAttribute<unsigned short>(n);
}

static bool
getBoolAttribute(const mfront::BehaviourData& d,
		 const std::string& n){
  return d.getAttribute<bool>(n);
}

void declareBehaviourData(void){
  using namespace boost::python;
  using namespace mfront;

  class_<BehaviourData>("BehaviourData")
    .def("getMaterialProperties",&BehaviourData::getMaterialProperties,
	 return_internal_reference<>())
    .def("getPersistentVariables",&BehaviourData::getPersistentVariables,
	 return_internal_reference<>())
    .def("getIntegrationVariables",&BehaviourData::getIntegrationVariables,
	 return_internal_reference<>())
    .def("getStateVariables",&BehaviourData::getStateVariables,
	 return_internal_reference<>())
    .def("getAuxiliaryStateVariables",&BehaviourData::getAuxiliaryStateVariables,
	 return_internal_reference<>())
    .def("getExternalStateVariables",&BehaviourData::getExternalStateVariables,
	 return_internal_reference<>())
    .def("getLocalVariables",&BehaviourData::getLocalVariables,
	 return_internal_reference<>())
    .def("getParameters",&BehaviourData::getParameters,
	 return_internal_reference<>())
    .def("getAuxiliaryStateVariables",&BehaviourData::getAuxiliaryStateVariables,
	 return_internal_reference<>())
    .def("getPersistentVariableDescription",&BehaviourData::getPersistentVariableDescription,
	 return_internal_reference<>())
    .def("getIntegrationVariableDescription",&BehaviourData::getIntegrationVariableDescription,
	 return_internal_reference<>())
    .def("getStateVariableDescription",&BehaviourData::getStateVariableDescription,
	 return_internal_reference<>())
    .def("isMemberUsedInCodeBlocks",&BehaviourData::isMemberUsedInCodeBlocks)
    .def("isMaterialPropertyName",&BehaviourData::isMaterialPropertyName)
    .def("isLocalVariableName",&BehaviourData::isLocalVariableName)
    .def("isPersistentVariableName",&BehaviourData::isPersistentVariableName)
    .def("isIntegrationVariableName",&BehaviourData::isIntegrationVariableName)
    .def("isIntegrationVariableIncrementName",&BehaviourData::isIntegrationVariableIncrementName)
    .def("isStateVariableName",&BehaviourData::isStateVariableName)
    .def("isStateVariableIncrementName",&BehaviourData::isStateVariableIncrementName)
    .def("isAuxiliaryStateVariableName",&BehaviourData::isAuxiliaryStateVariableName)
    .def("isExternalStateVariableName",&BehaviourData::isExternalStateVariableName)
    .def("isExternalStateVariableIncrementName",&BehaviourData::isExternalStateVariableIncrementName)
    .def("isParameterName",&BehaviourData::isParameterName)
    .def("isStaticVariableName",&BehaviourData::isStaticVariableName)
    .def("hasGlossaryName",&BehaviourData::hasGlossaryName)
    .def("hasEntryName",&BehaviourData::hasEntryName)
    .def("getExternalName",&BehaviourData::getExternalName)
    .def("getExternalNames",
	 static_cast<std::vector<std::string>(BehaviourData::*)(const VariableDescriptionContainer&) const>(&BehaviourData::getExternalNames))
    .def("setGlossaryName",&BehaviourData::setGlossaryName)
    .def("isGlossaryNameUsed",&BehaviourData::isGlossaryNameUsed)
    .def("setEntryName",&BehaviourData::setEntryName)
    .def("isUsedAsEntryName",&BehaviourData::isUsedAsEntryName)
    .def("getVariableNameFromGlossaryNameOrEntryName",
	 &BehaviourData::getVariableNameFromGlossaryNameOrEntryName)
    .def("hasAttribute",&BehaviourData::hasAttribute)
    .def("setAttribute",&BehaviourData::setAttribute)
    .def("getUnsignedShortAttribute",getUnsignedShortAttribute)
    .def("getStringAttribute",getStringAttribute)
    .def("getBoolAttribute",getBoolAttribute)
    ;
}
