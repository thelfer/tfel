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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/System/ExternalLibraryManager.hxx"

void declareExternalLibraryManager(pybind11::module_&);

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

void declareExternalLibraryManager(pybind11::module_& m) {
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

  pybind11::class_<ELM>(
      m, "ExternalLibraryManager",
      "Structure in charge of loading external function and\n"
      "retrieving information from shared libraries")
      .def_static("getExternalLibraryManager", ELM::getExternalLibraryManager,
                   pybind11::return_value_policy::reference)
      .def("loadLibrary", ELM_loadLibrary, pybind11::arg("name"))
      .def("getAuthor", &ELM::getAuthor)
      .def("getDate", &ELM::getDate)
      .def("getDescription", &ELM::getDescription)
      .def("getValidator", &ELM::getValidator)
      .def("getBuildId", &ELM::getBuildIdentifier)
      .def("getBuildIdentifier", &ELM::getBuildIdentifier)
      .def("getMaterial", &ELM::getMaterial)
      .def("getSource", &ELM::getSource)
      .def("getTFELVersion", &ELM::getTFELVersion)
      .def("getUnitSystem", &ELM::getUnitSystem)
      .def("getEntryPoints", &ELM::getEntryPoints)
      .def("getMaterialKnowledgeType", &ELM::getMaterialKnowledgeType)
      .def("getInterface", &ELM::getInterface)
      .def("setUShortParameter", ELM_setUShortParameter,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("p"),
           pybind11::arg("v"))
      .def("setIntParameter", ELM_setIntParameter,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("p"),
           pybind11::arg("v"))
      .def("setDoubleParameter", ELM_setDoubleParameter,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("p"),
           pybind11::arg("v"))
      .def("setParameter", ELM_setDoubleParameter,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("p"),
           pybind11::arg("v"))
      .def("setUShortParameter", ELM_setUShortParameter2,
           pybind11::arg("l"), pybind11::arg("n"), pybind11::arg("h"),
           pybind11::arg("p"), pybind11::arg("v"))
      .def("setIntParameter", ELM_setIntParameter2,
           pybind11::arg("l"), pybind11::arg("n"), pybind11::arg("h"),
           pybind11::arg("p"), pybind11::arg("v"))
      .def("setDoubleParameter", ELM_setDoubleParameter2,
           pybind11::arg("l"), pybind11::arg("n"), pybind11::arg("h"),
           pybind11::arg("p"), pybind11::arg("v"))
      .def("setParameter", ELM_setDoubleParameter2,
           pybind11::arg("l"), pybind11::arg("n"), pybind11::arg("h"),
           pybind11::arg("p"), pybind11::arg("v"))
      .def("getRealParameterDefaultValue", &ELM::getRealParameterDefaultValue)
      .def("getIntegerParameterDefaultValue",
           &ELM::getIntegerParameterDefaultValue)
      .def("getUnsignedShortParameterDefaultValue",
           &ELM::getUnsignedShortParameterDefaultValue)
      .def("getUMATDrivingVariablesNames", &ELM::getUMATDrivingVariablesNames)
      .def("getUMATGradientsNames", &ELM::getUMATGradientsNames)
      .def("getUMATParametersNames", &ELM::getUMATParametersNames)
      .def("getUMATParametersTypes", &ELM::getUMATParametersTypes)

      .def("hasBounds", hasBounds1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("n"))
      .def("hasBounds", hasBounds2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("n"))
      .def("hasLowerBound", hasLowerBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("n"))
      .def("hasLowerBound", hasLowerBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("n"))
      .def("hasUpperBound", hasUpperBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("n"))
      .def("hasUpperBound", hasUpperBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("n"))
      .def("getLowerBound", getLowerBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("n"))
      .def("getLowerBound", getLowerBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("n"))
      .def("getUpperBound", getUpperBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("n"))
      .def("getUpperBound", getUpperBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("n"))

      .def("hasPhysicalBounds", hasPhysicalBounds1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("n"))
      .def("hasPhysicalBounds", hasPhysicalBounds2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("n"))
      .def("hasLowerPhysicalBound", hasLowerPhysicalBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("n"))
      .def("hasLowerPhysicalBound", hasLowerPhysicalBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("n"))
      .def("hasUpperPhysicalBound", hasUpperPhysicalBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("n"))
      .def("hasUpperPhysicalBound", hasUpperPhysicalBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("n"))
      .def("getLowerPhysicalBound", getLowerPhysicalBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("n"))
      .def("getLowerPhysicalBound", getLowerPhysicalBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("n"))
      .def("getUpperPhysicalBound", getUpperPhysicalBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("n"))
      .def("getUpperPhysicalBound", getUpperPhysicalBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("n"))

      .def("contains", &ELM::contains,
           pybind11::arg("l"), pybind11::arg("s"))
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
               &ELM::getMaterialPropertyVariables),
           pybind11::arg("l"), pybind11::arg("f"))
      .def("getMaterialPropertyParameters",
           static_cast<std::vector<std::string> (ELM::*)(const std::string&,
                                                           const std::string&)>(
               &ELM::getMaterialPropertyParameters),
           pybind11::arg("l"), pybind11::arg("f"));

}  // end of declareExternalLibraryManager
