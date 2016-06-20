/*! 
 * \file  bindings/python/mfront/MTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 sept. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>

#include"MTest/MTest.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/StructureCurrentState.hxx"
#include"MTest/Constraint.hxx"
#include"MTest/ImposedThermodynamicForce.hxx"
#include"MTest/ImposedDrivingVariable.hxx"

namespace mtest{

  struct MTestCurrentState
    : public StudyCurrentState
  {};

}

static void
MTest_setImposedThermodynamicForce(mtest::MTest& t,
				   const std::string&  n,
				   const mtest::real& v)
{
  using namespace std;
  using namespace mtest;
  using std::shared_ptr; 
  shared_ptr<Constraint> sc;
  shared_ptr<Evolution> sev(new ConstantEvolution(v));
  sc = shared_ptr<Constraint>(new ImposedThermodynamicForce(*(t.getBehaviour()),
							    n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedThermodynamicForce

static void
MTest_setImposedThermodynamicForce2(mtest::MTest& t,
				   const std::string&  n,
				   const std::map<mtest::real,
						  mtest::real>& v)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  shared_ptr<Constraint> sc;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<Evolution> sev(new LPIEvolution(tv,ev));
  sc = shared_ptr<Constraint>(new ImposedThermodynamicForce(*(t.getBehaviour()),
							    n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedThermodynamicForce

static void
MTest_setImposedStress(mtest::MTest& t,
		       const std::string&  n,
		       const mtest::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if((t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
     (t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
    string msg("MTest::handleImposedStress : "
	       "the setImposedStress method is only valid "
	       "for small and finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce(t,n,v);
}

static void
MTest_setImposedStress2(mtest::MTest& t,
			const std::string&  n,
			const std::map<mtest::real,
			mtest::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if((t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
     (t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
    string msg("MTestParser::handleImposedStress : "
	       "the setImposedStress method is only valid "
	       "for small and finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce2(t,n,v);
}

static void
MTest_setImposedCohesiveForce(mtest::MTest& t,
			      const std::string&  n,
			      const mtest::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedCohesiveForce : "
	       "the setImposedCohesiveForce method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce(t,n,v);
}

static void
MTest_setImposedCohesiveForce2(mtest::MTest& t,
			       const std::string&  n,
			       const std::map<mtest::real,
			       mtest::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedCohesiveForce : "
	       "the setImposedCohesiveForce method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce2(t,n,v);
}

static void
MTest_setImposedDrivingVariable(mtest::MTest& t,
				const std::string&  n,
				const mtest::real& v)
{
  using namespace std;
  using namespace mtest;
  shared_ptr<Constraint> sc;
  shared_ptr<Evolution> sev(new ConstantEvolution(v));
  sc = shared_ptr<Constraint>(new ImposedDrivingVariable(*(t.getBehaviour()),n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedDrivingVariable

static void
MTest_setImposedDrivingVariable2(mtest::MTest& t,
				 const std::string&  n,
				 const std::map<mtest::real,
				 mtest::real>& v)
{
  using namespace std;
  using namespace mtest;
  using mtest::real;
  shared_ptr<Constraint> sc;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<Evolution> sev(new LPIEvolution(tv,ev));
  sc = shared_ptr<Constraint>(new ImposedDrivingVariable(*(t.getBehaviour()),n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedDrivingVariable

static void
MTest_setImposedStrain(mtest::MTest& t,
		       const std::string&  n,
		       const mtest::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedStrain : "
	       "the setImposedStrain method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable(t,n,v);
}

static void
MTest_setImposedStrain2(mtest::MTest& t,
			const std::string&  n,
			const std::map<mtest::real,
			mtest::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedStrain : "
	       "the setImposedStrain method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable2(t,n,v);
}

static void
MTest_setImposedDeformationGradient(mtest::MTest& t,
		       const std::string&  n,
		       const mtest::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedDeformationGradient : "
	       "the setImposedDeformationGradient method is only valid "
	       "for finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable(t,n,v);
}

static void
MTest_setImposedDeformationGradient2(mtest::MTest& t,
			const std::string&  n,
			const std::map<mtest::real,
			mtest::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedDeformationGradient : "
	       "the setImposedDeformationGradient method is only valid "
	       "for finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable2(t,n,v);
}

static void
MTest_setImposedOpeningDisplacement(mtest::MTest& t,
				    const std::string&  n,
				    const mtest::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedOpeningDisplacement : "
	       "the setImposedOpeningDisplacement method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable(t,n,v);
}

static void
MTest_setImposedOpeningDisplacement2(mtest::MTest& t,
				     const std::string&  n,
				     const std::map<mtest::real,
				     mtest::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedOpeningDisplacement : "
	       "the setImposedOpeningDisplacement method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable2(t,n,v);
}

static mtest::MTestCurrentState
MTestCurrentState_copy(const mtest::MTestCurrentState& src)
{
  return src;
}

static void
MTest_setRotationMatrix1(mtest::MTest& t,
			 const std::vector<std::vector<mtest::real> >& m,
			 const bool b){
  typedef tfel::math::tmatrix<3u,3u,mtest::real> matrix;
  matrix r;
  for(matrix::size_type i = 0; i!=3;++i){
    for(matrix::size_type j = 0; j!=3;++j){
      r(i,j) = m[i][j];
    }
  }
  t.setRotationMatrix(r,b);
} // end of MTest_setRotationMatrix

static void
MTest_setRotationMatrix2(mtest::MTest& t,
			 const std::vector<std::vector<mtest::real> >& m){
  MTest_setRotationMatrix1(t,m,false);
} // end of MTest_setRotationMatrix

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER( X )		        \
  static tfel::math::vector<mtest::real>				\
  MTestCurrentState_get##X(const mtest::MTestCurrentState& t)	\
  {                                                                     \
    return t. X ;                                                       \
  }

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER2( X )		        \
  static mtest::real			                         	\
  MTestCurrentState_get##X(const mtest::MTestCurrentState& t)	\
  {                                                                     \
    return t. X ;                                                       \
  }

TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER2(dt_1)

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER3( X )	      \
  static tfel::math::vector<mtest::real>		      \
  MTestCurrentState_get##X(const mtest::MTestCurrentState& t) \
  {                                                           \
    const auto& sc = t.getStructureCurrentState("");          \
    if(sc.istates.size()!=1){                                 \
      throw(std::runtime_error("MTestCurrentState::get: "     \
                               "uninitialized state"));       \
    }                                                         \
    return sc. istates[0]. X ;				      \
  }

TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(s_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(s0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(s1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e_th0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e_th1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(mprops1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(iv_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(iv0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(iv1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(esv0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(desv)

static mtest::real
MTestCurrentState_getTref(const mtest::MTestCurrentState& t)
{
  const auto& sc = t.getStructureCurrentState("");
  if(sc.istates.size()!=1){
    throw(std::runtime_error("MTestCurrentState::get: "
			     "uninitialized state"));
  }
  return sc. istates[0]. Tref ;
}

void declareMTest(void);

void declareMTest(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace mtest;
  using boost::python::arg;
  using tfel::tests::TestResult;

  class_<MTestCurrentState,bases<StudyCurrentState>>("MTestCurrentState")
    .def("copy",&MTestCurrentState_copy)
    .add_property("u_1",MTestCurrentState_getu_1)
    .add_property("u0",MTestCurrentState_getu0)
    .add_property("u1",MTestCurrentState_getu1)
    .add_property("s_1",MTestCurrentState_gets_1)
    .add_property("s0",MTestCurrentState_gets0)
    .add_property("s1",MTestCurrentState_gets1)
    .add_property("e0",MTestCurrentState_gete0)
    .add_property("e1",MTestCurrentState_gete1)
    .add_property("e_th0",MTestCurrentState_gete_th0)
    .add_property("e_th1",MTestCurrentState_gete_th1)
    .add_property("mprops1",MTestCurrentState_getmprops1)
    .add_property("iv_1",MTestCurrentState_getiv_1)
    .add_property("iv0",MTestCurrentState_getiv0)
    .add_property("iv1",MTestCurrentState_getiv1)
    .add_property("evs0",MTestCurrentState_getesv0)
    .add_property("desv",MTestCurrentState_getdesv)
    .add_property("dt_1",MTestCurrentState_getdt_1)
    .add_property("Tref",MTestCurrentState_getTref)
    ;
  
  TestResult (MTest:: *pm)(void) = &MTest::execute;
  void (MTest:: *pm2)(StudyCurrentState&,
		      SolverWorkSpace&,
		      const real,
		      const real) const = &MTest::execute;

  class_<MTest,noncopyable,
	 bases<SingleStructureScheme>>("MTest")
    .def("execute",pm)
    .def("execute",pm2)
    .def("completeInitialisation",
	 &MTest::completeInitialisation)
    .def("initializeCurrentState",
	 &MTest::initializeCurrentState)
    .def("initializeWorkSpace",
	 &MTest::initializeWorkSpace)
    .def("setEvolutionValue",
	 &MTest::setEvolutionValue)
    .def("setDrivingVariableEpsilon",
	 &MTest::setDrivingVariableEpsilon)
    .def("setThermodynamicForceEpsilon",
     &MTest::setThermodynamicForceEpsilon)
    .def("setImposedStress",MTest_setImposedStress,
     (arg("name"),"values"),
     "This method specify the constant evolution of a stresses component.\n"
     "* The first parameter (string) is the name of the choosen "
     "stresses component. The allowed components (see the "
     "'setModellingHypothesis' method) are:\n"
     "- 'AxisymmetricalGeneralisedPlaneStrain' : SRR SZZ STT\n"
     "- 'Axisymmetrical'                       : SRR SZZ STT SRZ\n"
     "- 'PlaneStress'                          : SXX SYY SXY\n"
     "- 'PlaneStrain'                          : SXX SYY SZZ SXY\n"
     "- 'GeneralisedPlaneStrain'               : SXX SYY SZZ SXY\n"
     "- 'Tridimensional'                       : SXX SYY SZZ SXY SXZ SYZ\n"
     "* The second parameter (double) is the constant value "
     "of the selected stresses component.")
    .def("setImposedStress",MTest_setImposedStress2,
     (arg("name"),"values"),
     "This method specify the linear evolution of a stresses component.\n"
     "* The first parameter (string) is the name of the choosen "
     "stresses component. The allowed components (see the "
     "'setModellingHypothesis' method) are:\n"
     "- 'AxisymmetricalGeneralisedPlaneStrain' : SRR SZZ STT\n"
     "- 'Axisymmetrical'                       : SRR SZZ STT SRZ\n"
     "- 'PlaneStress'                          : SXX SYY SXY\n"
     "- 'PlaneStrain'                          : SXX SYY SZZ SXY\n"
     "- 'GeneralisedPlaneStrain'               : SXX SYY SZZ SXY\n"
     "- 'Tridimensional'                       : SXX SYY SZZ SXY SXZ SYZ\n"
     "* The second parameter (map<double, double>) specify "
     "a linear evolution: the associative array of time-value "
     "is used to perform an interpolation. That interpolation "
     "is only performed between the minimum and maximum times "
     "given in the array. Should a time be out of the array, "
     "the returned value will be the one from the nearest "
     "association available.")
    .def("setImposedCohesiveForce",MTest_setImposedCohesiveForce,
     (arg("name"),"values"))
    .def("setImposedCohesiveForce",MTest_setImposedCohesiveForce2,
     (arg("name"),"values"))
    .def("setImposedThermodynamicForce",MTest_setImposedThermodynamicForce,
     (arg("name"),"values"))
    .def("setImposedThermodynamicForce",MTest_setImposedThermodynamicForce2,
     (arg("name"),"values"))
    .def("setImposedStrain",MTest_setImposedStrain,
     (arg("name"),"values"),
     "This method specify the constant evolution of a strains component.\n"
     "* The first parameter (string) is the name of the choosen "
     "strains component. The allowed components (see the "
     "'setModellingHypothesis' method) are:\n"
     "- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT\n"
     "- Axisymmetrical                       : ERR EZZ ETT ERZ\n"
     "- PlaneStress                          : EXX EYY EZZ EXY\n"
     "- PlaneStrain                          : EXX EYY     EXY\n"
     "- GeneralisedPlaneStrain               : EXX EYY EZZ EXY\n"
     "- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ\n"
     "* The second parameter (double) is the constant value "
     "of the selected strains component.")
    .def("setImposedStrain",MTest_setImposedStrain2,
     (arg("name"),"values"),
     "This method specify the linear evolution of a strains component.\n"
     "* The first parameter (string) is the name of the choosen "
     "strains component. The allowed components (see the "
     "'setModellingHypothesis' method) are:\n"
     "- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT\n"
     "- Axisymmetrical                       : ERR EZZ ETT ERZ\n"
     "- PlaneStress                          : EXX EYY EZZ EXY\n"
     "- PlaneStrain                          : EXX EYY     EXY\n"
     "- GeneralisedPlaneStrain               : EXX EYY EZZ EXY\n"
     "- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ\n"
     "* The second parameter (map<double, double>) specify "
     "a linear evolution: the associative array of time-value "
     "is used to perform an interpolation. That interpolation "
     "is only performed between the minimum and maximum times "
     "given in the array. Should a time be out of the array, "
     "the returned value will be the one from the nearest "
     "association available.")
    .def("setImposedDeformationGradient",MTest_setImposedDeformationGradient,
     (arg("name"),"values"),
     "This method specify the constant evolution of a deformation "
     "gradient component.\n"
     "* The first parameter (string) is the name of the choosen "
     "deformation gradient component. The allowed components (see "
     "the 'setModellingHypothesis' method) are:\n"
     "- 'AxisymmetricalGeneralisedPlaneStrain' : FRR FZZ FTT\n"
     "- 'Axisymmetrical'                       : FRR FZZ FTT ERZ FZR\n"
     "- 'PlaneStress'                          : FXX FYY FZZ FXY FYX\n"
     "- 'PlaneStrain'                          : FXX FYY     FXY FYX\n"
     "- 'GeneralisedPlaneStrain'               : FXX FYY FZZ FXY FYX\n"
     "- 'Tridimensional'                       : FXX FYY FZZ FXY FYX FXZ FZX FYZ FZY\n"
     "* The second parameter (double) is the constant value "
     "of the selected deformation gradient component.")
    .def("setImposedDeformationGradient",MTest_setImposedDeformationGradient2,
     (arg("name"),"values"),
     "This method specify the linear evolution of a deformation "
     "gradient component.\n"
     "* The first parameter (string) is the name of the choosen "
     "deformation gradient component. The allowed components (see "
     "the 'setModellingHypothesis' method) are:\n"
     "- 'AxisymmetricalGeneralisedPlaneStrain' : FRR FZZ FTT\n"
     "- 'Axisymmetrical'                       : FRR FZZ FTT ERZ FZR\n"
     "- 'PlaneStress'                          : FXX FYY FZZ FXY FYX\n"
     "- 'PlaneStrain'                          : FXX FYY     FXY FYX\n"
     "- 'GeneralisedPlaneStrain'               : FXX FYY FZZ FXY FYX\n"
     "- 'Tridimensional'                       : FXX FYY FZZ FXY FYX FXZ FZX FYZ FZY\n"
     "* The second parameter (map<double, double>) specify "
     "a linear evolution: the associative array of time-value "
     "is used to perform an interpolation. That interpolation "
     "is only performed between the minimum and maximum times "
     "given in the array. Should a time be out of the array, "
     "the returned value will be the one from the nearest "
     "association available.")
    .def("setImposedOpeningDisplacement",MTest_setImposedOpeningDisplacement,
     (arg("name"),"values"))
    .def("setImposedOpeningDisplacement",MTest_setImposedOpeningDisplacement2,
     (arg("name"),"values"))
    .def("setImposedDrivingVariable",MTest_setImposedDrivingVariable,
     (arg("name"),"values"))
    .def("setImposedDrivingVariable",MTest_setImposedDrivingVariable2,
     (arg("name"),"values"))
    .def("setScalarInternalStateVariableInitialValue",
	 &MTest::setScalarInternalStateVariableInitialValue)
    .def("setRotationMatrix",
     &MTest_setRotationMatrix1,
     "This method specify a rotation matrix, for the "
     "principal material directions to be different from "
     "those used for the resolution and the boundary "
     "conditions.\n"
     "* The parameter (vector<vector<double> >) specify the "
     "3x3 rotation matrix.")
    .def("setRotationMatrix",
     &MTest_setRotationMatrix2,
     "This method specify a rotation matrix, for the "
     "principal material directions to be different from "
     "those used for the resolution and the boundary "
     "conditions.\n"
     "* The parameter (vector<vector<double> >) specify the "
     "3x3 rotation matrix.")
    .def("setScalarInternalStateVariableInitialValue",
	 &MTest::setScalarInternalStateVariableInitialValue)
    .def("setStensorInternalStateVariableInitialValues",
	 &MTest::setStensorInternalStateVariableInitialValues)
    .def("setTensorInternalStateVariableInitialValues",
	 &MTest::setTensorInternalStateVariableInitialValues)
  ;

} // end of declareExternalLibraryManager
