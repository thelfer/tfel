/*!
 * \file  bindings/python/tfel/ExternalLibraryManager.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 juil. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "TFEL/System/ExternalLibraryManager.hxx"

void declareExternalLibraryManager();

static void ELM_loadLibrary(tfel::system::ExternalLibraryManager& elm,
                            const std::string& n) {
  elm.loadLibrary(n);
}

static void ELM_setUShortParameter(tfel::system::ExternalLibraryManager& elm,
                                   const std::string& l,
                                   const std::string& n,
                                   const std::string& p,
                                   const unsigned short v) {
  elm.setParameter(l, n, p, v);
}

static void ELM_setIntParameter(tfel::system::ExternalLibraryManager& elm,
                                const std::string& l,
                                const std::string& n,
                                const std::string& p,
                                const int v) {
  elm.setParameter(l, n, p, v);
}

static void ELM_setDoubleParameter(tfel::system::ExternalLibraryManager& elm,
                                   const std::string& l,
                                   const std::string& n,
                                   const std::string& p,
                                   const double v) {
  elm.setParameter(l, n, p, v);
}

static void ELM_setUShortParameter2(tfel::system::ExternalLibraryManager& elm,
                                    const std::string& l,
                                    const std::string& n,
                                    const std::string& h,
                                    const std::string& p,
                                    const unsigned short v) {
  elm.setParameter(l, n, h, p, v);
}

static void ELM_setIntParameter2(tfel::system::ExternalLibraryManager& elm,
                                 const std::string& l,
                                 const std::string& n,
                                 const std::string& h,
                                 const std::string& p,
                                 const int v) {
  elm.setParameter(l, n, h, p, v);
}

static void ELM_setDoubleParameter2(tfel::system::ExternalLibraryManager& elm,
                                    const std::string& l,
                                    const std::string& n,
                                    const std::string& h,
                                    const std::string& p,
                                    const double v) {
  elm.setParameter(l, n, h, p, v);
}

void declareExternalLibraryManager() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;
  using ELM = tfel::system::ExternalLibraryManager;

  bool (ELM::*hasBounds1)(const std::string&, const std::string&,
                          const std::string&, const std::string&) =
      &ELM::hasBounds;
  bool (ELM::*hasBounds2)(const std::string&, const std::string&,
                          const std::string&) = &ELM::hasBounds;

  bool (ELM::*hasLowerBound1)(const std::string&, const std::string&,
                              const std::string&, const std::string&) =
      &ELM::hasLowerBound;
  bool (ELM::*hasLowerBound2)(const std::string&, const std::string&,
                              const std::string&) = &ELM::hasLowerBound;

  bool (ELM::*hasUpperBound1)(const std::string&, const std::string&,
                              const std::string&, const std::string&) =
      &ELM::hasUpperBound;
  bool (ELM::*hasUpperBound2)(const std::string&, const std::string&,
                              const std::string&) = &ELM::hasUpperBound;

  long double (ELM::*getLowerBound1)(const std::string&, const std::string&,
                                     const std::string&, const std::string&) =
      &ELM::getLowerBound;
  long double (ELM::*getLowerBound2)(const std::string&, const std::string&,
                                     const std::string&) = &ELM::getLowerBound;

  long double (ELM::*getUpperBound1)(const std::string&, const std::string&,
                                     const std::string&, const std::string&) =
      &ELM::getUpperBound;
  long double (ELM::*getUpperBound2)(const std::string&, const std::string&,
                                     const std::string&) = &ELM::getUpperBound;

  bool (ELM::*hasPhysicalBounds1)(const std::string&, const std::string&,
                                  const std::string&, const std::string&) =
      &ELM::hasPhysicalBounds;
  bool (ELM::*hasPhysicalBounds2)(const std::string&, const std::string&,
                                  const std::string&) = &ELM::hasPhysicalBounds;

  bool (ELM::*hasLowerPhysicalBound1)(const std::string&, const std::string&,
                                      const std::string&, const std::string&) =
      &ELM::hasLowerPhysicalBound;
  bool (ELM::*hasLowerPhysicalBound2)(const std::string&, const std::string&,
                                      const std::string&) =
      &ELM::hasLowerPhysicalBound;

  bool (ELM::*hasUpperPhysicalBound1)(const std::string&, const std::string&,
                                      const std::string&, const std::string&) =
      &ELM::hasUpperPhysicalBound;
  bool (ELM::*hasUpperPhysicalBound2)(const std::string&, const std::string&,
                                      const std::string&) =
      &ELM::hasUpperPhysicalBound;

  long double (ELM::*getLowerPhysicalBound1)(
      const std::string&, const std::string&, const std::string&,
      const std::string&) = &ELM::getLowerPhysicalBound;
  long double (ELM::*getLowerPhysicalBound2)(
      const std::string&, const std::string&, const std::string&) =
      &ELM::getLowerPhysicalBound;

  long double (ELM::*getUpperPhysicalBound1)(
      const std::string&, const std::string&, const std::string&,
      const std::string&) = &ELM::getUpperPhysicalBound;
  long double (ELM::*getUpperPhysicalBound2)(
      const std::string&, const std::string&, const std::string&) =
      &ELM::getUpperPhysicalBound;

  class_<ELM, noncopyable>("ExternalLibraryManager", no_init)
      .def("getExternalLibraryManager", ELM::getExternalLibraryManager,
           return_value_policy<reference_existing_object>())
      .staticmethod("getExternalLibraryManager")
      .def("loadLibrary", ELM_loadLibrary)
      .def("getAuthor", &ELM::getAuthor)
      .def("getDate", &ELM::getDate)
      .def("getDescription", &ELM::getDescription)
      .def("getBuildId", &ELM::getBuildId)
      .def("getMaterial", &ELM::getMaterial)
      .def("getSource", &ELM::getSource)
      .def("getTFELVersion", &ELM::getTFELVersion,
           "return the TFEL Version used to generate the given entry point")
      .def("getUnitSystem", &ELM::getUnitSystem,
           "return the unit system associated with the given entry point")
      .def("getEntryPoints", &ELM::getEntryPoints,
           "return the list of all mfront generated entry points\n"
           "in a library")
      .def("getMaterialKnowledgeType", &ELM::getMaterialKnowledgeType,
           "return the list of all mfront generated entry points in a"
           "library. The returned value has the following meaning:\n"
           "- 0: material property\n"
           "- 1: behaviour\n"
           "- 2: model")
      .def("getInterface", &ELM::getInterface)
      .def("setUShortParameter", ELM_setUShortParameter)
      .def("setIntParameter", ELM_setIntParameter)
      .def("setDoubleParameter", ELM_setDoubleParameter)
      .def("setParameter", ELM_setDoubleParameter)
      .def("setUShortParameter", ELM_setUShortParameter2)
      .def("setIntParameter", ELM_setIntParameter2)
      .def("setDoubleParameter", ELM_setDoubleParameter2)
      .def("setParameter", ELM_setDoubleParameter2)
      .def("getRealParameterDefaultValue", &ELM::getRealParameterDefaultValue,
           "return the default value of a real parameter")
      .def("getIntegerParameterDefaultValue",
           &ELM::getIntegerParameterDefaultValue,
           "return the default value of an integer parameter")
      .def("getUnsignedShortParameterDefaultValue",
           &ELM::getUnsignedShortParameterDefaultValue,
           "return the default value of an unsigned short parameter")
      .def("getUMATDrivingVariablesNames", &ELM::getUMATDrivingVariablesNames,
           "return the list of the gradients")
      .def("getUMATGradientsNames", &ELM::getUMATGradientsNames,
           "return the list of the gradients")
      .def("getUMATParametersNames", &ELM::getUMATParametersNames,
           "return the list of the parameters")
      .def("getUMATParametersTypes", &ELM::getUMATParametersTypes,
           "return the types of the parameters")

      .def("hasBounds", hasBounds1,
           "return true if the given variable has bounds")
      .def("hasBounds", hasBounds2,
           "return true if the given variable has bounds")
      .def("hasLowerBound", hasLowerBound1,
           "return true if the given variable has lower bound")
      .def("hasLowerBound", hasLowerBound2,
           "return true if the given variable has lower bound")
      .def("hasUpperBound", hasUpperBound1,
           "return true if the given variable has upper bound")
      .def("hasUpperBound", hasUpperBound2,
           "return true if the given variable has upper bound")
      .def("getLowerBound", getLowerBound1,
           "return the lower bound of a variable")
      .def("getLowerBound", getLowerBound2,
           "return the lower bound of a variable")
      .def("getUpperBound", getUpperBound1,
           "return the upper bound of a variable")
      .def("getUpperBound", getUpperBound2,
           "return the upper bound of a variable")

      .def("hasPhysicalBounds", hasPhysicalBounds1,
           "return true if the given variable has physical bounds")
      .def("hasPhysicalBounds", hasPhysicalBounds2,
           "return true if the given variable has physical bounds")
      .def("hasLowerPhysicalBound", hasLowerPhysicalBound1,
           "return true if the given variable has lower physical bound")
      .def("hasLowerPhysicalBound", hasLowerPhysicalBound2,
           "return true if the given variable has lower physical bound")
      .def("hasUpperPhysicalBound", hasUpperPhysicalBound1,
           "return true if the given variable has upper physical bound")
      .def("hasUpperPhysicalBound", hasUpperPhysicalBound2,
           "return true if the given variable has upper physical bound")
      .def("getLowerPhysicalBound", getLowerPhysicalBound1,
           "return the lower physical bound of a variable")
      .def("getLowerPhysicalBound", getLowerPhysicalBound2,
           "return the lower physical bound of a variable")
      .def("getUpperPhysicalBound", getUpperPhysicalBound1,
           "return the upper physical bound of a variable")
      .def("getUpperPhysicalBound", getUpperPhysicalBound2,
           "return the upper physical bound of a variable")

      .def("contains", &ELM::contains)
      .def("getUMATBehaviourType", &ELM::getUMATBehaviourType)
      .def("getUMATSymmetryType", &ELM::getUMATSymmetryType)
      .def("getUMATElasticSymmetryType", &ELM::getUMATElasticSymmetryType)
      .def("getUMATMaterialPropertiesNames",
           &ELM::getUMATMaterialPropertiesNames)
      .def("getUMATInternalStateVariablesNames",
           &ELM::getUMATInternalStateVariablesNames)
      .def("getUMATInternalStateVariablesTypes",
           &ELM::getUMATInternalStateVariablesTypes)
      .def("getUMATExternalStateVariablesNames",
           &ELM::getUMATExternalStateVariablesNames)
      .def("getUMATExternalStateVariablesTypes",
           &ELM::getUMATExternalStateVariablesTypes)
      .def("getUMATRequiresStiffnessTensor",
           &ELM::getUMATRequiresStiffnessTensor)
      .def("getUMATRequiresThermalExpansionCoefficientTensor",
           &ELM::getUMATRequiresThermalExpansionCoefficientTensor)
      .def("checkIfAsterBehaviourSaveTangentOperator",
           &ELM::checkIfAsterBehaviourSavesTangentOperator)
      .def("getGenericBehaviourInitializeFunctions",
           &ELM::getGenericBehaviourInitializeFunctions)
      .def("getGenericBehaviourInitializeFunctionInputs",
           &ELM::getGenericBehaviourInitializeFunctionInputs)
      .def("getGenericBehaviourInitializeFunctionInputsTypes",
           &ELM::getGenericBehaviourInitializeFunctionInputsTypes)
      .def("getGenericBehaviourPostProcessingFunctions",
           &ELM::getGenericBehaviourPostProcessingFunctions)
      .def("getGenericBehaviourPostProcessingFunctionOutputs",
           &ELM::getGenericBehaviourPostProcessingFunctionOutputs)
      .def("getGenericBehaviourPostProcessingFunctionOutputsTypes",
           &ELM::getGenericBehaviourPostProcessingFunctionOutputsTypes)
      .def("getMaterialPropertyVariables",
           static_cast<std::vector<std::string> (ELM::*)(const std::string&,
                                                         const std::string&)>(
               &ELM::getMaterialPropertyVariables))
      .def("getMaterialPropertyParameters",
           static_cast<std::vector<std::string> (ELM::*)(const std::string&,
                                                         const std::string&)>(
               &ELM::getMaterialPropertyParameters));

}  // end of declareExternalLibraryManager
