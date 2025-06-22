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

#include <boost/python.hpp>
#include "MTest/Evolution.hxx"
#include "MTest/StudyCurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"

void declareStudyCurrentState();

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

void declareStudyCurrentState() {
  using mtest::StudyCurrentState;

  boost::python::class_<StudyCurrentState>("StudyCurrentState")
      .add_property("u_1", StudyCurrentState_getu_1)
      .add_property("u0", StudyCurrentState_getu0)
      .add_property("u1", StudyCurrentState_getu1)
      .add_property("u10", StudyCurrentState_getu10)
      .def_readonly("period", &StudyCurrentState::period)
      .def_readonly("iterations", &StudyCurrentState::iterations)
      .def_readonly("subSteps", &StudyCurrentState::subSteps)
      .def_readonly("dt_1", &StudyCurrentState::dt_1)
      .def("getEvolutionValue", &StudyCurrentState_getEvolutionValue)
      .def("getStructureCurrentState", getStructureCurrentState,
           boost::python::return_internal_reference<>())
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
