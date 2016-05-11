/*!
 * \file   StudyCurrentState.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   08 déc. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>
#include"MTest/StudyCurrentState.hxx"
#include"MTest/StructureCurrentState.hxx"

void declareStudyCurrentState(void);

static mtest::StructureCurrentState&
getStructureCurrentState(mtest::StudyCurrentState& s,
			 const std::string& n){
  return s.getStructureCurrentState(n);
}


void declareStudyCurrentState(void)
{
  using mtest::StudyCurrentState;
    
  boost::python::class_<StudyCurrentState>("StudyCurrentState")
    .def_readonly("u_1",&StudyCurrentState::u_1)
    .def_readonly("u0",&StudyCurrentState::u0)
    .def_readonly("u1",&StudyCurrentState::u1)
    .def_readonly("u10",&StudyCurrentState::u10)
    .def_readonly("period",&StudyCurrentState::period)
    .def_readonly("iterations",&StudyCurrentState::iterations)
    .def_readonly("subSteps",&StudyCurrentState::subSteps)
    .def_readonly("dt_1",&StudyCurrentState::dt_1)
    .def("getStructureCurrentState",getStructureCurrentState,
	 boost::python::return_internal_reference<>())
    ;

}
