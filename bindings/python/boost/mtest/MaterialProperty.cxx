/*!
 * \file   MaterialProperty.cxx
 * \brief
 * \author Thomas Helfer
 * \date   11/08/2021
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
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MTest/MaterialProperty.hxx"

static std::shared_ptr<mtest::MaterialProperty>
MaterialProperty_getMaterialProperty1(const std::string& i,
                                      const std::string& l,
                                      const std::string& f) {
  using mtest::MaterialProperty;
  return MaterialProperty::getMaterialProperty(i, l, f);
}  // end of std::shared_ptr<MaterialProperty> getMaterialProperty1

static std::shared_ptr<mtest::MaterialProperty>
MaterialProperty_getMaterialProperty2(const std::string& l,
                                      const std::string& f) {
  using mtest::MaterialProperty;
  using ELM = tfel::system::ExternalLibraryManager;
  const auto i = ELM::getExternalLibraryManager().getInterface(l, f);
  return MaterialProperty::getMaterialProperty(i, l, f);
}  // end of getMaterialProperty2

static mtest::real MaterialProperty_getValue(
    mtest::MaterialProperty& mp,
    const std::map<std::string, mtest::real>& values) {
  return mtest::getValue(mp, values);
}  // end of MaterialProperty_getValue

void declareMaterialProperty() {
  using boost::python::class_;
  using mtest::MaterialProperty;

  void (MaterialProperty::*setValue1)(const std::string&, const mtest::real) =
      &MaterialProperty::setVariableValue;
  void (MaterialProperty::*setValue2)(const std::size_t, const mtest::real) =
      &MaterialProperty::setVariableValue;

  mtest::real (*getValue1)(MaterialProperty&,
                           const std::map<std::string, mtest::real>&,
                           const bool) = &mtest::getValue;

  mtest::real (*getValue2)(MaterialProperty&, const std::vector<mtest::real>&) =
      &mtest::getValue;
  mtest::real (*getValue3)(MaterialProperty&, const mtest::real) =
      &mtest::getValue;
  mtest::real (*getValue4)(MaterialProperty&) = &mtest::getValue;

  class_<std::shared_ptr<MaterialProperty>>("MaterialProperty")
      .def("__init__",
           boost::python::make_constructor(
               MaterialProperty_getMaterialProperty1),
           "This constructor has the following arguments:\n"
           "- i(std::string): interface\n"
           "- l(std::string): library\n"
           "- f(std::string): function\n")
      .def("__init__",
           boost::python::make_constructor(
               MaterialProperty_getMaterialProperty2),
           "This constructor has the following arguments:\n"
           "- l(std::string): library\n"
           "- f(std::string): function\n")
      .def("getVariablesNames", &MaterialProperty::getVariablesNames,
           "return the names of the arguments of the function")
      .def("getParametersNames", &MaterialProperty::getParametersNames,
           "return the names of the parameters of the function")
      .def("getOutputName", &MaterialProperty::getOutputName,
           "return the names of the output of the function")
      .def("setVariableValue", setValue1, "set the value of a variable")
      .def("setVariableValue", setValue2, "set the value of a variable")
      .def("setParameter", &MaterialProperty::setParameter,
           "set the value of a parameter")
      .def("getValue", &MaterialProperty::getValue,
           "evaluate the material property")
      .def("getValue", getValue1, "evaluate the material property")
      .def("getValue", &MaterialProperty_getValue,
           "evaluate the material property")
      .def("__call__", getValue1, "evaluate the material property")
      .def("__call__", MaterialProperty_getValue,
           "evaluate the material property")
      .def("__call__", getValue2, "evaluate the material property")
      .def("__call__", getValue3, "evaluate the material property")
      .def("__call__", getValue4, "evaluate the material property");

}  // end of declareMaterialProperty
