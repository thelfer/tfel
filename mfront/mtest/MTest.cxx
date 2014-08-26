/*! 
 * \file  MTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avril 2013
 */


#include<map>
#include<cmath>
#include<string>
#include<vector>
#include<sstream>
#include<cstdlib>
#include<iterator>
#include<algorithm>
#include<stdexcept>

#include"tfel-config.hxx"

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/TextData.hxx"
#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tmatrix.hxx"

#include"MFront/MTest.hxx"
#include"MFront/MTestLogStream.hxx"
#include"MFront/MTestBehaviour.hxx"
#ifdef HAVE_CASTEM
#include"MFront/MTestUmatSmallStrainBehaviour.hxx"
#include"MFront/MTestUmatFiniteStrainBehaviour.hxx"
#include"MFront/MTestUmatCohesiveZoneModelBehaviour.hxx"
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
#include"MFront/MTestAsterSmallStrainBehaviour.hxx"
#endif /* HAVE_ASTER  */

#include"MFront/MTestAnalyticalTest.hxx"
#include"MFront/MTestReferenceFileComparisonTest.hxx"

#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestFunctionEvolution.hxx"
#include"MFront/MTestCastemEvolution.hxx"

#include"MFront/MTestConstraint.hxx"
#include"MFront/MTestImposedThermodynamicForce.hxx"
#include"MFront/MTestImposedDrivingVariable.hxx"

namespace mfront
{

  static void
  checkIfDeclared(const std::vector<std::string>& n,
		  const std::map<std::string,
				 tfel::utilities::shared_ptr<MTestEvolution> > m,
		  const std::string& type)
  {
    using namespace std;
    vector<string>::const_iterator p;
    for(p=n.begin();p!=n.end();++p){
      if(m.find(*p)==m.end()){
	string msg("no '"+type+"' named '"+*p+"' declared");
	throw(runtime_error(msg));
      }
    }
  }

  static unsigned short
  getStensorSize(const unsigned short d)
  {
    using namespace std;
    if(d==1){
      return 3;
    } else if(d==2){
      return 4;
    } else if(d==3){
      return 6;
    }
    string msg("mfront::getStensorSize : ");
    msg += "";
    throw(runtime_error(msg));
    return 0;
  }

  static unsigned short
  getTensorSize(const unsigned short d)
  {
    using namespace std;
    if(d==1){
      return 3;
    } else if(d==2){
      return 5;
    } else if(d==3){
      return 9;
    }
    string msg("mfront::getTensorSize : ");
    msg += "";
    throw(runtime_error(msg));
    return 0;
  }

  MTest::MTestCurrentState::MTestCurrentState()
  {}

  MTest::MTestCurrentState::MTestCurrentState(const MTest::MTestCurrentState& src)
    : u_1(src.u_1),
      u0(src.u0),
      u1(src.u1),
      s_1(src.s_1),
      s0(src.s0),
      s1(src.s1),
      e0(src.e0),
      e1(src.e1),
      e_th0(src.e_th0),
      e_th1(src.e_th1),
      mprops0(src.mprops0),
      mprops1(src.mprops1),
      iv_1(src.iv_1),
      iv0(src.iv0),
      iv1(src.iv1),
      esv0(src.esv0),
      desv(src.desv),
      period(src.period),
      dt_1(src.dt_1),
      Tref(src.Tref)
  {} // end of MTest::MTestCurrentState::MTestCurrentState

  MTest::MTestWorkSpace::MTestWorkSpace()
  {}

  MTest::UTest::~UTest()
  {}

  MTest::MTest()
    : oprec(-1),
      rm(real(0)),
      isRmDefined(false),
      evs(new std::map<std::string,
		       tfel::utilities::shared_ptr<MTestEvolution> >()),
      dimension(0u),
      hypothesis(tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS),
      eeps(-1.),
      seps(-1.),
      mSubSteps(-1),
      iterMax(-1),
      ks(MTest::UNSPECIFIEDSTIFFNESSUPDATINGPOLICY),
      ktype(MTestStiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE),
      ppolicy(MTest::UNSPECIFIEDPREDICTIONPOLICY),
      useCastemAcceleration(false),
      cat(-1),
      cap(-1),
      toeps(-1),
      pv(-1),
      cto(false),
      initialisationFinished(false)
  {
    // declare time variable
    this->declareVariable("t",true);
  }

  void
  MTest::setEvolutionValue(const std::string& n,
			   const real t,
			   const real v)
  {
    using namespace std;
    using tfel::utilities::shared_ptr;
    map<string,shared_ptr<MTestEvolution> >::iterator pev;
    pev = this->evs->find(n);
    if(pev==this->evs->end()){
      string msg("MTest::setEvolutionValue : no evolution '"+
		 n+"' declared");
      throw(runtime_error(msg));
    }
    MTestEvolution& ev = *(pev->second);
    ev.setValue(t,v);
  } // end of MTest::setEvolutionValue

  void
  MTest::setDescription(const std::string& d)
  {
    using namespace std;
    if(!this->description.empty()){
      string msg("MTest::setDescription : ");
      msg += "description already set.";
      throw(runtime_error(msg));
    }
    this->description = d;
  } // end of MTest::setDescription

  void
  MTest::setAuthor(const std::string& a)
  {
    using namespace std;
    if(!this->author.empty()){
      string msg("MTest::setAuthor : ");
      msg += "author already set.";
      throw(runtime_error(msg));
    }
    this->author = a;
  } // end of MTest::setAuthor

  void
  MTest::setDate(const std::string& d)
  {
    using namespace std;
    if(!this->date.empty()){
      string msg("MTest::setDate : ");
      msg += "date already set.";
      throw(runtime_error(msg));
    }
    this->date = d;
  } // end of MTest::setDate

  void
  MTest::setPredictionPolicy(const MTest::PredictionPolicy p)
  {
    using namespace std;
    if(this->ppolicy!=UNSPECIFIEDPREDICTIONPOLICY){
      string msg("MTest::setPredictionPolicy : "
		 "prediction policy already declared");
      throw(runtime_error(msg));
    }
    this->ppolicy = p;
  } // end of MTest::setPredictionPolicy

  void
  MTest::setStiffnessMatrixType(const MTestStiffnessMatrixType::mtype k)
  {
    using namespace std;
    if(this->ktype!=MTestStiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE){
      string msg("MTest::setStiffnessMatrixType : "
		 "stiffness matrix type already specificed");
      throw(runtime_error(msg));
    }
    this->ktype = k;
  }

  void
  MTest::setUseCastemAccelerationAlgorithm(const bool ucaa)
  {
    using namespace std;
    if(this->useCastemAcceleration){
      string msg("MTest::setUseCastemAccelerationAlgorithm : "
		 "useCastemAcceleration already set");
      throw(runtime_error(msg));
    }
    this->useCastemAcceleration = ucaa;
  }
  
  void
  MTest::setCastemAccelerationTrigger(const int i)
  {
    using namespace std;
    if(i!=-1){
      string msg("MTest::setCastemAccelerationTrigger : "
		 "the castem acceleration trigger has already "
		 "been defined");
      throw(runtime_error(msg));
    }
    if(i<3){
      string msg("MTest::setCastemAccelerationTrigger",
		 "invalid acceleration trigger value.");
      throw(runtime_error(msg));
    }
    this->cat = i;
  }

  void
  MTest::setCastemAccelerationPeriod(const int p)
  {
    using namespace std;
    if(p!=-1){
      string msg("MTest::setCastemAccelerationPeriod : "
		 "the castem acceleration period has already "
		 "been defined");
      throw(runtime_error(msg));
    }
    if(p==0){
      string msg("MTest::setCastemAccelerationPeriod",
		 "invalid acceleration period value.");
      throw(runtime_error(msg));
    }
    this->cap = p;
  }

  void
  MTest::setStiffnessUpdatingPolicy(const MTest::StiffnessUpdatingPolicy p)
  {
    using namespace std;
    if(this->ks!=MTest::UNSPECIFIEDSTIFFNESSUPDATINGPOLICY){
      string msg("MTest::setStiffnessUpdatePolicy : "
		 "stiffness matrix type already specificed");
      throw(runtime_error(msg));
    }
    this->ks = p;
  } // end of MTest::setStiffnessUpdatingPolicy

  std::string
  MTest::name(void) const
  {
    if(this->tname.empty()){
      return "unit behaviour test";
    }
    return this->tname;
  } // end of MTest::name
  
  std::string
  MTest::classname(void) const
  {
    return "MTest";
  }

  void
  MTest::addEvolution(const std::string& n,
		      const tfel::utilities::shared_ptr<MTestEvolution> p,
		      const bool b1,
		      const bool b2)
  {
    using namespace std;
    if(b1){
      this->declareVariable(n,b1);
    } else {
      if(find(this->vnames.begin(),this->vnames.end(),n)==this->vnames.end()){
	string msg("MTest::addEvolution : "
		   "variable '"+n+"' is not defined");
	throw(runtime_error(msg));
      }
    }
    if(b2){
      if(this->evs->find(n)!=this->evs->end()){
	string msg("MTest::addEvolution : "
		   "evolution '"+n+"' already defined");
	throw(runtime_error(msg));
      }
    }
    (*(this->evs))[n] = p;
  }

  void
  MTest::setMaterialProperty(const std::string& n,
			     const tfel::utilities::shared_ptr<MTestEvolution> p,
			     const bool check)
  {
    using namespace std;
    if(this->b.get()==0){
      string msg("MTest::setMaterialProperty : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    const vector<string>& mpnames = this->b->getMaterialPropertiesNames();
    bool is_mp = find(mpnames.begin(),mpnames.end(),n)==mpnames.end();
    if((is_mp)&&(n!="ThermalExpansion") &&(n!="ThermalExpansion1")&&
       (n!="ThermalExpansion2")&&(n!="ThermalExpansion3")){
      ostringstream msg;
      msg << "MTest::setMaterialProperty : "
	  << "the behaviour don't declare a material property '" << n+"'.";
      if(!mpnames.empty()){
	msg << "\nThe behaviour declares:";
	for(vector<string>::const_iterator pn=mpnames.begin();pn!=mpnames.end();++pn){
	  msg << "\n- '" << *pn << "'";
	}
      }
      throw(runtime_error(msg.str()));
    }
    if((n=="ThermalExpansion") ||(n=="ThermalExpansion1")||
       (n=="ThermalExpansion2")||(n=="ThermalExpansion3")){
      if(is_mp){
	this->addEvolution(n,p,true,check);
      } else {
	this->addEvolution(n,p,false,check);
      }
    } else {
      this->addEvolution(n,p,false,check);
    }
  }

  void
  MTest::setExternalStateVariable(const std::string& n,
				  const tfel::utilities::shared_ptr<MTestEvolution> p,
				  const bool check)
  {
    using namespace std;
    if(this->b.get()==0){
      string msg("MTest::setExternalStateVariable : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    const vector<string>& evsnames = this->b->getExternalStateVariablesNames();
    if(find(evsnames.begin(),evsnames.end(),n)==evsnames.end()){
      string msg("MTestParser::setExternalStateVariable : ");
      msg += "the behaviour don't declare an external state variable '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    this->addEvolution(n,p,false,check);
  }

  void
  MTest::addConstraint(const tfel::utilities::shared_ptr<MTestConstraint> c)
  {
    this->constraints.push_back(c);
  }

  void
  MTest::setMaximumNumberOfIterations(const unsigned int i)
  {
    using namespace std;
    if(this->iterMax!=-1){
      string msg("MTest::setMaximumNumberOfIterations : "
		 "the maximum number of iterations "
		 "has already been declared");
      throw(runtime_error(msg));
    }
    if(i==0){
      string msg("MTest::setMaximumNumberOfIterations : ");
      msg += "invalid number of iterations";
      throw(runtime_error(msg));
    }
    this->iterMax = static_cast<int>(i);
  }

  void
  MTest::setDrivingVariableEpsilon(const double e)
  {
    using namespace std;
    if(this->eeps>0){
      string msg("MTest::setDrivingVariableEpsilon : the epsilon "
    		 "value has already been declared");
      throw(runtime_error(msg));
    }
    if(e < 100*numeric_limits<real>::min()){
      string msg("MTest::setDrivingVariableEpsilon : invalid value");
      throw(runtime_error(msg));
    }
    this->eeps = e;
  }

  void
  MTest::setThermodynamicForceEpsilon(const double s)
  {
    using namespace std;
      if(this->seps>0){
      string msg("MTest::setThermodynamicForceEpsilon : the epsilon "
    		 "value has already been declared");
      throw(runtime_error(msg));
    }
    if(s < 100*numeric_limits<real>::min()){
      string msg("MTest::setThermodynamicForceEpsilon : invalid value");
      throw(runtime_error(msg));
    }
    this->seps = s;
  }

  void
  MTest::setParameter(const std::string& n,
		      const double v)
  {
    using namespace std;
    if(this->b.get()==0){
      string msg("MTest::setParameter : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    this->b->setParameter(n,v);
  }

  void
  MTest::setIntegerParameter(const std::string& n,
			     const int v)
  {
    using namespace std;
    if(this->b.get()==0){
      string msg("MTest::setIntegerParameter : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    this->b->setIntegerParameter(n,v);
  }

  void
  MTest::setUnsignedIntegerParameter(const std::string& n,
				     const unsigned int v)
  {
    using namespace std;
    if(this->b.get()==0){
      string msg("MTest::setUnsignedIntegerParameter : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    this->b->setUnsignedIntegerParameter(n,v);
  }

  void
  MTest::setModellingHypothesis(const std::string& h)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->dimension!=0){
      string msg("MTestParser::setModellingHypothesis : ");
      msg += "the modelling hypothesis is already defined";
      throw(runtime_error(msg));
    }
    if(h=="AxisymmetricalGeneralisedPlaneStrain"){
      this->dimension=1u;
      this->hypothesis = MH::AXISYMMETRICALGENERALISEDPLANESTRAIN;
    } else if(h=="Axisymmetrical"){
      this->dimension  = 2u;
      this->hypothesis = MH::AXISYMMETRICAL;
    } else if(h=="PlaneStress"){
      this->dimension  = 2u;
      this->hypothesis = MH::PLANESTRESS;
    } else if(h=="PlaneStrain"){
      this->dimension  = 2u;
      this->hypothesis = MH::PLANESTRAIN;
    } else if(h=="GeneralisedPlaneStrain"){
      this->dimension  = 2u;
      this->hypothesis = MH::GENERALISEDPLANESTRAIN;
    } else if(h=="Tridimensional"){
      this->dimension  = 3u;
      this->hypothesis = MH::TRIDIMENSIONAL;
    } else {
      string msg("MTestParser::setModellingHypothesis : ");
      msg += "unsupported hypothesis '"+h+"'";
      throw(runtime_error(msg));
    }
  } // end of MTest::setModellingHypothesis

  tfel::material::ModellingHypothesis::Hypothesis
  MTest::getModellingHypothesis() const
  {
    using namespace std;
    if(this->dimension==0){
      string msg("MTest::getModellingHypothesis : ");
      msg += "the modelling hypothesis is not defined";
      throw(runtime_error(msg));
    }
    return this->hypothesis;
  }

  unsigned short
  MTest::getDimension(void) const
  {
    using namespace std;
    if(this->dimension==0){
      string msg("MTest::getDimension : ");
      msg += "the modelling hypothesis is not defined";
      throw(runtime_error(msg));
    }
    return this->dimension;
  }

  tfel::material::MechanicalBehaviourBase::BehaviourType
  MTest::getBehaviourType() const{
    using namespace std;
    if(this->b.get()==0){
      string msg("MTest::getBehaviourType : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    return this->b->getBehaviourType();
  }

  tfel::utilities::shared_ptr<MTestBehaviour>
  MTest::getBehaviour(void){
    using namespace std;
    if(this->b.get()==0){
      string msg("MTest::getBehaviour : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    return this->b;
  }

  void
  MTest::setMaximumNumberOfSubSteps(const unsigned int i)
  {
    using namespace std;
    if(this->mSubSteps!=-1){
      string msg("MTest::setMaximumNumberOfSubSteps : "
		 "the maximum number of sub steps "
		 "has already been declared");
      throw(runtime_error(msg));
    }
    if(i==0){
      string msg("MTest::setMaximumNumberOfSubSteps : ");
      msg += "invalid number of sub steps";
      throw(runtime_error(msg));
    }
    this->mSubSteps = static_cast<int>(i);
  }

  void
  MTest::setRotationMatrix(const tfel::math::tmatrix<3u,3u,real>& r)
  {
    using namespace std;
    using namespace tfel::math;
    if(this->isRmDefined){
      string msg("MTest::setRotationMatrix : "
		 "rotation matrix already defined");
      throw(runtime_error(msg));
    }
    if(this->b.get()==0){
      string msg("MTest::setRotationMatrix : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    if(this->b->getSymmetryType()!=1){
      string msg("MTest::setRotationMatrix : "
		 "rotation matrix may only be defined "
		 "for orthotropic behaviours");
      throw(runtime_error(msg));
    }
    this->isRmDefined = true;
    // checking that the given matrix is a rotation one
    tvector<3u,real> c0 = r.column_view<0>();
    tvector<3u,real> c1 = r.column_view<1>();
    tvector<3u,real> c2 = r.column_view<2>();
    if((abs(norm(c0)-real(1))>100*numeric_limits<real>::epsilon())||
       (abs(norm(c1)-real(1))>100*numeric_limits<real>::epsilon())||
       (abs(norm(c2)-real(1))>100*numeric_limits<real>::epsilon())){
      string msg("MTest::setRotationMatrix : "
		 "at least one column is not normalised");
      throw(runtime_error(msg));
    }
    if((abs(c0|c1)>100*numeric_limits<real>::epsilon())||
       (abs(c0|c2)>100*numeric_limits<real>::epsilon())||
       (abs(c1|c2)>100*numeric_limits<real>::epsilon())){
      string msg("MTest::setRotationMatrix : "
		 "at least two columns are not orthogonals");
      throw(runtime_error(msg));
    }
    this->rm = r;
  }

  void
  MTest::getVariableTypeAndPosition(UTest::TestedVariable& type,
				    unsigned short& pos,
				    const std::string& n)
  {
    using namespace std;
    vector<string>::const_iterator p;
    if(this->b.get()==0){
      string msg("MTest::getVariableTypeAndPosition : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    vector<string> enames;
    vector<string> snames;
    this->b->getDrivingVariablesComponents(enames,this->hypothesis);
    this->b->getThermodynamicForcesComponents(snames,this->hypothesis);
    p=find(enames.begin(),enames.end(),n);
    if(p!=enames.end()){
      pos  = static_cast<unsigned short>(p-enames.begin());
      type = MTest::UTest::DRIVINGVARIABLE;
      return;
    } 
    p=find(snames.begin(),snames.end(),n);
    if(p!=snames.end()){
      pos  = static_cast<unsigned short>(p-snames.begin());
      type = MTest::UTest::THERMODYNAMICFORCE;
      return;
    } 
    p=find(this->ivfullnames.begin(),this->ivfullnames.end(),n);
    if(p!=this->ivfullnames.end()){
      pos  = static_cast<unsigned short>(p-this->ivfullnames.begin());
      type = MTest::UTest::INTERNALSTATEVARIABLE;
      return;
    } 
    string msg("MTest::getVariableTypeAndPosition : "
	       "no variable name '"+n+"'");
    throw(runtime_error(msg));
  } // end of MTest::getVariableTypeAndPosition

  void
  MTest::setDefaultHypothesis(void)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis!=MH::UNDEFINEDHYPOTHESIS){
      string msg("MTest::setDefaultHypothesis : "
		 "internal error : the modelling "
		 "hypothesis is already defined");
      throw(runtime_error(msg));
    }
    if(this->b.get()!=0){
      string msg("MTest::setDefaultHypothesis : ");
      msg += "behaviour already defined";
      throw(runtime_error(msg));
    }
    if(getVerboseMode()>=VERBOSE_LEVEL1){
      ostream& log = getLogStream();
      log << "No hypothesis defined, using default" << endl;
    }
    this->hypothesis = MH::TRIDIMENSIONAL;
    this->dimension  = 3u;
  }

  void
  MTest::setOutputFileName(const std::string& o)
  {
    using namespace std;
    if(!this->output.empty()){
      string msg("MTest::setOutputFileName : ");
      msg += "output file name already defined";
      throw(runtime_error(msg));
    }
    this->output = o;
  }

  void
  MTest::setOutputFilePrecision(const unsigned int p)
  {
    using namespace std;
    if(this->oprec!=-1){
      string msg("MTest::setOutputFileName : ");
      msg += "output file precision already defined";
      throw(runtime_error(msg));
    }
    this->oprec = static_cast<int>(p);
  }

  void
  MTest::addTest(const tfel::utilities::shared_ptr<mfront::MTest::UTest> t)
  {
    this->tests.push_back(t);
  }

  void
  MTest::setDrivingVariablesInitialValues(const std::vector<real>& v)
  {
    using namespace std;
    if(!this->e_t0.empty()){
      string msg("MTest::setDrivingVariablesInitialValues : ");
      msg += "the initial values of the strains have already been declared";
      throw(runtime_error(msg));
    }
    const unsigned short N = this->b->getDrivingVariablesSize(this->hypothesis);
    if(v.size()!=N){
      string msg("MTest::setDrivingVariablesInitialValues : ");
      msg += "invalid initial values size";
      throw(runtime_error(msg));
    }
    this->e_t0.resize(N,0);
    copy(v.begin(),v.end(),this->e_t0.begin());
  }
  
  void
  MTest::setThermodynamicForcesInitialValues(const std::vector<real>& v)
  {
    using namespace std;
    if(!this->s_t0.empty()){
      string msg("MTest::setThermodynamicForcesInitialValues : ");
      msg += "the initial values of the strains have already been declared";
      throw(runtime_error(msg));
    }
    const unsigned short N = this->b->getThermodynamicForcesSize(this->hypothesis);
    if(v.size()!=N){
      string msg("MTest::setThermodynamicForcesInitialValues : ");
      msg += "invalid initial values size";
      throw(runtime_error(msg));
    }
    this->s_t0.resize(N,0);
    copy(v.begin(),v.end(),this->s_t0.begin());
  } // end of MTest::setThermodynamicForcesInitialValues
  
  void
  MTest::setScalarInternalStateVariableInitialValue(const std::string& n,
						    const real v)
  {
    using namespace std;
    if(this->b.get()==0){
      string msg("MTest::setScalarInternalStateVariableInitialValue : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    const vector<string>& ivsnames = this->b->getInternalStateVariablesNames();
    if(find(ivsnames.begin(),ivsnames.end(),n)==ivsnames.end()){
      string msg("MTest::setScalarInternalStateVariableInitialValue : ");
      msg += "the behaviour don't declare an internal state variable named '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    const int type           = this->b->getInternalStateVariableType(n);
    const unsigned short pos = this->b->getInternalStateVariablePosition(this->hypothesis,n);
    if(type!=0){
      string msg("MTest::setScalarInternalStateVariableInitialValue : ");
      msg += "internal state variable '"+n+"' is not defined";
      throw(runtime_error(msg));
    }
    if(this->iv_t0.size()<=pos){
      this->iv_t0.resize(pos+1,0.);
    }
    this->iv_t0[pos] = v;
  }

  void
  MTest::setStensorInternalStateVariableInitialValues(const std::string& n,
						      const std::vector<real>& v)
  {
    using namespace std;
    if(this->b.get()==0){
      string msg("MTest::setStensorInternalStateVariableInitialValue : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    const vector<string>& ivsnames = this->b->getInternalStateVariablesNames();
    if(find(ivsnames.begin(),ivsnames.end(),n)==ivsnames.end()){
      string msg("MTest::setStensorInternalStateVariableInitialValue : ");
      msg += "the behaviour don't declare an internal state variable named '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    const int type           = this->b->getInternalStateVariableType(n);
    const unsigned short pos = this->b->getInternalStateVariablePosition(this->hypothesis,n);
    if(type!=1){
      string msg("MTest::setStensorInternalStateVariableInitialValue : ");
      msg += "internal state variable '"+n+"' is not defined";
      throw(runtime_error(msg));
    }
    const unsigned short N = getStensorSize(this->dimension);
    if(v.size()!=N){
      string msg("MTest::setStensorInternalStateVariableInitialValues : "
		 "invalid values size");
      throw(runtime_error(msg));
    }
    if(this->iv_t0.size()<pos+N){
      this->iv_t0.resize(pos+N,0.);
    }
    copy(v.begin(),v.end(),
	 this->iv_t0.begin()+pos);
  } // end of MTest::setStensorInternalStateVariableInitialValue

  void
  MTest::setTimes(const std::vector<real>& t)
  {
    using namespace std;
    if(!this->times.empty()){
      string msg("MTest::setTimes : "
		 "times already defined");
      throw(runtime_error(msg));
    }
    this->times=t;
  } // end of MTest::setTimes

  tfel::utilities::shared_ptr<std::map<std::string,	
				       tfel::utilities::shared_ptr<MTestEvolution> > >
  MTest::getEvolutions() const
  {
    return this->evs;
  } // end of MTest::getEvolutions() const

  void
  MTest::setBehaviour(const std::string& i,
		      const std::string& l,
		      const std::string& f)
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::system;
    using tfel::utilities::shared_ptr;
    typedef ExternalLibraryManager ELM;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    if(this->b.get()!=0){
      string msg("MTest::setBehaviour : "
		 "behaviour already defined");
      throw(runtime_error(msg));
    }
#ifdef HAVE_CASTEM
    if(i=="umat"){
      ELM& elm = ELM::getExternalLibraryManager();
      const unsigned short type = elm.getUMATBehaviourType(l,f);
      if(type==1u){
	this->b = shared_ptr<MTestBehaviour>(new MTestUmatSmallStrainBehaviour(this->hypothesis,l,f));
      } else if(type==2u){
	this->b = shared_ptr<MTestBehaviour>(new MTestUmatFiniteStrainBehaviour(this->hypothesis,l,f));
      } else if(type==3u){
	this->b = shared_ptr<MTestBehaviour>(new MTestUmatCohesiveZoneModelBehaviour(this->hypothesis,l,f));
      } else {
	ostringstream msg;
	msg << "MTest::setBehaviour : "
	  "unsupported behaviour type (" << type << ")";
	throw(runtime_error(msg.str()));
      }
    }
#endif
#ifdef HAVE_ASTER
    if(i=="aster"){
      this->b = shared_ptr<MTestBehaviour>(new MTestAsterSmallStrainBehaviour(this->hypothesis,l,f));
    }
#endif
    if(this->b.get()==0){
      string msg("MTest::setBehaviour : ");
      msg += "unknown interface '"+i+"'";
      throw(runtime_error(msg));
    }
    const vector<string>& ivnames = this->b->getInternalStateVariablesNames();
    this->declareVariables(this->b->getMaterialPropertiesNames(),true);
    this->declareVariables(ivnames,true);
    this->declareVariables(this->b->getExternalStateVariablesNames(),true);
    vector<string>::const_iterator pn;
    for(pn=ivnames.begin();pn!=ivnames.end();++pn){
      unsigned short t = this->b->getInternalStateVariableType(*pn);
      if(t==0){
	this->ivfullnames.push_back(*pn);
      } else if(t==1){
	//! suffixes of stensor components
	vector<string> sexts;
	this->b->getStensorComponentsSuffixes(sexts,this->hypothesis);
	for(vector<string>::size_type s=0;s!=sexts.size();++s){
	  const string& vn = *pn+sexts[s];
	  this->declareVariable(vn,true);
	  this->ivfullnames.push_back(vn);
	}
      } else {
	string msg("MTest::setBehaviour : "
		   "unsupported variable type for variable '"+*pn+"'");
	throw(runtime_error(msg));
      }
    }
    // declaring behaviour variables
    std::vector<string> enames;
    std::vector<string> snames;
    this->b->getDrivingVariablesComponents(enames,this->hypothesis);
    this->b->getThermodynamicForcesComponents(snames,this->hypothesis);
    this->declareVariables(enames,true);
    this->declareVariables(snames,true);
  } // end of MTest::setBehaviour

  void
  MTest::completeInitialisation()
  {
    using namespace std;
    using namespace tfel::material;
    using tfel::utilities::shared_ptr;
    typedef tfel::material::ModellingHypothesis MH;
    map<string,shared_ptr<MTestEvolution> >::const_iterator pev;
    if(this->initialisationFinished){
      string msg("MTest::completeInitialisation : "
		 "object already initialised");
      throw(runtime_error(msg));
    }
    if(this->dimension==0u){
      this->setDefaultHypothesis();
    }
    if(this->b.get()==0){
      string msg("MTest::completeInitialisation : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    // number of components of the driving variables and the thermodynamic forces
    const unsigned short ndv = this->b->getDrivingVariablesSize(this->hypothesis);
    const unsigned short nth = this->b->getThermodynamicForcesSize(this->hypothesis);
    // check if material properties and external state variables are declared
    vector<string> mpnames(this->b->getMaterialPropertiesNames());
    vector<string> esvnames(this->b->getExternalStateVariablesNames());
    checkIfDeclared(mpnames,*(this->evs),"material property");
    checkIfDeclared(esvnames,*(this->evs),"external state variable");
    // output
    if(!this->output.empty()){
      this->out.open(this->output.c_str());
      if(!this->out){
	string msg("MTest::completeInitialisation : ");
	msg += "can't open file '"+this->output+"'";
	throw(runtime_error(msg));
      }
      this->out.exceptions(ofstream::failbit|ofstream::badbit);
      if(this->oprec!=-1){
	this->out.precision(static_cast<streamsize>(this->oprec));
      }
    }
    // post-processing
    unsigned short cnbr = 2;
    this->out << "# first column : time" << endl;
    for(unsigned short i=0;i!=ndv;++i){
      this->out << "# " << cnbr << " column : " << i+1 << "th component of the driving variables" << endl;
      ++cnbr;
    }
    for(unsigned short i=0;i!=nth;++i){
      this->out << "# " << cnbr << " column : " << i+1 << "th component of the thermodynamic forces" << endl;
      ++cnbr;
    }
    const std::vector<string>& ivdes =
      this->b->getInternalStateVariablesDescriptions(this->hypothesis);
    if(ivdes.size()!=this->b->getInternalStateVariablesSize(this->hypothesis)){
      string msg("MTest::completeInitialisation : internal error "
		 "(the number of descriptions given by "
		 "the mechanical behaviour don't match "
		 "the number of internal state variables)");
      throw(runtime_error(msg));
    }
    for(std::vector<string>::size_type i=0;i!=ivdes.size();++i){
      this->out << "# " << cnbr << " column : " << ivdes[i] << endl;
      ++cnbr;
    }
    // convergence criterium value for driving variables
    if(this->eeps<0){
      this->eeps = 1.e-12;
    }
    // convergence criterium value for thermodynamic forces
    if(this->seps<0){
      this->seps = 1.e-3;
    }
    // tangent operator comparison criterium
    if(this->toeps<0){
      this->toeps = (this->seps/1e-3)*1.e7;
    }
    // perturbation value
    if(this->pv<0){
      this->pv = 10*this->eeps;
    }
    // additional constraints
    if(this->hypothesis==MH::PLANESTRAIN){
      shared_ptr<MTestEvolution>  eev(new MTestConstantEvolution(0.));
      shared_ptr<MTestConstraint> ec(new MTestImposedDrivingVariable(2,eev));
      this->constraints.push_back(ec);
    }
    if(this->hypothesis==MH::PLANESTRESS){
      // shall be in the behaviour
      if((this->b->getBehaviourType()==MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (this->b->getBehaviourType()==MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
	shared_ptr<MTestEvolution>  eev(new MTestConstantEvolution(0.));
	shared_ptr<MTestConstraint> ec(new MTestImposedDrivingVariable(2,eev));
	shared_ptr<MTestEvolution>  sev(new MTestConstantEvolution(0.));
	shared_ptr<MTestConstraint> sc(new MTestImposedThermodynamicForce(2,sev));
	this->constraints.push_back(ec);
	this->constraints.push_back(sc);
      } else {
	string msg("MTest::completeInitialisation : "
		   "plane stress is only handled for small and finite strain behaviours");
	throw(runtime_error(msg));
      }
    }
    if(!this->isRmDefined){
      for(unsigned short i=0;i!=3;++i){
	rm(i,i) = real(1);
      }
    }
    if(this->mSubSteps==-1){
      this->mSubSteps=10;
    }
    if(this->iterMax==-1){
      this->iterMax=100;
    }
    // castem acceleration
    if(this->useCastemAcceleration){
      if(this->cap==-1){
	this->cap=2;
      }
      if(this->cat==-1){
	this->cat=4;
      }
    }
    // prediction policy
    if(this->ppolicy==UNSPECIFIEDPREDICTIONPOLICY){
      this->ppolicy=NOPREDICTION;
    }
    // stiffness matrix type
    if(this->ktype==MTestStiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE){
      this->ktype = this->b->getDefaultStiffnessMatrixType();
    }
    // thermal expansion reference temperature
    pev = this->evs->find("ThermalExpansionReferenceTemperature");
    if(pev!=this->evs->end()){
      const MTestEvolution& ev = *(pev->second);
      if(!ev.isConstant()){
	string msg("MTest::completeInitialisation : 'ThermalExpansionReferenceTemperature' "
		   "must be a constant evolution");
	throw(runtime_error(msg));
      }
    }
    // options selected
    if(getVerboseMode()>=VERBOSE_LEVEL1){
      ostream& log = getLogStream();
      if(this->useCastemAcceleration){
	log << "mtest : castem acceleration selected" << endl;
      }
      if(this->ppolicy==LINEARPREDICTION){
	log << "mtest : using linear prediction" << endl;
      } else if(this->ppolicy==ELASTICPREDICTION){
	log << "mtest : prediction using elastic stiffness" << endl;
      } else if(this->ppolicy==TANGENTOPERATORPREDICTION){
	log << "mtest : prediction using tangent operator" << endl;
      } else {
	if(this->ppolicy!=NOPREDICTION){
	  string msg("MTest::completeInitialisation : "
		     "internal error, unsupported "
		     "prediction policy");
	  throw(runtime_error(msg));
	}	    
	log << "mtest : no prediction" << endl;
      }
    }
    // allocating behaviour workspace
    this->b->allocate(this->hypothesis);
    // the end
    this->initialisationFinished = true;
  }

  void
  MTest::initializeCurrentState(MTestCurrentState& s) const
  {
    using namespace std;
    using tfel::utilities::shared_ptr;
    // getting the names of the materials properties
    vector<string> mpnames(this->b->getMaterialPropertiesNames());
    vector<string> esvnames(this->b->getExternalStateVariablesNames());
    unsigned short psz = this->getNumberOfUnknowns();
    // clear
    s.s_1.clear();
    s.s0.clear();
    s.s1.clear();
    s.e0.clear();
    s.e1.clear();
    s.e_th0.clear();
    s.e_th1.clear();
    s.u_1.clear();
    s.u0.clear();
    s.u1.clear();
    s.mprops0.clear();
    s.mprops1.clear();
    s.iv_1.clear();
    s.iv0.clear();
    s.iv1.clear();
    s.esv0.clear();
    s.desv.clear();
    // resizing
    const unsigned short ndv = this->b->getDrivingVariablesSize(this->hypothesis);
    const unsigned short nth = this->b->getThermodynamicForcesSize(this->hypothesis);
    s.s_1.resize(nth,0.);
    s.s0.resize(nth,0.);
    s.s1.resize(nth,0.);
    s.e0.resize(ndv,0.);
    s.e1.resize(ndv,0.);
    s.e_th0.resize(ndv,0.);
    s.e_th1.resize(ndv,0.);
    s.u_1.resize(psz,0.);
    s.u0.resize(psz,0.);
    s.u1.resize(psz,0.);
    s.mprops0.resize(mpnames.size());
    s.mprops1.resize(mpnames.size());
    s.iv_1.resize(this->b->getInternalStateVariablesSize(this->hypothesis),0.);
    s.iv0.resize(s.iv_1.size(),0.);
    s.iv1.resize(s.iv0.size(),0.);
    s.esv0.resize(esvnames.size(),0.);
    s.desv.resize(esvnames.size(),0.);
    // setting the intial  values of strains
    this->b->getDrivingVariablesDefaultInitialValues(s.u_1);
    copy(this->e_t0.begin(),this->e_t0.end(),s.u_1.begin());
    s.u0 = s.u_1;
    // setting the intial  values of stresses
    copy(this->s_t0.begin(),this->s_t0.end(),s.s0.begin());
    // getting the initial values of internal state variables
    if(this->iv_t0.size()>s.iv0.size()){
      string msg("MTest::initializeCurrentState : the number of initial values declared "
		 "by the user for the internal state variables exceeds the "
		 "number of internal state variables declared by the behaviour");
      throw(runtime_error(msg));
    }
    copy(this->iv_t0.begin(),this->iv_t0.end(),s.iv_1.begin());
    copy(this->iv_t0.begin(),this->iv_t0.end(),s.iv0.begin());
    // mandatory for time step management
    s.period = 1u;
    s.dt_1   = 0.;
    // reference temperature
    map<string,shared_ptr<MTestEvolution> >::const_iterator pev;
    pev = this->evs->find("ThermalExpansionReferenceTemperature");
    if(pev!=this->evs->end()){
      const MTestEvolution& ev = *(pev->second);
      if(!ev.isConstant()){
	string msg("MTest::initializeCurrentState : "
		   "'ThermalExpansionReferenceTemperature' "
		   "must be a constant evolution");
	throw(runtime_error(msg));
      }
      s.Tref = ev(0);
    } else {
      s.Tref = real(293.15);
    }
  } // end of MTest::initializeCurrentState

  void
  MTest::initializeWorkSpace(MTestWorkSpace& wk) const
  {
    const unsigned short psz = this->getNumberOfUnknowns();
    const unsigned short ndv = this->b->getDrivingVariablesSize(this->hypothesis);
    const unsigned short nth = this->b->getThermodynamicForcesSize(this->hypothesis);
    const size_t nstatev = this->b->getInternalStateVariablesSize(this->hypothesis);
    // clear
    wk.K.clear();
    wk.Kt.clear();
    wk.Kp.clear();
    wk.p_lu.clear();
    wk.x.clear();
    wk.r.clear();
    wk.ca_u0.clear();      
    wk.ca_u1.clear();
    wk.ca_u2.clear();
    wk.ca_r0.clear();
    wk.ca_r1.clear();
    wk.ca_r2.clear();
    wk.ca_n0.clear();
    wk.ca_n1.clear();
    wk.ca_tmp0.clear();
    wk.ca_tmp1.clear();
    wk.ca_r.clear();
    //resizing
    wk.K.resize(psz,psz);
    wk.Kt.resize(nth,ndv);
    wk.nKt.resize(nth,ndv);
    wk.tKt.resize(nth,ndv);
    wk.s1.resize(nth);
    wk.s2.resize(nth);
    wk.statev.resize(nstatev);
    wk.Kp.resize(nth,ndv);
    wk.p_lu.resize(psz);
    wk.x.resize(psz);
    wk.r.resize(psz,0.);
    // allocating memory
    if(this->useCastemAcceleration){
      wk.ca_u0.resize(psz,0.);      
      wk.ca_u1.resize(psz,0.);
      wk.ca_u2.resize(psz,0.);
      wk.ca_r0.resize(psz,0.);
      wk.ca_r1.resize(psz,0.);
      wk.ca_r2.resize(psz,0.);
      wk.ca_n0.resize(psz,0.);
      wk.ca_n1.resize(psz,0.);
      wk.ca_tmp0.resize(psz,0.);
      wk.ca_tmp1.resize(psz,0.);
      wk.ca_r.resize(psz,0.);
    }
    wk.first = true;
    wk.a = 0;
  } // end of MTest::initializeWorkSpace
  
  size_t
  MTest::getNumberOfUnknowns() const
  {
    using tfel::utilities::shared_ptr;
    using tfel::math::vector;
    vector<shared_ptr<MTestConstraint> >::const_iterator pc;
    // number of components of the driving variables
    const unsigned short N = this->b->getDrivingVariablesSize(this->hypothesis);
    // getting the total number of unknowns
    size_t s = N;
    for(pc =this->constraints.begin();
	pc!=this->constraints.end();++pc){
      const MTestConstraint& c = *(*pc);
      s += c.getNumberOfLagrangeMultipliers();
    }
    return s;
  } // end of MTest::getNumberOfUnknowns

  tfel::tests::TestResult
  MTest::execute(void)
  {
    using namespace std;
    using namespace tfel::tests;
    using tfel::math::vector;
    using tfel::utilities::shared_ptr;
    TestResult tr;
    vector<real>::const_iterator pt;
    vector<real>::const_iterator pt2;
    vector<shared_ptr<UTest> >::iterator ptest;
    // some checks
    if(times.empty()){
      string msg("MTest::execute : no times defined");
      throw(runtime_error(msg));
    }
    if(times.size()<2){
      string msg("MTest::execute :"
		 "invalid number of times defined");
      throw(runtime_error(msg));
    }
    // finish initialization
    this->completeInitialisation();
    // initialize current state and work space
    MTestCurrentState state;
    MTestWorkSpace    wk;
    this->initializeCurrentState(state);
    this->initializeWorkSpace(wk);
    // integrating over the loading path
    pt  = this->times.begin();
    pt2 = pt+1;
    this->printOutput(*pt,state);
    // real work begins here
    while(pt2!=this->times.end()){
      // allowing subdivisions of the time step
      this->execute(state,wk,*pt,*pt2);
      this->printOutput(*pt2,state);
      ++pt;
      ++pt2;
    }
    for(ptest=this->tests.begin();ptest!=this->tests.end();++ptest){
      const UTest& test = *(*ptest);
      tr.append(test.getResults());
    }
    return tr; 
  }

  void
  MTest::setCompareToNumericalTangentOperator(const bool bo)
  {
    this->cto = bo;
  } // end of MTest::setCompareToNumericalTangentOperator

  void
  MTest::setTangentOperatorComparisonCriterium(const real v)
  {
    using namespace std;
    if(v<100*numeric_limits<real>::min()){
      string msg("MTest::setTangentOperatorComparisonCriterium : " );
      msg += "invalid comparison criterium";
      throw(runtime_error(msg));
    }
    this->toeps=v;
  } // end of MTest::handleTangentOperatorComparisonCriterium

  void
  MTest::setNumericalTangentOperatorPertubationValue(const real v)
  {
    using namespace std;
    if(v<100*numeric_limits<real>::min()){
      string msg("MTest::setNumericalTangentOperatorPertubationValue : " );
      msg += "invalid perturbation value";
      throw(runtime_error(msg));
    }
    this->pv=v;
  } // end of MTest::setNumericalTangentOperatorPertubationValue

  void
  MTest::execute(MTestCurrentState& state,
		 MTestWorkSpace& wk,
		 const real ti,
		 const real te)
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::math;
    using namespace tfel::material;
    using tfel::utilities::shared_ptr;
    using tfel::tests::TestResult;
    using tfel::math::vector;
    vector<string>::const_iterator p;
    vector<shared_ptr<MTestConstraint> >::const_iterator pc;
    map<string,shared_ptr<MTestEvolution> >::const_iterator pev;
    map<string,shared_ptr<MTestEvolution> >::const_iterator pev2;
    map<string,shared_ptr<MTestEvolution> >::const_iterator pev3;
    map<string,shared_ptr<MTestEvolution> >::const_iterator pev4;
    vector<shared_ptr<UTest> >::iterator ptest;
    // getting the names of the materials properties
    std::vector<string> mpnames(this->b->getMaterialPropertiesNames());
    // getting the names of the external state variables
    std::vector<string> esvnames(this->b->getExternalStateVariablesNames());
    unsigned short subStep = 0;
    // // number of components of the driving variables and the thermodynamic forces
    const unsigned short ndv = this->b->getDrivingVariablesSize(this->hypothesis);
    const unsigned short nth = this->b->getThermodynamicForcesSize(this->hypothesis);
    real t  = ti;
    real dt = te-ti;
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      ostream& log = getLogStream();
      log << "number of driving variables : "    << ndv << endl;
      log << "number of thermodynamic forces : " << nth << endl;
      log << "number of constraints       : " << this->constraints.size() << endl;
    }
    while((abs(te-t)>0.5*dt)&&
	  (subStep!=this->mSubSteps)){
      unsigned short i,j;
      // evaluations of the materials properties at the end of the
      // time step
      for(i=0,p=mpnames.begin();p!=mpnames.end();++p,++i){
	pev = this->evs->find(*p);
	if(pev==this->evs->end()){
	  string msg("MTest::execute : no evoluation named '"+*p+"'");
	  throw(runtime_error(msg));
	}
	const MTestEvolution& ev = *(pev->second);
	state.mprops1[i] = ev(t+dt);
      }
      for(i=0,p=esvnames.begin();p!=esvnames.end();++p,++i){
	pev = this->evs->find(*p);
	if(pev==this->evs->end()){
	  string msg("MTest::execute : no evoluation named '"+*p+"'");
	  throw(runtime_error(msg));
	}
	const MTestEvolution& ev = *(pev->second);
	const real evt = ev(t);
	state.esv0[i] = evt;
	state.desv[i] = ev(t+dt)-evt;
      }
      if(this->b->getBehaviourType()==MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
	// thermal expansion (isotropic case)
	pev   = this->evs->find("Temperature");
	pev2  = this->evs->find("ThermalExpansion");
	if((pev!=this->evs->end())&&(pev2!=this->evs->end())){
	  const MTestEvolution& T_ev = *(pev->second);
	  const MTestEvolution& a_ev = *(pev2->second);
	  const real eth0 = a_ev(t)*(T_ev(t)-state.Tref);
	  const real eth1 = a_ev(t+dt)*(T_ev(t+dt)-state.Tref);
	  for(i=0;i!=3;++i){
	    state.e_th0[i] = eth0;
	    state.e_th1[i] = eth1;
	  }
	}
	pev2  = this->evs->find("ThermalExpansion1");
	pev3  = this->evs->find("ThermalExpansion2");
	pev4  = this->evs->find("ThermalExpansion3");
	if((pev!=this->evs->end())&&
	   ((pev2!=this->evs->end())||
	    (pev3!=this->evs->end())||
	    (pev4!=this->evs->end()))){
	  if((pev2==this->evs->end())||
	     (pev3==this->evs->end())||
	     (pev4==this->evs->end())){
	    string msg("MTest::execute : at least one of the three "
		       "thermal expansion coefficient is defined and "
		       "at least one is not");
	    throw(runtime_error(msg));
	  }
	  const MTestEvolution& T_ev  = *(pev->second);
	  const MTestEvolution& a1_ev = *(pev2->second);
	  const MTestEvolution& a2_ev = *(pev3->second);
	  const MTestEvolution& a3_ev = *(pev4->second);
	  if(this->dimension==1u){
	    state.e_th0[0u] = a1_ev(t)*(T_ev(t)-state.Tref);
	    state.e_th1[0u] = a1_ev(t+dt)*(T_ev(t+dt)-state.Tref);
	    state.e_th0[1u] = a2_ev(t)*(T_ev(t)-state.Tref);
	    state.e_th1[1u] = a2_ev(t+dt)*(T_ev(t+dt)-state.Tref);
	    state.e_th0[2u] = a3_ev(t)*(T_ev(t)-state.Tref);
	    state.e_th1[2u] = a3_ev(t+dt)*(T_ev(t+dt)-state.Tref);
	  } else if((this->dimension==2u)||
		    (this->dimension==3u)){
	    // thermal expansion tensors in the material referential
	    stensor<3u,real> se_th0(real(0));
	    stensor<3u,real> se_th1(real(0));
	    se_th0[0u] = a1_ev(t)*(T_ev(t)-state.Tref);
	    se_th1[0u] = a1_ev(t+dt)*(T_ev(t+dt)-state.Tref);
	    se_th0[1u] = a2_ev(t)*(T_ev(t)-state.Tref);
	    se_th1[1u] = a2_ev(t+dt)*(T_ev(t+dt)-state.Tref);
	    se_th0[2u] = a3_ev(t)*(T_ev(t)-state.Tref);
	    se_th1[2u] = a3_ev(t+dt)*(T_ev(t+dt)-state.Tref);
	    // backward rotation matrix
	    tmatrix<3u,3u,real> brm = transpose(this->rm);
	    se_th0.changeBasis(brm);
	    se_th1.changeBasis(brm);
	    copy(se_th0.begin(),se_th0.begin()+getStensorSize(this->dimension),state.e_th0.begin());
	    copy(se_th1.begin(),se_th1.begin()+getStensorSize(this->dimension),state.e_th1.begin());
	  } else {
	    string msg("MTest::execute : invalid dimension");
	    throw(runtime_error(msg));
	  }
	}
      }
      // driving variables at the beginning of the time step
      for(i=0;i!=ndv;++i){
	state.e0[i] = state.u0[i]-state.e_th0[i];
      }
      // resolution
      bool converged = false;
      unsigned short iter = 0;
      if(getVerboseMode()>=VERBOSE_LEVEL1){
	ostream& log = getLogStream();
	log << "resolution from " << ti << " to " << t+dt << endl;
      }
      real ne  = 0.;  // norm of the increment of the driving variables
      real nep = 0.;  // norm of the increment of the driving variables at the
      // previous iteration
      real nep2 = 0.; // norm of the increment of the driving variables two
      // iterations before...
      /* prediction */
      if(this->ppolicy==LINEARPREDICTION){
	if(state.period>1){
	  state.u1  = state.u0 +(state.u0 -state.u_1)*dt/state.dt_1;
	  state.iv1 = state.iv0+(state.iv0-state.iv_1)*dt/state.dt_1;
	  state.s1  = state.s0 +(state.s0 -state.s_1)*dt/state.dt_1;
	}
      } else if((this->ppolicy==ELASTICPREDICTION)||
		(this->ppolicy==SECANTOPERATORPREDICTION)||
		(this->ppolicy==TANGENTOPERATORPREDICTION)){
	// evaluations of the materials properties at the beginning
	// of the time step
	for(i=0,p=mpnames.begin();p!=mpnames.end();++p,++i){
	  pev = this->evs->find(*p);
	  if(pev==this->evs->end()){
	    string msg("MTest::execute : no evoluation named '"+*p+"'");
	    throw(runtime_error(msg));
	  }
	  const MTestEvolution& ev = *(pev->second);
	  state.mprops0[i] = ev(t);
	}
	fill(wk.Kp.begin(),wk.Kp.end(),0.);
	bool sp(true);
	if(this->ppolicy==ELASTICPREDICTION){
	  sp = this->b->computePredictionOperator(wk.Kp,this->rm,state.e0,
						  state.s0,state.mprops0,
						  state.iv0,state.esv0,
						  this->hypothesis,
						  MTestStiffnessMatrixType::ELASTIC);
	} else if (this->ppolicy==SECANTOPERATORPREDICTION){
	  sp = this->b->computePredictionOperator(wk.Kp,this->rm,state.e0,
						  state.s0,state.mprops0,
						  state.iv0,state.esv0,
						  this->hypothesis,
						  MTestStiffnessMatrixType::SECANTOPERATOR);
	} else if (this->ppolicy==TANGENTOPERATORPREDICTION){
	  sp = this->b->computePredictionOperator(wk.Kp,this->rm,state.e0,
						  state.s0,state.mprops0,
						  state.iv0,state.esv0,
						  this->hypothesis,
						  MTestStiffnessMatrixType::TANGENTOPERATOR);
	}
	if(sp){
	  fill(wk.K.begin(),wk.K.end(),0.);
	  fill(wk.r.begin(),wk.r.end(),0.);
	  state.u1 = state.u0;
	  for(i=0;i!=nth;++i){
	    wk.r(i) += state.s0(i);
	    for(j=0;j!=ndv;++j){
	      wk.K(i,j)+=wk.Kp(i,j);
	      // free dilatation treatment
	      wk.r(i) -= wk.Kp(i,j)*(state.e_th1[j]-state.e_th0[j]);
	    }
	  }
	  if(wk.first){
	    wk.a = *(max_element(wk.K.begin(),wk.K.end()));
	    wk.first = false;
	  }
	  unsigned short pos = ndv;
	  for(pc =this->constraints.begin();
	      pc!=this->constraints.end();++pc){
	    const MTestConstraint& c = *(*pc);
	    c.setValues(wk.K,wk.r,state.u0,state.u1,pos,
			this->dimension,t,dt,wk.a);
	    pos = static_cast<unsigned short>(pos+c.getNumberOfLagrangeMultipliers());
	  }
	  LUSolve::exe(wk.K,wk.r,wk.x,wk.p_lu);
	  state.u1 -= wk.r;
	} else {
	  if(getVerboseMode()>VERBOSE_QUIET){
	    ostream& log = getLogStream();
	    log << "MTest::execute : behaviour compute prediction matrix failed" << endl;
	  }
	  state.u1  = state.u0;
	  state.s1  = state.s0;
	  state.iv1 = state.iv0;
	}
      } else {
	if(this->ppolicy != NOPREDICTION){
	  string msg("MTest::execute : "
		     "internal error, unsupported "
		     "prediction policy");
	  throw(runtime_error(msg));
	}	    
	state.u1  = state.u0;
	state.s1  = state.s0;
	state.iv1 = state.iv0;
      }
      /* resolution */
      bool cont = true;
      vector<string> failed_criteria;
      while((!converged)&&(iter!=this->iterMax)&&(cont)){
	failed_criteria.clear();
	++iter;
	nep2 = nep;
	nep  = ne;
	fill(wk.K.begin(), wk.K.end(),0.);
	fill(wk.Kt.begin(),wk.Kt.end(),0.);
	fill(wk.r.begin(), wk.r.end(),0.);
	for(i=0;i!=ndv;++i){
	  state.e1[i] = state.u1[i]-state.e_th1[i];
	}
	const bool bi = this->b->integrate(wk.Kt,state.s1,state.iv1,this->rm,
					   state.e0,state.e1,state.s0,
					   state.mprops1,state.iv0,
					   state.esv0,state.desv,
					   this->hypothesis,dt,
					   this->ktype);
	if(this->cto){
	  fill(wk.nKt.begin(),wk.nKt.end(),real(0));
	  bool ok = true;
	  for(j=0;(j!=ndv)&&(ok);++j){
	    copy(state.s0.begin(),state.s0.end(),wk.s1.begin());
	    copy(state.s0.begin(),state.s0.end(),wk.s2.begin());
	    copy(state.iv0.begin(),state.iv0.end(),wk.statev.begin());
	    state.e1[j]+=this->pv;
	    const bool ok1 = this->b->integrate(wk.tKt,wk.s1,wk.statev,this->rm,
						state.e0,state.e1,state.s0,
						state.mprops1,state.iv0,
						state.esv0,state.desv,
						this->hypothesis,dt,
						MTestStiffnessMatrixType::NOSTIFFNESS);
	    copy(state.iv0.begin(),state.iv0.end(),wk.statev.begin());
	    state.e1[j]-=2*(this->pv);
	    const bool ok2 = this->b->integrate(wk.tKt,wk.s2,wk.statev,this->rm,
						state.e0,state.e1,state.s0,
						state.mprops1,state.iv0,
						state.esv0,state.desv,
						this->hypothesis,dt,
						MTestStiffnessMatrixType::NOSTIFFNESS);
	    state.e1[j]+=this->pv;
	    ok = ok1 && ok2;
	    if(ok){
	      for(i=0;i!=nth;++i){
		wk.nKt(i,j) = (wk.s1(i)-wk.s2(i))/(2*(this->pv));
	      }
	    }
	  }
	  if(ok){
	    real merr(0);
	    for(i=0;i!=nth;++i){
	      for(j=0;j!=ndv;++j){
		merr = max(merr,abs(wk.Kt(i,j)-wk.nKt(i,j)));
	      }
	    }
	    if(merr>this->toeps){
	      ostream& log = getLogStream();
	      log << "Compaison to numerical jacobian failed (error : " << merr << ", criterium " << this->toeps << ")." << endl;
	      log << "Tangent operator returned by the behaviour : " << endl;
	      for(i=0;i!=nth;++i){
		for(j=0;j!=ndv;){
		  log << wk.Kt(i,j);
		  if(++j!=ndv){
		    log << " ";
		  }
		}
		log << "]" << endl;
	      }
	      log << "Numerical tangent operator (perturbation value : " << this-> pv << ") : " << endl;
	      for(i=0;i!=nth;++i){
		for(j=0;j!=ndv;){
		  log << wk.nKt(i,j);
		  if(++j!=ndv){
		    log << " ";
		  }
		}
		log << "]" << endl;
	      }
	    }
	  } else {
	    ostream& log = getLogStream();
	    log << "Numerical evalution of tangent operator failed." << endl << endl;
	  }
	}
	if(bi){
	  // behaviour integration is a success
	  for(i=0;i!=nth;++i){
	    wk.r(i) += state.s1(i);
	    for(j=0;j!=ndv;++j){
	      wk.K(i,j) += wk.Kt(i,j);
	    }
	  }
	  if(wk.first){
	    wk.a = *(max_element(wk.K.begin(),wk.K.end()));
	    wk.first = false;
	  }
	  unsigned short pos = ndv;
	  for(pc =this->constraints.begin();
	      pc!=this->constraints.end();++pc){
	    const MTestConstraint& c = *(*pc);
	    c.setValues(wk.K,wk.r,state.u0,state.u1,pos,
			this->dimension,t,dt,wk.a);
	    pos = static_cast<unsigned short>(pos+c.getNumberOfLagrangeMultipliers());
	  }
	  if(getVerboseMode()>=VERBOSE_DEBUG){
	    ostream& log = getLogStream();
	    for(i=0;i!=wk.K.getNbRows();++i){
	      for(j=0;j!=wk.K.getNbCols();++j){
		log << wk.K(i,j) << " ";
	      }
	      log << endl;
	    }
	    log << endl;
	  }
	  if(this->useCastemAcceleration){
	    wk.ca_r = wk.r;
	  }
	  real nr = 0.;
	  for(i=0;i!=nth;++i){
	    nr += abs(wk.r(i));
	  }
	  LUSolve::exe(wk.K,wk.r,wk.x,wk.p_lu);
	  state.u1 -= wk.r;
	  ne = 0;
	  for(i=0;i!=ndv;++i){
	    ne += abs(wk.r(i));
	  }
	  if(getVerboseMode()>=VERBOSE_LEVEL2){
	    ostream& log = getLogStream();
	    log << "iteration " << iter << " : " << ne << " " 
		<< nr << " (";
	    for(i=0;i!=ndv;){
	      log << state.u1(i);
	      if(++i!=ndv){
		log << " ";
	      }
	    }
	    log << ")" << endl;
	  }
	  converged = (ne<this->eeps)&&(nr<this->seps);
	  for(pc =this->constraints.begin();
	      (pc!=this->constraints.end())&&(converged);++pc){
	    const MTestConstraint& c = *(*pc);
	    converged = c.checkConvergence(state.u1,state.s1,
					   this->eeps,this->seps,
					   t,dt);
	  }
	  if((!converged)&&(iter==this->iterMax)){
	    if(ne>this->eeps){
	      ostringstream msg;
	      msg << "test on driving variables (error : " << ne
		  << ", criteria : " << this->eeps << ")";
	      failed_criteria.push_back(msg.str());
	    }
	    if(nr>this->seps){
	      ostringstream msg;
	      msg << "test on thermodynamic forces (error : " << nr
		  << ", criteria : " << this->seps << ")";
	      failed_criteria.push_back(msg.str());
	    }
	    for(pc =this->constraints.begin();
		pc!=this->constraints.end();++pc){
	      const MTestConstraint& c = *(*pc);
	      bool bc = c.checkConvergence(state.u1,state.s1,
					   this->eeps,this->seps,
					   t,dt);
	      if(!bc){
		failed_criteria.push_back(c.getFailedCriteriaDiagnostic(state.u1,state.s1,
									this->eeps,this->seps,
									t,dt));
	      }
	    }
	  }
	  if(this->ppolicy == NOPREDICTION){
	    converged = (converged) && (iter>1);
	  }
	  if((!converged)&&(this->useCastemAcceleration)){
	    const real ca_eps = 100*(this->seps*numeric_limits<real>::epsilon());
	    wk.ca_u0.swap(wk.ca_u1);
	    wk.ca_u1.swap(wk.ca_u2);
	    wk.ca_r0.swap(wk.ca_r1);
	    wk.ca_r1.swap(wk.ca_r2);
	    wk.ca_u2 = state.u1;
	    wk.ca_r2 = wk.ca_r;
	    if((iter>=this->cat)&&((iter-this->cat)%this->cap==0)){
	      if(getVerboseMode()>=VERBOSE_LEVEL1){
		ostream& log = getLogStream();
		log << "castem acceleration convergence" << endl;
	      }
	      bool c  = true;
	      wk.ca_tmp0 = wk.ca_r1-wk.ca_r0;
	      wk.ca_tmp1 = wk.ca_r2-wk.ca_r0;
	      const real nr0 = norm(wk.ca_tmp0);
	      c = nr0>ca_eps;
	      if(c){
		wk.ca_n0   = wk.ca_tmp0/nr0;
		const real ntmp1  = wk.ca_tmp1|wk.ca_n0;
		wk.ca_tmp1          -= ntmp1*wk.ca_n0;
		const real nr1    = norm(wk.ca_tmp1);
		c = nr1>0.1*abs(ntmp1);
		if(c){
		  wk.ca_n1      =   wk.ca_tmp1/nr1;
		  const real ca_p0 = -(wk.ca_r0|wk.ca_n0);
		  const real ca_p1 = -(wk.ca_r0|wk.ca_n1);
		  // La projection du vecteur nul est
		  // donnée par
		  // (1-ca_c0-ca_c1)*r0+ca_c0*ca_r1+ca_c1*r2;
		  // avec - ca_c0 = (ca_p0/nr0-ca_p1/nr1)
		  //      - ca_c1 = ca_p1/nr1
		  // maintenant on applique la même relation
		  // linéaire
		  // aux inconnues..
		  const real ca_c0 = (ca_p0/nr0-ca_p1/nr1);
		  const real ca_c1 = ca_p1/nr1;
		  state.u1 = (1-ca_c0-ca_c1)*wk.ca_u0+ca_c0*wk.ca_u1+ca_c1*wk.ca_u2;
		} else {
		  // the previous iterations were (almost) colinear
		  const real ca_c0 = -(wk.ca_r0|wk.ca_n0)/nr0;
		  state.u1 = (1-ca_c0)*wk.ca_u0+ca_c0*wk.ca_u1;
		}
	      }
	    }
	  }
	} else {
	  if(getVerboseMode()>VERBOSE_QUIET){
	    ostream& log = getLogStream();
	    log << "MTest::execute : behaviour intregration failed" << endl;
	  }
	  cont = false;
	}
      }
      if(!converged){
	// no convergence
	if(iter==this->iterMax){
	  if(getVerboseMode()>=VERBOSE_LEVEL1){
	    ostream& log = getLogStream();
	    log << "No convergence, the following criteria were not met : " << endl;
	    for(vector<string>::const_iterator pfc=failed_criteria.begin();
		pfc!=failed_criteria.end();++pfc){
	      log << "- " << *pfc << endl;
	    }
	    log << endl;
	  }
	}
	++subStep;
	if(subStep==this->mSubSteps){
	  string msg("MTest::execute : behaviour "
		     "maximum number of sub stepping reached");
	  throw(runtime_error(msg));
	}
	if(getVerboseMode()>=VERBOSE_LEVEL1){
	  ostream& log = getLogStream();
	  log << "Dividing time "
	      << "step by two" << endl << endl;
	}
	dt *= 0.5;
	state.u1  = state.u0;
	state.s1  = state.s0;
	state.iv1 = state.iv0;
      } else {
	if(getVerboseMode()>=VERBOSE_LEVEL1){
	  if(iter==1u){
	    ostream& log = getLogStream();
	    log << "convergence, after one iteration "
		<< endl << endl;
	  } else {
	    ostream& log = getLogStream();
	    if((iter>=3)&&
	       (nep >100*numeric_limits<real>::min())&&
	       (nep2>100*numeric_limits<real>::min())){
	      log << "convergence, after " << iter << " iterations, "
		  << "order " << std::log(ne/nep)/std::log(nep/nep2)
		  << endl << endl;
	    } else {
	      log << "convergence, after " << iter << " iterations, "
		  << "order undefined"
		  << endl << endl;
	    }
	  }
	}
	// testing
	for(ptest=this->tests.begin();ptest!=this->tests.end();++ptest){
	  UTest& test = *(*ptest);
	  test.check(state.u1,state.s1,state.iv1,t,dt,state.period);
	}
	// update variables
	state.u_1  = state.u0;
	state.s_1  = state.s0;
	state.iv_1 = state.iv0;
	state.dt_1 = dt;
	state.u0   = state.u1;
	state.s0   = state.s1;
	state.iv0  = state.iv1;
	t+=dt;
	++state.period;
      }
    }
  }

  void
  MTest::printOutput(const real t,const MTestCurrentState& s)
  {
    using namespace std;
    if(this->out){
      // number of components of the driving variables and the thermodynamic forces
      const unsigned short ndv = this->b->getDrivingVariablesSize(this->hypothesis);
      const unsigned short nth = this->b->getThermodynamicForcesSize(this->hypothesis);
      unsigned short i;
      this->out << t << " ";
      for(i=0;i!=ndv;++i){
	this->out << s.u0[i] << " ";
      }
      for(i=0;i!=nth;++i){
	this->out << s.s0[i] << " ";
      }
      copy(s.iv0.begin(),s.iv0.end(),
	   ostream_iterator<real>(this->out," "));
      this->out << endl;
    }
  }

  void
  MTest::declareVariable(const std::string& v,
			 const bool check)
  {
    using namespace std;
    if(find(this->vnames.begin(),this->vnames.end(),v)!=
       this->vnames.end()){
      if(check){
	string msg("MTest::declareVariable : ");
	msg += "variable '"+v+"' already declared";
	throw(runtime_error(msg));
      }
    } else {
      this->vnames.push_back(v);
    }
  }

void
  MTest::declareVariables(const std::vector<std::string>& v,
			  const bool check)
  {
    using namespace std;
    vector<string>::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      this->declareVariable(*p,check);
    }
  }

MTest::~MTest()
{}

} // end namespace mfront
