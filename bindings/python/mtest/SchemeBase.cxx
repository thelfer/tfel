/*!
 * \file   SchemeBase.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   09 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <boost/python.hpp>
#include "MTest/SchemeBase.hxx"
#include "MTest/StudyCurrentState.hxx"

static void setPredictionPolicy(mtest::SchemeBase& s, const std::string& p) {
  if (p == "NoPrediction") {
    s.setPredictionPolicy(mtest::PredictionPolicy::NOPREDICTION);
  } else if (p == "LinearPrediction") {
    s.setPredictionPolicy(mtest::PredictionPolicy::LINEARPREDICTION);
  } else if (p == "ElasticPrediction") {
    s.setPredictionPolicy(mtest::PredictionPolicy::ELASTICPREDICTION);
  } else if (p == "SecantOperatorPrediction") {
    s.setPredictionPolicy(mtest::PredictionPolicy::SECANTOPERATORPREDICTION);
  } else if (p == "TangentOperatorPrediction") {
    s.setPredictionPolicy(mtest::PredictionPolicy::TANGENTOPERATORPREDICTION);
  } else {
    throw(
        std::runtime_error("SchemeBase::setPredictionPolicy: "
                           "invalid policy '" +
                           p + "'"));
  }
}  // end of setPredictionPolicy

static void setStiffnessMatrixType(mtest::SchemeBase& s, const std::string& t) {
  if (t == "NoStiffness") {
    s.setStiffnessMatrixType(mtest::StiffnessMatrixType::NOSTIFFNESS);
  } else if (t == "Elastic") {
    s.setStiffnessMatrixType(mtest::StiffnessMatrixType::ELASTIC);
  } else if (t == "SecantOperator") {
    s.setStiffnessMatrixType(mtest::StiffnessMatrixType::SECANTOPERATOR);
  } else if (t == "TangentOperator") {
    s.setStiffnessMatrixType(mtest::StiffnessMatrixType::TANGENTOPERATOR);
  } else if (t == "ConsistentTangentOperator") {
    s.setStiffnessMatrixType(
        mtest::StiffnessMatrixType::CONSISTENTTANGENTOPERATOR);
  } else {
    throw(
        std::runtime_error("SchemeBase::setStiffnessMatrixType: "
                           "invalid type '" +
                           t + "'"));
  }
}  // end of setStiffnessMatrixType

static void setStiffnessUpdatingPolicy(mtest::SchemeBase& s,
                                       const std::string& p) {
  if (p == "CONSTANTSTIFFNESS") {
    s.setStiffnessUpdatingPolicy(
        mtest::StiffnessUpdatingPolicy::CONSTANTSTIFFNESS);
  } else if (p == "CONSTANTSTIFFNESSBYPERIOD") {
    s.setStiffnessUpdatingPolicy(
        mtest::StiffnessUpdatingPolicy::CONSTANTSTIFFNESSBYPERIOD);
  } else if (p == "UPDATEDSTIFFNESSMATRIX") {
    s.setStiffnessUpdatingPolicy(
        mtest::StiffnessUpdatingPolicy::UPDATEDSTIFFNESSMATRIX);
  } else {
    throw(
        std::runtime_error("SchemeBase::setStiffnessUpdatingPolicy: "
                           "invalid policy '" +
                           p + "'"));
  }
}

static void setOutputFrequency(mtest::SchemeBase& s, const std::string& v) {
  if (v == "UserDefinedTimes") {
    s.setOutputFrequency(mtest::SchemeBase::USERDEFINEDTIMES);
  } else if (v == "EveryPeriod") {
    s.setOutputFrequency(mtest::SchemeBase::EVERYPERIOD);
  } else {
    throw(
        std::runtime_error("SchemeBase::setOutputFrequency: "
                           "invalid frequency '" +
                           v + "'"));
  }
}

static void SchemeBase_printOutput(mtest::SchemeBase& s,
                                   const mtest::real t,
                                   const mtest::StudyCurrentState& scs) {
  s.printOutput(t, scs, true);
}

void declareSchemeBase();

void declareSchemeBase() {
  using namespace mtest;

  boost::python::class_<SchemeBase, boost::noncopyable>("SchemeBase",
                                                        boost::python::no_init)
      .def("setDescription", &SchemeBase::setDescription,
           "This method describes the test  which is implemented "
           "in a mtest file.\n"
           "* The parameter (string) is the description of the "
           "test.")
      .def("setAuthor", &SchemeBase::setAuthor,
           "This method is used give the name of the person who "
           "wrote the mtest file.\n"
           "* The parameter (string) is the author's name.")
      .def("setDate", &SchemeBase::setDate,
           "This method allows the user to precise when the mtest "
           "file was written.\n"
           "* The parameter (string) is the mtest's creation date.")
      .def("setPredictionPolicy", &SchemeBase::setPredictionPolicy,
           "This method specify how the initial estimate of the "
           "solution shall be obtained.\n"
           "* The parameter (PredictionPolicy) is the selected "
           "policy. The policies allowed are:\n"
           "- NOPREDICTION\n"
           "- LINEARPREDICTION\n"
           "- ELASTICPREDICTION\n"
           "- SECANTOPERATORPREDICTION\n"
           "- TANGENTOPERATORPREDICTION\n")
      .def("setPredictionPolicy", setPredictionPolicy,
           "This method specify how the initial estimate of the "
           "solution shall be obtained.\n"
           "* The parameter (string) is the selected policy. "
           "The policies allowed are:\n"
           "- 'NoPrediction'\n"
           "- 'LinearPrediction'\n"
           "- 'ElasticPrediction'\n"
           "- 'ElasticPredictionFromMaterialProperties'\n"
           "- 'SecantOperatorPrediction'\n"
           "- 'TangentOperatorPrediction'\n"
           "Note : The 'ElasticPredictionFromMaterialProperties' "
           "policy is only supported by the 'umat' interface as "
           "the 'Cast3M' finite element solver requires the "
           "stiffness matrix to be defined.")
      .def("setStiffnessMatrixType", &SchemeBase::setStiffnessMatrixType,
           "This method specify the type of stiffness matrix that "
           "shall be given by the mechanical behaviour and which "
           "will be used by the resolution algorithm.\n"
           "* The parameter (StiffnessMatrixType) is the selected "
           "type of matrix. The allowed types are:\n"
           "- NOSTIFFNESS\n"
           "- ELASTIC\n"
           "- SECANTOPERATOR\n"
           "- TANGENTOPERATOR\n"
           "- CONSISTENTTANGENTOPERATOR")
      .def("setStiffnessMatrixType", setStiffnessMatrixType,
           "This method specify the type of stiffness matrix that "
           "shall be given by the mechanical behaviour and which "
           "will be used by the resolution algorithm.\n"
           "* The parameter (string) is the selected type of "
           "matrix. The allowed types are:\n"
           "- 'NoStiffness'\n"
           "- 'Elastic' (undammaged)\n"
           "- 'SecantOperator' (damaged)\n"
           "- 'TangentOperator'\n"
           "- 'ConsistentTangentOperator'")
      .def("setStiffnessUpdatingPolicy",
           &SchemeBase::setStiffnessUpdatingPolicy)
      .def("setStiffnessUpdatingPolicy", setStiffnessUpdatingPolicy)
      .def("setUseCastemAccelerationAlgorithm",
           &SchemeBase::setUseCastemAccelerationAlgorithm,
           "This method specifies if the acceleration algorithm "
           "introduced by the resolution procedures of the castem "
           "finite element shall be used.\n"
           "* If the parameter (bool) is 'true', the algorithm "
           "will be used. Otherwise, it wont be.")
      .def("setCastemAccelerationTrigger",
           &SchemeBase::setCastemAccelerationTrigger,
           "This method specifies iterations number "
           "before the first call to the acceleration algorithm.\n"
           "* The parameter (int) specify the iterations number.")
      .def("setCastemAccelerationPeriod",
           &SchemeBase::setCastemAccelerationPeriod,
           "This method specifies iterations number "
           "between two calls to the acceleration algorithm.\n"
           "* The parameter (int) specify the iterations number.")
      .def("setAccelerationAlgorithm", &SchemeBase::setAccelerationAlgorithm,
           "This method specify the name of the acceleration "
           "algorithm used to accelerate the convergence toward "
           "equilibrium.\n"
           "* The parameter (string) is the algorithm's name. "
           "The following algorithms are available:\n"
           "- 'Cast3M'\n"
           "- 'Secant'\n"
           "- 'Steffensen'\n"
           "- 'Irons-Tuck'")
      .def("setAccelerationParameter",
           &SchemeBase::setAccelerationAlgorithmParameter,
           "This method fine-tune the acceleration algorithm "
           "used to reach equilibrium. "
           "* The first parameter (string) is the name of the "
           "parameter.\n"
           "* The second parameter (string) is the value of "
           "this parameter.")
      .def("setMaximumNumberOfIterations",
           &SchemeBase::setMaximumNumberOfIterations,
           "This method specify the maximum number of iterations "
           "allowed to the global algorithm to reach the equilibrium.\n"
           "* The parameter (uint) is the maximum number of iterations.\n"
           "If the number of iterations reaches the maximal authorized value, "
           "the time step is divided by two. The maximum number of sub-steps "
           "can be specified using the 'setMaximumNumberOfSubSteps' method.")
      .def("setMaximumNumberOfSubSteps",
           &SchemeBase::setMaximumNumberOfSubSteps,
           "This method specify the maximum number of sub-steps allowed.\n"
           "* The parameter (uint) is the maximum number of sub-steps.\n"
           "When the global algorithm fails to reach equilibrium (see "
           "the 'setMaximumNumberOfIterations' method), the time step can "
           "be divided by two. The maximal number of times the time step "
           "is reduced if given by maximum number of sub steps.")
      .def("setModellingHypothesis", &SchemeBase::setModellingHypothesis,
           "This method specify the modelling hypothesis to use.\n"
           "* The parameter (string) is the selected modelling "
           "hypothesis. The available modeling hypothesis (including "
           "the components of symmetric tensors) are:\n"
           "- AxisymmetricalGeneralisedPlaneStrain (1D) : ERR EZZ ETT\n"
           "- Axisymmetrical                       (2D) : ERR EZZ ETT ERZ\n"
           "- PlaneStress                          (2D) : EXX EYY EZZ EXY\n"
           "- PlaneStrain                          (2D) : EXX EYY EZZ EXY\n"
           "- GeneralisedPlaneStrain               (2D) : EXX EYY EZZ EXY\n"
           "- Tridimensional                       (3D) : EXX EYY EZZ EXY EXZ "
           "EYZ")
      .def("setTimes", &SchemeBase::setTimes,
           "This method specify a list of times used for the "
           "computations.\n "
           "* The parameter (vector<double>) is the specified "
           "list of times.\n"
           "Notes : The loadings and the external state variables "
           "evolutions are independently described (see the "
           "'setImposedStrain', 'setImposedStress' and the "
           "'setExternalStateVariable' methods).\n"
           "In case of non convergence, the algorithm handles sub "
           "steppings time. See the 'setMaximumNumberOfSubSteps' "
           "method for details.")
      .def("setOutputFileName", &SchemeBase::setOutputFileName,
           "This method specifiy the name of the output file.\n"
           "* The parameter (string) is the selected name of the "
           "output file. Generaly, it's similar to the input file, "
           "replacing the optionnal .mtest' extension by '.res'.")
      .def("setOutputFilePrecision", &SchemeBase::setOutputFilePrecision,
           "This method specify the number of digits used to print "
           "the results in the output file.\n"
           "* The parameter (uint) is the number of digits wanted.")
      .def("printOutput", &SchemeBase::printOutput)
      .def("printOutput", &SchemeBase_printOutput)
      .def("setResidualFileName", &SchemeBase::setResidualFileName,
           "This method specify the name of a file where the "
           "residuals at each iteration will be printed.\n"
           "* The parameter (string) is the name of the choosen file.")
      .def("setResidualFilePrecision", &SchemeBase::setResidualFilePrecision,
           "This method specify the number of significant digits "
           "used when printing residuals at each iteration.\n"
           "* The parameter (uint) is the number of digits wanted.\n"
           "Note : By default, the precision used for printing the "
           "residuals is the same than the one used for printing results "
           "(see the 'setOutputFilePrecision' method for details).")
      .def("setDynamicTimeStepScaling", &SchemeBase::setDynamicTimeStepScaling,
           "This method enable or disable the dynamic time step "
           "adaptation.\n"
           "* If the parameter (bool) is 'true', the dynamic time "
           "step adaptation is enabled. Otherwise, it's disabled.")
      .def("setMaximalTimeStep", &SchemeBase::setMaximalTimeStep,
           "This method define the maximal time step allowed.\n"
           "* The parameter (double) specify the maximal time step.")
      .def("setMinimalTimeStep", &SchemeBase::setMinimalTimeStep,
           "This method define the minimal time step allowed.\n"
           "* The parameter (double) specify the minimal time step.")
      .def("setMaximalTimeStepScalingFactor",
           &SchemeBase::setMaximalTimeStepScalingFactor,
           "This method define the maximal time step scaling factor "
           "allowed during automatic time step adaptation (see the "
           "'setDynamicTimeStepScaling' method for details).\n"
           "* The parameter (double) is the maximal time step "
           "scaling factor wich must be greater than one.")
      .def("setMinimalTimeStepScalingFactor",
           &SchemeBase::setMinimalTimeStepScalingFactor,
           "This method define the minimal time step scaling factor "
           "allowed during automatic time step adaptation (see the "
           "'setDynamicTimeStepScaling' method for details).\n"
           "* The parameter (double) is the minimal time step "
           "scaling factor wich must be positive and lower than one.")
      .def("setOutputFrequency", setOutputFrequency,
           "This method specify when the results are written in the "
           "output files.\n"
           "* The parameter (string) specify the choosen option. "
           "The two allowed options are:\n"
           "- 'UserDefinedTimes': the results are written at each "
           "user defined time (default).\n"
           "- 'EveryIteration': the outputs are written after each "
           "successful iteration.\n"
           "Note : These options only differs in case of substepping.");
}
