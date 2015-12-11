/*!
 * \file   PipeTest.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   07 déc. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<memory>
#include<limits>
#include<boost/python.hpp>

#include"MTest/Evolution.hxx"
#include"MTest/StudyCurrentState.hxx"
#include"MTest/SolverWorkSpace.hxx"
#include"MTest/PipeTest.hxx"

static void
PipeTest_setInnerPressureEvolution(mtest::PipeTest& t,
				   const mtest::real& v)
{
  t.setInnerPressureEvolution(mtest::make_evolution(v));
} // end of PipeTest_setInnerPressureEvolution

static void
PipeTest_setInnerPressureEvolution2(mtest::PipeTest& t,
				    const std::map<mtest::real,
				                   mtest::real>& v)
{
  t.setInnerPressureEvolution(mtest::make_evolution(v));
} // end of PipeTest_setInnerPressureEvolution2

static void
PipeTest_setOuterPressureEvolution(mtest::PipeTest& t,
				   const mtest::real& v)
{
  t.setOuterPressureEvolution(mtest::make_evolution(v));
} // end of PipeTest_setOuterPressureEvolution

static void
PipeTest_setOuterPressureEvolution2(mtest::PipeTest& t,
				    const std::map<mtest::real,
				                   mtest::real>& v)
{
  t.setOuterPressureEvolution(mtest::make_evolution(v));
} // end of PipeTest_setOuterPressureEvolution2

static void
PipeTest_setAxialForceEvolution(mtest::PipeTest& t,
				const mtest::real& v)
{
  t.setAxialForceEvolution(mtest::make_evolution(v));
} // end of PipeTest_setAxialForceEvolution

static void
PipeTest_setAxialForceEvolution2(mtest::PipeTest& t,
				 const std::map<mtest::real,
				                mtest::real>& v)
{
  t.setAxialForceEvolution(mtest::make_evolution(v));
} // end of PipeTest_setAxialForceEvolution2

static void
PipeTest_setPipeModellingHypothesis(mtest::PipeTest& t,
				    const std::string& h){
  if(h=="None"){
    t.setPipeModellingHypothesis(mtest::PipeTest::NONE);
  } else if(h=="EndCapEffect"){
    t.setPipeModellingHypothesis(mtest::PipeTest::ENDCAPEFFECT);
  } else if(h=="ImposedAxialForce"){
    t.setPipeModellingHypothesis(mtest::PipeTest::IMPOSEDAXIALFORCE);
  } else {
    throw(std::runtime_error("PipeTest::setPipeModellingHypothesis: "
			     "invalid pipe modelling hypothesis ('"+h+"').\n"
			     "Valid modelling hypothesis are "
			     "'None' and 'EndCapEffect'"));
  }
} // end of PipeTest_setPipeModellingHypothesis

void declarePipeTest(void);

void declarePipeTest(void)
{
  using namespace mtest;
  tfel::tests::TestResult (PipeTest:: *pm)(void) = &PipeTest::execute;
  void (PipeTest:: *pm2)(StudyCurrentState&,
  			 SolverWorkSpace&,
  			 const real,
  			 const real) const = &PipeTest::execute;
  boost::python::class_<PipeTest,boost::noncopyable,
			boost::python::bases<SingleStructureScheme>>("PipeTest")
    .def("setInnerRadius",&PipeTest::setInnerRadius)
    .def("setOuterRadius",&PipeTest::setOuterRadius)
    .def("setNumberOfElements",&PipeTest::setNumberOfElements)
    .def("getNumberOfUnknowns",&PipeTest::getNumberOfUnknowns)
    .def("completeInitialisation",&PipeTest::completeInitialisation)
    .def("execute",pm)
    .def("execute",pm2)
    .def("initializeCurrentState",
	 &PipeTest::initializeCurrentState)
    .def("initializeWorkSpace",
	 &PipeTest::initializeWorkSpace)
    .def("setInnerPressureEvolution",&PipeTest_setInnerPressureEvolution)
    .def("setInnerPressureEvolution",&PipeTest_setInnerPressureEvolution2)
    .def("setOuterPressureEvolution",&PipeTest_setOuterPressureEvolution)
    .def("setOuterPressureEvolution",&PipeTest_setOuterPressureEvolution2)
    .def("setAxialForceEvolution",&PipeTest_setAxialForceEvolution)
    .def("setAxialForceEvolution",&PipeTest_setAxialForceEvolution2)
    .def("performSmallStrainAnalysis",&PipeTest::performSmallStrainAnalysis)
    .def("setDisplacementEpsilon",&PipeTest::setDisplacementEpsilon)
    .def("setResidualEpsilon",&PipeTest::setResidualEpsilon)
    .def("setPipeModellingHypothesis",&PipeTest_setPipeModellingHypothesis)
    ;

}
