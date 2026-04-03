/*!
 * \file   bindings/python/mfront/pymtest.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  10 avril 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Macros.hxx"
#include "MTest/RoundingMode.hxx"
#include "MTest/SolverOptions.hxx"

#define MTEST_MODULE_NAME TFEL_PP_JOIN(_mtest_, TFEL_SUFFIX_FOR_PYTHON_MODULES)

void declareMaterialProperty(pybind11::module_&);
void declareBehaviour(pybind11::module_&);
void declareSchemeBase(pybind11::module_&);
void declareSingleStructureScheme(pybind11::module_&);
void declareConstraint(pybind11::module_&);
void declareMTest(pybind11::module_&);
void declareMTestParser(pybind11::module_&);
void declarePipeMesh(pybind11::module_&);
void declarePipeTest(pybind11::module_&);
void declareCurrentState(pybind11::module_&);
void declareStructureCurrentState(pybind11::module_&);
void declareStudyCurrentState(pybind11::module_&);
void declareSolverWorkSpace(pybind11::module_&);
void declareMFrontLogStream(pybind11::module_&);
void declareMTestFileExport(pybind11::module_&);

#ifdef TFEL_SUFFIX_FOR_PYTHON_MODULES
PYBIND11_MODULE(MTEST_MODULE_NAME, m) {
#else
PYBIND11_MODULE(_mtest, m) {
#endif
  pybind11::enum_<mtest::StiffnessUpdatingPolicy>(m, "StiffnessUpdatingPolicy")
      .value("CONSTANTSTIFFNESS",
             mtest::StiffnessUpdatingPolicy::CONSTANTSTIFFNESS)
      .value("CONSTANTSTIFFNESSBYPERIOD",
             mtest::StiffnessUpdatingPolicy::CONSTANTSTIFFNESSBYPERIOD)
      .value("UPDATEDSTIFFNESSMATRIX",
             mtest::StiffnessUpdatingPolicy::UPDATEDSTIFFNESSMATRIX);

  pybind11::enum_<mtest::PredictionPolicy>(m, "PredictionPolicy")
      .value("NOPREDICTION", mtest::PredictionPolicy::NOPREDICTION)
      .value("LINEARPREDICTION", mtest::PredictionPolicy::LINEARPREDICTION)
      .value("ELASTICPREDICTION", mtest::PredictionPolicy::ELASTICPREDICTION)
      .value("SECANTOPERATORPREDICTION",
             mtest::PredictionPolicy::SECANTOPERATORPREDICTION)
      .value("TANGENTOPERATORPREDICTION",
             mtest::PredictionPolicy::TANGENTOPERATORPREDICTION);

  pybind11::enum_<mtest::StiffnessMatrixType>(m, "StiffnessMatrixType")
      .value("NOSTIFFNESS", mtest::StiffnessMatrixType::NOSTIFFNESS)
      .value("ELASTIC", mtest::StiffnessMatrixType::ELASTIC)
      .value("SECANTOPERATOR", mtest::StiffnessMatrixType::SECANTOPERATOR)
      .value("TANGENTOPERATOR", mtest::StiffnessMatrixType::TANGENTOPERATOR)
      .value("CONSISTENTTANGENTOPERATOR",
             mtest::StiffnessMatrixType::CONSISTENTTANGENTOPERATOR);

  declareMaterialProperty(m);
  declareBehaviour(m);
  declareCurrentState(m);
  declareStructureCurrentState(m);
  declareStudyCurrentState(m);
  declareSolverWorkSpace(m);
  declareMTestParser(m);
  declareSchemeBase(m);
  declareSingleStructureScheme(m);
  declareConstraint(m);
  declareMTest(m);
  declarePipeMesh(m);
  declarePipeTest(m);
  declareMFrontLogStream(m);
  declareMTestFileExport(m);

  void (*ptr)(const std::string&) = mtest::setRoundingMode;
  void (*ptr2)() = mtest::setRoundingMode;
  m.def("setRoundingMode", ptr);
  m.def("setRoundingMode", ptr2);
}
