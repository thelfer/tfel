/*!
 * \file   Evaluator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   26/12/2017
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
  pybind11::class_<Evaluator>(m, "Evaluator")
      .def(pybind11::init<std::string>())
      .def("__call__", ptr1, "evaluates the formula")
      .def("__call__", ptr2, "evaluates the formula")
      .def("getValue", ptr3, "evaluates the formula")
      .def("getValue", ptr4, "evaluates the formula")
      .def("setVariableValue", ptr5, "set the value of a variable")
      .def("getVariablesNames", &Evaluator::getVariablesNames,
           "return the variable names")
      .def("removeDependencies", &Evaluator::removeDependencies,
           "resolves dependencies and removes them");
}
