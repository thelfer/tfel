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

#include<sstream>

#include"TFEL/System/ExternalLibraryManager.hxx"

#include"MTest/Behaviour.hxx"
#ifdef HAVE_CASTEM
#include"MTest/CastemSmallStrainBehaviour.hxx"
#include"MTest/CastemFiniteStrainBehaviour.hxx"
#include"MTest/CastemCohesiveZoneModel.hxx"
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
#include"MTest/AsterSmallStrainBehaviour.hxx"
#include"MTest/AsterFiniteStrainBehaviour.hxx"
#include"MTest/AsterCohesiveZoneModel.hxx"
#endif /* HAVE_ASTER  */
#ifdef HAVE_ABAQUS
#include"MTest/AbaqusSmallStrainBehaviour.hxx"
#endif /* HAVE_ABAQUS  */
#ifdef HAVE_CYRANO
#include"MTest/CyranoBehaviour.hxx"
#endif /* HAVE_CYRANO  */

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
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setMaterialProperty: "
			       "no behaviour defined"));
    }
    const auto& mpnames = this->b->getMaterialPropertiesNames();
    bool is_mp = find(mpnames.begin(),mpnames.end(),n)==mpnames.end();
    if((is_mp)&&(n!="ThermalExpansion") &&(n!="ThermalExpansion1")&&
       (n!="ThermalExpansion2")&&(n!="ThermalExpansion3")){
      ostringstream msg;
      msg << "SingleStructureScheme::setMaterialProperty: "
	  << "the behaviour don't declare a material property '" << n << "'.";
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
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setExternalStateVariable: "
			       "no behaviour defined"));
    }
    const auto& evsnames = this->b->getExternalStateVariablesNames();
    if(find(evsnames.begin(),evsnames.end(),n)==evsnames.end()){
      throw(std::runtime_error("SingleStructureScheme::setExternalStateVariable: "
			       "the behaviour don't declare an "
			       "external state variable named '"+n+"'"));
    }
    this->addEvolution(n,p,false,check);
  } // end of SingleStructureScheme::setExternalStateVariable

  void
  SingleStructureScheme::setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy p)
  {
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setOutOfBoundsPolicy: "
			       "no behaviour defined"));
    }
    this->b->setOutOfBoundsPolicy(p);
  }

  void
  SingleStructureScheme::setBehaviour(const std::string& i,
				      const std::string& l,
				      const std::string& f)
  {
    using namespace std;
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultModellingHypothesis();
    }
    if(this->b.get()!=nullptr){
      throw(std::runtime_error("SingleStructureScheme::setBehaviour: "
			       "behaviour already defined"));
    }
#ifdef HAVE_CASTEM
    if((i=="castem")||(i=="umat")){
      auto& elm = ELM::getExternalLibraryManager();
      const auto type = elm.getUMATBehaviourType(l,f);
      if(type==1u){
	this->b = shared_ptr<Behaviour>(new CastemSmallStrainBehaviour(this->hypothesis,l,f));
      } else if(type==2u){
	this->b = shared_ptr<Behaviour>(new CastemFiniteStrainBehaviour(this->hypothesis,l,f));
      } else if(type==3u){
	this->b = shared_ptr<Behaviour>(new CastemCohesiveZoneModel(this->hypothesis,l,f));
      } else {
	ostringstream msg;
	msg << "SingleStructureScheme::setBehaviour: "
	  "unsupported behaviour type (" << type << ")";
	throw(runtime_error(msg.str()));
      }
    }
#endif
#ifdef HAVE_ASTER
    if(i=="aster"){
      auto& elm = ELM::getExternalLibraryManager();
      const auto type = elm.getUMATBehaviourType(l,f);
      if(type==1u){
	this->b = shared_ptr<Behaviour>(new AsterSmallStrainBehaviour(this->hypothesis,l,f));
      } else if(type==2u){
	this->b = shared_ptr<Behaviour>(new AsterFiniteStrainBehaviour(this->hypothesis,l,f));
      } else if(type==3u){
	this->b = shared_ptr<Behaviour>(new AsterCohesiveZoneModel(this->hypothesis,l,f));
      } else {
	ostringstream msg;
	msg << "SingleStructureScheme::setBehaviour: "
	  "unsupported behaviour type (" << type << ")";
	throw(runtime_error(msg.str()));
      }
    }
#endif
#ifdef HAVE_ABAQUS
    if(i=="abaqus"){
      auto& elm = ELM::getExternalLibraryManager();
      const auto type = elm.getUMATBehaviourType(l,f);
      if(type==1u){
	this->b = shared_ptr<Behaviour>(new AbaqusSmallStrainBehaviour(this->hypothesis,l,f));
	// }  else if(type==2u){
	// 	this->b = shared_ptr<Behaviour>(new AbaqusFiniteStrainBehaviour(this->hypothesis,l,f));
      } else {
	ostringstream msg;
	msg << "SingleStructureScheme::setBehaviour: "
	  "unsupported behaviour type (" << type << ")";
	throw(runtime_error(msg.str()));
      }
    }
#endif
#ifdef HAVE_CYRANO
    if(i=="cyrano"){
      this->b = shared_ptr<Behaviour>(new CyranoBehaviour(this->hypothesis,l,f));
    }
#endif
    if(this->b.get()==nullptr){
      throw(runtime_error("SingleStructureScheme::setBehaviour: "
			  "unknown interface '"+i+"'"));
    }
    const auto& ivnames = this->b->getInternalStateVariablesNames();
    this->declareVariables(this->b->getMaterialPropertiesNames(),true);
    this->declareVariables(ivnames,true);
    this->declareVariables(this->b->getExternalStateVariablesNames(),true);
    for(const auto& n : ivnames){
      unsigned short t = this->b->getInternalStateVariableType(n);
      if(t==0){
	this->ivfullnames.push_back(n);
      } else if(t==1){
	//! suffixes of stensor components
	const auto& sexts = this->b->getStensorComponentsSuffixes(this->hypothesis);
	for(decltype(sexts.size()) s=0;s!=sexts.size();++s){
	  const auto vn = n+sexts[s];
	  this->declareVariable(vn,true);
	  this->ivfullnames.push_back(vn);
	}
      } else if(t==3){
	//! suffixes f stensor components
	const auto& exts = this->b->getTensorComponentsSuffixes(this->hypothesis);
	for(decltype(exts.size()) s=0;s!=exts.size();++s){
	  const auto vn = n+exts[s];
	  this->declareVariable(vn,true);
	  this->ivfullnames.push_back(vn);
	}
      } else {
	throw(runtime_error("SingleStructureScheme::setBehaviour: "
			    "unsupported variable type for variable '"+n+"'"));
      }
    }
    // declaring behaviour variables
    this->declareVariables(this->b->getDrivingVariablesComponents(this->hypothesis),true);
    this->declareVariables(this->b->getThermodynamicForcesComponents(this->hypothesis),true);
  } // end of SingleStructureScheme::setBehaviour

  void
  SingleStructureScheme::setParameter(const std::string& n,
				      const double v)
  {
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setParameter: "
			       "no behaviour defined"));
    }
    this->b->setParameter(n,v);
  }

  void
  SingleStructureScheme::setIntegerParameter(const std::string& n,
					     const int v)
  {
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setIntegerParameter: "
			       "no behaviour defined"));
    }
    this->b->setIntegerParameter(n,v);
  }

  void
  SingleStructureScheme::setUnsignedIntegerParameter(const std::string& n,
						     const unsigned int v)
  {
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SingleStructureScheme::setUnsignedIntegerParameter: "
			       "no behaviour defined"));
    }
    this->b->setUnsignedIntegerParameter(n,v);
  }

  tfel::material::MechanicalBehaviourBase::BehaviourType
  SingleStructureScheme::getBehaviourType() const{
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SingleStructureScheme::getBehaviourType: "
			       "no behaviour defined"));
    }
    return this->b->getBehaviourType();
  }

  std::shared_ptr<Behaviour>
  SingleStructureScheme::getBehaviour(void){
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SingleStructureScheme::getBehaviour: "
			       "no behaviour defined"));
    }
    return this->b;
  }

  void
  SingleStructureScheme::completeInitialisation(void)
  {
    SchemeBase::completeInitialisation();
    if(this->b.get()==nullptr){
      throw(std::runtime_error("MTest::completeInitialisation : "
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

  StiffnessMatrixType
  SingleStructureScheme::getDefaultStiffnessMatrixType(void) const{
    return this->b->getDefaultStiffnessMatrixType();
  } // end of SingleStructureScheme::getDefaultStiffnessMatrixType
  
  SingleStructureScheme::~SingleStructureScheme() = default;
  
}
