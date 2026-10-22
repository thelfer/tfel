/*!
 * \file   Evaluator.cxx
 * \brief
 * \author Thomas Helfer
 * \date 26/12/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "TFEL/Math/Evaluator.hxx"

void declareEvaluator(pybind11::module_& m);

void declareEvaluator(pybind11::module_& m) {
  using tfel::math::Evaluator;
  double (Evaluator::*ptr1)() const = &Evaluator::operator();
  double (Evaluator::*ptr2)(const std::map<std::string, double>&) =
      &Evaluator::operator();
  double (Evaluator::*ptr3)() const = &Evaluator::getValue;
  double (Evaluator::*ptr4)(const std::map<std::string, double>&) =
      &Evaluator::getValue;
  void (Evaluator::*ptr5)(const std::string&, const double) =
      &Evaluator::setVariableValue;
  pybind11::class_<Evaluator>(m, "Evaluator",
                              "Class in charge of handling a function defined by a\n"
                              "string")
      .def(pybind11::init<std::string>(), pybind11::arg("f"))
      .def("__call__", ptr1,
           "Evaluate the formula\n"
           "Return the result of the evaluation\n"
           "Variables values shall have been set with the\n"
           "`setVariableValue` method")
      .def("__call__", ptr2, pybind11::arg("vs"),
           "Evaluate the formula\n"
           "vs: a map giving the values of some of the\n"
           "variables required to evaluate the formula\n"
           "Return the result of the evaluation\n"
           "The values of the variables not given in argument shall\n"
           "have been set with the `setVariableValue` method")
      .def("getValue", ptr3,
           "Evaluate the formula\n"
           "Return the result of the evaluation\n"
           "Variables values shall have been set with the\n"
           "`setVariableValue` method")
      .def("getValue", ptr4, pybind11::arg("vs"),
           "Evaluate the formula\n"
           "vs: a map giving the values of some of the\n"
           "variables required to evaluate the formula\n"
           "Return the result of the evaluation\n"
           "The values of the variables not given in argument shall\n"
           "have been set with the `setVariableValue` method")
      .def("setVariableValue", ptr5,
           pybind11::arg("vname"), pybind11::arg("value"))
      .def("getVariablesNames", &Evaluator::getVariablesNames)
      .def("removeDependencies", &Evaluator::removeDependencies);
}
