/*!
 * \file   SingleStructureScheme.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 déc. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<tuple>
#include<boost/python.hpp>
#include"MTest/Behaviour.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/FunctionEvolution.hxx"
#include"MTest/CastemEvolution.hxx"
#include"MTest/SingleStructureScheme.hxx"

static void
SingleStructureScheme_addEvolution(mtest::SingleStructureScheme& t,
				   const std::string&  n,
				   const mtest::real& v,
				   const bool b1,
				   const bool b2)
{
  t.addEvolution(n,mtest::make_evolution(v),b1,b2);
}

static void
SingleStructureScheme_addEvolution2(mtest::SingleStructureScheme& t,
				    const std::string&  n,
				    const std::map<mtest::real,
				    mtest::real>& v,
				    const bool b1,
				    const bool b2)
{
  t.addEvolution(n,mtest::make_evolution(v),b1,b2);
}

static void
SingleStructureScheme_addEvolution3(mtest::SingleStructureScheme& t,
				    const std::string&  n,
				    const std::string& f,
				    const bool b1,
				    const bool b2)
{
  using namespace std;
  using namespace mtest;
  shared_ptr<Evolution> pev(new FunctionEvolution(f,t.getEvolutions()));
  t.addEvolution(n,pev,b1,b2);
}

static void
SingleStructureScheme_setMaterialProperty(mtest::SingleStructureScheme& t,
					  const std::string&  n,
					  const mtest::real& v,
					  const bool b)
{
  using namespace mtest;
  std::shared_ptr<Evolution> pev(new ConstantEvolution(v));
  t.setMaterialProperty(n,pev,b);
}

static void
SingleStructureScheme_setMaterialProperty2(mtest::SingleStructureScheme& t,
					   const std::string& n,
					   const std::string& f,
					   const bool b)
{
  using namespace mtest;
  using mtest::real;
  std::shared_ptr<Evolution> mpev;
  mpev = std::shared_ptr<Evolution>(new FunctionEvolution(f,t.getEvolutions()));
  t.setMaterialProperty(n,mpev,b);
}

static void
SingleStructureScheme_setMaterialProperty3(mtest::SingleStructureScheme& t,
					   const std::string&  n,
					   const std::string& f,
					   const bool b)
{
  using namespace mtest;
  std::shared_ptr<Evolution> pev(new FunctionEvolution(f,t.getEvolutions()));
  t.setMaterialProperty(n,pev,b);
}

static void
SingleStructureScheme_setCastemMaterialProperty(mtest::SingleStructureScheme& t,
						const std::string& n,
						const std::string& l,
						const std::string& f,
						const bool b)
{
  using namespace mtest;
  std::shared_ptr<Evolution> mpev;
  mpev = std::shared_ptr<Evolution>(new CastemEvolution(l,f,t.getEvolutions()));
  t.setMaterialProperty(n,mpev,b);
}

static void
SingleStructureScheme_setExternalStateVariable(mtest::SingleStructureScheme& t,
					       const std::string&  n,
					       const mtest::real& v,
					       const bool b)
{
  using namespace mtest;
  std::shared_ptr<Evolution> pev(new ConstantEvolution(v));
  t.setExternalStateVariable(n,pev,b);
}

static void
SingleStructureScheme_setExternalStateVariable2(mtest::SingleStructureScheme& t,
						const std::string&  n,
						const std::map<mtest::real,
						mtest::real>& values,
						const bool b)
{
  using namespace mtest;
  std::vector<real> tv(values.size());
  std::vector<real> ev(values.size());
  std::vector<real>::size_type i = 0;
  for(const auto& v : values){
    std::tie(tv[i],ev[i]) = v;
    ++i;
  }
  std::shared_ptr<Evolution> pev(new LPIEvolution(tv,ev));
  t.setExternalStateVariable(n,pev,b);
}

static void
SingleStructureScheme_setExternalStateVariable3(mtest::SingleStructureScheme& t,
						const std::string&  n,
						const std::string& f,
						const bool b)
{
  using namespace mtest;
  std::shared_ptr<Evolution> pev(new FunctionEvolution(f,t.getEvolutions()));
  t.setExternalStateVariable(n,pev,b);
}

static void
SingleStructureScheme_setInternalStateVariableInitialValue1(mtest::SingleStructureScheme& s,
							    const std::string&  n,
							    const mtest::real v){
  s.setScalarInternalStateVariableInitialValue(n,v);
}

static void
SingleStructureScheme_setInternalStateVariableInitialValue2(mtest::SingleStructureScheme& s,
							   const std::string&  n,
							   const std::vector<mtest::real> v){
  const auto& b   = *(s.getBehaviour());
  const auto type = b.getInternalStateVariableType(n);
  if(type==0){
    if(v.size()!=1u){
      throw(std::runtime_error("SingleStructureScheme::setInternalStateVariableInitialValue: "
			       "unsupported number of values for scalar internal "
			       "state variable '"+n+"'"));
    }
    s.setScalarInternalStateVariableInitialValue(n,v[0]);
  } else  if(type==1){
    s.setStensorInternalStateVariableInitialValues(n,v);
  } else if(type==3){
    s.setTensorInternalStateVariableInitialValues(n,v);
  } else {
    throw(std::runtime_error("SingleStructureScheme::setInternalStateVariableInitialValue: "
			     "unsupported type for internal state variable '"+n+"'"));
  }
}

void declareSingleStructureScheme(void);

void declareSingleStructureScheme(void)
{
  using namespace mtest;
  void (SingleStructureScheme::* ptr1)(const std::string&,
				       const std::string&,
				       const std::string&) =
    &SingleStructureScheme::setBehaviour;
  void (SingleStructureScheme::* ptr2)(const std::string&,
				       const std::string&,
				       const std::string&,
				       const std::string&) =
    &SingleStructureScheme::setBehaviour;
  boost::python::class_<SingleStructureScheme,boost::noncopyable,
			boost::python::bases<SchemeBase>>("SingleStructureScheme",
							  boost::python::no_init)
    .def("setBehaviour",ptr1,
	 "This method declares the behaviour used for the "
	 "test.\n"
	 "* The first parameter (string) specify the interface "
	 "used by the behaviour. Supported interfaces are:\n"
	 "- 'umat'\n"
	 "- 'cyrano'\n"
	 "- 'aster'\n"
	 "* The second parameter (string) is the path to the "
	 "dynamic library which implement the selected behaviour.\n"
	 "* The third parameter (string) is the name of the function.")
    .def("setBehaviour",ptr2)
    .def("handleThermalExpansion",
	 &SingleStructureScheme::setHandleThermalExpansion,
	 "This method override the (de)activation of the thermal "
	 "strain computation. It's activated by default if a "
	 "material property named 'ThermalExpansion' is defined "
	 "(isotropic materials).\n"
	 "* If the parameter (bool) is 'true', the computation is "
	 "activated. Otherwise, the computation is deactivated.")
    .def("setParameter",&SingleStructureScheme::setParameter,
	 "This method specifies the value of a parameter's "
	 "behaviour.\n"
	 "* The first parameter (string) is the name of the "
	 "parameter's behaviour.\n"
	 "* The second parameter (double) is the value of that "
	 "parameter's behaviour.")
    .def("setIntegerParameter",
	 &SingleStructureScheme::setIntegerParameter,
	 "This keyword specifies the value of an "
	 "integer parameter of the behaviour.\n"
	 "* The first parameter (string) is the name of the "
	 "parameter's behaviour.\n"
	 "* The second parameter (int) is the value of that "
	 "parameter's behaviour.")
    .def("setUnsignedIntegerParameter",
	 &SingleStructureScheme::setUnsignedIntegerParameter,
	 "This method specifies the value of an "
	 "unsigned integer parameter of the behaviour.\n"
	 "* The first parameter (string) is the name of the "
	 "parameter's behaviour.\n"
	 "* The second parameter (uint) is the value of that "
	 "parameter's behaviour.")
    .def("setOutOfBoundsPolicy",
	 &SingleStructureScheme::setOutOfBoundsPolicy,
	 "This method let the user to precise the out "
	 "of bounds policy.\n"
	 "* The parameter (OutOfBoundsPolicy) is the selected "
	 "policy. Three policies are available:\n"
	 "- 'None' : nothing is done when a variable is out of "
	 "bounds.\n"
	 "- 'Warning' : a message is selected when a variable "
	 "is out of bounds.\n"
	 "- 'Strict' : the computation fails when a variable "
	 "is out of bounds. Most mechanical behaviours "
	 "implementations throw an exception which is caught "
	 "by 'MTest'.")
    .def("setMaterialProperty",
	 SingleStructureScheme_setMaterialProperty,
	 (boost::python::arg("name"),"value",boost::python::arg("check")=true),
	 "This method defines a constant material property.\n"
	 "* The first parameter (string) is the selected material "
	 "property. Only the mechanical properties defined by the "
	 "behaviour for the modelling hypothesis considered may be "
	 "used. (Note : isotropic thermal expansion is defined "
	 "through the 'ThermalExpansion' property and the "
	 "orthotropic thermal expansion is defined through the "
	 "'ThermalExpansion1', 'ThermalExpansion2' and "
	 "'ThermalExpansion3' material properties).\n"
	 "* The second parameter (double) is the constant value of "
	 "the selected material property.")
    .def("setMaterialProperty",
	 SingleStructureScheme_setMaterialProperty2,
	 (boost::python::arg("name"),"value",boost::python::arg("check")=true),
	 "This method defines a function material property.\n"
	 "* The first parameter (string) is the selected material "
	 "property. Only the mechanical properties defined by the "
	 "behaviour for the modelling hypothesis considered may be "
	 "used. (Note : isotropic thermal expansion is defined "
	 "through the 'ThermalExpansion' property and the "
	 "orthotropic thermal expansion is defined through the "
	 "'ThermalExpansion1', 'ThermalExpansion2' and "
	 "'ThermalExpansion3' material properties).\n"
	 "* The second parameter (string) is the function used "
	 "to represent the behaviour of the selected material "
	 "property.")
    .def("setMaterialProperty",
	 SingleStructureScheme_setMaterialProperty3,
	 (boost::python::arg("name"),"value","function",
	  boost::python::arg("check")=true),
	 "This method defines a function material property.\n"
	 "* The first parameter (string) is the selected material "
	 "property. Only the mechanical properties defined by the "
	 "behaviour for the modelling hypothesis considered may "
	 "be used. (Note : isotropic thermal expansion is defined "
	 "through the 'ThermalExpansion' property and the "
	 "orthotropic thermal expansion is defined through the "
	 "'ThermalExpansion1', 'ThermalExpansion2' and "
	 "'ThermalExpansion3' material properties).\n"
	 "* The second parameter (string) is the function used "
	 "to represent the behaviour of the selected material "
	 "property.")
    .def("setCastemMaterialProperty",
	 SingleStructureScheme_setCastemMaterialProperty,
	 (boost::python::arg("name"),boost::python::arg("library"),
	  boost::python::arg("function"),boost::python::arg("check")=true),
	 "This method defines a castem material property.\n"
	 "* The first parameter (string) is the selected material "
	 "property. Only the mechanical properties defined by the "
	 "behaviour for the modelling hypothesis considered may be "
	 "used. (Note : isotropic thermal expansion is defined "
	 "through the 'ThermalExpansion' property and the "
	 "orthotropic thermal expansion is defined through the "
	 "'ThermalExpansion1', 'ThermalExpansion2' and "
	 "'ThermalExpansion3' material properties).\n"
	 "* The second parameter (string) is the path to the "
	 "dynamic library which implement the behaviour of the "
	 "selected material property.\n"
	 "* The third parameter (string) is the name of the "
	 "function.")
    .def("setExternalStateVariable",
	 SingleStructureScheme_setExternalStateVariable,
	 (boost::python::arg("name"),"value",
	  boost::python::arg("check")=true),
	 "This method specify the constant evolution of an "
	 "external state variable.\n"
	 "* The first parameter (string) is the name of the "
	 "external state variable (generally the temperature "
	 "which is by default defined in behaviour interfaces).\n"
	 "* The second parameter (double) is the constant value "
	 "of the selected external state variable.")
    .def("setExternalStateVariable",
	 SingleStructureScheme_setExternalStateVariable2,
	 (boost::python::arg("name"),"value",
	  boost::python::arg("check")=true),
	 "This method specify the linear evolution of an "
	 "external state variable.\n"
	 "* The first parameter (string) is the name of the "
	 "external state variable (generally the temperature "
	 "which is by default defined in behaviour interfaces).\n"
	 "* The second parameter (map<double, double>) specify "
	 "a linear evolution: the associative array of time-value "
	 "is used to perform an interpolation. That interpolation "
	 "is only performed between the minimum and maximum times "
	 "given in the array. Should a time be out of the array, "
	 "the returned value will be the one from the nearest "
	 "association available.")
    .def("setExternalStateVariable",
	 SingleStructureScheme_setExternalStateVariable3,
	 (boost::python::arg("name"),"value","function",
	  boost::python::arg("check")=true),
	 "This method specify the complex evolution of an "
	 "external state variable.\n"
	 "* The first parameter (string) is the name of the "
	 "external state variable (generally the temperature "
	 "which is by default defined in behaviour interfaces).\n"
	 "* The second parameter (string) is the function used "
	 "to represent the evolution of the selected external "
	 "state variable. The string will be interpreted as a "
	 "function of time (represented by the variable 't' in "
	 "the string).")
    .def("addReal",
	 SingleStructureScheme_addEvolution,
	 (boost::python::arg("name"),"value",boost::python::arg("b1")=true,
	  boost::python::arg("b2")=true))
    .def("addEvolution",
	 SingleStructureScheme_addEvolution,
	 (boost::python::arg("name"),"value",boost::python::arg("b1")=true,
	  boost::python::arg("b2")=true))
    .def("addEvolution",
	 SingleStructureScheme_addEvolution2,
	 (boost::python::arg("name"),"value",boost::python::arg("b1")=true,
	  boost::python::arg("b2")=true))
    .def("addEvolution",
	 SingleStructureScheme_addEvolution3,
	 (boost::python::arg("name"),"value","function",boost::python::arg("b1")=true,
	  boost::python::arg("b2")=true))
    .def("setInternalStateVariableInitialValue",
	 SingleStructureScheme_setInternalStateVariableInitialValue1,
	 "This method allow to specify the initial  value of an "
	 "internal state variable.")
    .def("setInternalStateVariableInitialValue",
	 SingleStructureScheme_setInternalStateVariableInitialValue2,
	 "This method allow to specify the initial  value of an "
	 "internal state variable.")
    ;

}
