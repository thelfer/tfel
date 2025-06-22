/*!
 * \file   Behaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Python/SharedPtr.hxx"
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MTest/Behaviour.hxx"

static std::shared_ptr<mtest::Behaviour> getBehaviour1(
    const std::string& i,
    const std::string& l,
    const std::string& f,
    const mtest::Behaviour::Parameters& p,
    const std::string& h) {
  using mtest::Behaviour;
  return Behaviour::getBehaviour(i, l, f, p,
                                 Behaviour::ModellingHypothesis::fromString(h));
}  // end of std::shared_ptr<Behaviour> getBehaviour1

static std::shared_ptr<mtest::Behaviour> getBehaviour2(
    const std::string& i,
    const std::string& l,
    const std::string& f,
    const mtest::Behaviour::ModellingHypothesis::Hypothesis h) {
  using mtest::Behaviour;
  return Behaviour::getBehaviour(i, l, f, Behaviour::Parameters(), h);
}  // end of std::shared_ptr<Behaviour> getBehaviour1

static std::shared_ptr<mtest::Behaviour> getBehaviour3(const std::string& i,
                                                       const std::string& l,
                                                       const std::string& f,
                                                       const std::string& h) {
  using mtest::Behaviour;
  return Behaviour::getBehaviour(i, l, f, Behaviour::Parameters(),
                                 Behaviour::ModellingHypothesis::fromString(h));
}  // end of std::shared_ptr<Behaviour> getBehaviour1

static std::shared_ptr<mtest::Behaviour> getBehaviour4(
    const std::string& l,
    const std::string& f,
    const mtest::Behaviour::Parameters& p,
    const std::string& h) {
  using mtest::Behaviour;
  return Behaviour::getBehaviour("", l, f, p,
                                 Behaviour::ModellingHypothesis::fromString(h));
}  // end of std::shared_ptr<Behaviour> getBehaviour1

static std::shared_ptr<mtest::Behaviour> getBehaviour5(
    const std::string& l,
    const std::string& f,
    const mtest::Behaviour::ModellingHypothesis::Hypothesis h) {
  using mtest::Behaviour;
  return Behaviour::getBehaviour("", l, f, Behaviour::Parameters(), h);
}  // end of std::shared_ptr<Behaviour> getBehaviour1

static std::shared_ptr<mtest::Behaviour> getBehaviour6(const std::string& l,
                                                       const std::string& f,
                                                       const std::string& h) {
  using mtest::Behaviour;
  return Behaviour::getBehaviour("", l, f, Behaviour::Parameters(),
                                 Behaviour::ModellingHypothesis::fromString(h));
}  // end of std::shared_ptr<Behaviour> getBehaviour1

static int Behaviour_getBehaviourType(const mtest::Behaviour& b) {
  using tfel::material::MechanicalBehaviourBase;
  const auto bt = b.getBehaviourType();
  if (bt == MechanicalBehaviourBase::GENERALBEHAVIOUR) {
    return 0;
  } else if (bt == MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) {
    return 1;
  } else if (bt == MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) {
    return 2;
  }
  if (bt != MechanicalBehaviourBase::COHESIVEZONEMODEL) {
    tfel::raise("Behaviour_getBehaviourType: unsupported behaviour type");
  }
  return 3;
}  // end of Behaviour_getBehaviourType

static int Behaviour_getBehaviourKinematic(const mtest::Behaviour& b) {
  using tfel::material::MechanicalBehaviourBase;
  const auto bk = b.getBehaviourKinematic();
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
        "Behaviour_getBehaviourKinematic: unsupported behaviour kinematic");
  }
  return 4;
}  // end of Behaviour_getBehaviourKinematic

void declareBehaviour() {
  using boost::python::class_;
  using mtest::Behaviour;
  class_<std::shared_ptr<Behaviour>>("Behaviour")
      .def("__init__", boost::python::make_constructor(Behaviour::getBehaviour),
           "This constructor has the following arguments:\n"
           "- i(std::string): interface\n"
           "- l(std::string): library\n"
           "- f(std::string): function\n"
           "- d(tfel::utilities::Data): parameter\n"
           "- h(tfel::material::ModellingHypothesis): modelling hypothesis\n")
      .def("__init__", boost::python::make_constructor(getBehaviour1),
           "This constructor has the following arguments:\n"
           "- i(std::string): interface\n"
           "- l(std::string): library\n"
           "- f(std::string): function\n"
           "- d(tfel::utilities::Data): parameter\n"
           "- h(std::string): modelling hypothesis\n")
      .def("__init__", boost::python::make_constructor(getBehaviour2),
           "This constructor has the following arguments:\n"
           "- i(std::string): interface\n"
           "- l(std::string): library\n"
           "- f(std::string): function\n"
           "- h(tfel::material::ModellingHypothesis): modelling hypothesis\n")
      .def("__init__", boost::python::make_constructor(getBehaviour3),
           "This constructor has the following arguments:\n"
           "- i(std::string): interface\n"
           "- l(std::string): library\n"
           "- f(std::string): function\n"
           "- d(tfel::utilities::Data): parameter\n"
           "- h(std::string): modelling hypothesis\n")
      .def("__init__", boost::python::make_constructor(getBehaviour4),
           "This constructor has the following arguments:\n"
           "- l(std::string): library\n"
           "- f(std::string): function\n"
           "- d(tfel::utilities::Data): parameter\n"
           "- h(std::string): modelling hypothesis\n")
      .def("__init__", boost::python::make_constructor(getBehaviour5),
           "This constructor has the following arguments:\n"
           "- l(std::string): library\n"
           "- f(std::string): function\n"
           "- h(tfel::material::ModellingHypothesis): modelling hypothesis\n")
      .def("__init__", boost::python::make_constructor(getBehaviour6),
           "This constructor has the following arguments:\n"
           "- l(std::string): library\n"
           "- f(std::string): function\n"
           "- d(tfel::utilities::Data): parameter\n"
           "- h(std::string): modelling hypothesis\n")
      .def("getBehaviourType", &Behaviour_getBehaviourType,
           "Return the behaviour type.\n"
           "The value returned has the following meaning:\n"
           "- 0: general behaviour\n"
           "- 1: small strain behaviour\n"
           "- 2: finite strain behaviour\n"
           "- 3: cohesive zone model\n")
      .def("getBehaviourKinematic", &Behaviour_getBehaviourKinematic,
           "Return the behaviour kinematic.\n"
           "The value returned has the following meaning:\n"
           "- 0: undefined kinematic\n"
           "- 1: standard small strain behaviour kinematic\n"
           "- 2: cohesive zone model kinematic\n"
           "- 3: standard finite strain kinematic (F-Cauchy)\n"
           "- 4: ptest finite strain kinematic (eto-pk1)\n")
      .def("getGradientsSize", &Behaviour::getGradientsSize,
           "Return the size of a vector able to contain "
           "all the components of the driving variables")
      .def("getThermodynamicForcesSize", &Behaviour::getThermodynamicForcesSize,
           "Return the size of a vector able to contain "
           "all the components of the thermodynamic forces")
      .def("getStensorComponentsSuffixes",
           &Behaviour::getStensorComponentsSuffixes,
           "Return the components suffixes of a symmetric tensor")
      .def("getVectorComponentsSuffixes",
           &Behaviour::getVectorComponentsSuffixes,
           "Return the components suffixes of a vector")
      .def("getTensorComponentsSuffixes",
           &Behaviour::getTensorComponentsSuffixes,
           "Return the components suffixes of a tensor")
      .def("getGradientsComponents", &Behaviour::getGradientsComponents,
           "Return the components of the driving variables")
      .def("getThermodynamicForcesComponents",
           &Behaviour::getThermodynamicForcesComponents,
           "Return the components of the thermodynamic forces")
      .def("getGradientComponentPosition",
           &Behaviour::getGradientComponentPosition,
           "Return the position of the component of a driving variable")
      .def("getThermodynamicForceComponentPosition",
           &Behaviour::getThermodynamicForceComponentPosition,
           "Return the position of the component of a "
           "thermodynamic force")
      .def("getSymmetryType", &Behaviour::getSymmetryType,
           "Return the symmetry of the behaviour:\n"
           "- 0 means that the behaviour is isotropic.\n"
           "- 1 means that the behaviour is orthotropic.\n")
      .def("getMaterialPropertiesNames", &Behaviour::getMaterialPropertiesNames,
           "Return the names of the material properties")
      .def("getInternalStateVariablesNames",
           &Behaviour::getInternalStateVariablesNames,
           "Return the names of the internal state variables")
      .def("expandInternalStateVariablesNames",
           &Behaviour::expandInternalStateVariablesNames,
           "Return the names of the internal state variables, taking "
           "into account the suffixes for vectors, symmetric "
           "tensors and tensors")
      .def("getInternalStateVariablesSize",
           &Behaviour::getInternalStateVariablesSize,
           "Return the size of the array of internal variables")
      .def("getInternalStateVariablesDescriptions",
           &Behaviour::getInternalStateVariablesDescriptions,
           "Return the descriptions the internal variables")
      .def("getInternalStateVariableType",
           &Behaviour::getInternalStateVariableType,
           "Return the type of an internal variable:\n"
           "- 0 means that the internal state variable is a scalar\n"
           "- 1 means that the internal state variable is a symmetric tensor\n"
           "- 3 means that the internal state variable is a tensor\n")
      .def("getInternalStateVariablePosition",
           &Behaviour::getInternalStateVariablePosition,
           "Return the internal state variable position")
      .def("getExternalStateVariablesNames",
           &Behaviour::getExternalStateVariablesNames,
           "Return the names of the external state variables")
      .def("getParametersNames", &Behaviour::getParametersNames,
           "Return the names of the floating point parameters")
      .def("getIntegerParametersNames", &Behaviour::getIntegerParametersNames,
           "Return the names of the integer parameters")
      .def("getUnsignedShortParametersNames",
           &Behaviour::getUnsignedShortParametersNames,
           "Return the names of the unsigned short parameters")
      .def("getRealParameterDefaultValue",
           &Behaviour::getRealParameterDefaultValue,
           "return the default value of a parameter")
      .def("getIntegerParameterDefaultValue",
           &Behaviour::getIntegerParameterDefaultValue,
           "return the default value of an integer parameter")
      .def("getUnsignedShortParameterDefaultValue",
           &Behaviour::getUnsignedShortParameterDefaultValue,
           "return the default value of an unsigned short parameter")
      .def("setParameter", &Behaviour::setParameter,
           "set the given parameter' value")
      .def("setIntegerParameter", &Behaviour::setIntegerParameter,
           "set the given parameter' value")
      .def("setUnsignedIntegerParameter",
           &Behaviour::setUnsignedIntegerParameter,
           "set the given parameter' value")
      .def("hasBounds", &Behaviour::hasBounds,
           "return true if the given variable has bounds")
      .def("hasLowerBound", &Behaviour::hasLowerBound,
           "return true if the given variable has a lower bound")
      .def("hasUpperBound", &Behaviour::hasUpperBound,
           "return true if the given variable has an upper bound")
      .def("getLowerBound", &Behaviour::getLowerBound,
           "return the lower bound of the given variable")
      .def("getUpperBound", &Behaviour::getUpperBound,
           "return the uppert bound of the given variable")
      .def("hasPhysicalBounds", &Behaviour::hasPhysicalBounds,
           "return true if the given variable has physical bounds")
      .def("hasLowerPhysicalBound", &Behaviour::hasLowerPhysicalBound,
           "return true if the given variable has a physical lower bound")
      .def("hasUpperPhysicalBound", &Behaviour::hasUpperPhysicalBound,
           "return true if the given variable has a physical upper bound")
      .def("getLowerPhysicalBound", &Behaviour::getLowerPhysicalBound,
           "return the lower bound of the given variable")
      .def("getUpperPhysicalBound", &Behaviour::getUpperPhysicalBound,
           "return the upper bound of the given variable");

}  // end of declareBehaviour()
