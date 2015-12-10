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

#include<boost/python.hpp>
#include"MTest/SchemeBase.hxx"

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
    .def("setStiffnessMatrixType",
	 &SchemeBase::setStiffnessMatrixType)
    .def("setStiffnessUpdatingPolicy",
	 &SchemeBase::setStiffnessUpdatingPolicy)
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
    .def("setResidualFileName",
	 &SchemeBase::setResidualFileName)
    .def("setResidualFilePrecision",
	 &SchemeBase::setResidualFilePrecision)
    ;

}
