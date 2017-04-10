/*!
 * \file   Behaviour.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   28/03/2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Python/SharedPtr.hxx"
#include<boost/python.hpp>
#include<boost/python/make_constructor.hpp>
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"MTest/Behaviour.hxx"

static std::shared_ptr<mtest::Behaviour> getBehaviour1(const std::string& i,
						       const std::string& l,
						       const std::string& f,
						       const mtest::Behaviour::Parameters& p,
						       const std::string& h)
{
  using mtest::Behaviour;
  return Behaviour::getBehaviour(i,l,f,p,
				 Behaviour::ModellingHypothesis::fromString(h));
} // end of std::shared_ptr<Behaviour> getBehaviour1

static std::shared_ptr<mtest::Behaviour> getBehaviour2(const std::string& i,
						       const std::string& l,
						       const std::string& f,
						       const mtest::Behaviour::ModellingHypothesis::Hypothesis h)
{
  using mtest::Behaviour;
  return Behaviour::getBehaviour(i,l,f,Behaviour::Parameters(),h);
} // end of std::shared_ptr<Behaviour> getBehaviour1

static std::shared_ptr<mtest::Behaviour> getBehaviour3(const std::string& i,
						       const std::string& l,
						       const std::string& f,
						       const std::string& h)
{
  using mtest::Behaviour;
  return Behaviour::getBehaviour(i,l,f,Behaviour::Parameters(),
				 Behaviour::ModellingHypothesis::fromString(h));
} // end of std::shared_ptr<Behaviour> getBehaviour1

void declareBehaviour(){
  using boost::python::class_;
  using mtest::Behaviour;
  class_<std::shared_ptr<Behaviour>>("Behaviour")
    .def("__init__",boost::python::make_constructor(Behaviour::getBehaviour),
	 "This constructor has the following arguments:\n"
	 "-  i(std::string): interface\n"
	 "-  l(std::string): library\n"
	 "-  f(std::string): function\n"
	 "-  d(tfel::utilities::Data): parameter\n"
	 "-  h(tfel::material::ModellingHypothesis): modelling hypothesis\n")
    .def("__init__",boost::python::make_constructor(getBehaviour1),
	 "This constructor has the following arguments:\n"
	 "-  i(std::string): interface\n"
	 "-  l(std::string): library\n"
	 "-  f(std::string): function\n"
	 "-  d(tfel::utilities::Data): parameter\n"
	 "-  h(std::string): modelling hypothesis\n")
    .def("__init__",boost::python::make_constructor(getBehaviour2),
	 "This constructor has the following arguments:\n"
	 "-  i(std::string): interface\n"
	 "-  l(std::string): library\n"
	 "-  f(std::string): function\n"
	 "-  h(tfel::material::ModellingHypothesis): modelling hypothesis\n")
    .def("__init__",boost::python::make_constructor(getBehaviour3),
	 "This constructor has the following arguments:\n"
	 "-  i(std::string): interface\n"
	 "-  l(std::string): library\n"
	 "-  f(std::string): function\n"
	 "-  d(tfel::utilities::Data): parameter\n"
	 "-  h(std::string): modelling hypothesis\n")
    .def("getBehaviourType",&Behaviour::getBehaviourType,
	 "Return the behaviour type.\n"
	 "The value returned has the following meaning:\n"
	 "- 0: general behaviour\n"
	 "- 1: small strain behaviour\n"
	 "- 2: finite strain behaviour\n"
	 "- 3: cohesive zone model\n")
    .def("getBehaviourKinematic",
	 &Behaviour::getBehaviourKinematic,
	 "Return the behaviour kinematic.\n"
	 "The value returned has the following meaning:\n"
	 "- 0: undefined kinematic\n"
	 "- 1: standard small strain behaviour kinematic\n"
	 "- 2: cohesive zone model kinematic\n"
	 "- 3: standard finite strain kinematic (F-Cauchy)\n"
	 "- 4: ptest finite strain kinematic (eto-pk1)\n")
    .def("getDrivingVariablesSize",
	 &Behaviour::getDrivingVariablesSize,
	 "Return the size of a vector able to contain "
	 "all the components of the driving variables")
    .def("getThermodynamicForcesSize",
	 &Behaviour::getThermodynamicForcesSize,
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
    .def("getDrivingVariablesComponents",
	 &Behaviour::getDrivingVariablesComponents,
	 "Return the components of the driving variables")
    .def("getThermodynamicForcesComponents",
	 &Behaviour::getThermodynamicForcesComponents,
	 "Return the components of the thermodynamic forces")
    .def("getDrivingVariableComponentPosition",
	 &Behaviour::getDrivingVariableComponentPosition,
	 "Return the position of the component of a driving variable")
    .def("getThermodynamicForceComponentPosition",
	 &Behaviour::getThermodynamicForceComponentPosition,
	 "Return the position of the component of a "
	 "thermodynamic force")
    .def("getSymmetryType",
	 &Behaviour::getSymmetryType,
	 "Return the symmetry of the behaviour:\n"
	 "- 0 means that the behaviour is isotropic.\n"
	 "- 1 means that the behaviour is orthotropic.\n")
    .def("getMaterialPropertiesNames",
	 &Behaviour::getMaterialPropertiesNames,
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
    .def("getParametersNames",
	 &Behaviour::getParametersNames,
	 "Return the names of the floating point parameters")
    .def("getIntegerParametersNames",
	 &Behaviour::getIntegerParametersNames,
	 "Return the names of the integer parameters")
    .def("getUnsignedShortParametersNames",
	 &Behaviour::getUnsignedShortParametersNames,
	 "Return the names of the unsigned short parameters")
    ;
  
} // end of declareBehaviour()
