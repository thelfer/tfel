/*! 
 * \file  mtest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#include<cstdlib>
#include<iostream>

#include<map>
#include<string>
#include<vector>
#include<sstream>
#include<stdexcept>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/LUSolve.hxx"

#include"MFront/MTest.hxx"
#include"MFront/MTestBehaviour.hxx"
#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestConstraint.hxx"

#include"MFront/MTestImposedStress.hxx"
#include"MFront/MTestImposedStrain.hxx"

#ifdef HAVE_CASTEM
#include"MFront/MTestUmatBehaviour.hxx"
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
#include"MFront/MTestAsterBehaviour.hxx"
#endif /* HAVE_ASTER  */

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
  getSTensorSize(const unsigned short d)
  {
    using namespace std;
    if(d==1){
      return 3;
    } else if(d==2){
      return 4;
    } else if(d==3){
      return 6;
    }
    string msg("mfront::getTensorSize : ");
    msg += "";
    throw(runtime_error(msg));
    return 0;
  }

  MTest::MTest(const std::string& f)
    : dimension(0u),
      hypothesis(tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS),
      eeps(-1.),
      seps(-1.),
      mSubSteps(10),
      iterMax(100)      
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef tfel::material::ModellingHypothesis MH;
    typedef map<string,CallBack>::value_type MVType;
    map<string,CallBack>::const_iterator pc;
    this->callbacks.insert(MVType("@OutputFile",
				  &MTest::handleOutputFile));
    this->callbacks.insert(MVType("@StrainEpsilon",
				  &MTest::handleStrainEpsilon));
    this->callbacks.insert(MVType("@StressEpsilon",
				  &MTest::handleStressEpsilon));
    this->callbacks.insert(MVType("@ModellingHypothesis",
				  &MTest::handleModellingHypothesis));
    this->callbacks.insert(MVType("@Strain",&MTest::handleStrain));
    this->callbacks.insert(MVType("@Times",&MTest::handleTimes));
    this->callbacks.insert(MVType("@Behaviour",&MTest::handleBehaviour));
    this->callbacks.insert(MVType("@MaterialProperty",
				  &MTest::handleMaterialProperty));
    this->callbacks.insert(MVType("@InternalStateVariable",
				  &MTest::handleInternalStateVariable));
    this->callbacks.insert(MVType("@ExternalStateVariable",
				  &MTest::handleExternalStateVariable));
    this->callbacks.insert(MVType("@ImposedStrain",
				  &MTest::handleImposedStrain));
    this->callbacks.insert(MVType("@ImposedStress",
				  &MTest::handleImposedStress));
    this->treatCharAsString(true);
    this->openFile(f);
    this->stripComments();
    TokensContainer::const_iterator p;
    p = this->fileTokens.begin();
    while(p!=fileTokens.end()){
      pc=this->callbacks.find(p->value);
      if(pc==this->callbacks.end()){
	ostringstream msg;
	msg << "MTest::MTest : invalid keyword '"
	    << p->value << "'. Error at line " 
	    << p->line<< ".";
	throw(runtime_error(msg.str()));
      }
      ++p;
      unsigned int line = p->line;
      try{
	(this->*(pc->second))(p);
      } catch(exception& e){
	ostringstream msg;
	msg << "MTest::MTest : error while "
	    << "parsing file '" << f << "' at line '"
	    << line << "'.\n" << e.what();
	throw(runtime_error(msg.str()));
      }
    }
    // some checks
    if(times.empty()){
      string msg("MTest::MTest : no times defined");
      throw(runtime_error(msg));
    }
    if(this->dimension==0u){
      this->dimension  = 3u;
      this->hypothesis = MH::TRIDIMENSIONAL;
    }
    if(this->output.empty()){
      string::size_type pos = f.rfind('.');
      if(pos!=string::npos){
	this->output = f.substr(0,pos);
      } else {
	this->output = f;
      }
      this->output += ".res";
    }
    this->out.open(this->output.c_str());
    if(!this->out){
      string msg("MTest::MTest : ");
      msg += "can't open file '"+this->output+"'";
      throw(runtime_error(msg));
    }
    if(this->eeps<0){
      this->eeps = 1.e-12;
    }
    if(this->seps<0){
      this->seps = 1.e-3;
    }
    // additional constraints
    if(this->hypothesis==MH::PLANESTRAIN){
      shared_ptr<MTestEvolution>  eev(new MTestConstantEvolution(0.));
      shared_ptr<MTestConstraint> ec(new MTestImposedStrain(3,eev));
      this->constraints.push_back(ec);
    }
    if(this->hypothesis==MH::PLANESTRESS){
      shared_ptr<MTestEvolution>  sev(new MTestConstantEvolution(0.));
      shared_ptr<MTestConstraint> sc(new MTestImposedStress(3,sev));
      this->constraints.push_back(sc);
    }
  } // end of MTest::MTest

  void
  MTest::handleStrainEpsilon(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->eeps>0){
      string msg("MTest::handleStrainEpsilon : the epsilon "
		 "value has already been declared");
      throw(runtime_error(msg));
    }
    this->eeps = this->readDouble(p,this->fileTokens.end());
    if(this->eeps < 100*numeric_limits<real>::min()){
      string msg("MTest::handleStrainEpsilon : invalid value");
      throw(runtime_error(msg));
    }
  }

  void
  MTest::handleStressEpsilon(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->seps>0){
      string msg("MTest::handleStressEpsilon : the epsilon "
		 "value has already been declared");
      throw(runtime_error(msg));
    }
    this->seps = this->readDouble(p,this->fileTokens.end());
    if(this->seps < 100*numeric_limits<real>::min()){
      string msg("MTest::handleStressEpsilon : invalid value");
      throw(runtime_error(msg));
    }
  }


  void
  MTest::handleOutputFile(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->output.empty()){
      string msg("MTest::handleOutputFile : ");
      msg += "message already defined";
      throw(runtime_error(msg));
    }
    this->output = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTest::handleOutputFiles",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleOutputFile

  void
  MTest::handleModellingHypothesis(TokensContainer::const_iterator& p)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->dimension!=0){
      string msg("MTest::handleModellingHypothesis : ");
      msg += "the modelling hypothesis is already defined";
      throw(runtime_error(msg));
    }
    const string& h = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTest::handleTimes",";",p,
			     this->fileTokens.end());
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
      string msg("MTest::handleModellingHypothesis : ");
      msg += "unsupported hypothesis '"+h+"'";
      throw(runtime_error(msg));
    }
  }

  void
  MTest::handleTimes(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(!times.empty()){
      string msg("MTest::handleTimes : ");
      msg += "times already defined";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleTimes","{",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleTimes",p,
			    this->fileTokens.end());
    while(p->value!="}"){
      const real t_dt = this->readTime(p);
      this->checkNotEndOfLine("MTest::handleTimes",p,
			      this->fileTokens.end());
      if(!this->times.empty()){
	if(p->value=="in"){
	  ++p;
	  unsigned int n = this->readUnsignedInt(p,this->fileTokens.end());
	  if(n==0){
	    string msg("MTest::handleTimes : ");
	    msg += "invalid number of intervals";
	    throw(runtime_error(msg));
	  }
	  const real t = this->times.back();
	  real dt = (t_dt-t)/(static_cast<real>(n));
	  for(unsigned int i=1;i!=n;++i){
	    this->times.push_back(t+i*dt);
	  }
	}
	this->checkNotEndOfLine("MTest::handleTimes",p,
				this->fileTokens.end());
      }
      this->times.push_back(t_dt);
      if(p->value==","){
	++p;
	this->checkNotEndOfLine("MTest::handleTimes",p,
				this->fileTokens.end());
	if(p->value=="}"){
	  string msg("MTest::handleTimes : ");
	  msg += "unexpected token '}'";
	  throw(runtime_error(msg));
	}
      } else {
	if(p->value!="}"){
	  string msg("MTest::handleTimes : ");
	  msg += "unexpected token '"+p->value+"', expected ',' or '}'";
	  throw(runtime_error(msg));
	}
      }
    }
    this->readSpecifiedToken("MTest::handleTimes","}",p,
			     this->fileTokens.end());
    this->readSpecifiedToken("MTest::handleTimes",";",p,
			     this->fileTokens.end());
    if(times.empty()){
      string msg("MTest::handleTimes : ");
      msg += "no time defined";
      throw(runtime_error(msg));
    }
    if(times.size()==1){
      string msg("MTest::handleTimes : ");
      msg += "at least two different times must be defined";
      throw(runtime_error(msg));
    }
    vector<real>::const_iterator pt  = times.begin();
    vector<real>::const_iterator pt2 = pt+1u;
    while(pt2!=times.end()){
      if((*pt2<=*pt)||abs(*pt2-*pt)<1.e-3){
	ostringstream msg;
	msg << "MTest::handleTimes : times '" << *pt2 
	    << "' is lesser than or too close to  time '"
	    << *pt << "'";
	throw(runtime_error(msg.str()));
      }
      ++pt2;
      ++pt;
    }
  }

  void
  MTest::setDefaultHypothesis(void)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    cout << "No hypothesis defined, using default" << endl;
    if(this->hypothesis!=MH::UNDEFINEDHYPOTHESIS){
      string msg("MTest::setDefaultHypothesis : "
		 "internal error : the modelling "
		 "hypothesis is already defined");
      throw(runtime_error(msg));
    }
    this->hypothesis = MH::TRIDIMENSIONAL;
    this->dimension  = 3u;
  }
  
  void
  MTest::handleImposedStress(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    const string& c = this->readString(p,this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleImposedStress",p,
			    this->fileTokens.end());
    shared_ptr<MTestEvolution> sev = this->parseEvolution(p);
    shared_ptr<MTestConstraint> sc(new MTestImposedStress(this->hypothesis,c,sev));
    this->constraints.push_back(sc);
    this->readSpecifiedToken("MTest::handleImposedStress",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleImposedStress

  void
  MTest::handleImposedStrain(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    const string& c = this->readString(p,this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleImposedStrain",p,
			    this->fileTokens.end());
    shared_ptr<MTestEvolution> sev = this->parseEvolution(p);
    shared_ptr<MTestConstraint> sc(new MTestImposedStrain(this->hypothesis,c,sev));
    this->constraints.push_back(sc);
    this->readSpecifiedToken("MTest::handleImposedStrain",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleImposedStrain

  void
  MTest::handleBehaviour(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    string interface;
    this->readSpecifiedToken("MTest::handleBehaviour","<",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleBehaviour",p,
			    this->fileTokens.end());
#ifdef HAVE_CASTEM
    if(p->value=="umat"){
      interface = p->value;
    }
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
    if(p->value=="aster"){
      interface = p->value;
    }
#endif /* HAVE_ASTER */
    if(interface.empty()){
      string msg("MTest::handleBehaviour : ");
      msg += "unknown interface '"+p->value+"'";
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTest::handleBehaviour",">",p,
			     this->fileTokens.end());
    const string& l = this->readString(p,this->fileTokens.end());
    const string& f = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTest::handleBehaviour",";",p,
			     this->fileTokens.end());
#ifdef HAVE_CASTEM
    if(interface=="umat"){
      this->b = shared_ptr<MTestBehaviour>(new MTestUmatBehaviour(l,f));
    }
#endif
#ifdef HAVE_ASTER
    if(interface=="aster"){
      this->b = shared_ptr<MTestBehaviour>(new MTestAsterBehaviour(l,f));
    }
#endif
    if(this->b.get()==0){
      string msg("MTest::handleBehaviour : ");
      msg += "unknown interface '"+interface+"'";
      throw(runtime_error(msg));
    }
  } // end of MTest::handleBehaviour

  void
  MTest::handleMaterialProperty(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    string interface;
    if(this->b.get()==0){
      string msg("MTest::handleMaterialProperty : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleMaterialProperty","<",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleMaterialProperty",p,
			    this->fileTokens.end());
    if(p->value=="constant"){
      interface = p->value;
    } else {
      string msg("MTest::handleMaterialProperty : ");
      msg += "unknown interface '"+interface+"'";
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTest::handleMaterialProperty",">",p,
			     this->fileTokens.end());
    const string& n = this->readString(p,this->fileTokens.end());
    const vector<string>& mpnames = this->b->getMaterialPropertiesNames();
    if(find(mpnames.begin(),mpnames.end(),n)==mpnames.end()){
      string msg("MTest::handleMaterialProperty : ");
      msg += "the behaviour don't declare a material property '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    if(this->evs.find(n)!=this->evs.end()){
      string msg("MTest::handleMaterialProperty : ");
      msg += "'"+n+"' already declared";
      throw(runtime_error(msg));
    }
    if(interface=="constant"){
      shared_ptr<MTestEvolution> mpev;
      const real v = this->readDouble(p,this->fileTokens.end());
      mpev = shared_ptr<MTestEvolution>(new MTestConstantEvolution(v));
      this->evs[n] = mpev;
    } else {
      string msg("MTest::handleMaterialProperty : ");
      msg += "unknown interface '"+interface+"'";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleMaterialProperty",";",p,
			     this->fileTokens.end());
  }

  void
  MTest::handleStrain(TokensContainer::const_iterator& p)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    if(this->b.get()==0){
      string msg("MTest::handleStrain : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    if(!this->e_t0.empty()){
      string msg("MTest::handleStrain : ");
      msg += "the initial values of the strains have already been declared";
      throw(runtime_error(msg));
    }
    const unsigned short N = getSTensorSize(this->dimension);
    this->e_t0.resize(N,0);
    this->readArrayOfSpecifiedSize(this->e_t0,p);
    this->readSpecifiedToken("MTest::handleStrain",
			     ";",p,this->fileTokens.end());
  }

  void
  MTest::handleInternalStateVariable(TokensContainer::const_iterator& p)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    if(this->b.get()==0){
      string msg("MTest::handleInternalStateVariable : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    const string& n = this->readString(p,this->fileTokens.end());
    const vector<string>& ivsnames = this->b->getInternalStateVariablesNames();
    if(find(ivsnames.begin(),ivsnames.end(),n)==ivsnames.end()){
      string msg("MTest::handleInternalStateVariable : ");
      msg += "the behaviour don't declare an external state variable '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    const int type           = b->getInternalStateVariableType(n);
    const unsigned short pos = b->getInternalStateVariablePosition(this->dimension,n);
    if(type==0){
      const real v = this->readDouble(p,this->fileTokens.end());
      if(this->iv_t0.size()<=pos){
	this->iv_t0.resize(pos+1,0.);
      }
      this->iv_t0[pos] = v;
    } else if(type==1){
      const unsigned short N = getSTensorSize(this->dimension);
      vector<real> v(N);
      this->readArrayOfSpecifiedSize(v,p);
      if(this->iv_t0.size()<pos+N){
	this->iv_t0.resize(pos+N,0.);
      }
      copy(v.begin(),v.end(),
	   this->iv_t0.begin()+pos);
    } else {
      string msg("MTest::handleInternalStateVariable : "
		 "unsupported state variable type for "
		 "internal state variable '"+n+"'");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleInternalStateVariable",
			     ";",p,this->fileTokens.end());
  }

  void
  MTest::handleExternalStateVariable(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    string interface;
    if(this->b.get()==0){
      string msg("MTest::handleExternalStateVariable : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    const string& n = this->readString(p,this->fileTokens.end());
    const vector<string>& evsnames = this->b->getExternalStateVariablesNames();
    if(find(evsnames.begin(),evsnames.end(),n)==evsnames.end()){
      string msg("MTest::handleExternalStateVariable : ");
      msg += "the behaviour don't declare an external state variable '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    if(this->evs.find(n)!=this->evs.end()){
      string msg("MTest::handleExternalStateVariable : ");
      msg += "'"+n+"' already declared";
      throw(runtime_error(msg));
    }
    this->evs[n] = this->parseEvolution(p);
    this->readSpecifiedToken("MTest::handleExternalStateVariable",";",p,
			     this->fileTokens.end());
  }

  int MTest::execute(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::math;
    using tfel::math::vector;
    vector<shared_ptr<MTestConstraint> >::const_iterator pc;
    vector<string>::const_iterator p;
    vector<real>::const_iterator pt;
    vector<real>::const_iterator pt2;
    const unsigned short N = getSTensorSize(this->dimension);
    //! stiffness matrix
    matrix<real> K;
    // residual
    vector<real> r;
    // vector of unknows at 
    // the beginning of the
    // time step.
    vector<real> u0;
    // vector of unknows at 
    // the end of the
    // time step
    vector<real> u1;
    // material properties at
    // the end of the
    // time step
    vector<real> mprops;
    // internal variables at
    // the beginning of the
    // time step
    vector<real> iv0;
    // internal variables at
    // the end of the
    // time step
    vector<real> iv1;
    // external variables at
    // the beginning of the
    // time step
    vector<real> esv0;
    // external variables increments
    vector<real> desv;
    // dimension
    // getting the names of the materials properties
    std::vector<string> mpnames;
    if(this->b.get()==0){
      string msg("MTest::execute : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    mpnames = this->b->getMaterialPropertiesNames();
    // getting the names of the internal state variables
    std::vector<string> ivnames;
    ivnames = this->b->getInternalStateVariablesNames();
    // getting the names of the external state variables
    std::vector<string> esvnames;
    esvnames = this->b->getExternalStateVariablesNames();
    // getting the total number of unknowns
    size_t s = N;
    for(pc =this->constraints.begin();
	pc!=this->constraints.end();++pc){
      const MTestConstraint& c = *(*pc);
      s += c.getNumberOfLagrangeMultipliers();
    }
    // allocating memory
    u0.resize(s,0.);
    u1.resize(s);
    r.resize(s);
    K.resize(s,s);
    mprops.resize(mpnames.size());
    iv0.resize(this->b->getInternalStateVariablesSize(this->dimension),0.);
    iv1.resize(iv0.size());
    esv0.resize(esvnames.size());
    desv.resize(esvnames.size());
    // getting the intial  values of strains
    copy(this->e_t0.begin(),this->e_t0.end(),u0.begin());
    u1 = u0;
    // getting the initial values of internal state variables
    if(this->iv_t0.size()>iv0.size()){
      string msg("MTest::execute : the number of initial values declared "
		 "by the user for the internal state variables exceeds the "
		 "number of internal state variables declared by the behaviour");
      throw(runtime_error(msg));
    }
    copy(this->iv_t0.begin(),this->iv_t0.end(),iv0.begin());
    copy(iv0.begin(),iv0.end(),iv1.begin());
    this->iv_t0.clear();
    // integrating over the loading path
    pt  = this->times.begin();
    pt2 = pt+1;
    // stresses at the beginning of the time step
    tvector<6u,real> s0(0.);
    // stiffness tensor
    matrix<real> Kt(N,N);
    checkIfDeclared(mpnames,this->evs,"material property");
    checkIfDeclared(esvnames,this->evs,"external state variable");
    // rotation matrix
    tmatrix<3u,3u,real> d(0.);
    tmatrix<3u,3u,real>::size_type di;
    for(di=0;di!=3;++di){
      d(di,di) = 1.;
    }
    // post-processing
    unsigned short cnbr = 2;
    this->out << "# first column : time" << endl;
    for(unsigned short i=0;i!=N;++i){
      this->out << "# " << cnbr << " column : " << i+1 << "th component of the total strain" << endl;
      ++cnbr;
    }
    for(unsigned short i=0;i!=N;++i){
      this->out << "# " << cnbr << " column : " << i+1 << "th component of the stresses" << endl;
      ++cnbr;
    }
    const std::vector<string>& ivdes =
      this->b->getInternalStateVariablesDescriptions(this->dimension);
    if(ivdes.size()!=iv0.size()){
      string msg("MTest::execute : internal error "
		 "(the number of descriptions given by "
		 "the mechanical behaviour don't match "
		 "the number of internal state variables)");
      throw(runtime_error(msg));
    }
    for(std::vector<string>::size_type i=0;i!=ivdes.size();++i){
      this->out << "# " << cnbr << " column : " << ivdes[i] << endl;
      ++cnbr;
    }
    this->printOutput(*pt,N,u0,s0,iv0);
    bool first = true;
    real a(0);
    while(pt2!=this->times.end()){
      // allowing subdivisions of the time step
      const real t_dt = *pt2;
      real t  = *pt;
      real dt = t_dt-t;
      unsigned short subStep = 0;
      while((abs(t_dt-t)>0.5*dt)&&
	    (subStep!=this->mSubSteps)){
	unsigned short i,j;
	// stresses at the end of the time step
	tvector<6u,real> s1(0.);
	// stresses at the previous iterations
	tvector<6u,real> sp(0.);
	// strain at the beginning of the time step
	tvector<6u,real> e0(0.);
	// strain increment
	tvector<6u,real> de(0.);
	// evaluations of the materials properties at the end of the
	// time step
	for(i=0,p=mpnames.begin();p!=mpnames.end();++p,++i){
	  const MTestEvolution& ev = *(this->evs.at(*p));
	  mprops[i] = ev(t_dt);
	}
	for(i=0,p=esvnames.begin();p!=esvnames.end();++p,++i){
	  const MTestEvolution& ev = *(this->evs.at(*p));
	  const real evt = ev(t);
	  esv0[i] = evt;
	  desv[i] = ev(t_dt)-evt;
	}
	// strain increment
	for(i=0;i!=N;++i){
	  e0[i] = u0[i];
	}
	// resolution
	bool converged = false;
	unsigned short iter = 0;
	cout << "resolution from " << t << " to " << t+dt << endl;
	sp = s0;
	while((!converged)&&(iter!=this->iterMax)){
	  ++iter;
	  fill(K.begin(),K.end(),0.);
	  fill(Kt.begin(),Kt.end(),0.);
	  fill(r.begin(),r.end(),0.);
	  s1  = s0;
	  iv1 = iv0;
	  for(i=0;i!=N;++i){
	    de[i] = u1[i]-u0[i];
	  }
	  if(!this->b->integrate(Kt,s1,iv1,d,e0,de,s0,
				 mprops,iv0,esv0,desv,
				 this->hypothesis,dt)){
	    string msg("behaviour integration failed");
	    throw(runtime_error(msg));
	  }
	  for(i=0;i!=N;++i){
	    r(i) += s1(i);
	    for(j=0;j!=N;++j){
	      K(i,j)+=Kt(i,j);
	    }
	  }
	  if(first){
	    a = *(max_element(K.begin(),K.end()));
	    first = false;
	  }
	  unsigned short pos = N;
	  for(pc =this->constraints.begin();
	      pc!=this->constraints.end();++pc){
	    const MTestConstraint& c = *(*pc);
	    c.setValues(K,r,u0,u1,pos,
			this->dimension,t,dt,a);
	    pos = static_cast<unsigned short>(pos+c.getNumberOfLagrangeMultipliers());
	  }
	  // cout << "matrix to be solved : " << endl;
	  // for(i=0;i!=K.getNbRows();++i){
	  //   for(j=0;j!=K.getNbCols();++j){
	  //     cout << K(i,j) << " ";
	  //   }
	  //   cout << endl;
	  // }
	  // cout << endl;
	  LUSolve::exe(K,r);
	  u1 -= r;
	  real ne = 0.;
	  real ns = 0.;
	  for(i=0;i!=N;++i){
	    ne += abs(r(i));
	    ns += abs(s1(i)-sp(i));
	  }
	  sp = s1;
	  cout << "iteration " << iter << " : " << ne << " " 
	       << ns << " (";
	  for(i=0;i!=N;){
	    cout << u1(i);
	    if(++i!=N){
	      cout << " ";
	    }
	  }
	  cout << ")" << endl;
	  converged = (ne<this->eeps)&&(ns<this->seps);
	  for(pc =this->constraints.begin();
	      (pc!=this->constraints.end())&&(converged);++pc){
	    const MTestConstraint& c = *(*pc);
	    converged = c.checkConvergence(N,u1,s1,
					   this->eeps,
					   this->seps,
					   t,dt);
	  }
	}
	if(iter==this->iterMax){
	  ++subStep;
	  if(subStep!=this->mSubSteps){
	    cout << "non convergence, dividing time "
		 << "step by two" << endl << endl;
	    dt *= 0.5;
	  }
	} else {
	  if(iter==1u){
	    cout << "convergence, after one iteration "
		 << endl << endl;
	  } else {
	    cout << "convergence, after " << iter << " iterations "
		 << endl << endl;
	  }
	  // post-processing
	  this->printOutput(t+dt,N,u1,s1,iv1);
	  // update variables
	  u0  = u1;
	  s0  = s1;
	  iv0 = iv1;
	  t+=dt;
	}
      }
      ++pt;
      ++pt2;
    }
    return EXIT_SUCCESS;
  }

  real
  MTest::readTime(TokensContainer::const_iterator& p)
  {
    using namespace std;
    return this->readDouble(p,this->fileTokens.end());
  } // end of MTest::readTimes

  void
  MTest::readArrayOfSpecifiedSize(std::vector<real>& v,
				  TokensContainer::const_iterator& p)
  {
    using namespace std;
    vector<real>::size_type i = 0;
    this->readSpecifiedToken("MTest::readArrayOfSpecifiedSize","{",p,
			     this->fileTokens.end());
    
    while(i!=v.size()){
      v[i] = this->readDouble(p,this->fileTokens.end());
      if(++i!=v.size()){
	this->readSpecifiedToken("MTest::readArrayOfSpecifiedSize",",",p,
				 this->fileTokens.end());
      }
    }
    this->readSpecifiedToken("MTest::readArrayOfSpecifiedSize","}",p,
			     this->fileTokens.end());
  } // end of MTest::readArrayOfSpecifiedSize

  void
  MTest::printOutput(const real t,
		     const unsigned short N,
		     const tfel::math::vector<real>& u,
		     const tfel::math::tvector<6u,real>& s,
		     const tfel::math::vector<real>& iv)
  {
    using namespace std;
    unsigned short i;
    this->out << t << " ";
    for(i=0;i!=N;++i){
      this->out << u(i) << " ";
    }
    for(i=0;i!=N;++i){
      this->out << s(i) << " ";
    }
    copy(iv.begin(),iv.end(),
	 ostream_iterator<real>(this->out," "));
    this->out << endl;
  }
  
  tfel::utilities::shared_ptr<MTestEvolution>
  MTest::parseEvolution(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    shared_ptr<MTestEvolution> ev;
    this->checkNotEndOfLine("MTest::parseEvolution",p,
			    this->fileTokens.end());
    if(p->value=="{"){
      vector<real> t;
      vector<real> v;
      this->readSpecifiedToken("MTest::parseEvolution","{",p,
			       this->fileTokens.end());
      this->checkNotEndOfLine("MTest::parseEvolution",p,
			      this->fileTokens.end());
      while(p->value!="}"){
	t.push_back(this->readTime(p));
	this->readSpecifiedToken("MTest::parseEvolution",":",p,
				 this->fileTokens.end());
	v.push_back(this->readDouble(p,this->fileTokens.end()));
	this->checkNotEndOfLine("MTest::parseEvolution",p,
				this->fileTokens.end());
	if(p->value==","){
	  ++p;
	  this->checkNotEndOfLine("MTest::parseEvolution",p,
				  this->fileTokens.end());
	  if(p->value=="}"){
	    string msg("MTest::parseEvolution : ");
	    msg += "unexpected token '}'";
	    throw(runtime_error(msg));
	  }
	} else {
	  if(p->value!="}"){
	    string msg("MTest::parseEvolution : ");
	    msg += "unexpected token '"+p->value+"', expected ',' or '}'";
	    throw(runtime_error(msg));
	  }
	}
      }
      this->readSpecifiedToken("MTest::parseEvolution","}",p,
			       this->fileTokens.end());
      ev = shared_ptr<MTestEvolution>(new MTestLPIEvolution(t,v));
    } else {
      const real s = this->readDouble(p,this->fileTokens.end());
      ev = shared_ptr<MTestEvolution>(new MTestConstantEvolution(s));
    }
    return ev;
  } // end of MTest::parseEvolution

  struct MTestMain
    : tfel::utilities::ArgumentParserBase<MTestMain>
  {
    MTestMain(const int, 
	      const char *const *const);

    
    int execute(void);

  protected:
    friend class tfel::utilities::ArgumentParserBase<MTestMain>;
    void
    treatUnknownArgument(void);
    std::string 
    getVersionDescription(void) const;
    std::string 
    getUsageDescription(void) const;
    void 
    registerArgumentCallBacks(void);
    // input files
    std::vector<std::string> inputs;
  };

  MTestMain::MTestMain(const int argc, const char *const *const argv)
    : tfel::utilities::ArgumentParserBase<MTestMain>(argc,argv)
  {
    using namespace std;
    this->registerArgumentCallBacks();
    this->parseArguments();
    if(this->inputs.empty()){
      string msg("MTestMain::MTestMain : ");
      msg += "no input file defined";
      throw(runtime_error(msg));
    }
  }

  void 
  MTestMain::registerArgumentCallBacks(void)
  {
    //    this->registerNewCallBack("--verbose",&MTestMain::treatVerbose,"set verbose output");
  }

  void
  MTestMain::treatUnknownArgument(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    if((*(this->currentArgument))[0]=='-'){
#if not (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
      ArgumentParserBase<MTestMain>::treatUnknownArgument();
#else
      cerr << "mfront : unsupported option '" << *(this->currentArgument) << "'\n";
      exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
    this->inputs.push_back(*(this->currentArgument));
    return;
  } // end of MTestMain::treatUnknownArgument()

  std::string 
  MTestMain::getVersionDescription(void) const
  {
    using namespace std;
    return "mtest is an behaviour testing utility";
  }

  std::string 
  MTestMain::getUsageDescription(void) const
  {
    using namespace std;
    string usage("Usage : ");
    usage += "mtest [options] [files]";
    return usage;
  }

  int
  MTestMain::execute(void)
  {
    using namespace std;
    vector<string>::const_iterator p;
    for(p=this->inputs.begin();
	p!=this->inputs.end();++p){
      MTest t(*p);
      t.execute();
    }
    return EXIT_SUCCESS;
  }


} // end of namespace mfront



int main(const int argc,
	 const char * const * const argv)
{
  using namespace mfront;
  MTestMain m(argc,argv);
  return m.execute();
} // end of main
