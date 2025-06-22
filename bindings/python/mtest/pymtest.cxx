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

#include <boost/python.hpp>
#include "MTest/RoundingMode.hxx"
#include "MTest/SolverOptions.hxx"

void declareMaterialProperty();
void declareBehaviour();
void declareSchemeBase();
void declareSingleStructureScheme();
void declareConstraint();
void declareMTest();
void declareMTestParser();
void declarePipeMesh();
void declarePipeTest();
void declareCurrentState();
void declareStructureCurrentState();
void declareStudyCurrentState();
void declareSolverWorkSpace();
void declareMFrontLogStream();

BOOST_PYTHON_MODULE(_mtest) {
  boost::python::enum_<mtest::StiffnessUpdatingPolicy>(
      "StiffnessUpdatingPolicy")
      .value("CONSTANTSTIFFNESS",
             mtest::StiffnessUpdatingPolicy::CONSTANTSTIFFNESS)
      .value("CONSTANTSTIFFNESSBYPERIOD",
             mtest::StiffnessUpdatingPolicy::CONSTANTSTIFFNESSBYPERIOD)
      .value("UPDATEDSTIFFNESSMATRIX",
             mtest::StiffnessUpdatingPolicy::UPDATEDSTIFFNESSMATRIX);

  boost::python::enum_<mtest::PredictionPolicy>("PredictionPolicy")
      .value("NOPREDICTION", mtest::PredictionPolicy::NOPREDICTION)
      .value("LINEARPREDICTION", mtest::PredictionPolicy::LINEARPREDICTION)
      .value("ELASTICPREDICTION", mtest::PredictionPolicy::ELASTICPREDICTION)
      .value("SECANTOPERATORPREDICTION",
             mtest::PredictionPolicy::SECANTOPERATORPREDICTION)
      .value("TANGENTOPERATORPREDICTION",
             mtest::PredictionPolicy::TANGENTOPERATORPREDICTION);

  boost::python::enum_<mtest::StiffnessMatrixType>("StiffnessMatrixType")
      .value("NOSTIFFNESS", mtest::StiffnessMatrixType::NOSTIFFNESS)
      .value("ELASTIC", mtest::StiffnessMatrixType::ELASTIC)
      .value("SECANTOPERATOR", mtest::StiffnessMatrixType::SECANTOPERATOR)
      .value("TANGENTOPERATOR", mtest::StiffnessMatrixType::TANGENTOPERATOR)
      .value("CONSISTENTTANGENTOPERATOR",
             mtest::StiffnessMatrixType::CONSISTENTTANGENTOPERATOR);

  declareMaterialProperty();
  declareBehaviour();
  declareCurrentState();
  declareStructureCurrentState();
  declareStudyCurrentState();
  declareSolverWorkSpace();
  declareMTestParser();
  declareSchemeBase();
  declareSingleStructureScheme();
  declareConstraint();
  declareMTest();
  declarePipeMesh();
  declarePipeTest();
  declareMFrontLogStream();

  void (*ptr)(const std::string&) = mtest::setRoundingMode;
  void (*ptr2)() = mtest::setRoundingMode;
  boost::python::def("setRoundingMode", ptr);
  boost::python::def("setRoundingMode", ptr2);
}
