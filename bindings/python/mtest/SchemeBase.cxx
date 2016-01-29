/*!
 * \file   SchemeBase.cxx
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

#include<string>
#include<boost/python.hpp>
#include"MTest/SchemeBase.hxx"
#include"MTest/StudyCurrentState.hxx"

static void
setPredictionPolicy(mtest::SchemeBase& s,const std::string& p){
  if (p=="NoPrediction"){
    s.setPredictionPolicy(mtest::PredictionPolicy::NOPREDICTION);
  } else if (p=="LinearPrediction"){
    s.setPredictionPolicy(mtest::PredictionPolicy::LINEARPREDICTION);
  } else if (p=="ElasticPrediction"){
    s.setPredictionPolicy(mtest::PredictionPolicy::ELASTICPREDICTION);
  } else if (p=="SecantOperatorPrediction"){
    s.setPredictionPolicy(mtest::PredictionPolicy::SECANTOPERATORPREDICTION);
  } else if (p=="TangentOperatorPrediction"){
    s.setPredictionPolicy(mtest::PredictionPolicy::TANGENTOPERATORPREDICTION);
  } else {
    throw(std::runtime_error("SchemeBase::setPredictionPolicy: "
			     "invalid policy '"+p+"'"));
  }
} // end of setPredictionPolicy

static void
setStiffnessMatrixType(mtest::SchemeBase& s,const std::string& t){
  if (t=="NoStiffness"){
    s.setStiffnessMatrixType(mtest::StiffnessMatrixType::NOSTIFFNESS);
  } else if (t=="Elastic"){
    s.setStiffnessMatrixType(mtest::StiffnessMatrixType::ELASTIC);
  } else if (t=="SecantOperator"){
    s.setStiffnessMatrixType(mtest::StiffnessMatrixType::SECANTOPERATOR);
  } else if (t=="TangentOperator"){
    s.setStiffnessMatrixType(mtest::StiffnessMatrixType::TANGENTOPERATOR);
  } else if (t=="ConsistentTangentOperator"){
    s.setStiffnessMatrixType(mtest::StiffnessMatrixType::CONSISTENTTANGENTOPERATOR);
  } else {
    throw(std::runtime_error("SchemeBase::setStiffnessMatrixType: "
			     "invalid type '"+t+"'"));
  }
} // end of setStiffnessMatrixType

static void
setStiffnessUpdatingPolicy(mtest::SchemeBase& s,const std::string& p){
  if (p=="CONSTANTSTIFFNESS"){
    s.setStiffnessUpdatingPolicy(mtest::StiffnessUpdatingPolicy::CONSTANTSTIFFNESS);
  } else if (p=="CONSTANTSTIFFNESSBYPERIOD"){
    s.setStiffnessUpdatingPolicy(mtest::StiffnessUpdatingPolicy::CONSTANTSTIFFNESSBYPERIOD);
  } else if (p=="UPDATEDSTIFFNESSMATRIX"){
    s.setStiffnessUpdatingPolicy(mtest::StiffnessUpdatingPolicy::UPDATEDSTIFFNESSMATRIX);
  } else {
    throw(std::runtime_error("SchemeBase::setStiffnessUpdatingPolicy: "
			     "invalid policy '"+p+"'"));
  }
}

static void
setOutputFrequency(mtest::SchemeBase& s,const std::string& v){
  if(v=="UserDefinedTimes"){
    s.setOutputFrequency(mtest::SchemeBase::USERDEFINEDTIMES);
  } else if(v=="EveryPeriod"){
    s.setOutputFrequency(mtest::SchemeBase::EVERYPERIOD);
  } else {
    throw(std::runtime_error("SchemeBase::setOutputFrequency: "
			       "invalid frequency '"+v+"'"));
  }
}

static void
SchemeBase_printOutput(mtest::SchemeBase& s,
		       const mtest::real t,
		       const mtest::StudyCurrentState& scs){
  s.printOutput(t,scs,true);
}



void declareSchemeBase(void);

void declareSchemeBase(void)
{
  using namespace mtest;

  boost::python::class_<SchemeBase,boost::noncopyable>("SchemeBase",
						       boost::python::no_init)
    .def("setDescription",&SchemeBase::setDescription)
    .def("setAuthor",&SchemeBase::setAuthor)
    .def("setDate",&SchemeBase::setDate)
    .def("setPredictionPolicy",&SchemeBase::setPredictionPolicy)
    .def("setPredictionPolicy",setPredictionPolicy)
    .def("setStiffnessMatrixType",
	 &SchemeBase::setStiffnessMatrixType)
    .def("setStiffnessMatrixType",setStiffnessMatrixType)
    .def("setStiffnessUpdatingPolicy",
	 &SchemeBase::setStiffnessUpdatingPolicy)
    .def("setStiffnessUpdatingPolicy",setStiffnessUpdatingPolicy)
    .def("setUseCastemAccelerationAlgorithm",
	 &SchemeBase::setUseCastemAccelerationAlgorithm)
    .def("setCastemAccelerationTrigger",
	 &SchemeBase::setCastemAccelerationTrigger)
    .def("setCastemAccelerationPeriod",
	 &SchemeBase::setCastemAccelerationPeriod)
    .def("setAccelerationAlgorithm",
	 &SchemeBase::setAccelerationAlgorithm)
    .def("setAccelerationParameter",
	 &SchemeBase::setAccelerationAlgorithmParameter)
    .def("setMaximumNumberOfIterations",
	 &SchemeBase::setMaximumNumberOfIterations)
    .def("setMaximumNumberOfSubSteps",
	 &SchemeBase::setMaximumNumberOfSubSteps)
    .def("setModellingHypothesis",
	 &SchemeBase::setModellingHypothesis)
    .def("setTimes",&SchemeBase::setTimes)
    .def("setOutputFileName",
	 &SchemeBase::setOutputFileName)
    .def("setOutputFilePrecision",
	 &SchemeBase::setOutputFilePrecision)
    .def("printOutput",
	 &SchemeBase::printOutput)
    .def("printOutput",
	 &SchemeBase_printOutput)
    .def("setResidualFileName",
	 &SchemeBase::setResidualFileName)
    .def("setResidualFilePrecision",
	 &SchemeBase::setResidualFilePrecision)
    .def("setDynamicTimeStepScaling",
	 &SchemeBase::setDynamicTimeStepScaling)
    .def("setMaximalTimeStep",
	 &SchemeBase::setMaximalTimeStep)
    .def("setMinimalTimeStep",
	 &SchemeBase::setMinimalTimeStep)
    .def("setMaximalTimeStepScalingFactor",
	 &SchemeBase::setMaximalTimeStepScalingFactor)
    .def("setMinimalTimeStepScalingFactor",
	 &SchemeBase::setMinimalTimeStepScalingFactor)
    .def("setOutputFrequency",
	 setOutputFrequency)
    ;

}
