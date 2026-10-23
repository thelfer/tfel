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
                             const std::string& name) {
  elm.loadLibrary(name);
}

static void ELM_setUShortParameter(tfel::system::ExternalLibraryManager& elm,
                                    const std::string& l,
                                    const std::string& s,
                                    const std::string& p,
                                    const unsigned short v) {
  elm.setParameter(l, s, p, v);
}

static void ELM_setIntParameter(tfel::system::ExternalLibraryManager& elm,
                                 const std::string& l,
                                 const std::string& s,
                                 const std::string& p,
                                 const int v) {
  elm.setParameter(l, s, p, v);
}

static void ELM_setDoubleParameter(tfel::system::ExternalLibraryManager& elm,
                                    const std::string& l,
                                    const std::string& s,
                                    const std::string& p,
                                    const double v) {
  elm.setParameter(l, s, p, v);
}

static void ELM_setUShortParameter2(tfel::system::ExternalLibraryManager& elm,
                                     const std::string& l,
                                     const std::string& s,
                                     const std::string& h,
                                     const std::string& p,
                                     const unsigned short v) {
  elm.setParameter(l, s, h, p, v);
}

static void ELM_setIntParameter2(tfel::system::ExternalLibraryManager& elm,
                                   const std::string& l,
                                   const std::string& s,
                                   const std::string& h,
                                   const std::string& p,
                                   const int v) {
  elm.setParameter(l, s, h, p, v);
}

static void ELM_setDoubleParameter2(tfel::system::ExternalLibraryManager& elm,
                                     const std::string& l,
                                     const std::string& s,
                                     const std::string& h,
                                     const std::string& p,
                                     const double v) {
  elm.setParameter(l, s, h, p, v);
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
                                   const std::string&) =
      &ELM::hasPhysicalBounds;

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
                   pybind11::return_value_policy::reference,
                   "Return the uniq instance of this class")
      .def("loadLibrary", ELM_loadLibrary, pybind11::arg("name"),
           "A wrapper around the ::dlopen system call\n"
           "name: name of the library\n"
           "On success, the pointer is registred in a map using its\n"
           "name as a key. This name is used in the methods of this\n"
           "call")
      .def("getAuthor", &ELM::getAuthor,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the author associated with the current symbol, if any\n"
           "l: name of the library\n"
           "s: symbol name")
      .def("getDate", &ELM::getDate,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the date associated with the current symbol, if any\n"
           "l: name of the library\n"
           "s: symbol name")
      .def("getDescription", &ELM::getDescription,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the description associated with the current symbol, "
           "if any\n"
           "l: name of the library\n"
           "s: symbol name")
      .def("getValidator", &ELM::getValidator,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the validator associated with the current symbol, "
           "if any.\n"
           "l: name of the library\n"
           "s: symbol name")
      .def("getBuildId", &ELM::getBuildIdentifier,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the build id associated with the current symbol, if any.\n"
           "l: name of the library\n"
           "s: symbol name")
      .def("getBuildIdentifier", &ELM::getBuildIdentifier,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the build id associated with the current symbol, if any.\n"
           "l: name of the library\n"
           "s: symbol name")
      .def("getMaterial", &ELM::getMaterial,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the material associated with the entry point\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "This function looks for the symbol s+'_mfront_material' in "
           "the library\n"
           "and expect it to a pointer to characters.\n"
           "If the symbol is not found, an empty string is returned.")
      .def("getSource", &ELM::getSource,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the src of the function or the mechanical behaviour\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "This function looks for the symbol s+'_src' in the library\n"
           "and expect it to a pointer to characters.\n"
           "If the symbol is not found, an empty string is returned.")
      .def("getTFELVersion", &ELM::getTFELVersion,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the TFEL version used to generate the given entry\n"
           "point.\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "This function looks for the symbol s+'_tfel_version' in the\n"
           "library and expect it to a pointer to characters.\n"
           "If the symbol is not found, an empty string is returned.")
      .def("getUnitSystem", &ELM::getUnitSystem,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the unit system used by the given entry point.\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "This function looks for the symbol s+'_unit_system' in the\n"
           "library and expect it to a pointer to characters.\n"
           "If the symbol is not found, an empty string is returned.")
      .def("getEntryPoints", &ELM::getEntryPoints,
           pybind11::arg("l"),
           "Return the list of all mfront generated entry points\n"
           "in a library.\n"
           "l: library name")
      .def("getMaterialKnowledgeType", &ELM::getMaterialKnowledgeType,
           pybind11::arg("l"), pybind11::arg("f"),
           "Return the material knowledge type of an entry point in a\n"
           "library. The returned value has the following meaning:\n"
           "- 0: material property\n"
           "- 1: behaviour\n"
           "- 2: model\n"
           "l: library name")
      .def("getInterface", &ELM::getInterface,
           pybind11::arg("l"), pybind11::arg("s"),
           "Return the interface of the function or the mechanical behaviour\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "This function looks for the symbol s+'_mfront_interface' in "
           "the library\n"
           "and expect it to a pointer to characters.\n"
           "If the symbol is not found, an empty string is returned.")
      .def("setUShortParameter", ELM_setUShortParameter,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("p"),
           pybind11::arg("v"),
           "Set the value of an unsigned short parameter\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "p: parameter name\n"
           "v: value")
      .def("setIntParameter", ELM_setIntParameter,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("p"),
           pybind11::arg("v"),
           "Set the value of an integer parameter\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "p: parameter name\n"
           "v: value")
      .def("setDoubleParameter", ELM_setDoubleParameter,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("p"),
           pybind11::arg("v"),
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "p: parameter name\n"
           "v: value")
      .def("setParameter", ELM_setDoubleParameter,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("p"),
           pybind11::arg("v"),
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "p: parameter name\n"
           "v: value")
      .def("setUShortParameter", ELM_setUShortParameter2,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("h"),
           pybind11::arg("p"), pybind11::arg("v"),
           "Set the value of an unsigned short parameter\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "p: parameter name\n"
           "v: value")
      .def("setIntParameter", ELM_setIntParameter2,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("h"),
           pybind11::arg("p"), pybind11::arg("v"),
           "Set the value of an integer parameter\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "p: parameter name\n"
           "v: value")
      .def("setDoubleParameter", ELM_setDoubleParameter2,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("h"),
           pybind11::arg("p"), pybind11::arg("v"),
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "p: parameter name\n"
           "v: value")
      .def("setParameter", ELM_setDoubleParameter2,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("h"),
           pybind11::arg("p"), pybind11::arg("v"),
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "p: parameter name\n"
           "v: value")
      .def("getRealParameterDefaultValue", &ELM::getRealParameterDefaultValue,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("h"),
           pybind11::arg("p"),
           "Get the default value of a double parameter\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "p: parameter name")
      .def("getIntegerParameterDefaultValue",
           &ELM::getIntegerParameterDefaultValue,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("h"),
           pybind11::arg("p"),
           "Get the default value of an integer parameter\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "p: parameter name")
      .def("getUnsignedShortParameterDefaultValue",
           &ELM::getUnsignedShortParameterDefaultValue,
           pybind11::arg("l"), pybind11::arg("s"), pybind11::arg("h"),
           pybind11::arg("p"),
           "Get the default value of an unsigned short parameter\n"
           "l: name of the library\n"
           "s: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "p: parameter name")
      .def("getUMATDrivingVariablesNames", &ELM::getUMATDrivingVariablesNames,
           pybind11::arg("l"), pybind11::arg("f"),
           "l: name of the library\n"
           "f: law name")
      .def("getUMATGradientsNames", &ELM::getUMATGradientsNames,
           pybind11::arg("l"), pybind11::arg("f"),
           "l: name of the library\n"
           "f: law name")
      .def("getUMATParametersNames", &ELM::getUMATParametersNames,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           "l: name of the library\n"
           "f: law name\n"
           "h: modelling hypothesis")
      .def("getUMATParametersTypes", &ELM::getUMATParametersTypes,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           "Return the types associated with each parameter. The integer\n"
           "values returned have the following meaning:\n"
           "- 0: floatting point value\n"
           "- 1: integer value\n"
           "- 2: unsigned short value\n"
           "l: name of the library\n"
           "f: law name\n"
           "h: modelling hypothesis")

      .def("hasBounds", hasBounds1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("v"),
           "Return true if the given variable has bounds\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "v: variable name")
      .def("hasBounds", hasBounds2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("v"),
           "Return true if the given variable (material property) has bounds\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "v: variable name")
      .def("hasLowerBound", hasLowerBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("v"),
           "Return true if the given variable has a lower bound\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "v: variable name")
      .def("hasLowerBound", hasLowerBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("v"),
           "Return true if the given variable (material property) has "
           "lower bound\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "v: variable name")
      .def("hasUpperBound", hasUpperBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("v"),
           "Return true if the given variable has a upper bound\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "v: variable name")
      .def("hasUpperBound", hasUpperBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("v"),
           "Return true if the given variable (material property) has a "
           "upper\nbound\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "v: variable name")
      .def("getLowerBound", getLowerBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("v"),
           "Return the lower bound of the given variable\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "v: variable name")
      .def("getLowerBound", getLowerBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("v"),
           "Return the lower bound of the given variable (material property)\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "v: variable name")
      .def("getUpperBound", getUpperBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("v"),
           "Return the upper bound of the given variable\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "v: variable name")
      .def("getUpperBound", getUpperBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("v"),
           "Return the upper bound of the given variable (material property)\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "v: variable name")

      .def("hasPhysicalBounds", hasPhysicalBounds1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("v"),
           "Return true if the given variable has bounds\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "v: variable name")
      .def("hasPhysicalBounds", hasPhysicalBounds2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("v"),
           "Return true if the given variable (material property) has "
           "physical\nbounds\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "v: variable name")
      .def("hasLowerPhysicalBound", hasLowerPhysicalBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("v"),
           "Return true if the given variable has a lower physical bound\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "v: variable name")
      .def("hasLowerPhysicalBound", hasLowerPhysicalBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("v"),
           "Return true if the given variable (material property) has a "
           "lower\nphysical bound\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "v: variable name")
      .def("hasUpperPhysicalBound", hasUpperPhysicalBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("v"),
           "Return true if the given variable has a upper physical bound\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "v: variable name")
      .def("hasUpperPhysicalBound", hasUpperPhysicalBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("v"),
           "Return true if the given variable (material property) has a "
           "upper\nphysical bound\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "v: variable name")
      .def("getLowerPhysicalBound", getLowerPhysicalBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("v"),
           "Return the lower bound of the given variable\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "v: variable name")
      .def("getLowerPhysicalBound", getLowerPhysicalBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("v"),
           "Return the lower bound of the given variable (material property)\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "v: variable name")
      .def("getUpperPhysicalBound", getUpperPhysicalBound1,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("v"),
           "Return the upper bound of the given variable\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "h: modelling hypothesis\n"
           "v: variable name")
      .def("getUpperPhysicalBound", getUpperPhysicalBound2,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("v"),
           "Return the upper bound of the given variable (material property)\n"
           "l: name of the library\n"
           "f: name of function or mechanical behaviour\n"
           "v: variable name")

      .def("contains", &ELM::contains,
           pybind11::arg("l"), pybind11::arg("s"),
           "l: name of the library\n"
           "s: symbol name")
      .def("getUMATBehaviourType", &ELM::getUMATBehaviourType,
           pybind11::arg("l"), pybind11::arg("f"),
           "Return the type of the behaviour\n"
           "The value returned has the following meaning:\n"
           "- 0: general behaviour\n"
           "- 1: strain based behaviour\n"
           "- 2: standard finite strain behaviour\n"
           "- 3: cohesive zone model\n"
           "l: name of the library\n"
           "f: law name")
      .def("getUMATSymmetryType", &ELM::getUMATSymmetryType,
           pybind11::arg("l"), pybind11::arg("f"),
           "Return the symmetry of the behaviour (isotropic or\n"
           "orthotropic) If the returned value is 0, the behaviour is\n"
           "isotropic. If the returned value is 1, the behaviour is\n"
           "orthotropic.\n"
           "l: name of the library\n"
           "f: law name")
      .def("getUMATElasticSymmetryType", &ELM::getUMATElasticSymmetryType,
           pybind11::arg("l"), pybind11::arg("f"),
           "Return the symmetry of the elastic behaviour (isotropic or\n"
           "orthotropic) If the returned value is 0, the behaviour is\n"
           "isotropic. If the returned value is 1, the behaviour is\n"
           "orthotropic.\n"
           "l: name of the library\n"
           "f: law name")
      .def("getUMATMaterialPropertiesNames",
           &ELM::getUMATMaterialPropertiesNames,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           "l: name of the library\n"
           "f: law name\n"
           "h: modelling hypothesis")
      .def("getUMATInternalStateVariablesNames",
           &ELM::getUMATInternalStateVariablesNames,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           "l: name of the library\n"
           "f: law name\n"
           "h: modelling hypothesis")
      .def("getUMATInternalStateVariablesTypes",
           &ELM::getUMATInternalStateVariablesTypes,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           "A type is associated to an integer as follows:\n"
           "- 0: scalar\n"
           "- 1: symmetric tensor\n"
           "- 2: vector (size of the space dimension)\n"
           "- 3: unsymmetric tensor\n"
           "l: name of the library\n"
           "f: law name\n"
           "h: modelling hypothesis")
      .def("getUMATExternalStateVariablesNames",
           &ELM::getUMATExternalStateVariablesNames,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           "Return the list of the external state variables.\n"
           "By convention, the temperature is generally not included in "
           "this\nlist when it is declared as the first external state "
           "variable, as it is\ntreated specifically by the `UMAT` "
           "interface. The user may thus need to\ncall the "
           "`hasTemperatureBeenRemovedFromExternalStateVariables` method "
           "to\nsee if the temperature was indeed removed from the list "
           "of external state\nvariables.\n"
           "l: name of the library\n"
           "f: law name\n"
           "h: modelling hypothesis")
      .def("getUMATExternalStateVariablesTypes",
           &ELM::getUMATExternalStateVariablesTypes,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           "A type is associated to an integer as follows:\n"
           "- 0: scalar\n"
           "- 1: symmetric tensor\n"
           "- 2: vector (size of the space dimension)\n"
           "- 3: unsymmetric tensor\n"
           "l: name of the library\n"
           "f: law name\n"
           "h: modelling hypothesis")
      .def("getUMATRequiresStiffnessTensor",
           &ELM::getUMATRequiresStiffnessTensor,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           "Return true if a behaviour generated throught the aster\n"
           "interface requires a offset for the elastic properties\n"
           "l: name of the library\n"
           "f: law name\n"
           "h: modelling hypothesis")
      .def("getUMATRequiresThermalExpansionCoefficientTensor",
           &ELM::getUMATRequiresThermalExpansionCoefficientTensor,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           "Return true if a behaviour generated throught the aster\n"
           "interface requires a offset for the elastic properties\n"
           "l: name of the library\n"
           "f: law name\n"
           "h: modelling hypothesis")
      .def("checkIfAsterBehaviourSaveTangentOperator",
           &ELM::checkIfAsterBehaviourSavesTangentOperator,
           pybind11::arg("l"), pybind11::arg("f"),
           "Return true if a behaviour generated throught the aster\n"
           "interface saves the tangent operator\n"
           "l: name of the library\n"
           "f: law name")
      .def("getGenericBehaviourInitializeFunctions",
           &ELM::getGenericBehaviourInitializeFunctions,
           pybind11::arg("l"), pybind11::arg("b"), pybind11::arg("h"),
           "Return the post-processings associated with a behaviour generated\n"
           "through the `generic` interface.\n"
           "l: name of the library\n"
           "b: behaviour name\n"
           "h: modelling hypothesis")
      .def("getGenericBehaviourInitializeFunctionInputs",
           &ELM::getGenericBehaviourInitializeFunctionInputs,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("p"),
           "Return the inputs of a initialize function function\n"
           "l: name of the library\n"
           "f: function name\n"
           "h: modelling hypothesis\n"
           "p: initialize function function name")
      .def("getGenericBehaviourInitializeFunctionInputsTypes",
           &ELM::getGenericBehaviourInitializeFunctionInputsTypes,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("p"),
           "Return the types of the inputs of a initialize function function\n"
           "l: name of the library\n"
           "f: function name\n"
           "h: modelling hypothesis\n"
           "p: initialize function function name")
      .def("getGenericBehaviourPostProcessingFunctions",
           &ELM::getGenericBehaviourPostProcessingFunctions,
           pybind11::arg("l"), pybind11::arg("b"), pybind11::arg("h"),
           "Return the post-processings associated with a behaviour generated\n"
           "through the `generic` interface.\n"
           "l: name of the library\n"
           "b: behaviour name\n"
           "h: modelling hypothesis")
      .def("getGenericBehaviourPostProcessingFunctionOutputs",
           &ELM::getGenericBehaviourPostProcessingFunctionOutputs,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("p"),
           "Return the outputs of a post-processing function\n"
           "l: name of the library\n"
           "f: function name\n"
           "h: modelling hypothesis\n"
           "p: post-processing function name")
      .def("getGenericBehaviourPostProcessingFunctionOutputsTypes",
           &ELM::getGenericBehaviourPostProcessingFunctionOutputsTypes,
           pybind11::arg("l"), pybind11::arg("f"), pybind11::arg("h"),
           pybind11::arg("p"),
           "Return the types of the outputs of a post-processing function\n"
           "l: name of the library\n"
           "f: function name\n"
           "h: modelling hypothesis\n"
           "p: post-processing function name")
      .def("getMaterialPropertyVariables",
            static_cast<std::vector<std::string> (ELM::*)(const std::string&,
                                                            const std::string&)>(
                &ELM::getMaterialPropertyVariables),
            pybind11::arg("l"), pybind11::arg("f"),
            "Return the external name associated with the inputs of a material\n"
            "property.\n"
            "l: name of the library\n"
            "f: function name")
      .def("getMaterialPropertyParameters",
            static_cast<std::vector<std::string> (ELM::*)(const std::string&,
                                                            const std::string&)>(
                &ELM::getMaterialPropertyParameters),
            pybind11::arg("l"), pybind11::arg("f"),
            "l: name of the library\n"
            "f: function name");

}  // end of declareExternalLibraryManager
