/*!
 * \file   SingleStructureScheme.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <tuple>
#include <boost/python.hpp>
#include "TFEL/Raise.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/FunctionEvolution.hxx"
#include "MTest/CastemEvolution.hxx"
#include "MTest/SingleStructureScheme.hxx"

static void SingleStructureScheme_addEvolution(mtest::SingleStructureScheme& t,
                                               const std::string& n,
                                               const mtest::real& v,
                                               const bool b1,
                                               const bool b2) {
  t.addEvolution(n, mtest::make_evolution(v), b1, b2);
}

static void SingleStructureScheme_addEvolution2(
    mtest::SingleStructureScheme& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v,
    const bool b1,
    const bool b2) {
  t.addEvolution(n, mtest::make_evolution(v), b1, b2);
}

static void SingleStructureScheme_addEvolution3(mtest::SingleStructureScheme& t,
                                                const std::string& n,
                                                const std::string& f,
                                                const bool b1,
                                                const bool b2) {
  auto pev = std::make_shared<mtest::FunctionEvolution>(f, t.getEvolutions());
  t.addEvolution(n, pev, b1, b2);
}

static void SingleStructureScheme_setMaterialProperty(
    mtest::SingleStructureScheme& t,
    const std::string& n,
    const mtest::real& v,
    const bool b) {
  auto pev = mtest::make_evolution(v);
  t.setMaterialProperty(n, pev, b);
}

static void SingleStructureScheme_setMaterialProperty2(
    mtest::SingleStructureScheme& t,
    const std::string& n,
    const std::string& f,
    const bool b) {
  auto mpev = std::make_shared<mtest::FunctionEvolution>(f, t.getEvolutions());
  t.setMaterialProperty(n, mpev, b);
}

static void SingleStructureScheme_setMaterialProperty3(
    mtest::SingleStructureScheme& t,
    const std::string& n,
    const std::string& f,
    const bool b) {
  auto pev = std::make_shared<mtest::FunctionEvolution>(f, t.getEvolutions());
  t.setMaterialProperty(n, pev, b);
}

static void SingleStructureScheme_setCastemMaterialProperty(
    mtest::SingleStructureScheme& t,
    const std::string& n,
    const std::string& l,
    const std::string& f,
    const bool b) {
  auto mpev = std::make_shared<mtest::CastemEvolution>(l, f, t.getEvolutions());
  t.setMaterialProperty(n, mpev, b);
}

static void SingleStructureScheme_setExternalStateVariable(
    mtest::SingleStructureScheme& t,
    const std::string& n,
    const mtest::real& v,
    const bool b) {
  auto pev = mtest::make_evolution(v);
  t.setExternalStateVariable(n, pev, b);
}

static void SingleStructureScheme_setExternalStateVariable2(
    mtest::SingleStructureScheme& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& values,
    const bool b) {
  auto pev = mtest::make_evolution(values);
  t.setExternalStateVariable(n, pev, b);
}

static void SingleStructureScheme_setExternalStateVariable3(
    mtest::SingleStructureScheme& t,
    const std::string& n,
    const std::string& f,
    const bool b) {
  auto pev = std::make_shared<mtest::FunctionEvolution>(f, t.getEvolutions());
  t.setExternalStateVariable(n, pev, b);
}

static void SingleStructureScheme_setInternalStateVariableInitialValue1(
    mtest::SingleStructureScheme& s,
    const std::string& n,
    const mtest::real v) {
  s.setScalarInternalStateVariableInitialValue(n, v);
}

static void SingleStructureScheme_setInternalStateVariableInitialValue2(
    mtest::SingleStructureScheme& s,
    const std::string& n,
    const std::vector<mtest::real>& v) {
  const auto& b = *(s.getBehaviour());
  const auto type = b.getInternalStateVariableType(n);
  if (type == 0) {
    tfel::raise_if(v.size() != 1u,
                   "SingleStructureScheme::"
                   "setInternalStateVariableInitialValue: "
                   "unsupported number of values for scalar internal "
                   "state variable '" +
                       n + "'");
    s.setScalarInternalStateVariableInitialValue(n, v[0]);
  } else if (type == 1) {
    s.setStensorInternalStateVariableInitialValues(n, v);
  } else if (type == 3) {
    s.setTensorInternalStateVariableInitialValues(n, v);
  } else {
    tfel::raise(
        "SingleStructureScheme::setInternalStateVariableInitialValue: "
        "unsupported type for internal state variable '" +
        n + "'");
  }
}

static void SingleStructureScheme_setBehaviour(mtest::SingleStructureScheme& s,
                                               const std::string& i,
                                               const std::string& l,
                                               const std::string& f) {
  s.setBehaviour(i, l, f);
}

static void SingleStructureScheme_setBehaviour2(mtest::SingleStructureScheme& s,
                                                const std::string& w,
                                                const std::string& i,
                                                const std::string& l,
                                                const std::string& f) {
  s.setBehaviour(w, i, l, f);
}

static int SingleStructureScheme_getBehaviourType(
    const mtest::SingleStructureScheme& s) {
  using tfel::material::MechanicalBehaviourBase;
  const auto bt = s.getBehaviourType();
  if (bt == MechanicalBehaviourBase::GENERALBEHAVIOUR) {
    return 0;
  } else if (bt == MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) {
    return 1;
  } else if (bt == MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) {
    return 2;
  }
  if (bt != MechanicalBehaviourBase::COHESIVEZONEMODEL) {
    tfel::raise(
        "SingleStructureScheme_getBehaviourType: unsupported behaviour type");
  }
  return 3;
}  // end of SingleStructureScheme_getBehaviourType

static int SingleStructureScheme_getBehaviourKinematic(
    const mtest::SingleStructureScheme& s) {
  using tfel::material::MechanicalBehaviourBase;
  const auto bk = s.getBehaviourKinematic();
  if (bk == MechanicalBehaviourBase::UNDEFINEDKINEMATIC) {
    return 0;
  } else if (bk == MechanicalBehaviourBase::SMALLSTRAINKINEMATIC) {
    return 1;
  } else if (bk == MechanicalBehaviourBase::COHESIVEZONEKINEMATIC) {
    return 2;
  } else if (bk == MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY) {
    return 3;
  }
  if (bk != MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1) {
    tfel::raise(
        "SingleStructureScheme_getBehaviourKinematic: unsupported behaviour "
        "kinematic");
  }
  return 4;
}  // end of SingleStructureScheme_getBehaviourKinematic

void declareSingleStructureScheme();

void declareSingleStructureScheme() {
  using namespace mtest;
  using Parameters = SingleStructureScheme::Parameters;
  void (SingleStructureScheme::*ptr1)(const std::string&, const std::string&,
                                      const std::string&, const Parameters&) =
      &SingleStructureScheme::setBehaviour;
  void (SingleStructureScheme::*ptr2)(const std::string&, const std::string&,
                                      const std::string&, const std::string&,
                                      const Parameters&) =
      &SingleStructureScheme::setBehaviour;
  boost::python::class_<SingleStructureScheme, boost::noncopyable,
                        boost::python::bases<SchemeBase>>(
      "SingleStructureScheme", boost::python::no_init)
      .def("getBehaviour",
           static_cast<std::shared_ptr<Behaviour> (SingleStructureScheme::*)()>(
               &SingleStructureScheme::getBehaviour))
      .def("getBehaviour", static_cast<std::shared_ptr<const Behaviour> (
                               SingleStructureScheme::*)() const>(
                               &SingleStructureScheme::getBehaviour))
      .def("getBehaviourType", &SingleStructureScheme_getBehaviourType)
      .def("getBehaviourKinematic",
           &SingleStructureScheme_getBehaviourKinematic)
      .def("setBehaviour", ptr1)
      .def("setBehaviour", ptr2)
      .def("setBehaviour", SingleStructureScheme_setBehaviour,
           "This method declares the behaviour used for the "
           "test.\n"
           "* The first argument (string) specify a wrapper "
           "aroung the behaviour.\n"
           "* The second argument (string) specify the interface "
           "used by the behaviour. Supported interfaces are:\n"
           "- 'umat'\n"
           "- 'cyrano'\n"
           "- 'aster'\n"
           "* The second argument (string) is the path to the "
           "dynamic library which implement the selected behaviour.\n"
           "* The third argument (string) is the name of the function.")
      .def("setBehaviour", SingleStructureScheme_setBehaviour2,
           "This method declares the behaviour used for the "
           "test.\n"
           "* The first argument (string) specify the interface "
           "used by the behaviour. Supported interfaces are:\n"
           "- 'umat'\n"
           "- 'cyrano'\n"
           "- 'aster'\n"
           "- 'abaqus'\n"
           "* The second argument (string) is the path to the "
           "dynamic library which implement the selected behaviour.\n"
           "* The third argument (string) is the name of the function.")
      .def("setModel", ptr1)
      .def("setModel", ptr2)
      .def("setModel", SingleStructureScheme_setBehaviour,
           "see the documentation of the setBehaviour method")
      .def("setModel", SingleStructureScheme_setBehaviour2,
           "see the documentation of the setBehaviour method")
      .def("handleThermalExpansion",
           &SingleStructureScheme::setHandleThermalExpansion,
           "This method override the (de)activation of the thermal "
           "strain computation. It's activated by default if a "
           "material property named 'ThermalExpansion' is defined "
           "(isotropic materials).\n"
           "* If the argument (bool) is 'true', the computation is "
           "activated. Otherwise, the computation is deactivated.")
      .def("setParameter", &SingleStructureScheme::setParameter,
           "This method specifies the value of a parameter's "
           "behaviour.\n"
           "* The first argument (string) is the name of the "
           "parameter's behaviour.\n"
           "* The second argument (double) is the value of that "
           "parameter's behaviour.")
      .def("setIntegerParameter", &SingleStructureScheme::setIntegerParameter,
           "This keyword specifies the value of an "
           "integer parameter of the behaviour.\n"
           "* The first argument (string) is the name of the "
           "parameter's behaviour.\n"
           "* The second argument (int) is the value of that "
           "parameter's behaviour.")
      .def("setUnsignedIntegerParameter",
           &SingleStructureScheme::setUnsignedIntegerParameter,
           "This method specifies the value of an "
           "unsigned integer parameter of the behaviour.\n"
           "* The first argument (string) is the name of the "
           "parameter's behaviour.\n"
           "* The second argument (uint) is the value of that "
           "parameter's behaviour.")
      .def("setOutOfBoundsPolicy", &SingleStructureScheme::setOutOfBoundsPolicy,
           "This method let the user to precise the out "
           "of bounds policy.\n"
           "* The argument (OutOfBoundsPolicy) is the selected "
           "policy. Three policies are available:\n"
           "- 'None' : nothing is done when a variable is out of "
           "bounds.\n"
           "- 'Warning' : a message is selected when a variable "
           "is out of bounds.\n"
           "- 'Strict' : the computation fails when a variable "
           "is out of bounds. Most mechanical behaviours "
           "implementations throw an exception which is caught "
           "by 'MTest'.")
      .def("setMaterialProperty", SingleStructureScheme_setMaterialProperty,
           (boost::python::arg("name"), "value",
            boost::python::arg("check") = true),
           "This method defines a constant material property.\n"
           "* The first argument (string) is the selected material "
           "property. Only the mechanical properties defined by the "
           "behaviour for the modelling hypothesis considered may be "
           "used. (Note : isotropic thermal expansion is defined "
           "through the 'ThermalExpansion' property and the "
           "orthotropic thermal expansion is defined through the "
           "'ThermalExpansion1', 'ThermalExpansion2' and "
           "'ThermalExpansion3' material properties).\n"
           "* The second argument (double) is the constant value of "
           "the selected material property.")
      .def("setMaterialProperty", SingleStructureScheme_setMaterialProperty2,
           (boost::python::arg("name"), "value",
            boost::python::arg("check") = true),
           "This method defines a function material property.\n"
           "* The first argument (string) is the selected material "
           "property. Only the mechanical properties defined by the "
           "behaviour for the modelling hypothesis considered may be "
           "used. (Note : isotropic thermal expansion is defined "
           "through the 'ThermalExpansion' property and the "
           "orthotropic thermal expansion is defined through the "
           "'ThermalExpansion1', 'ThermalExpansion2' and "
           "'ThermalExpansion3' material properties).\n"
           "* The second argument (string) is the function used "
           "to represent the behaviour of the selected material "
           "property.")
      .def("setMaterialProperty", SingleStructureScheme_setMaterialProperty3,
           (boost::python::arg("name"), "value", "function",
            boost::python::arg("check") = true),
           "This method defines a function material property.\n"
           "* The first argument (string) is the selected material "
           "property. Only the mechanical properties defined by the "
           "behaviour for the modelling hypothesis considered may "
           "be used. (Note : isotropic thermal expansion is defined "
           "through the 'ThermalExpansion' property and the "
           "orthotropic thermal expansion is defined through the "
           "'ThermalExpansion1', 'ThermalExpansion2' and "
           "'ThermalExpansion3' material properties).\n"
           "* The second argument (string) is the function used "
           "to represent the behaviour of the selected material "
           "property.")
      .def("setCastemMaterialProperty",
           SingleStructureScheme_setCastemMaterialProperty,
           (boost::python::arg("name"), boost::python::arg("library"),
            boost::python::arg("function"), boost::python::arg("check") = true),
           "This method defines a castem material property.\n"
           "* The first argument (string) is the selected material "
           "property. Only the mechanical properties defined by the "
           "behaviour for the modelling hypothesis considered may be "
           "used. (Note : isotropic thermal expansion is defined "
           "through the 'ThermalExpansion' property and the "
           "orthotropic thermal expansion is defined through the "
           "'ThermalExpansion1', 'ThermalExpansion2' and "
           "'ThermalExpansion3' material properties).\n"
           "* The second argument (string) is the path to the "
           "dynamic library which implement the behaviour of the "
           "selected material property.\n"
           "* The third argument (string) is the name of the "
           "function.")
      .def("setExternalStateVariable",
           SingleStructureScheme_setExternalStateVariable,
           (boost::python::arg("name"), "value",
            boost::python::arg("check") = true),
           "This method specify the constant evolution of an "
           "external state variable.\n"
           "* The first argument (string) is the name of the "
           "external state variable (generally the temperature "
           "which is by default defined in behaviour interfaces).\n"
           "* The second argument (double) is the constant value "
           "of the selected external state variable.")
      .def("setExternalStateVariable",
           SingleStructureScheme_setExternalStateVariable2,
           (boost::python::arg("name"), "value",
            boost::python::arg("check") = true),
           "This method specify the linear evolution of an "
           "external state variable.\n"
           "* The first argument (string) is the name of the "
           "external state variable (generally the temperature "
           "which is by default defined in behaviour interfaces).\n"
           "* The second argument (map<double, double>) specify "
           "a linear evolution: the associative array of time-value "
           "is used to perform an interpolation. That interpolation "
           "is only performed between the minimum and maximum times "
           "given in the array. Should a time be out of the array, "
           "the returned value will be the one from the nearest "
           "association available.")
      .def("setExternalStateVariable",
           SingleStructureScheme_setExternalStateVariable3,
           (boost::python::arg("name"), "value", "function",
            boost::python::arg("check") = true),
           "This method specify the complex evolution of an "
           "external state variable.\n"
           "* The first argument (string) is the name of the "
           "external state variable (generally the temperature "
           "which is by default defined in behaviour interfaces).\n"
           "* The second argument (string) is the function used "
           "to represent the evolution of the selected external "
           "state variable. The string will be interpreted as a "
           "function of time (represented by the variable 't' in "
           "the string).")
      .def("addReal", SingleStructureScheme_addEvolution,
           (boost::python::arg("name"), "value",
            boost::python::arg("b1") = true, boost::python::arg("b2") = true))
      .def("addEvolution", SingleStructureScheme_addEvolution,
           (boost::python::arg("name"), "value",
            boost::python::arg("b1") = true, boost::python::arg("b2") = true))
      .def("addEvolution", SingleStructureScheme_addEvolution2,
           (boost::python::arg("name"), "value",
            boost::python::arg("b1") = true, boost::python::arg("b2") = true))
      .def("addEvolution", SingleStructureScheme_addEvolution3,
           (boost::python::arg("name"), "value", "function",
            boost::python::arg("b1") = true, boost::python::arg("b2") = true))
      .def("setInternalStateVariableInitialValue",
           SingleStructureScheme_setInternalStateVariableInitialValue1,
           "This method allow to specify the initial  value of an "
           "internal state variable.")
      .def("setInternalStateVariableInitialValue",
           SingleStructureScheme_setInternalStateVariableInitialValue2,
           "This method allow to specify the initial  value of an "
           "internal state variable.")
      .def("setStateVariableInitialValue",
           SingleStructureScheme_setInternalStateVariableInitialValue1,
           "see the doumentation of the "
           "setInternalStateVariableInitialValue method")
      .def("setStateVariableInitialValue",
           SingleStructureScheme_setInternalStateVariableInitialValue2,
           "see the doumentation of the "
           "setInternalStateVariableInitialValue method");
}
