/*!
 * \file   PipeTest.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   24 nov. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<memory>
#include<fstream>
#include<sstream>
#include<stdexcept>

#include"MFront/MFrontLogStream.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/SolverWorkSpace.hxx"
#include"MTest/Behaviour.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/StudyCurrentState.hxx"
#include"MTest/StructureCurrentState.hxx"
#include"MTest/GenericSolver.hxx"
#include"MTest/PipeLinearElement.hxx"
#include"MTest/PipeQuadraticElement.hxx"
#include"MTest/PipeCubicElement.hxx"
#include"MTest/PipeProfile.hxx"
#include"MTest/PipeProfileHandler.hxx"
#include"MTest/PipeTest.hxx"

namespace mtest{
  
  template<typename T>
  static void setMeshValue(T& v,
			   const char* const m,
			   const char* const n,
			   const T nv, const bool b){
    if(v>=0){
      throw(std::runtime_error(std::string(m)+": "
			       "value '"+std::string(n)+
			       "' already set"));
    }
    if(b ? nv<T(0) : nv<=T(0)){
      throw(std::runtime_error(std::string(m)+": "
			       "invalid value for '"+std::string(n)+"'"));
    }
    v = nv;
  } // end of setMeshValue
  
  void PipeTest::setInnerRadius(const real r)
  {
    if(this->mesh.outer_radius>0){
      if(r>=this->mesh.outer_radius){
	throw(std::runtime_error("PipeTest::setInnerRadius: invalid value. "
				 "Inner radius would be greater than the "
				 "external radius"));
      }
    }
    setMeshValue(this->mesh.inner_radius,
		 "PipeTest::setInnerRadius",
		 "inner radius",r,true);
  } // end of PipeTest::setInnerRadius

  void PipeTest::setOuterRadius(const real r)
  {
    if(this->mesh.inner_radius>0){
      if(r<=this->mesh.inner_radius){
	throw(std::runtime_error("PipeTest::setOuterRadius: invalid value. "
				 "Inner radius would be greater than the "
				 "external radius"));
      }
    }
    setMeshValue(this->mesh.outer_radius,
		 "PipeTest::setOuterRadius",
		 "outer radius",r,false);
  } // end of PipeTest::setOuterRadius

  void PipeTest::setNumberOfElements(const int r)
  {
    setMeshValue(this->mesh.number_of_elements,
		 "PipeTest::setNumberOfElements",
		 "number of elements",r,false);
  } // end of PipeTest::setNumberOfElements

  template<typename T>
  static void checkValue(const T& v,const char* const n){
    if(v<T(0)){
      throw(std::runtime_error("PipeTest::completeInitialisation: "
			       "uninitialized value for '"+std::string(n)+"'"));
    }
  } // end of checkValue
  
  void PipeTest::completeInitialisation(void)
  {
    SingleStructureScheme::completeInitialisation();
    checkValue(mesh.inner_radius,"inner radius");
    checkValue(mesh.outer_radius,"outer radius");
    checkValue(mesh.number_of_elements,"number of elements");
    if(this->pmh==IMPOSEDAXIALFORCE){
      if(this->axial_force.get()==nullptr){
	throw(std::runtime_error("PipeTest::completeInitialisation: "
				 "the axial force evolution must be defined"));
      }
    } else {
      if((this->inner_pressure.get()==nullptr)&&
	 (this->outer_pressure.get()==nullptr)){
	throw(std::runtime_error("PipeTest::completeInitialisation: "
				 "either an inner pressure evolution or "
				 "an outer pressure evolution must be defined"));
      }
    }
    if(this->options.eeps<0){
      this->options.eeps=1.e-11;
    }
    if(this->options.seps<0){
      this->options.seps=1.e-3;
    }
    if(this->hypothesis!=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      throw(std::runtime_error("PipeTest::completeInitialisation: "
			       "invalid modelling hypothesis ('"+
			       ModellingHypothesis::toString(this->hypothesis)+"')"));
    }
    if(this->pmh==DEFAULT){
      this->pmh=ENDCAPEFFECT;
    }
    if(this->pmh==IMPOSEDAXIALFORCE){
      if(this->axial_force.get()==nullptr){
	throw(std::runtime_error("PipeTest::completeInitialisation: "
				 "no axial force defined"));
      }
    }
    if(this->mesh.etype==PipeMesh::DEFAULT){
      this->mesh.etype=PipeMesh::CUBIC;
    }
    if(this->out){
      this->out << "# first  column : time\n"
	"# second column : inner radius\n"
	"# third  column : outer radius\n"
	"# fourth column : inner radius displacement\n"
	"# fifth  column : outer radius displacement\n"
	"# sixth  column : axial displacement" << std::endl;
    }
  } // end of PipeTest::completeInitialisation

  PipeTest::size_type
  PipeTest::getNumberOfNodes(void) const
  {
    if(this->mesh.number_of_elements<=0){
      throw(std::runtime_error("PipeTest::getNumberOfUnknowns: "
			       "uninitialized number of elements"));
    }
    if(this->mesh.etype==PipeMesh::DEFAULT){
      throw(std::runtime_error("PipeTest::getNumberOfUnknowns: "
			       "element type not defined"));
    }
    // number of elements
    const auto ne = size_type(this->mesh.number_of_elements);
    if(this->mesh.etype==PipeMesh::LINEAR){
      return ne+1;
    } else if(this->mesh.etype==PipeMesh::QUADRATIC){
      return 2*ne+1;
    } else if(this->mesh.etype==PipeMesh::CUBIC){
      return 3*ne+1;
    }
    throw(std::runtime_error("PipeTest::getNumberOfUnknowns: "
			     "unknown element type"));
  }
  
  PipeTest::size_type
  PipeTest::getNumberOfUnknowns(void) const
  {
    return this->getNumberOfNodes()+1;
  } // end of PipeTest::getNumberOfUnknowns

  void
  PipeTest::setDisplacementEpsilon(const real e){
    if(this->options.eeps>0){
      throw(std::runtime_error("PipeTest::setDisplacementEpsilon: "
			       "criterion value already set"));
    }
    if(e< 100*std::numeric_limits<real>::min()){
      throw(std::runtime_error("PipeTest::setDisplacementEpsilon: "
			       "invalid criterion value"));
    }
    this->options.eeps=e;
  } // end of PipeTest::setDisplacementEpsilon

  void
  PipeTest::setResidualEpsilon(const real s)
  {
    if(this->options.seps>0){
      throw(std::runtime_error("PipeTest::setResidualEpsilon: the epsilon "
			       "value has already been declared"));
    }
    if(s < 100*std::numeric_limits<real>::min()){
      throw(std::runtime_error("PipeTest::setResidualEpsilon: invalid value"));
    }
    this->options.seps = s;
  }
  
  void
  PipeTest::initializeCurrentState(StudyCurrentState& s) const
  {
    if(this->b.get()==nullptr){
      throw(std::runtime_error("PipeTest::initializeCurrentState: "
    			       "mechanical behaviour not set"));
    }
    if(this->hypothesis==tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      throw(std::runtime_error("PipeTest::initializeCurrentState: "
    			       "modelling hypothesis not set"));
    }
    if((this->mesh.number_of_elements==-1)||
       (this->mesh.inner_radius<0)||
       (this->mesh.outer_radius<0)||
       (this->mesh.etype==PipeMesh::DEFAULT)){
      throw(std::runtime_error("PipeTest::initializeCurrentState: "
    			       "mesh not properly initialised"));
    }
    // unknowns
    const auto psz = this->getNumberOfUnknowns();
    s.initialize(psz);
    // initializing the state of the one and only structure
    std::fill(s.u_1.begin(),s.u_1.end(),real(0));
    s.u0 = s.u_1;
    s.u1 = s.u_1;
    auto& ss = s.getStructureCurrentState("");
    ss.setBehaviour(this->b);
    ss.setModellingHypothesis(this->hypothesis);
    if(this->mesh.etype==PipeMesh::LINEAR){
      // each element has two integration points
      ss.istates.resize(2*this->mesh.number_of_elements);
      PipeLinearElement::setGaussPointsPositions(ss,this->mesh);
    } else if(this->mesh.etype==PipeMesh::QUADRATIC){
      // each element has three integration points
      ss.istates.resize(3*this->mesh.number_of_elements);
      PipeQuadraticElement::setGaussPointsPositions(ss,this->mesh);
    } else if(this->mesh.etype==PipeMesh::CUBIC){
      // each element has three integration points
      ss.istates.resize(4*this->mesh.number_of_elements);
      PipeCubicElement::setGaussPointsPositions(ss,this->mesh);
    } else {
      throw(std::runtime_error("PipeTest::getNumberOfUnknowns: "
			       "unknown element type"));
    }
    for(auto& cs : ss.istates){
      mtest::allocate(cs,*(this->b),this->hypothesis);
      // // setting the intial  values of strains
      // this->b->getDrivingVariablesDefaultInitialValues(s.u_1);
      // copy(this->e_t0.begin(),this->e_t0.end(),s.u_1.begin());
      // // setting the intial  values of stresses
      // copy(this->s_t0.begin(),this->s_t0.end(),cs.s0.begin());
      // // getting the initial values of internal state variables
      // if((this->iv_t0.size()>cs.iv_1.size())||
      //    (this->iv_t0.size()>cs.iv0.size())||
      //    (this->iv_t0.size()>cs.iv1.size())){
      //   throw(std::runtime_error("PipeTest::initializeCurrentState: the number of initial values declared "
      // 			       "by the user for the internal state variables exceeds the "
      // 			       "number of internal state variables declared by the behaviour"));
      // }
      // std::copy(this->iv_t0.begin(),this->iv_t0.end(),cs.iv_1.begin());
      // std::copy(this->iv_t0.begin(),this->iv_t0.end(),cs.iv0.begin());
      // revert the current state
      mtest::revert(ss);
    // // rotation matrix
    // cs.r = this->rm;
    // reference temperature
      const auto pev = this->evm->find("ThermalExpansionReferenceTemperature");
      if(pev!=this->evm->end()){
	const auto& ev = *(pev->second);
	if(!ev.isConstant()){
	  throw(std::runtime_error("PipeTest::initializeCurrentState : "
				   "'ThermalExpansionReferenceTemperature' "
				   "must be a constant evolution"));
	}
	cs.Tref = ev(0);
      }
    }
  } // end of PipeTest::initializeCurrentState

  std::string
  PipeTest::name(void) const
  {
    return "pipe test";
  } // end of PipeTest::name
  
  std::string
  PipeTest::classname(void) const
  {
    return "MTest";
  }

  tfel::tests::TestResult
  PipeTest::execute(void)
  {
    using namespace std;
    using namespace tfel::tests;
    using tfel::math::vector;
    // some checks
    if(times.empty()){
      throw(runtime_error("PipeTest::execute: no times defined"));
    }
    if(times.size()<2){
      throw(runtime_error("PipeTest::execute: invalid number of times defined"));
    }
    // finish initialization
    this->completeInitialisation();
    // initialize current state and work space
    StudyCurrentState state;
    SolverWorkSpace    wk;
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
    tfel::tests::TestResult tr;
    // for(const auto& t : this->tests){
    //   tr.append(t->getResults());
    // }
    return tr; 
  }

  void
  PipeTest::execute(StudyCurrentState& state,
		    SolverWorkSpace& wk,
		    const real ti,
		    const real te) const
  {
    GenericSolver().execute(state,wk,*this,this->options,ti,te);
  }
  
  void
  PipeTest::initializeWorkSpace(SolverWorkSpace& wk) const
  {
    const auto psz = this->getNumberOfUnknowns();
    // clear
    wk.K.clear();
    wk.p_lu.clear();
    wk.x.clear();
    wk.r.clear();
    wk.du.clear();
    //resizing
    wk.K.resize(psz,psz);
    wk.p_lu.resize(psz);
    wk.x.resize(psz);
    wk.r.resize(psz,0.);
    wk.du.resize(psz,0.);
  } // end of PipeTest::initializeWorkSpace
  
  void
  PipeTest::prepare(StudyCurrentState& state,
		    const real t,
		    const real dt) const
  {
    using LE  = PipeLinearElement;
    using QE  = PipeQuadraticElement;
    using CE  = PipeCubicElement;
    auto& scs = state.getStructureCurrentState("");
    // number of elements
    const auto ne = size_t(this->mesh.number_of_elements);
    // loop over the elements
    for(size_type i=0;i!=ne;++i){
      if(this->mesh.etype==PipeMesh::LINEAR){
	LE::computeStrain(scs,this->mesh,state.u0,i,false);
      } else if(this->mesh.etype==PipeMesh::QUADRATIC){
	QE::computeStrain(scs,this->mesh,state.u0,i,false);
      } else if(this->mesh.etype==PipeMesh::CUBIC){
	CE::computeStrain(scs,this->mesh,state.u0,i,false);
      } else {
	throw(std::runtime_error("PipeTest::prepare: "
				 "unknown element type"));
      }
    }
    for(auto& s: scs.istates){
      computeMaterialProperties(s,*(this->evm),*(this->dmpv),
  				this->b->getMaterialPropertiesNames(),t,dt);
      computeExternalStateVariables(s,*(this->evm),
  				    this->b->getExternalStateVariablesNames(),t,dt);
    }
    if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
      auto& log = mfront::getLogStream();
      log << "resolution from " << t << " to " << t+dt << '\n';
    }
  } // end of PipeTest::prepare

  void PipeTest::makeLinearPrediction(StudyCurrentState& state,
				      const real dt) const
  {
    if(state.period>1){
      const auto r = dt/state.dt_1;
      state.u1  = state.u0 +(state.u0 -state.u_1)*r;
      auto& scs = state.getStructureCurrentState("");
      for(auto& s : scs.istates){
	s.iv1     = s.iv0+(s.iv0-s.iv_1)*r;
	s.s1      = s.s0 +(s.s0 -s.s_1 )*r;
      }
    }
  } // end of PipeTest::makeLinearPrediction

  bool
  PipeTest::computePredictionStiffnessAndResidual(StudyCurrentState&,
						  tfel::math::matrix<real>&,
						  tfel::math::vector<real>&,
						  const real&,
						  const real&,
						  const StiffnessMatrixType) const
  {
    return false;
  } // end of PipeTest
  
  bool
  PipeTest::computeStiffnessMatrixAndResidual(StudyCurrentState& state,
					      tfel::math::matrix<real>& k,
					      tfel::math::vector<real>& r,
					      const real t,
					      const real dt,
					      const StiffnessMatrixType mt) const
  {
    using LE  = PipeLinearElement;
    using QE  = PipeQuadraticElement;
    using CE  = PipeCubicElement;
    constexpr const real pi = 3.14159265358979323846;
    // reset r and k
    std::fill(r.begin(),r.end(),real(0));
    if(mt!=StiffnessMatrixType::NOSTIFFNESS){
      std::fill(k.begin(),k.end(),real(0));
    }
    // current pipe state
    auto& scs = state.getStructureCurrentState("");
    // inner radius
    const auto Ri = this->mesh.inner_radius;
    // outer radius
    const auto Re = this->mesh.outer_radius;
    // number of elements
    const auto ne = size_type(this->mesh.number_of_elements);
    // number of nodes
    const auto n  = this->getNumberOfNodes();
    // axial strain
    const auto& ezz = state.u1[n];
    /* external forces */
    if(this->inner_pressure.get()!=nullptr){
      const auto Pi  = (*(this->inner_pressure))(t+dt);
      const auto Ri_ = (this->hpp) ? Ri : Ri+state.u1[0];
      if(this->hpp){
	r(0) += -2*pi*Pi*Ri_;
      } else {
	r(0) += -2*pi*(1+ezz)*Pi*Ri_;
	if(mt!=StiffnessMatrixType::NOSTIFFNESS){
	  k(0,0) += -2*pi*(1+ezz)*Pi;
	  k(0,n) += -2*pi*Pi*Ri_;
	}
      }
      if(this->pmh==ENDCAPEFFECT){
	r(n)   -=   pi*Ri_*Ri_*Pi;
	if(!this->hpp){
	  k(n,0) -= 2*pi*Ri_*Pi;
	}
      }
    }
    if(this->outer_pressure.get()!=nullptr){
      const size_type ln = n-1;
      const auto Pe  = (*(this->outer_pressure))(t+dt);
      const auto Re_ = (this->hpp) ? Re : Re+state.u1[ln];
      if(this->hpp){
	r(ln) += 2*pi*Pe*Re_;
      } else {
	r(ln) += 2*pi*Pe*(1+ezz)*Re_;
	if(mt!=StiffnessMatrixType::NOSTIFFNESS){
	  k(ln,ln) += 2*pi*Pe*(1+ezz);
	  k(ln,n)  += 2*pi*Pe*Re_;
	}
      }
      if(this->pmh==ENDCAPEFFECT){
	r(n)    +=   pi*Re_*Re_*Pe;
	if(!this->hpp){
	  k(n,ln) += 2*pi*Re_*Pe;
	}
      }
    }
    if(this->pmh==IMPOSEDAXIALFORCE){
      r(n) -= (*(this->axial_force))(t+dt);
    }
    // loop over the elements
    for(size_type i=0;i!=ne;++i){
      if(this->mesh.etype==PipeMesh::LINEAR){
	if(!LE::updateStiffnessMatrixAndInnerForces(k,r,scs,*(this->b),
						    state.u1,this->mesh,dt,mt,i)){
	  return false;
	}
      } else if(this->mesh.etype==PipeMesh::QUADRATIC){
	if(!QE::updateStiffnessMatrixAndInnerForces(k,r,scs,*(this->b),
						    state.u1,this->mesh,dt,mt,i)){
	  return false;
	}
      } else if(this->mesh.etype==PipeMesh::CUBIC){
	if(!CE::updateStiffnessMatrixAndInnerForces(k,r,scs,*(this->b),
						    state.u1,this->mesh,dt,mt,i)){
	  return false;
	}
      } else {
	throw(std::runtime_error("PipeTest::computeStiffnessMatrixAndResidual: "
				 "unknown element type"));
      }
    }
    return true;
  } // end of PipeTest::computeStiffnessMatrixAndResidual

  void PipeTest::performSmallStrainAnalysis(void){
    this->hpp=true;
  } // en dof PipeTest::performSmallStrainAnalysis
  
  static real
  PipeTest_getErrorNorm(const tfel::math::vector<real>& v,
			const tfel::math::vector<real>::size_type s)
  {
    using size_type = tfel::math::vector<real>::size_type;
    auto n = real(0);
    for(size_type i=0;i!=s;++i){
      n = std::max(n,std::abs(v(i)));
    }
    return n;
  }
  
  real
  PipeTest::getErrorNorm(const tfel::math::vector<real>& du) const
  {
    return PipeTest_getErrorNorm(du,this->getNumberOfUnknowns());
  } // end of 

  bool
  PipeTest::checkConvergence(const StudyCurrentState&,
			     const tfel::math::vector<real>& du,
			     const tfel::math::vector<real>& r,
			     const SolverOptions&,
			     const unsigned int iter,
			     const real,
			     const real) const
  {
    auto report = [&iter](std::ostream& os,
			  const real ne,
			  const real nr){
      os << "iteration " << iter << " : " << ne << " " << nr << std::endl;
    };
    constexpr const real pi = 3.14159265358979323846;
    const auto Re = this->mesh.outer_radius;
    auto nu = PipeTest_getErrorNorm(du,this->getNumberOfUnknowns());
    auto nr = PipeTest_getErrorNorm(r,this->getNumberOfUnknowns())/(2*pi*Re);
    if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
      auto& log = mfront::getLogStream();
      report(log,nu,nr);
    }
    if(this->residual){
      report(this->residual,nu,nr);
    }
    return ((nu<Re*this->options.eeps)&&(nr<this->options.seps));
  } // end of 

  std::vector<std::string>
  PipeTest::getFailedCriteriaDiagnostic(const StudyCurrentState&,
					const tfel::math::vector<real>& du,
					const tfel::math::vector<real>& r,
					const SolverOptions& o,
					const real,
					const real) const
  {
    constexpr const real pi = 3.14159265358979323846;
    auto cd = std::vector<std::string>{};
    const auto Re = this->mesh.outer_radius;
    auto nu = PipeTest_getErrorNorm(du,this->getNumberOfUnknowns());
    auto nr = PipeTest_getErrorNorm(r,this->getNumberOfUnknowns())/(2*pi*Re);
    if(nu>Re*o.eeps){
      std::ostringstream msg;
      msg << "test on displacement (error : " << nu
  	  << ", criterion value : " << Re*o.eeps << ")";
      cd.push_back(msg.str());
    }
    if(nr>o.seps){
      std::ostringstream msg;
      msg << "test on residual (error : " << nr
  	  << ", criterion value : " << o.seps << ")";
      cd.push_back(msg.str());
    }
    return cd;
  } // end of 

  static PipeTest::size_type
  getNumberOfGaussPoints(const PipeMesh& m){
    // number of elements
    const auto ne = PipeTest::size_type(m.number_of_elements);
    if(m.etype==PipeMesh::LINEAR){
      return 2*ne;
    } else if(m.etype==PipeMesh::QUADRATIC){
      return 3*ne;
    } else if(m.etype==PipeMesh::CUBIC){
      return 4*ne;
    }
    throw(std::runtime_error("getNumberOfGaussPoints: "
			     "unknown element type"));
  } // end of getNumberOfGaussPoints
  
  void
  PipeTest::postConvergence(StudyCurrentState& state,
			    const real t,
			    const real dt,
			    const unsigned int) const
  {
    // current pipe state
    auto& scs = state.getStructureCurrentState("");
    for(const auto& p: this->profiles){
      *(p.out) << "#Time " << t+dt << '\n';
      // loop over the elements
      for(size_type i=0;i!=getNumberOfGaussPoints(this->mesh);++i){
	const auto& s = scs.istates[i];
	*(p.out) << s.position;
	for(const auto& pp: p.profiles){
	  pp->report(*(p.out),s);
	}
	*(p.out) << '\n';
      }
    }
  } // end of PipeTest::postConvergence

  void
  PipeTest::addProfile(const std::string& f,
		       const std::vector<std::string>& cn)
  {
    auto ph = PipeProfileHandler{};
    ph.out = std::shared_ptr<std::ostream>(new std::ofstream(f));
    if(!(*(ph.out))){
      throw(std::runtime_error("PipeTest::addProfile: "
			       "can't open file '"+f+"'"));
    }
    ph.out->exceptions(std::ofstream::failbit|std::ofstream::badbit);
    *(ph.out) << "#First column: radial position\n";
    auto i=1;
    for(const auto& c: cn){
      ++i;
      *(ph.out) << "#Column " << i << ": ";
      if(c=="SRR"){
	*(ph.out) << "radial stress \n";
	ph.profiles.emplace_back(new PipeStressProfile(0u));
      } else if(c=="STT"){
	*(ph.out) << "hoop stress \n";
	ph.profiles.emplace_back(new PipeStressProfile(2u));
      } else if(c=="SZZ"){
	*(ph.out) << "axial stress \n";
	ph.profiles.emplace_back(new PipeStressProfile(1u));
      } else if(c=="ERR"){
	*(ph.out) << "radial strain \n";
	ph.profiles.emplace_back(new PipeStrainProfile(0u));
      } else if(c=="ETT"){
	*(ph.out) << "hoop strain \n";
	ph.profiles.emplace_back(new PipeStrainProfile(2u));
      } else if(c=="EZZ"){
	*(ph.out) << "axial strain \n";
	ph.profiles.emplace_back(new PipeStrainProfile(1u));
      } else {
	const auto ivnames = this->b->getInternalStateVariablesNames();
	const auto piv = std::find(ivnames.begin(),ivnames.end(),c);
	if(piv==ivnames.end()){
	  throw(std::runtime_error("PipeTest::addProfile: "
				   "no internal state variable named '"+c+"'"));
	}
	*(ph.out) << c << " internal state variable\n";
	ph.profiles.emplace_back(new PipeInternalStateVariableProfile(piv-ivnames.begin()));
      }
    }
    this->profiles.push_back(ph);
  } // end of PipeTest::addProfile

  void
  PipeTest::setModellingHypothesis(const std::string& h)
  {
    if((h!="AxisymmetricalGeneralisedPlaneStrain")&&
       (h!="AxisymmetricalGeneralisedPlaneStress")){
      throw(std::runtime_error("PipeTest::setModellingHypothesis: "
			       "invalid modelling hypothesis '"+h+"'"));
    }
    if(this->hypothesis!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      throw(std::runtime_error("PipeTest::setModellingHypothesis: "
			       "modelling hypothesis already defined"));
    }
    this->hypothesis=ModellingHypothesis::fromString(h);
  } // end of PipeTest::setModellingHypothesis

  void
  PipeTest::setDefaultModellingHypothesis(void)
  {
    if(this->hypothesis!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      throw(std::runtime_error("PipeTest::setDefaultModellingHypothesis: "
			       "modelling hypothesis already defined"));
    }
    this->hypothesis=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
  } // end of PipeTest::setDefaultModellingHypothesis

  void
  PipeTest::setPipeModellingHypothesis(const PipeTest::PipeModellingHypothesis ph){
    if(this->pmh!=DEFAULT){
      throw(std::runtime_error("PipeTest::setPipeModellingHypothesis: "
			       "modelling hypothesis already defined"));
    }
    this->pmh=ph;
  } // end of PipeTest::setPipeModellingHypothesis

  void
  PipeTest::setElementType(const PipeMesh::ElementType ph){
    if(this->mesh.etype!=PipeMesh::DEFAULT){
      throw(std::runtime_error("PipeTest::setElementType: "
			       "element type already defined"));
    }
    this->mesh.etype=ph;
  } // end of PipeTest::setElementType
  
  void
  PipeTest::setInnerPressureEvolution(std::shared_ptr<Evolution> p)
  {
    if(this->inner_pressure.get()!=nullptr){
      throw(std::runtime_error("PipeTest::setInnerPressureEvolution: "
			       "inner pressure evolution already set"));
    }
    this->inner_pressure = p;
  } // end of PipeTest::setInnerPressureEvolution

  void
  PipeTest::setOuterPressureEvolution(std::shared_ptr<Evolution> p)
  {
    if(this->outer_pressure.get()!=nullptr){
      throw(std::runtime_error("PipeTest::setOuterPressureEvolution: "
			       "outer pressure evolution already set"));
    }
    this->outer_pressure = p;
  } // end of PipeTest::setOuterPressureEvolution

  void
  PipeTest::setAxialForceEvolution(std::shared_ptr<Evolution> f)
  {
    if(this->pmh!=IMPOSEDAXIALFORCE){
      throw(std::runtime_error("PipeTest::setAxialForceEvolution: "
			       "axial force evolution can be set "
			       "only if the pipe modelling hypothesis is "
			       "'ImposedAxialForce'"));
    }
    if(this->axial_force.get()!=nullptr){
      throw(std::runtime_error("PipeTest::setAxialForceEvolution: "
			       "axial force evolution already set"));
    }
    this->axial_force = f;
  } // end of PipeTest::setAxialForceEvolution
  
  void
  PipeTest::printOutput(const real t,
			const StudyCurrentState& state){
    const auto& u1 = state.u1;
    const auto n  = this->getNumberOfNodes();
    // inner radius
    const auto Ri = this->mesh.inner_radius;
    // outer radius
    const auto Re = this->mesh.outer_radius;
    this->out << t   << " " << Ri+u1[0] << " " << Re+u1[n-1]
	      << " " << u1[0] << " " << u1[n-1] << " "
	      << u1[n] << std::endl;
  } // end of PipeTest::printOutput
  
  PipeTest::~PipeTest() = default;
  
} // end of namespace mtest
