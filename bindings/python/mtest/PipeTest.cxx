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
#include"MTest/PipeProfile.hxx"
#include"MTest/PipeProfileHandler.hxx"
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
PipeTest_setAxialGrowthEvolution(mtest::PipeTest& t,
				const mtest::real& v)
{
  t.setAxialGrowthEvolution(mtest::make_evolution(v));
} // end of PipeTest_setAxialGrowthEvolution

static void
PipeTest_setAxialGrowthEvolution2(mtest::PipeTest& t,
				 const std::map<mtest::real,
				                mtest::real>& v)
{
  t.setAxialGrowthEvolution(mtest::make_evolution(v));
} // end of PipeTest_setAxialGrowthEvolution2

static void
PipeTest_setAxialLoading(mtest::PipeTest& t,
				    const std::string& h){
  if(h=="None"){
    t.setAxialLoading(mtest::PipeTest::NONE);
  } else if(h=="EndCapEffect"){
    t.setAxialLoading(mtest::PipeTest::ENDCAPEFFECT);
  } else if(h=="ImposedAxialForce"){
    t.setAxialLoading(mtest::PipeTest::IMPOSEDAXIALFORCE);
  } else if(h=="ImposedAxialGrowth"){
    t.setAxialLoading(mtest::PipeTest::IMPOSEDAXIALGROWTH);
  } else {
    throw(std::runtime_error("PipeTest::setAxialLoading: "
			     "invalid pipe modelling hypothesis ('"+h+"').\n"
			     "Valid modelling hypothesis are "
			     "'None', 'ImposedAxialForce' and 'EndCapEffect'"));
  }
} // end of PipeTest_setAxialLoading

static void
PipeTest_setRadialLoading(mtest::PipeTest& t,
			const std::string& h){
  if(h=="ImposedPressure"){
    t.setRadialLoading(mtest::PipeTest::IMPOSEDPRESSURE);
  } else if(h=="ImposedOuterRadius"){
    t.setRadialLoading(mtest::PipeTest::IMPOSEDOUTERRADIUS);
  } else if(h=="TightPipe"){
    t.setRadialLoading(mtest::PipeTest::TIGHTPIPE);
  } else {
    throw(std::runtime_error("PipeTest::setRadialLoading: "
			     "invalid pipe loading type ('"+h+"').\n"
			     "Valid loading type are "
			     "'ImposedPressure', 'TightPipe' "
			     "and 'ImposedOuterRadius'"));
  }
} // end of PipeTest_setRadialLoading

static void
PipeTest_setOuterRadiusEvolution(mtest::PipeTest& t,
				 const mtest::real& v)
{
  t.setOuterRadiusEvolution(mtest::make_evolution(v));
} // end of PipeTest_setOuterRadiusEvolution

static void
PipeTest_setOuterRadiusEvolution2(mtest::PipeTest& t,
				  const std::map<mtest::real,
				                 mtest::real>& v)
{
  t.setOuterRadiusEvolution(mtest::make_evolution(v));
} // end of PipeTest_setOuterRadiusEvolution2

static void
PipeTest_setElementType(mtest::PipeTest& t,
			const std::string& e){
  if(e=="Linear"){
    t.setElementType(mtest::PipeMesh::LINEAR);
  } else if(e=="Quadratic"){
    t.setElementType(mtest::PipeMesh::QUADRATIC);
  } else if(e=="Cubic"){
    t.setElementType(mtest::PipeMesh::CUBIC);
  } else {
    throw(std::runtime_error("PipeTest::setElementType: "
			     "invalid element type ('"+e+"').\n"
			     "Valid element type are "
			     "'Linear' and 'Quadratic'"));
  }
} // end of PipeTest_setElementType

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
    .def("setAxialGrowthEvolution",&PipeTest_setAxialGrowthEvolution)
    .def("setAxialGrowthEvolution",&PipeTest_setAxialGrowthEvolution2)
    .def("setOuterRadiusEvolution",&PipeTest_setOuterRadiusEvolution)
    .def("setOuterRadiusEvolution",&PipeTest_setOuterRadiusEvolution2)
    .def("setFillingPressure",&PipeTest::setFillingPressure)
    .def("setFillingTemperature",&PipeTest::setFillingTemperature)
    .def("performSmallStrainAnalysis",&PipeTest::performSmallStrainAnalysis)
    .def("setDisplacementEpsilon",&PipeTest::setDisplacementEpsilon)
    .def("setResidualEpsilon",&PipeTest::setResidualEpsilon)
    .def("setAxialLoading",&PipeTest_setAxialLoading)
    .def("setRadialLoading",&PipeTest_setRadialLoading)
    .def("setElementType",&PipeTest_setElementType)
    .def("addProfile",&PipeTest::addProfile)
    ;

}
