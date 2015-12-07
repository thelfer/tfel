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

#include<stdexcept>

#include"MFront/MFrontLogStream.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/SolverWorkSpace.hxx"
#include"MTest/Behaviour.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/StudyCurrentState.hxx"
#include"MTest/StructureCurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/PipeTest.hxx"

namespace mtest{

  template<typename T>
  static void setMeshValue(T& v,
			   const char* const m,
			   const char* const n,
			   const T nv){
    if(v>=0){
      throw(std::runtime_error(std::string(m)+": "
			       "value '"+std::string(n)+
			       "' already set"));
    }
    if(nv<=real(0)){
      throw(std::runtime_error(std::string(m)+": "
			       "invalid value for '"+std::string(n)+"'"));
    }
    v = nv;
  }
  
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
		 "inner radius",r);
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
		 "outer radius",r);
  } // end of PipeTest::setOuterRadius

  void PipeTest::setNumberOfElements(const int r)
  {
    setMeshValue(this->mesh.number_of_elements,
		 "PipeTest::setNumberOfElements",
		 "number of elements",r);
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
    checkValue(mesh.inner_radius,"inner radius");
    checkValue(mesh.outer_radius,"outer radius");
    checkValue(mesh.number_of_elements,"number of elements");
  } // end of PipeTest::completeInitialisation
  
  PipeTest::size_type
  PipeTest::getNumberOfUnknowns(void) const
  {
    if(this->mesh.number_of_elements<=0){
      throw(std::runtime_error("PipeTest::getNumberOfUnknowns: "
			       "uninitialized number of elements"));
    }
    // number of nodes+axial displacement
    return size_type{this->mesh.number_of_elements+2};
  } // end of 

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
    // each element has two integration points
    ss.istates.resize(2*this->mesh.number_of_elements);
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
    //    this->printOutput(*pt,state);
    // real work begins here
    while(pt2!=this->times.end()){
      // allowing subdivisions of the time step
      //      this->execute(state,wk,*pt,*pt2);
      //      this->printOutput(*pt2,state);
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
    using namespace tfel::material;
    auto& scs = state.getStructureCurrentState("");
    // evaluations of the materials properties at the end of the
    // time step
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
  PipeTest::computePredictionStiffnessAndResidual(StudyCurrentState& state,
						  tfel::math::matrix<real>& k,
						  tfel::math::vector<real>& r,
						  const real& t,
						  const real& dt,
						  const StiffnessMatrixType smt) const
  {
    // auto& scs = state.getStructureCurrentState("");
    // auto& bwk = scs.getBehaviourWorkSpace();
    // for(size_type i=0;i!=size_type(this->mesh.number_of_elements);++i){
    //   // treating the ith element
    //   auto& s = scs.istates[i];
    //   if(!this->b->computePredictionOperator(bwk,s,this->hypothesis,smt)){
    // 	return false;
    //   }
    //   updateStiffnessAndResidual(k,r,bwk.kt,s.s0,this->hypothesis);
    // }
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
    constexpr const real pi = 3.14159265358979323846;
    auto interpolate = [](const real v0,const real v1,const real x){
      return 0.5*((1-x)*v0+(1+x)*v1);
    };
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
    const auto n  = ne+1;
    // axial strain
    const auto& ezz = state.u1[n];
    /* external forces */
    if(scs.evm.find("InnerPressure")!=scs.evm.end()){
      const auto Pi = (*(scs.evm.at("InnerPressure")))(t+dt);
      r(0)  = -2*pi*(1+ezz)*Pi*(Ri+state.u1[0])*Ri;
      if(mt!=StiffnessMatrixType::NOSTIFFNESS){
	k(0,0) = -2*pi*(1+ezz)*Pi*Ri;
	k(0,n) = -2*pi*Pi*(Ri+state.u1[n-1])*Ri;
      }
    }
    if(scs.evm.find("OuterPressure")!=scs.evm.end()){
      const auto Pe = (*(scs.evm.at("OuterPressure")))(t+dt);
      r(n-1) = -2*pi*(1+ezz)*Pe*(Re+state.u1[n-1])*Re;
      if(mt!=StiffnessMatrixType::NOSTIFFNESS){
	k(n-1,n-1) = -2*pi*(1+ezz)*Pe*Re;
	k(n-1,n)   = -2*pi*Pe*(Re+state.u1[n-1])*Re;
      }
    }
    /* inner forces */
    auto& bwk = scs.getBehaviourWorkSpace();
    const real dr = (Re-Ri)/ne;
    // absolute value of the Gauss points position in the reference
    // element
    const real abs_pg = 1./std::sqrt(real(3));
    // value of the Gauss points position in the reference element
    const real pg_radii[2] = {-abs_pg,abs_pg};
    // Gauss point weight
    constexpr real wg = real(0.5);
    // loop over the elements
    for(size_type i=0;i!=ne;++i){
      // radial position of the first node
      const auto r0 = Ri+dr*i;
      // radial position of the second node
      const auto r1 = Ri+dr*(i+1);
      // jacobian of the transformation
      const auto J = dr/2;
      // radial displacement of the first node
      const auto ur0 = state.u1[i];
      // radial displacement of the second node
      const auto ur1 = state.u1[i+1];
      for(const auto g : {0,1}){
	// Gauss point position in the reference element
	const auto pg = pg_radii[g];
	// radial position of the Gauss point
	const auto rg = interpolate(r0,r1,pg);
	/* treating the first Gauss point of the ith element */
	auto& s = scs.istates[2*i+g];
	// strain
	s.e1[0] = (ur1-ur0)/dr;
	s.e1[1] = ezz;
	s.e1[2] = interpolate(ur0,ur1,pg)/rg;
	if(!this->b->integrate(s,bwk,this->hypothesis,dt,mt)){
	  if(mfront::getVerboseMode()>mfront::VERBOSE_QUIET){
	    auto& log = mfront::getLogStream();
	    log << "PTest::computeStiffnessMatrixAndResidual : "
		<< "behaviour intregration failed" << std::endl;
	  }
	  return false;
	}
	// stress tensor
	const auto pi_rr = s.s1[0];
	const auto pi_zz = s.s1[1];
	const auto pi_tt = s.s1[2];
	// innner forces
	r[i]   += 2*pi*wg*J*(pi_rr*(-rg/dr)+pi_tt*(1-pg)/2);
	r[i+1] += 2*pi*wg*J*(pi_rr*( rg/dr)+pi_tt*(1+pg)/2);
	r[n-1] += 2*pi*wg*J*rg*pi_zz;
	// jacobian matrix
	if(mt!=StiffnessMatrixType::NOSTIFFNESS){
	  const auto& bk = bwk.k;
	  const real de10_dur0 = -1/dr;
	  const real de12_dur0 = (1-pg)/(2*rg);
	  const real de10_dur1 = 1/dr;
	  const real de12_dur1 = (1+pg)/(2*rg);
	  const real w = 2*pi*wg*J;
	  k(i,i)   += w*(bk(0,0)*de10_dur0*(-rg/dr)+
			 bk(0,2)*de12_dur0*(-rg/dr)+
			 bk(2,0)*de10_dur0*(1-pg)/2+
			 bk(2,2)*de12_dur0*(1-pg)/2);
	  k(i,i+1) += w*(bk(0,0)*de10_dur1*(-rg/dr)+
			 bk(0,2)*de12_dur1*(-rg/dr)+
			 bk(2,0)*de10_dur1*(1-pg)/2+
			 bk(2,2)*de12_dur1*(1-pg)/2);
	  k(i,n-1) += w*(bk(0,1)*(-rg/dr)+bk(2,1)*(1-pg)/2);
	  k(i+1,i) += w*(bk(0,0)*de10_dur0*( rg/dr)+
			 bk(0,2)*de12_dur0*( rg/dr)+
			 bk(2,0)*de10_dur0*(1+pg)/2+
			 bk(2,2)*de12_dur0*(1+pg)/2);
	  k(i+1,i+1) += w*(bk(0,0)*de10_dur1*( rg/dr)+
			   bk(0,2)*de12_dur1*( rg/dr)+
			   bk(2,0)*de10_dur1*(1+pg)/2+
			   bk(2,2)*de12_dur1*(1+pg)/2);
	  k(i+1,n-1) += w*(bk(0,1)*( rg/dr)+bk(2,1)*(1+pg)/2);
	  k(n-1,i)   += w*rg*(bk(1,0)*de10_dur0+
			      bk(1,2)*de12_dur0);
	  k(n-1,i+1) += w*rg*(bk(1,0)*de10_dur1+
			      bk(1,2)*de12_dur1);
	  k(n,n)     += w*rg*bk(1,1);
	}
      }
    }
    return true;
  } // end of computeStiffnessMatrixAndResidual

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
			     const tfel::math::vector<real>&,
			     const tfel::math::vector<real>&,
			     const SolverOptions&,
			     const unsigned int,
			     const real,
			     const real) const
  {
    return true;
  } // end of 

  std::vector<std::string>
  PipeTest::getFailedCriteriaDiagnostic(const StudyCurrentState&,
					const tfel::math::vector<real>&,
					const tfel::math::vector<real>&,
					const SolverOptions&,
					const real,
					const real) const
  {
    return {};
  } // end of 

  void
  PipeTest::postConvergence(StudyCurrentState&,
			    const real,
			    const real,
			    const unsigned int) const
  {} // end of 
  
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
  
  PipeTest::~PipeTest() = default;
  
} // end of namespace mtest
