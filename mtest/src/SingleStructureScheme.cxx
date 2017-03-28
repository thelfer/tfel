/*!
 * \file   SingleStructureScheme.cxx
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

#include<cmath>
#include<vector>
#include<string>
#include<cstdlib>
#include<iostream>
#include<iterator>
#include<algorithm>
#include<sstream>

#include"MFront/MFrontLogStream.hxx"

#include"MTest/LogarithmicStrain1DBehaviourWrapper.hxx"
#include"MTest/Behaviour.hxx"
#include"MTest/StudyCurrentState.hxx"
#include"MTest/StructureCurrentState.hxx"
#include"MTest/CurrentState.hxx"

#include"MTest/SingleStructureScheme.hxx"

namespace mtest{
  
  static void
  checkIfDeclared(const std::vector<std::string>& names,
		  const EvolutionManager& m,
		  const std::string& type)
  {
    for(const auto& n : names){
      if(m.find(n)==m.end()){
	throw(std::runtime_error("no '"+type+"' named '"+
				 n+"' declared"));
      }
    }
  }

  static void
  checkIfDeclared(const std::vector<std::string>& names,
		  const EvolutionManager& evm1,
		  const EvolutionManager& evm2,
		  const std::string& type)
  {
    for(const auto& n : names){
      if(evm1.find(n)==evm1.end()){
	if(evm2.find(n)==evm2.end()){
	  throw(std::runtime_error("no "+type+" named '"+
				   n+"' declared"));
	}
      }
    }
  }
  
  SingleStructureScheme::SingleStructureScheme()
    : dmpv(new EvolutionManager())
  {} // end of SingleStructureScheme::SingleStructureScheme

  void
  SingleStructureScheme::setMaterialProperty(const std::string& n,
					     const EvolutionPtr p,
					     const bool check)
  {
    using namespace std;
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setMaterialProperty: "
			       "no behaviour defined"));
    }
    const auto& mpnames = this->b->getMaterialPropertiesNames();
    bool is_mp = find(mpnames.begin(),mpnames.end(),n)==mpnames.end();
    if((is_mp)&&(n!="ThermalExpansion") &&(n!="ThermalExpansion1")&&
       (n!="ThermalExpansion2")&&(n!="ThermalExpansion3")){
      ostringstream msg;
      msg << "SingleStructureScheme::setMaterialProperty: "
	  << "the behaviour does not declare a material property '"
	  << n << "'.";
      if(!mpnames.empty()){
	msg << "\nThe behaviour declares:";
	for(const auto& mp : mpnames){
	  msg << "\n- '" << mp << "'";
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
  } // end of SingleStructureScheme::setMaterialProperty

  void
  SingleStructureScheme::setExternalStateVariable(const std::string& n,
						  const EvolutionPtr p,
						  const bool check)
  {
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setExternalStateVariable: "
			       "no behaviour defined"));
    }
    const auto& evsnames = this->b->getExternalStateVariablesNames();
    if(find(evsnames.begin(),evsnames.end(),n)==evsnames.end()){
      throw(std::runtime_error("SingleStructureScheme::setExternalStateVariable: "
			       "the behaviour does not declare an "
			       "external state variable named '"+n+"'"));
    }
    this->addEvolution(n,p,false,check);
  } // end of SingleStructureScheme::setExternalStateVariable

  void
  SingleStructureScheme::setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy p)
  {
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setOutOfBoundsPolicy: "
			       "no behaviour defined"));
    }
    this->b->setOutOfBoundsPolicy(p);
  }

  void SingleStructureScheme::setBehaviour(const std::string& i,
					   const std::string& l,
					   const std::string& f,
					   const Parameters& d)
  {
    if(this->hypothesis==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->setDefaultModellingHypothesis();
    }
    this->setBehaviour(Behaviour::getBehaviour(i,l,f,d,this->hypothesis));
  }

  void SingleStructureScheme::setBehaviour(const std::string& w,
					   const std::string& i,
					   const std::string& l,
					   const std::string& f,
					   const Parameters& d)
  {
    if(this->hypothesis==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->setDefaultModellingHypothesis();
    }
    auto bp = Behaviour::getBehaviour(i,l,f,d,this->hypothesis);
    if(w=="LogarithmicStrain1D"){
      auto wp = std::shared_ptr<Behaviour>(new LogarithmicStrain1DBehaviourWrapper(bp));
      this->setBehaviour(wp);
    } else {
      throw(std::runtime_error("SingleStructureScheme::setBehaviour: "
			       "unknown wrapper '"+w+"'"));
    }
  }

  void SingleStructureScheme::checkBehaviourConsistency(const std::shared_ptr<Behaviour>&)
  {}
  
  void SingleStructureScheme::setBehaviour(const std::shared_ptr<Behaviour>& bp)
  {
    if(this->b!=nullptr){
      throw(std::runtime_error("SingleStructureScheme::setBehaviour: "
			       "behaviour already defined"));
    }
    this->checkBehaviourConsistency(bp);
    this->b = bp;
    this->declareVariables(this->b->getMaterialPropertiesNames(),true);
    this->declareVariables(this->b->getExternalStateVariablesNames(),true);
    this->ivfullnames = this->b->expandInternalStateVariablesNames();
    for(const auto& n : this->ivfullnames){
      this->declareVariable(n,true);
    }
    // declaring behaviour variables
    this->declareVariables(this->b->getDrivingVariablesComponents(),true);
    this->declareVariables(this->b->getThermodynamicForcesComponents(),true);
  } // end of SingleStructureScheme::setBehaviour

  void SingleStructureScheme::setParameter(const std::string& n,
					   const double v)
  {
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setParameter: "
			       "no behaviour defined"));
    }
    this->b->setParameter(n,v);
  }

  void SingleStructureScheme::setIntegerParameter(const std::string& n,
						  const int v)
  {
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setIntegerParameter: "
			       "no behaviour defined"));
    }
    this->b->setIntegerParameter(n,v);
  }

  void
  SingleStructureScheme::setUnsignedIntegerParameter(const std::string& n,
						     const unsigned int v)
  {
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setUnsignedIntegerParameter: "
			       "no behaviour defined"));
    }
    this->b->setUnsignedIntegerParameter(n,v);
  }

  tfel::material::MechanicalBehaviourBase::BehaviourType
  SingleStructureScheme::getBehaviourType() const{
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::getBehaviourType: "
			       "no behaviour defined"));
    }
    return this->b->getBehaviourType();
  }

  tfel::material::MechanicalBehaviourBase::Kinematic
  SingleStructureScheme::getBehaviourKinematic() const{
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::getBehaviourKinematic: "
			       "no behaviour defined"));
    }
    return this->b->getBehaviourKinematic();
  }
  
  std::shared_ptr<Behaviour>
  SingleStructureScheme::getBehaviour(){
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::getBehaviour: "
			       "no behaviour defined"));
    }
    return this->b;
  }

  void
  SingleStructureScheme::completeInitialisation()
  {
    SchemeBase::completeInitialisation();
    if(this->b==nullptr){
      throw(std::runtime_error("MTest::completeInitialisation: "
			       "no behaviour defined"));
    }
    // check if material properties and external state variables are declared
    const auto mpnames  = this->b->getMaterialPropertiesNames();
    const auto esvnames = this->b->getExternalStateVariablesNames();
    this->b->setOptionalMaterialPropertiesDefaultValues(*(this->dmpv),
							*(this->evm));
    checkIfDeclared(mpnames,*(this->evm),*(this->dmpv),
		    "material property");
    checkIfDeclared(esvnames,*(this->evm),"external state variable");
  } // end of SingleStructureScheme::completeInitialisation

  bool
  SingleStructureScheme::doPackagingStep(StudyCurrentState& state) const
  {
    auto& scs = state.getStructureCurrentState("");
    auto& bwk = scs.getBehaviourWorkSpace();
    for(auto& s : scs.istates){
      if(!this->b->doPackagingStep(s,bwk)){
	return false;
      }
    }
    return true;
  } // end of SingleStructureScheme::doPackagingStep
  
  StiffnessMatrixType
  SingleStructureScheme::getDefaultStiffnessMatrixType() const{
    return this->b->getDefaultStiffnessMatrixType();
  } // end of SingleStructureScheme::getDefaultStiffnessMatrixType

  void
  SingleStructureScheme::setScalarInternalStateVariableInitialValue(const std::string& n,
								    const real v)
  {
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setScalarInternalStateVariableInitialValue: "
			       "no behaviour defined"));
    }
    const auto& ivsnames = this->b->getInternalStateVariablesNames();
    if(std::find(ivsnames.begin(),ivsnames.end(),n)==ivsnames.end()){
      throw(std::runtime_error("SingleStructureScheme::setScalarInternalStateVariableInitialValue: "
			       "the behaviour does not declare an internal "
			       "state variable named '"+n+"'"));
    }
    const auto type = this->b->getInternalStateVariableType(n);
    const auto pos  = this->b->getInternalStateVariablePosition(n);
    if(type!=0){
      throw(std::runtime_error("SingleStructureScheme::setScalarInternalStateVariableInitialValue: "
			       "internal state variable '"+n+"' is not defined"));
    }
    if(this->iv_t0.size()<=pos){
      this->iv_t0.resize(pos+1,0.);
    }
    this->iv_t0[pos] = v;
  }

  void
  SingleStructureScheme::setStensorInternalStateVariableInitialValues(const std::string& n,
								      const std::vector<real>& v)
  {
    if(this->b==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setStensorInternalStateVariableInitialValue: "
			       "no behaviour defined"));
    }
    const auto& ivsnames = this->b->getInternalStateVariablesNames();
    if(std::find(ivsnames.begin(),ivsnames.end(),n)==ivsnames.end()){
      throw(std::runtime_error("SingleStructureScheme::setStensorInternalStateVariableInitialValue: "
			       "the behaviour don't declare an internal "
			       "state variable named '"+n+"'"));
    }
    const auto type = this->b->getInternalStateVariableType(n);
    const auto pos  = this->b->getInternalStateVariablePosition(n);
    if(type!=1){
      throw(std::runtime_error("SingleStructureScheme::setStensorInternalStateVariableInitialValue: "
			       "internal state variable '"+n+"' is not defined"));
    }
    const auto N = tfel::material::getStensorSize(this->hypothesis);
    if(v.size()!=N){
      throw(std::runtime_error("SingleStructureScheme::setStensorInternalStateVariableInitialValues: "
			       "invalid values size"));
    }
    if(this->iv_t0.size()<pos+N){
      this->iv_t0.resize(pos+N,0.);
    }
    std::copy(v.begin(),v.end(),this->iv_t0.begin()+pos);
  } // end of SingleStructureScheme::setStensorInternalStateVariableInitialValue

  void
  SingleStructureScheme::setTensorInternalStateVariableInitialValues(const std::string& n,
								     const std::vector<real>& v)
  {
    using namespace std;
    if(this->b==nullptr){
      throw(runtime_error("SingleStructureScheme::setTensorInternalStateVariableInitialValue: "
			  "no behaviour defined"));
    }
    const auto& ivsnames = this->b->getInternalStateVariablesNames();
    if(find(ivsnames.begin(),ivsnames.end(),n)==ivsnames.end()){
      throw(runtime_error("SingleStructureScheme::setTensorInternalStateVariableInitialValue: "
			  "the behaviour does not declare an internal state "
			  "variable named '"+n+"'"));
    }
    const auto type = this->b->getInternalStateVariableType(n);
    const auto pos  = this->b->getInternalStateVariablePosition(n);
    if(type!=3){
      throw(runtime_error("SingleStructureScheme::setTensorInternalStateVariableInitialValue: "
			  "internal state variable '"+n+"' is not defined"));
    }
    const auto N = tfel::material::getTensorSize(this->hypothesis);
    if(v.size()!=N){
      throw(runtime_error("SingleStructureScheme::setTensorInternalStateVariableInitialValues: "
			  "invalid values size"));
    }
    if(this->iv_t0.size()<pos+N){
      this->iv_t0.resize(pos+N,0.);
    }
    copy(v.begin(),v.end(),this->iv_t0.begin()+pos);
  } // end of SingleStructureScheme::setTensorInternalStateVariableInitialValue

  void
  SingleStructureScheme::prepare(StudyCurrentState& state,
				 const real t,
				 const real dt) const
  {
    using namespace tfel::material;
    auto& scs = state.getStructureCurrentState("");
    // evaluations of the materials properties, state variables at the
    // end of the time step. Computation of thermal expansion if needed.
    for(auto& s: scs.istates){
      this->setGaussPointPositionForEvolutionsEvaluation(s);
      computeMaterialProperties(s,*(this->evm),*(this->dmpv),
  				this->b->getMaterialPropertiesNames(),t,dt);
      computeExternalStateVariables(s,*(this->evm),
  				    this->b->getExternalStateVariablesNames(),t,dt);
      // thermal expansion
      if((this->handleThermalExpansion)&&
	 ((this->b->getBehaviourType()==MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)||
	  ((this->b->getBehaviourType()==MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)&&
	   (this->b->getBehaviourKinematic()==MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1)))){
	if(this->b->getSymmetryType()==0){
	  // isotropic case
	  computeThermalExpansion(s,*(this->evm),t,dt);
	} else if(this->b->getSymmetryType()==1){
	  // orthotropic case
	  computeThermalExpansion(s,*(this->evm),t,dt,
				  getSpaceDimension(this->hypothesis));
	} else {
	  throw(std::runtime_error("SingleStructureScheme::prepare: "
				   "unsupported behaviour symmetry"));
	}
      }
    }
    if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
      auto& log = mfront::getLogStream();
      log << "resolution from " << t << " to " << t+dt << '\n';
    }
    if(this->residual){
      this->residual << '\n' << "#resolution from " << t << " to " << t+dt << '\n';
    }
  } // end of SingleStructureScheme::prepare

  void SingleStructureScheme::setHandleThermalExpansion(const bool b1)
  {
    if(!this->handleThermalExpansion){
      throw(std::runtime_error("SingleStructureScheme::setHandleThermalExpansion: "
			       "thermal expansion is not handled"));
    }
    this->handleThermalExpansion = b1;
  }
  
  SingleStructureScheme::~SingleStructureScheme() = default;
  
}
