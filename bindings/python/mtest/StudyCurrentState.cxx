/*!
 * \file   StudyCurrentState.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MTest/Evolution.hxx"
#include "MTest/StudyCurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"

void declareStudyCurrentState(pybind11::module_&);

#define TFEL_PYTHON_STUDYCURRENTSTATEGETTER(X)                     \
  static tfel::math::vector<mtest::real> StudyCurrentState_get##X( \
      const mtest::StudyCurrentState& t) {                         \
    return t.X;                                                    \
  }

TFEL_PYTHON_STUDYCURRENTSTATEGETTER(u_1)
TFEL_PYTHON_STUDYCURRENTSTATEGETTER(u0)
TFEL_PYTHON_STUDYCURRENTSTATEGETTER(u1)
TFEL_PYTHON_STUDYCURRENTSTATEGETTER(u10)

static mtest::StructureCurrentState& getStructureCurrentState(
    mtest::StudyCurrentState& s, const std::string& n) {
  return s.getStructureCurrentState(n);
}

static double StudyCurrentState_getEvolutionValue(mtest::StudyCurrentState& s,
                                                  const std::string& n,
                                                  const double t) {
  return s.getEvolution(n)(t);
}

void declareStudyCurrentState(pybind11::module_& m) {
  using mtest::StudyCurrentState;
  pybind11::class_<StudyCurrentState>(m, "StudyCurrentState")
      .def(pybind11::init<>())
      .def_property_readonly("u_1", StudyCurrentState_getu_1)
      .def_property_readonly("u0", StudyCurrentState_getu0)
      .def_property_readonly("u1", StudyCurrentState_getu1)
      .def_property_readonly("u10", StudyCurrentState_getu10)
      .def_readonly("period", &StudyCurrentState::period)
      .def_readonly("iterations", &StudyCurrentState::iterations)
      .def_readonly("subSteps", &StudyCurrentState::subSteps)
      .def_readonly("dt_1", &StudyCurrentState::dt_1)
      .def("makeDeepCopy", &StudyCurrentState::makeDeepCopy)
      .def("getEvolutionValue", &StudyCurrentState_getEvolutionValue)
      .def("getStructureCurrentState", getStructureCurrentState,
           pybind11::return_value_policy::reference)
      .def("getFailureStatus", &mtest::StudyCurrentState::getFailureStatus)
      .def("getFailureCriterionStatus",
           &mtest::StudyCurrentState::getFailureCriterionStatus)
      .def("setFailureCriterionStatus",
           &mtest::StudyCurrentState::setFailureCriterionStatus)
      .def("setNumberOfFailureCriterionStatus",
           &mtest::StudyCurrentState::setNumberOfFailureCriterionStatus)
      .def("getNumberOfFailureCriterionStatus",
           &mtest::StudyCurrentState::getNumberOfFailureCriterionStatus);
}
