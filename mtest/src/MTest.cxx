/*! 
 * \file  mfront/mtest/MTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<map>
#include<cmath>
#include<string>
#include<vector>
#include<memory>
#include<sstream>
#include<cstdlib>
#include<iterator>
#include<algorithm>
#include<stdexcept>

#include"tfel-config.hxx"
#include"TFEL/Utilities/TextData.hxx"
#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tmatrix.hxx"

#include"MFront/MFrontLogStream.hxx"

#include"MTest/MTest.hxx"
#include"MTest/Behaviour.hxx"
#include"MTest/MTestParser.hxx"

#include"MTest/AnalyticalTest.hxx"
#include"MTest/ReferenceFileComparisonTest.hxx"

#include"MTest/Evolution.hxx"
#include"MTest/FunctionEvolution.hxx"
#include"MTest/CastemEvolution.hxx"

#include"MTest/Constraint.hxx"
#include"MTest/ImposedThermodynamicForce.hxx"
#include"MTest/ImposedDrivingVariable.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/StructureCurrentState.hxx"
#include"MTest/GenericSolver.hxx"

#ifdef min
#undef min
#endif

namespace mtest
{

  static void
  updateStiffnessAndResidual(tfel::math::matrix<real>& k,
			     tfel::math::vector<real>& r,
			     const Behaviour& b,
			     const tfel::math::matrix<real>& kt,
			     const tfel::math::vector<real>& s,
			     const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    using namespace tfel::material;
    using size_type = tfel::math::matrix<real>::size_type;
    const auto ndv = b.getDrivingVariablesSize(h);
    const auto nth = b.getThermodynamicForcesSize(h);
    std::fill(k.begin(),k.end(),real(0));
    std::fill(r.begin(),r.end(),real(0));
    if(b.getBehaviourType()==MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
      for(size_type i=0;i!=3u;++i){
	r(i) += s(i);
	for(size_type j=0;j!=ndv;++j){
	  k(i,j) += kt(i,j);
	}
      }
      if(getSpaceDimension(h)>1u){
	for(size_type i=0;i!=nth-3u;++i){
	  r(2*i+3u)   += s(i);
	  r(2*i+3u+1) += s(i);
	  for(size_type j=0;j!=ndv;++j){
	    k(2*i+3u,j)   += kt(i+3u,j);
	    k(2*i+3u+1,j) += kt(i+3u,j);
	  }
	}
      }
    } else {
      for(size_type i=0;i!=nth;++i){
	r(i) += s(i);
	for(size_type j=0;j!=ndv;++j){
	  k(i,j) += kt(i,j);
	}
      }
    }
  } // end of updateStiffnessAndResidual
  
  static unsigned short
  getStensorSize(const unsigned short d)
  {
    if(d==1){
      return 3;
    } else if(d==2){
      return 4;
    } else if(d==3){
      return 6;
    }
    throw(std::runtime_error("mtest::getStensorSize: "
			     "invalid dimenstion"));
  }

  static unsigned short
  getTensorSize(const unsigned short d)
  {
    if(d==1){
      return 3;
    } else if(d==2){
      return 5;
    } else if(d==3){
      return 9;
    }
    throw(std::runtime_error("mtest::getTensorSize: "
			     "invalid dimenstion"));
  }

  MTestCurrentState::MTestCurrentState() = default;
  MTestCurrentState::MTestCurrentState(const MTestCurrentState&) = default;
  MTestCurrentState::MTestCurrentState(MTestCurrentState&&)      = default;
  MTestCurrentState::~MTestCurrentState() noexcept = default;

  MTestWorkSpace::MTestWorkSpace() = default;
  MTestWorkSpace::~MTestWorkSpace() noexcept =  default;

  MTest::UTest::~UTest()
  {}

  MTest::MTest()
    : oprec(-1),
      rprec(-1),
      rm(real(0)),
      isRmDefined(false),
      handleThermalExpansion(true),
      toeps(-1),
      pv(-1),
      cto(false)
  {}

  void MTest::readInputFile(const std::string& f){
    MTestParser p;
    p.execute(*this,f);
  } // end of MTest::readInputFile

  void
  MTest::setHandleThermalExpansion(const bool b1)
  {
    if(!this->handleThermalExpansion){
      throw(std::runtime_error("MTest::setHandleThermalExpansion: "
			       "thermal expansion is not handled"));
    }
    this->handleThermalExpansion = b1;
  }

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
  MTest::addConstraint(const std::shared_ptr<Constraint> c)
  {
    this->constraints.push_back(c);
  }

  void
  MTest::setDrivingVariableEpsilon(const double e)
  {
    if(this->options.eeps>0){
      throw(std::runtime_error("MTest::setDrivingVariableEpsilon: the epsilon "
			       "value has already been declared"));
    }
    if(e < 100*std::numeric_limits<real>::min()){
      throw(std::runtime_error("MTest::setDrivingVariableEpsilon:"
			       " invalid value"));
    }
    this->options.eeps = e;
  }

  void
  MTest::setThermodynamicForceEpsilon(const double s)
  {
    if(this->options.seps>0){
      throw(std::runtime_error("MTest::setThermodynamicForceEpsilon: the epsilon "
			       "value has already been declared"));
    }
    if(s < 100*std::numeric_limits<real>::min()){
      throw(std::runtime_error("MTest::setThermodynamicForceEpsilon: invalid value"));
    }
    this->options.seps = s;
  }

  void
  MTest::setRotationMatrix(const tfel::math::tmatrix<3u,3u,real>& r,
			   const bool bo)
  {
    using namespace tfel::math;
    if((this->isRmDefined)&&(!bo)){
      throw(std::runtime_error("MTest::setRotationMatrix: "
			       "rotation matrix already defined"));
    }
    if(this->b.get()==nullptr){
      throw(std::runtime_error("MTest::setRotationMatrix: "
			       "no behaviour defined"));
    }
    if(this->b->getSymmetryType()!=1){
      throw(std::runtime_error("MTest::setRotationMatrix: "
			       "rotation matrix may only be defined "
			       "for orthotropic behaviours"));
    }
    this->isRmDefined = true;
    // checking that the given matrix is a rotation one
    const tvector<3u,real> c0 = r.column_view<0>();
    const tvector<3u,real> c1 = r.column_view<1>();
    const tvector<3u,real> c2 = r.column_view<2>();
    if((abs(norm(c0)-real(1))>100*std::numeric_limits<real>::epsilon())||
       (abs(norm(c1)-real(1))>100*std::numeric_limits<real>::epsilon())||
       (abs(norm(c2)-real(1))>100*std::numeric_limits<real>::epsilon())){
      throw(std::runtime_error("MTest::setRotationMatrix: "
			       "at least one column is not normalised"));
    }
    if((abs(c0|c1)>100*std::numeric_limits<real>::epsilon())||
       (abs(c0|c2)>100*std::numeric_limits<real>::epsilon())||
       (abs(c1|c2)>100*std::numeric_limits<real>::epsilon())){
      throw(std::runtime_error("MTest::setRotationMatrix : "
			       "at least two columns are not orthogonals"));
    }
    this->rm = r;
  }

  void
  MTest::getVariableTypeAndPosition(UTest::TestedVariable& type,
				    unsigned short& pos,
				    const std::string& n)
  {
    if(this->b.get()==nullptr){
      throw(std::runtime_error("MTest::getVariableTypeAndPosition : "
			       "no behaviour defined"));
    }
    const auto enames = this->b->getDrivingVariablesComponents(this->hypothesis);
    const auto snames = this->b->getThermodynamicForcesComponents(this->hypothesis);
    auto p=find(enames.begin(),enames.end(),n);
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
    throw(std::runtime_error("MTest::getVariableTypeAndPosition : "
			     "no variable name '"+n+"'"));
  } // end of MTest::getVariableTypeAndPosition

  void
  MTest::setDefaultHypothesis(void)
  {
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis!=MH::UNDEFINEDHYPOTHESIS){
      throw(std::runtime_error("MTest::setDefaultHypothesis: "
			       "internal error : the modelling "
			       "hypothesis is already defined"));
    }
    if(this->b.get()!=nullptr){
      throw(std::runtime_error("MTest::setDefaultHypothesis: "
			       "behaviour already defined"));
    }
    if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
      auto& log = mfront::getLogStream();
      log << "No hypothesis defined, using default" << std::endl;
    }
    this->hypothesis = MH::TRIDIMENSIONAL;
    this->dimension  = 3u;
  }

  void
  MTest::setOutputFileName(const std::string& o)
  {
    if(!this->output.empty()){
      throw(std::runtime_error("MTest::setOutputFileName: "
			       "output file name already defined"));
    }
    this->output = o;
  }

  void
  MTest::setOutputFilePrecision(const unsigned int p)
  {
    if(this->oprec!=-1){
      throw(std::runtime_error("MTest::setOutputFileName: "
			       "output file precision already defined"));
    }
    this->oprec = static_cast<int>(p);
  }

  void
  MTest::setResidualFileName(const std::string& o)
  {
    if(!this->residualFileName.empty()){
      throw(std::runtime_error("MTest::setResidualFileName : "
			       "residual file name already defined"));
    }
    this->residualFileName = o;
  }

  void
  MTest::setResidualFilePrecision(const unsigned int p)
  {
    if(this->rprec!=-1){
      throw(std::runtime_error("MTest::setResidualFileName: "
			       "residual file precision already defined"));
    }
    this->rprec = static_cast<int>(p);
  }

  void
  MTest::addTest(const std::shared_ptr<MTest::UTest> t)
  {
    this->tests.push_back(t);
  }

  void
  MTest::setDrivingVariablesInitialValues(const std::vector<real>& v)
  {
    if(!this->e_t0.empty()){
      throw(std::runtime_error("MTest::setDrivingVariablesInitialValues: "
			       "the initial values of the strains have "
			       "already been declared"));
    }
    const auto N = this->b->getDrivingVariablesSize(this->hypothesis);
    if(v.size()!=N){
      throw(std::runtime_error("MTest::setDrivingVariablesInitialValues: "
			       "invalid initial values size"));
    }
    this->e_t0.resize(N,0);
    std::copy(v.begin(),v.end(),this->e_t0.begin());
  }
  
  void
  MTest::setThermodynamicForcesInitialValues(const std::vector<real>& v)
  {
    if(!this->s_t0.empty()){
      throw(std::runtime_error("MTest::setThermodynamicForcesInitialValues: "
			       "the initial values of the strains have "
			       "already been declared"));
    }
    const auto N = this->b->getThermodynamicForcesSize(this->hypothesis);
    if(v.size()!=N){
      throw(std::runtime_error("MTest::setThermodynamicForcesInitialValues: "
			       "invalid initial values size"));
    }
    this->s_t0.resize(N,0);
    std::copy(v.begin(),v.end(),this->s_t0.begin());
  } // end of MTest::setThermodynamicForcesInitialValues
  
  void
  MTest::setScalarInternalStateVariableInitialValue(const std::string& n,
						    const real v)
  {
    if(this->b.get()==nullptr){
      throw(std::runtime_error("MTest::setScalarInternalStateVariableInitialValue: "
			       "no behaviour defined"));
    }
    const auto& ivsnames = this->b->getInternalStateVariablesNames();
    if(std::find(ivsnames.begin(),ivsnames.end(),n)==ivsnames.end()){
      throw(std::runtime_error("MTest::setScalarInternalStateVariableInitialValue : "
			       "the behaviour don't declare an internal state "
			       "variable named '"+n+"'"));
    }
    const auto type = this->b->getInternalStateVariableType(n);
    const auto pos  = this->b->getInternalStateVariablePosition(this->hypothesis,n);
    if(type!=0){
      throw(std::runtime_error("MTest::setScalarInternalStateVariableInitialValue: "
			       "internal state variable '"+n+"' is not defined"));
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
    if(this->b.get()==nullptr){
      throw(std::runtime_error("MTest::setStensorInternalStateVariableInitialValue: "
			       "no behaviour defined"));
    }
    const auto& ivsnames = this->b->getInternalStateVariablesNames();
    if(std::find(ivsnames.begin(),ivsnames.end(),n)==ivsnames.end()){
      throw(std::runtime_error("MTest::setStensorInternalStateVariableInitialValue: "
			       "the behaviour don't declare an internal "
			       "state variable named '"+n+"'"));
    }
    const auto type = this->b->getInternalStateVariableType(n);
    const auto pos  = this->b->getInternalStateVariablePosition(this->hypothesis,n);
    if(type!=1){
      throw(std::runtime_error("MTest::setStensorInternalStateVariableInitialValue: "
			       "internal state variable '"+n+"' is not defined"));
    }
    const auto N = getStensorSize(this->dimension);
    if(v.size()!=N){
      throw(std::runtime_error("MTest::setStensorInternalStateVariableInitialValues : "
			       "invalid values size"));
    }
    if(this->iv_t0.size()<pos+N){
      this->iv_t0.resize(pos+N,0.);
    }
    std::copy(v.begin(),v.end(),this->iv_t0.begin()+pos);
  } // end of MTest::setStensorInternalStateVariableInitialValue

  void
  MTest::setTensorInternalStateVariableInitialValues(const std::string& n,
						      const std::vector<real>& v)
  {
    using namespace std;
    if(this->b.get()==nullptr){
      throw(runtime_error("MTest::setTensorInternalStateVariableInitialValue: "
			  "no behaviour defined"));
    }
    const auto& ivsnames = this->b->getInternalStateVariablesNames();
    if(find(ivsnames.begin(),ivsnames.end(),n)==ivsnames.end()){
      string msg("MTest::setTensorInternalStateVariableInitialValue : ");
      msg += "the behaviour don't declare an internal state variable named '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    const int type           = this->b->getInternalStateVariableType(n);
    const unsigned short pos = this->b->getInternalStateVariablePosition(this->hypothesis,n);
    if(type!=3){
      string msg("MTest::setTensorInternalStateVariableInitialValue : ");
      msg += "internal state variable '"+n+"' is not defined";
      throw(runtime_error(msg));
    }
    const unsigned short N = getTensorSize(this->dimension);
    if(v.size()!=N){
      string msg("MTest::setTensorInternalStateVariableInitialValues : "
		 "invalid values size");
      throw(runtime_error(msg));
    }
    if(this->iv_t0.size()<pos+N){
      this->iv_t0.resize(pos+N,0.);
    }
    copy(v.begin(),v.end(),this->iv_t0.begin()+pos);
  } // end of MTest::setTensorInternalStateVariableInitialValue

  void
  MTest::completeInitialisation()
  {
    using namespace std;
    using namespace tfel::material;
    SchemeBase::completeInitialisation();
    // output
    if(!this->output.empty()){
      this->out.open(this->output.c_str());
      if(!this->out){
	throw(runtime_error("MTest::completeInitialisation : "
			    "can't open file '"+this->output+"'"));
      }
      this->out.exceptions(ofstream::failbit|ofstream::badbit);
      if(this->oprec!=-1){
	this->out.precision(static_cast<streamsize>(this->oprec));
      }
    }
    // post-processing
    unsigned short cnbr = 2;
    const char* dvn;
    const char* thn;
    this->out << "# first column : time" << endl;
    if(this->b->getBehaviourType()==MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
      dvn = "strain";
      thn = "stress";
    } else if(this->b->getBehaviourType()==MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
      dvn = "deformation gradient";
      thn = "Cauchy stress";
    } else if(this->b->getBehaviourType()==MechanicalBehaviourBase::COHESIVEZONEMODEL){
      dvn = "opening displacement";
      thn = "cohesive force";
    } else {
      dvn = "driving variables";
      thn = "thermodynamic forces";
    }
    const auto ndv = this->b->getDrivingVariablesSize(this->hypothesis);
    const auto nth = this->b->getThermodynamicForcesSize(this->hypothesis);
    for(unsigned short i=0;i!=ndv;++i){
      this->out << "# " << cnbr << " column : " << i+1 << "th component of the " << dvn << '\n';
      ++cnbr;
    }
    for(unsigned short i=0;i!=nth;++i){
      this->out << "# " << cnbr << " column : " << i+1 << "th component of the " << thn << '\n';
      ++cnbr;
    }
    const auto& ivdes = this->b->getInternalStateVariablesDescriptions(this->hypothesis);
    if(ivdes.size()!=this->b->getInternalStateVariablesSize(this->hypothesis)){
      throw(std::runtime_error("MTest::completeInitialisation : internal error "
			       "(the number of descriptions given by "
			       "the mechanical behaviour don't match "
			       "the number of internal state variables)"));
    }
    for(std::vector<string>::size_type i=0;i!=ivdes.size();++i){
      this->out << "# " << cnbr << " column : " << ivdes[i] << '\n';
      ++cnbr;
    }
    // convergence criterium value for driving variables
    if(this->options.eeps<0){
      this->options.eeps = 1.e-12;
    }
    // convergence criterium value for thermodynamic forces
    if(this->options.seps<0){
      this->options.seps = 1.e-3;
    }
    // tangent operator comparison criterium
    if(this->toeps<0){
      this->toeps = (this->options.seps/1e-3)*1.e7;
    }
    // perturbation value
    if(this->pv<0){
      this->pv = 10*this->options.eeps;
    }
    // additional constraints
    if(this->hypothesis==ModellingHypothesis::PLANESTRAIN){
      shared_ptr<Evolution>  eev(new ConstantEvolution(0.));
      shared_ptr<Constraint> ec(new ImposedDrivingVariable(2,eev));
      this->constraints.push_back(ec);
    }
    if(this->hypothesis==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS){
      // shall be in the behaviour
      if((this->b->getBehaviourType()==MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (this->b->getBehaviourType()==MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
	shared_ptr<Evolution>  eev(new ConstantEvolution(0.));
	shared_ptr<Constraint> ec(new ImposedDrivingVariable(1,eev));
	shared_ptr<Evolution>  sev;
	auto pev = this->evm->find("AxialStress");
	if(pev!=this->evm->end()){
	  sev = pev->second;
	} else {
	  sev = shared_ptr<Evolution>(new ConstantEvolution(0.));
	}
	shared_ptr<Constraint> sc(new ImposedThermodynamicForce(1,sev));
	this->constraints.push_back(ec);
	this->constraints.push_back(sc);
      } else {
	throw(std::runtime_error("MTest::completeInitialisation : "
				 "generalised plane stress is only "
				 "handled for small and finite strain behaviours"));
      }
    }
    if(this->hypothesis==ModellingHypothesis::PLANESTRESS){
      // shall be in the behaviour
      if((this->b->getBehaviourType()==MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (this->b->getBehaviourType()==MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
	shared_ptr<Evolution>  eev(new ConstantEvolution(0.));
	shared_ptr<Constraint> ec(new ImposedDrivingVariable(2,eev));
	shared_ptr<Evolution>  sev(new ConstantEvolution(0.));
	shared_ptr<Constraint> sc(new ImposedThermodynamicForce(2,sev));
	this->constraints.push_back(ec);
	this->constraints.push_back(sc);
      } else {
	throw(std::runtime_error("MTest::completeInitialisation : "
				 "plane stress is only handled for small and "
				 "finite strain behaviours"));
      }
    }
    if(!this->isRmDefined){
      for(unsigned short i=0;i!=3;++i){
	rm(i,i) = real(1);
      }
    }
    // thermal expansion reference temperature
    auto pev = this->evm->find("ThermalExpansionReferenceTemperature");
    if(pev!=this->evm->end()){
      const Evolution& ev = *(pev->second);
      if(!ev.isConstant()){
	throw(runtime_error("MTest::completeInitialisation: "
			    "'ThermalExpansionReferenceTemperature' "
			    "must be a constant evolution"));
      }
    }
    //
    if(!this->residualFileName.empty()){
      this->residual.open(this->residualFileName.c_str());
      if(!this->residual){
	throw(runtime_error("MTest::completeInitialisation : "
			    "unable to open file '"+this->residualFileName+"'"));
      }
      this->residual.exceptions(ofstream::failbit|ofstream::badbit);
      if(this->rprec!=-1){
	this->residual.precision(static_cast<streamsize>(this->rprec));
      } else {
	if(this->oprec!=-1){
	  this->residual.precision(static_cast<streamsize>(this->oprec));
	}
      }
      this->residual << "#first  column : iteration number\n"
		     << "#second column : driving variable residual\n"
		     << "#third  column : thermodynamic force residual\n"
		     << "#The following columns are the components of the driving variable\n";
    }
  }

  void
  MTest::initializeCurrentState(MTestCurrentState& s) const
  {
    if(this->b.get()==nullptr){
      throw(std::runtime_error("MTest::initializeCurrentState: "
			       "mechanical behaviour not set"));
    }
    if(this->hypothesis==tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      throw(std::runtime_error("MTest::initializeCurrentState: "
			       "modelling hypothesis not set"));
    }
    // unknowns
    const auto psz = this->getNumberOfUnknowns();
    s.initialize(psz);
    // initializing the state of the one and only structure
    auto& ss = s.getStructureCurrentState("");
    ss.setBehaviour(this->b);
    ss.setModellingHypothesis(this->hypothesis);
    // state of the one and only integration point
    ss.istates.resize(1);
    auto& cs = ss.istates[0];
    mtest::allocate(cs,*(this->b),this->hypothesis);
    // setting the intial  values of strains
    this->b->getDrivingVariablesDefaultInitialValues(s.u_1);
    copy(this->e_t0.begin(),this->e_t0.end(),s.u_1.begin());
    s.u0 = s.u_1;
    s.u1 = s.u_1;
    // setting the intial  values of stresses
    copy(this->s_t0.begin(),this->s_t0.end(),cs.s0.begin());
    // getting the initial values of internal state variables
    if((this->iv_t0.size()>cs.iv_1.size())||
       (this->iv_t0.size()>cs.iv0.size())||
       (this->iv_t0.size()>cs.iv1.size())){
      throw(std::runtime_error("MTest::initializeCurrentState: the number of initial values declared "
			       "by the user for the internal state variables exceeds the "
			       "number of internal state variables declared by the behaviour"));
    }
    std::copy(this->iv_t0.begin(),this->iv_t0.end(),cs.iv_1.begin());
    std::copy(this->iv_t0.begin(),this->iv_t0.end(),cs.iv0.begin());
    // revert the current state
    mtest::revert(ss);
    // rotation matrix
    cs.r = this->rm;
    // reference temperature
    const auto pev = this->evm->find("ThermalExpansionReferenceTemperature");
    if(pev!=this->evm->end()){
      const auto& ev = *(pev->second);
      if(!ev.isConstant()){
	throw(std::runtime_error("MTest::initializeCurrentState : "
				 "'ThermalExpansionReferenceTemperature' "
				 "must be a constant evolution"));
      }
      cs.Tref = ev(0);
    }
  } // end of MTest::initializeCurrentState

  void
  MTest::initializeWorkSpace(MTestWorkSpace& wk) const
  {
    const auto psz = this->getNumberOfUnknowns();
    const auto ndv = this->b->getDrivingVariablesSize(this->hypothesis);
    const auto nth = this->b->getThermodynamicForcesSize(this->hypothesis);
    const auto nstatev = this->b->getInternalStateVariablesSize(this->hypothesis);
    // clear
    wk.K.clear();
    wk.Kt.clear();
    wk.Kp.clear();
    wk.p_lu.clear();
    wk.x.clear();
    wk.r.clear();
    wk.du.clear();
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
    wk.du.resize(psz,0.);
    wk.first = true;
    wk.a = 0;
  } // end of MTest::initializeWorkSpace
  
  size_t
  MTest::getNumberOfUnknowns() const
  {
    using tfel::math::vector;
    // number of components of the driving variables
    const auto N = this->b->getDrivingVariablesSize(this->hypothesis);
    // getting the total number of unknowns
    size_t s = N;
    for(const auto& pc : this->constraints){
      const auto& c = *pc;
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
    // some checks
    if(times.empty()){
      throw(runtime_error("MTest::execute: no times defined"));
    }
    if(times.size()<2){
      throw(runtime_error("MTest::execute: invalid number of times defined"));
    }
    // finish initialization
    this->completeInitialisation();
    // initialize current state and work space
    MTestCurrentState state;
    MTestWorkSpace    wk;
    this->initializeCurrentState(state);
    this->initializeWorkSpace(wk);
    // integrating over the loading path
    auto pt  = this->times.begin();
    auto pt2 = pt+1;
    this->printOutput(*pt,state);
    // real work begins here
    while(pt2!=this->times.end()){
      // allowing subdivisions of the time step
      this->execute(state,wk,*pt,*pt2);
      this->printOutput(*pt2,state);
      ++pt;
      ++pt2;
    }
    TestResult tr;
    for(const auto& t : this->tests){
      tr.append(t->getResults());
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
    if(v<100*std::numeric_limits<real>::min()){
      throw(std::runtime_error("MTest::setTangentOperatorComparisonCriterium: " 
			       "invalid comparison criterium"));
    }
    this->toeps=v;
  } // end of MTest::handleTangentOperatorComparisonCriterium

  void
  MTest::setNumericalTangentOperatorPerturbationValue(const real v)
  {
    if(v<100*std::numeric_limits<real>::min()){
      throw(std::runtime_error("MTest::setNumericalTangentOperatorPerturbationValue: "
			       "invalid perturbation value"));
    }
    this->pv=v;
  } // end of MTest::setNumericalTangentOperatorPerturbationValue

  void
  MTest::prepare(StudyCurrentState& state,
		 const real t,
		 const real dt) const{
    using namespace tfel::material;
    auto& scs = state.getStructureCurrentState("");
    if(scs.istates.size()!=1u){
      throw(std::runtime_error("MTest::prepare: "
			       "invalid state"));
    }
    // evaluations of the materials properties at the end of the
    // time step
    for(auto& s: scs.istates){
      computeMaterialProperties(s,*(this->evm),*(this->dmpv),
  				this->b->getMaterialPropertiesNames(),t,dt);
      computeExternalStateVariables(s,*(this->evm),
  				    this->b->getExternalStateVariablesNames(),t,dt);
      if(this->handleThermalExpansion){
  	if(this->b->getBehaviourType()==MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
  	  // thermal expansion (isotropic case)
  	  auto pev   = this->evm->find("Temperature");
  	  auto pev2  = this->evm->find("ThermalExpansion");
  	  if((pev!=this->evm->end())&&(pev2!=this->evm->end())){
  	    const auto& T_ev = *(pev->second);
  	    const auto& a_ev = *(pev2->second);
  	    const real eth0 = a_ev(t)*(T_ev(t)-s.Tref);
  	    const real eth1 = a_ev(t+dt)*(T_ev(t+dt)-s.Tref);
  	    for(unsigned short i=0;i!=3;++i){
  	      s.e_th0[i] = eth0;
  	      s.e_th1[i] = eth1;
  	    }
  	  }
  	  pev2  = this->evm->find("ThermalExpansion1");
  	  auto pev3  = this->evm->find("ThermalExpansion2");
  	  auto pev4  = this->evm->find("ThermalExpansion3");
  	  if((pev!=this->evm->end())&&
  	     ((pev2!=this->evm->end())||(pev3!=this->evm->end())||(pev4!=this->evm->end()))){
  	    if((pev2==this->evm->end())||(pev3==this->evm->end())||(pev4==this->evm->end())){
  	      throw(std::runtime_error("MTest::prepare: at least one "
  				       "of the three thermal expansion coefficient is "
  				       "defined and at least one is not"));
  	    }
  	    const auto& T_ev  = *(pev->second);
  	    const auto& a1_ev = *(pev2->second);
  	    const auto& a2_ev = *(pev3->second);
  	    const auto& a3_ev = *(pev4->second);
  	    const auto d = getSpaceDimension(this->hypothesis);
  	    if(d==1u){
  	      s.e_th0[0u] = a1_ev(t)*(T_ev(t)-s.Tref);
  	      s.e_th1[0u] = a1_ev(t+dt)*(T_ev(t+dt)-s.Tref);
  	      s.e_th0[1u] = a2_ev(t)*(T_ev(t)-s.Tref);
  	      s.e_th1[1u] = a2_ev(t+dt)*(T_ev(t+dt)-s.Tref);
  	      s.e_th0[2u] = a3_ev(t)*(T_ev(t)-s.Tref);
  	      s.e_th1[2u] = a3_ev(t+dt)*(T_ev(t+dt)-s.Tref);
  	    } else if((d==2u)||
  		      (d==3u)){
  	      // thermal expansion tensors in the material frame
  	      tfel::math::stensor<3u,real> se_th0(real(0));
  	      tfel::math::stensor<3u,real> se_th1(real(0));
  	      se_th0[0u] = a1_ev(t)*(T_ev(t)-s.Tref);
  	      se_th1[0u] = a1_ev(t+dt)*(T_ev(t+dt)-s.Tref);
  	      se_th0[1u] = a2_ev(t)*(T_ev(t)-s.Tref);
  	      se_th1[1u] = a2_ev(t+dt)*(T_ev(t+dt)-s.Tref);
  	      se_th0[2u] = a3_ev(t)*(T_ev(t)-s.Tref);
  	      se_th1[2u] = a3_ev(t+dt)*(T_ev(t+dt)-s.Tref);
  	      // backward rotation matrix
  	      auto brm = transpose(this->b->getRotationMatrix(s.mprops1,this->rm));
  	      se_th0.changeBasis(brm);
  	      se_th1.changeBasis(brm);
  	      const auto ss = getStensorSize(this->hypothesis);
  	      std::copy(se_th0.begin(),se_th0.begin()+ss,s.e_th0.begin());
  	      std::copy(se_th1.begin(),se_th1.begin()+ss,s.e_th1.begin());
  	    } else {
  	      throw(std::runtime_error("MTest::prepare: invalid dimension"));
  	    }
  	  }
  	}
      }
      // driving variables at the beginning of the time step
      const auto ndv = this->b->getDrivingVariablesSize(this->hypothesis);
      for(unsigned short i=0;i!=ndv;++i){
  	s.e0[i] = state.u0[i]-s.e_th0[i];
      }
    }
    if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
      auto& log = mfront::getLogStream();
      log << "resolution from " << t << " to " << t+dt << '\n';
    }
    if(this->residual){
      this->residual << '\n' << "#resolution from " << t << " to " << t+dt << '\n';
    }
  } // end of MTest::prepare

  void
  MTest::makeLinearPrediction(StudyCurrentState& state,
			      const real dt) const
  {
    auto& scs = state.getStructureCurrentState("");
    if(scs.istates.size()!=1u){
      throw(std::runtime_error("MTest::prepare: "
			       "invalid state"));
    }
    auto& s = scs.istates[0];
    if(state.period>1){
      const auto r = dt/state.dt_1;
      state.u1  = state.u0 +(state.u0 -state.u_1)*r;
      s.iv1     = s.iv0+(s.iv0-s.iv_1)*r;
      s.s1      = s.s0 +(s.s0 -s.s_1 )*r;
    }
  } // end of MTest::makeLinearPrediction
  
  bool
  MTest::computePredictionStiffnessAndResidual(StudyCurrentState& state,
					       tfel::math::matrix<real>& k,
					       tfel::math::vector<real>& r,
					       const real& t,
					       const real& dt,
					       const StiffnessMatrixType smt) const{
    using tfel::material::MechanicalBehaviourBase;
    auto& scs = state.getStructureCurrentState("");
    auto& bwk = scs.getBehaviourWorkSpace();
    if(scs.istates.size()!=1u){
      throw(std::runtime_error("MTest::prepare: "
			       "invalid state"));
    }
    auto& s = scs.istates[0];
    if(!this->b->computePredictionOperator(bwk,s,this->hypothesis,smt)){
      return false;
    }
    std::fill(k.begin(),k.end(),0.);
    std::fill(r.begin(),r.end(),0.);
    updateStiffnessAndResidual(k,r,*(this->b),bwk.kt,s.s0,this->hypothesis);
    const auto ndv = this->b->getDrivingVariablesSize(this->hypothesis);
    const auto nth = this->b->getThermodynamicForcesSize(this->hypothesis);
    // free dilatation treatment
    if(this->b->getBehaviourType()==MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
      for(size_type i=0;i!=nth;++i){
  	for(size_type j=0;j!=ndv;++j){
  	  r(i) -= k(i,j)*(s.e_th1[j]-s.e_th0[j]);
  	}
      }
    }
#pragma message("HERE")    
    //    if(this->first){
      auto a = *(std::max_element(k.begin(),k.end()));
      //      this->first = false;
      //    }
    unsigned short pos = ndv;
    for(const auto& c : this->constraints){
      c->setValues(k,r,state.u0,state.u0,pos,getSpaceDimension(this->hypothesis),t,dt,a);
      pos = static_cast<unsigned short>(pos+c->getNumberOfLagrangeMultipliers());
    }
    return true;
  } // end of MTest::computePredictionStiffnessAndResidual
  
  bool
  MTest::computeStiffnessMatrixAndResidual(StudyCurrentState& state,
					   tfel::math::matrix<real>& k,
					   tfel::math::vector<real>& r,
					   const real t,
					   const real dt,
					   const StiffnessMatrixType mt) const{
    using namespace tfel::material;
    auto& scs = state.getStructureCurrentState("");
    auto& bwk = scs.getBehaviourWorkSpace();
    if(scs.istates.size()!=1u){
      throw(std::runtime_error("MTest::prepare: "
			       "invalid state"));
    }
    auto& s = scs.istates[0];
    const auto ndv = this->b->getDrivingVariablesSize(this->hypothesis);
    if(this->b->getBehaviourType()==MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
      for(size_type i=0;i!=ndv;++i){
  	s.e1[i] = state.u1[i]-s.e_th1[i];
      }
    } else {
      for(size_type i=0;i!=ndv;++i){
  	s.e1[i] = state.u1[i];
      }
    }
    // behaviour integration
    if(!this->b->integrate(s,bwk,this->hypothesis,dt,mt)){
      if(mfront::getVerboseMode()>mfront::VERBOSE_QUIET){
  	auto& log = mfront::getLogStream();
  	log << "MTest::computeStiffnessMatrixAndResidual : "
  	    << "behaviour intregration failed" << std::endl;
      }
      return false;
    }
    updateStiffnessAndResidual(k,r,*(this->b),bwk.k,s.s1,this->hypothesis);
#pragma message("HERE")
    //    if(this->first){
      auto a = *(std::max_element(k.begin(),k.end()));
      //      this->first = false;
      //    }
    const auto d = getSpaceDimension(this->hypothesis);
    unsigned short pos = ndv;
    for(const auto c : this->constraints){
      c->setValues(k,r,state.u0,state.u1,pos,d,t,dt,a);
      pos = static_cast<unsigned short>(pos+c->getNumberOfLagrangeMultipliers());
    }
    return true;
  } // end of MTest::computeStiffnessMatrixAndResidual
  
  std::pair<bool,real>
  MTest::checkConvergence(const StudyCurrentState&  state,
			  const tfel::math::vector<real>& du,
			  const tfel::math::vector<real>& r,
			  const SolverOptions& o,
			  const unsigned int iter,
			  const real t,
			  const real dt) const
  {
    auto report = [&iter,&state](std::ostream& os,
				 const tfel::math::vector<real>::size_type ndv,
				 const real ne,
				 const real nr){
      os << "iteration " << iter << " : " << ne << " " << nr << " (";
      for(size_type i=0;i!=ndv;){
        os << state.u1(i);
        if(++i!=ndv){
          os << " ";
        }
      }
      os << ")" << std::endl;
    };
    const auto& scs = state.getStructureCurrentState("");
    if(scs.istates.size()!=1u){
      throw(std::runtime_error("MTest::prepare: "
			       "invalid state"));
    }
    const auto& s = scs.istates[0];
    const auto ndv = this->b->getDrivingVariablesSize(this->hypothesis);
    auto ne = real(0);
    for(size_type i=0;i!=ndv;++i){
      ne = std::max(ne,std::abs(du(i)));
    }
    auto nr = real(0);
    for(size_type i=0;i!=ndv;++i){
      nr = std::max(nr,std::abs(r(i)));
    }
    if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
      auto& log = mfront::getLogStream();
      report(log,ndv,ne,nr);
    }
    if(this->residual){
      report(this->residual,ndv,ne,nr);
    }
    if((ne>o.eeps)||(nr>o.seps)){
      return {false,ne};
    }
    for(const auto& c : this->constraints){
      if(!c->checkConvergence(state.u1,s.s1,
  			      o.eeps,o.seps,t,dt)){
	return {false,ne};
      }
    }
    return {true,ne};
  }

  std::vector<std::string>
  MTest::getFailedCriteriaDiagnostic(const StudyCurrentState&  state,
				     const tfel::math::vector<real>& du,
				     const tfel::math::vector<real>& r,
				     const SolverOptions& o,
				     const real t,
				     const real dt) const
  {
    const auto& scs = state.getStructureCurrentState("");
    if(scs.istates.size()!=1u){
      throw(std::runtime_error("MTest::prepare: "
			       "invalid state"));
    }
    const auto& s = scs.istates[0];
    const auto ndv = this->b->getDrivingVariablesSize(this->hypothesis);
    auto ne = real(0);
    for(size_type i=0;i!=ndv;++i){
      ne = std::max(ne,std::abs(du(i)));
    }
    auto nr = real(0);
    for(size_type i=0;i!=ndv;++i){
      nr = std::max(nr,std::abs(r(i)));
    }
    auto fc = std::vector<std::string>{};
    if(ne>o.eeps){
      std::ostringstream msg;
      msg << "test on driving variables (error : " << ne
  	  << ", criteria : " << o.eeps << ")";
      fc.push_back(msg.str());
    }
    if(nr>o.seps){
      std::ostringstream msg;
      msg << "test on thermodynamic forces (error : " << nr
  	  << ", criteria : " << o.seps << ")";
      fc.push_back(msg.str());
    }
    for(const auto& c : this->constraints){
      if(!c->checkConvergence(state.u1,s.s1,
  			      o.eeps,o.seps,t,dt)){
  	fc.push_back(c->getFailedCriteriaDiagnostic(state.u1,s.s1,
						    o.eeps,o.seps,
						    t,dt));
      }
    }
    return fc;
  } // end of MTest::getFailedCriteriaDiagnostic
  
  void MTest::postConvergence(StudyCurrentState& state,
			      const real t,
  			      const real dt,
  			      const unsigned int p) const{
    auto& scs = state.getStructureCurrentState("");
    if(scs.istates.size()!=1u){
      throw(std::runtime_error("MTest::prepare: "
			       "invalid state"));
    }
    auto& s = scs.istates[0];
    for(const auto& test : this->tests){
      test->check(state.u1,s.s1,s.iv1,t,dt,p);
    }
  } // end of MTest::postConvergence
  
  void
  MTest::execute(MTestCurrentState& state,
		 MTestWorkSpace& wk,
		 const real ti,
		 const real te) const
  {
    GenericSolver s;
    s.execute(state,wk,*this,this->options,ti,te);
  }
  
  void
  MTest::printOutput(const real t,const MTestCurrentState& s)
  {
    if(this->out){
      auto& cs = s.getStructureCurrentState("").istates[0];
      // number of components of the driving variables and the thermodynamic forces
      const auto ndv = this->b->getDrivingVariablesSize(this->hypothesis);
      const auto nth = this->b->getThermodynamicForcesSize(this->hypothesis);
      unsigned short i;
      this->out << t << " ";
      for(i=0;i!=ndv;++i){
	this->out << s.u0[i] << " ";
      }
      for(i=0;i!=nth;++i){
	this->out << cs.s0[i] << " ";
      }
      std::copy(cs.iv0.begin(),cs.iv0.end(),
		std::ostream_iterator<real>(this->out," "));
      this->out << '\n';
    }
  }
  
  MTest::~MTest()
  {}

} // end namespace mtest
