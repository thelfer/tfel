/*!
 * \file   mtest/src/SchemeBase.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   02 oct. 2015
 */

#include<sstream>

#include"TFEL/System/ExternalLibraryManager.hxx"

#include"MFront/MFrontLogStream.hxx"
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
#include"MTest/AccelerationAlgorithmFactory.hxx"
#include"MTest/CastemAccelerationAlgorithm.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/SchemeBase.hxx"

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
  
  SchemeBase::SchemeBase()
    : dmpv(new EvolutionManager()),
      evm(new EvolutionManager())
  {
    // declare time variable
    this->declareVariable("t",true);
  } // end of SchemeBase::SchemeBase

  void
  SchemeBase::addEvolution(const std::string& n,
			   const EvolutionPtr p,
			   const bool b1,
			   const bool b2)
  {
    if(b1){
      this->declareVariable(n,b1);
    } else {
      if(find(this->vnames.begin(),this->vnames.end(),n)==this->vnames.end()){
	throw(std::runtime_error("SchemeBase::addEvolution: "
				 "variable '"+n+"' is not defined"));
      }
    }
    if(b2){
      if(this->evm->find(n)!=this->evm->end()){
	throw(std::runtime_error("SchemeBase::addEvolution: "
				 "evolution '"+n+"' already defined"));
      }
    }
    (*(this->evm))[n] = p;
  }

  void
  SchemeBase::setEvolutionValue(const std::string& n,
				const real t,
				const real v)
  {
    const auto pev = this->evm->find(n);
    if(pev==this->evm->end()){
      throw(std::runtime_error("SchemeBase::setEvolutionValue : no evolution '"+
			       n+"' declared"));
    }
    pev->second->setValue(t,v);
  } // end of SchemeBase::setEvolutionValue

  std::shared_ptr<EvolutionManager>
  SchemeBase::getEvolutions() const
  {
    return this->evm;
  } // end of SchemeBase::getEvolutions() const

  void
  SchemeBase::setMaterialProperty(const std::string& n,
				  const EvolutionPtr p,
				  const bool check)
  {
    using namespace std;
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SchemeBase::setMaterialProperty: "
			       "no behaviour defined"));
    }
    const auto& mpnames = this->b->getMaterialPropertiesNames();
    bool is_mp = find(mpnames.begin(),mpnames.end(),n)==mpnames.end();
    if((is_mp)&&(n!="ThermalExpansion") &&(n!="ThermalExpansion1")&&
       (n!="ThermalExpansion2")&&(n!="ThermalExpansion3")){
      ostringstream msg;
      msg << "SchemeBase::setMaterialProperty: "
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
  }

  void
  SchemeBase::setExternalStateVariable(const std::string& n,
				       const EvolutionPtr p,
				       const bool check)
  {
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SchemeBase::setExternalStateVariable: "
			       "no behaviour defined"));
    }
    const auto& evsnames = this->b->getExternalStateVariablesNames();
    if(find(evsnames.begin(),evsnames.end(),n)==evsnames.end()){
      throw(std::runtime_error("SchemeBase::setExternalStateVariable: "
			       "the behaviour don't declare an "
			       "external state variable named '"+n+"'"));
    }
    this->addEvolution(n,p,false,check);
  }
  
  void
  SchemeBase::setTimes(const std::vector<real>& t)
  {
    if(!this->times.empty()){
      throw(std::runtime_error("SchemeBase::setTimes: "
			       "times already defined"));
    }
    this->times=t;
  } // end of SchemeBase::setTimes

  void
  SchemeBase::setMaximumNumberOfIterations(const unsigned int i)
  {
    if(this->options.iterMax!=-1){
      throw(std::runtime_error("SchemeBase::setMaximumNumberOfIterations: "
			       "the maximum number of iterations "
			       "has already been declared"));
    }
    if(i==0){
      throw(std::runtime_error("SchemeBase::setMaximumNumberOfIterations: "
			       "invalid number of iterations"));
    }
    this->options.iterMax = static_cast<int>(i);
  }

  void
  SchemeBase::setMaximumNumberOfSubSteps(const unsigned int i)
  {
    if(this->options.mSubSteps!=-1){
      throw(std::runtime_error("SchemeBase::setMaximumNumberOfSubSteps: "
			       "the maximum number of sub steps "
			       "has already been declared"));
    }
    if(i==0){
      throw(std::runtime_error("SchemeBase::setMaximumNumberOfSubSteps: "
			       "invalid number of sub steps"));
    }
    this->options.mSubSteps = static_cast<int>(i);
  }
  
  void
  SchemeBase::setDescription(const std::string& d)
  {
    if(!this->description.empty()){
      throw(std::runtime_error("SchemeBase::setDescription: "
			       "description already set."));
    }
    this->description = d;
  } // end of SchemeBase::setDescription

  void
  SchemeBase::setAuthor(const std::string& a)
  {
    if(!this->author.empty()){
      throw(std::runtime_error("SchemeBase::setAuthor: "
			       "author already set."));
    }
    this->author = a;
  } // end of SchemeBase::setAuthor

  void
  SchemeBase::setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy p)
  {
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SchemeBase::setOutOfBoundsPolicy: "
			       "no behaviour defined"));
    }
    this->b->setOutOfBoundsPolicy(p);
  }
  
  void
  SchemeBase::setDate(const std::string& d)
  {
    if(!this->date.empty()){
      throw(std::runtime_error("SchemeBase::setDate: "
			       "date already set."));
    }
    this->date = d;
  } // end of SchemeBase::setDate
  
  void
  SchemeBase::setBehaviour(const std::string& i,
			   const std::string& l,
			   const std::string& f)
  {
    using namespace std;
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    if(this->b.get()!=nullptr){
      throw(std::runtime_error("SchemeBase::setBehaviour: "
			       "behaviour already defined"));
    }
#ifdef HAVE_CASTEM
    if(i=="castem"){
      auto& elm = ELM::getExternalLibraryManager();
      const unsigned short type = elm.getUMATBehaviourType(l,f);
      if(type==1u){
	this->b = shared_ptr<Behaviour>(new CastemSmallStrainBehaviour(this->hypothesis,l,f));
      } else if(type==2u){
	this->b = shared_ptr<Behaviour>(new CastemFiniteStrainBehaviour(this->hypothesis,l,f));
      } else if(type==3u){
	this->b = shared_ptr<Behaviour>(new CastemCohesiveZoneModel(this->hypothesis,l,f));
      } else {
	ostringstream msg;
	msg << "SchemeBase::setBehaviour: "
	  "unsupported behaviour type (" << type << ")";
	throw(runtime_error(msg.str()));
      }
    }
#endif
#ifdef HAVE_ASTER
    if(i=="aster"){
      auto& elm = ELM::getExternalLibraryManager();
      const unsigned short type = elm.getUMATBehaviourType(l,f);
      if(type==1u){
      this->b = shared_ptr<Behaviour>(new AsterSmallStrainBehaviour(this->hypothesis,l,f));
      } else if(type==2u){
	this->b = shared_ptr<Behaviour>(new AsterFiniteStrainBehaviour(this->hypothesis,l,f));
      } else if(type==3u){
	this->b = shared_ptr<Behaviour>(new AsterCohesiveZoneModel(this->hypothesis,l,f));
      } else {
	ostringstream msg;
	msg << "SchemeBase::setBehaviour: "
	  "unsupported behaviour type (" << type << ")";
	throw(runtime_error(msg.str()));
      }
    }
#endif
#ifdef HAVE_ABAQUS
    if(i=="abaqus"){
      auto& elm = ELM::getExternalLibraryManager();
      const unsigned short type = elm.getUMATBehaviourType(l,f);
      if(type==1u){
	this->b = shared_ptr<Behaviour>(new AbaqusSmallStrainBehaviour(this->hypothesis,l,f));
      // }  else if(type==2u){
      // 	this->b = shared_ptr<Behaviour>(new AbaqusFiniteStrainBehaviour(this->hypothesis,l,f));
      } else {
	ostringstream msg;
	msg << "SchemeBase::setBehaviour: "
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
      throw(runtime_error("SchemeBase::setBehaviour: "
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
	throw(runtime_error("SchemeBase::setBehaviour: "
			    "unsupported variable type for variable '"+n+"'"));
      }
    }
    // declaring behaviour variables
    this->declareVariables(this->b->getDrivingVariablesComponents(this->hypothesis),true);
    this->declareVariables(this->b->getThermodynamicForcesComponents(this->hypothesis),true);
  } // end of SchemeBase::setBehaviour

  void
  SchemeBase::setParameter(const std::string& n,
			   const double v)
  {
    using namespace std;
    if(this->b.get()==nullptr){
      string msg("SchemeBase::setParameter: ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    this->b->setParameter(n,v);
  }

  void
  SchemeBase::setIntegerParameter(const std::string& n,
				  const int v)
  {
    using namespace std;
    if(this->b.get()==nullptr){
      string msg("SchemeBase::setIntegerParameter: ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    this->b->setIntegerParameter(n,v);
  }

  void
  SchemeBase::setUnsignedIntegerParameter(const std::string& n,
					  const unsigned int v)
  {
    using namespace std;
    if(this->b.get()==nullptr){
      string msg("SchemeBase::setUnsignedIntegerParameter: ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    this->b->setUnsignedIntegerParameter(n,v);
  }

  void
  SchemeBase::setModellingHypothesis(const std::string& h)
  {
    typedef tfel::material::ModellingHypothesis MH;
    if(this->dimension!=0){
      throw(std::runtime_error("SchemeBase::setModellingHypothesis: "
			       "the modelling hypothesis is already defined"));
    }
    if(h=="AxisymmetricalGeneralisedPlaneStrain"){
      this->dimension=1u;
      this->hypothesis = MH::AXISYMMETRICALGENERALISEDPLANESTRAIN;
    } else if(h=="AxisymmetricalGeneralisedPlaneStress"){
      this->dimension=1u;
      this->hypothesis = MH::AXISYMMETRICALGENERALISEDPLANESTRESS;
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
      throw(std::runtime_error("SchemeBase::setModellingHypothesis: "
			       "unsupported hypothesis '"+h+"'"));
    }
  } // end of SchemeBase::setModellingHypothesis

  tfel::material::ModellingHypothesis::Hypothesis
  SchemeBase::getModellingHypothesis() const
  {
    if(this->dimension==0){
      throw(std::runtime_error("SchemeBase::getModellingHypothesis: "
			       "the modelling hypothesis is not defined"));
    }
    return this->hypothesis;
  }

  unsigned short
  SchemeBase::getDimension(void) const
  {
    if(this->dimension==0){
      throw(std::runtime_error("SchemeBase::getDimension: "
			       "the modelling hypothesis is "
			       "not defined"));
    }
    return this->dimension;
  }

  tfel::material::MechanicalBehaviourBase::BehaviourType
  SchemeBase::getBehaviourType() const{
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SchemeBase::getBehaviourType: "
			       "no behaviour defined"));
    }
    return this->b->getBehaviourType();
  }

  std::shared_ptr<Behaviour>
  SchemeBase::getBehaviour(void){
    if(this->b.get()==nullptr){
      throw(std::runtime_error("SchemeBase::getBehaviour: "
			       "no behaviour defined"));
    }
    return this->b;
  }

  void
  SchemeBase::setAccelerationAlgorithm(const std::string& a)
  {
    if(this->options.aa.get()!=nullptr){
      throw(std::runtime_error("SchemeBase::setAccelerationAlgorithm: "
			       "acceleration algorithm already set"));
    }
    auto& f = AccelerationAlgorithmFactory::getAccelerationAlgorithmFactory();
    this->options.aa = f.getAlgorithm(a);
  }

  void
  SchemeBase::setAccelerationAlgorithmParameter(const std::string& p,
					   const std::string& v)
  {
    if(this->options.aa.get()==nullptr){
      throw(std::runtime_error("SchemeBase::setAccelerationAlgorithmParameter: "
			       "no acceleration algorithm set"));
    }
    this->options.aa->setParameter(p,v);
  }

  void
  SchemeBase::setPredictionPolicy(const PredictionPolicy p)
  {
    if(this->options.ppolicy!=PredictionPolicy::UNSPECIFIEDPREDICTIONPOLICY){
      throw(std::runtime_error("SchemeBase::setPredictionPolicy: "
			       "prediction policy already declared"));
    }
    this->options.ppolicy = p;
  } // end of SchemeBase::setPredictionPolicy

  void
  SchemeBase::setStiffnessMatrixType(const StiffnessMatrixType k)
  {
    if(this->options.ktype!=StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE){
      throw(std::runtime_error("SchemeBase::setStiffnessMatrixType: "
			       "stiffness matrix type already specificed"));
    }
    this->options.ktype = k;
  }

  void
  SchemeBase::setUseCastemAccelerationAlgorithm(const bool ucaa)
  {
    if(ucaa){
      if(this->options.aa.get()!=nullptr){
	throw(std::runtime_error("SchemeBase::setUseCastemAccelerationAlgorithm: "
				 "an algorithm was already set"));
      }
      this->options.aa = std::shared_ptr<AccelerationAlgorithm>(new CastemAccelerationAlgorithm);
    }
    this->options.useCastemAcceleration = ucaa;
  }
  
  void
  SchemeBase::setCastemAccelerationTrigger(const int i)
  {
    if(!this->options.useCastemAcceleration){
      throw(std::runtime_error("SchemeBase::setCastemAccelerationTrigger: "
			       "the castem acceleration algorithm has "
			       "not been set using the "
			       "@UseCast3mAccelerationAlgorithm keyword. "
			       "If the Cast3M acceleration algorithm "
			       "was specified using the "
			       "@AccelerationAlgorithm keyword, please "
			       "use the @AccelerationAlgorithmParameter "
			       "keyword to specify the acceleration trigger."));
    }
    if(this->options.aa.get()==nullptr){
      throw(std::runtime_error("SchemeBase::setCastemAccelerationTrigger: "
			       "internal error"));
    }
    std::ostringstream nb;
    nb << i;
    this->options.aa->setParameter("AccelerationTrigger",nb.str());
  }


  void
  SchemeBase::setCastemAccelerationPeriod(const int p)
  {
    if(!this->options.useCastemAcceleration){
      throw(std::runtime_error("SchemeBase::setCastemAccelerationPeriod: "
			       "the castem acceleration algorithm has not "
			       "been set using the "
			       "@UseCast3mAccelerationAlgorithm keyword. "
			       "If the Cast3M acceleration algorithm was "
			       "specified using the @AccelerationAlgorithm "
			       "keyword, please use the "
			       "@AccelerationAlgorithmParameter keyword to "
			       "specify the acceleration period."));
    }
    if(this->options.aa.get()==nullptr){
      throw(std::runtime_error("SchemeBase::setCastemAccelerationPeriod: "
			       "internal error"));
    }
    std::ostringstream nb;
    nb << p;
    this->options.aa->setParameter("AccelerationPeriod",nb.str());
  }
  
  void
  SchemeBase::setStiffnessUpdatingPolicy(const StiffnessUpdatingPolicy p)
  {
    if(this->options.ks!=StiffnessUpdatingPolicy::UNSPECIFIEDSTIFFNESSUPDATINGPOLICY){
      throw(std::runtime_error("SchemeBase::setStiffnessUpdatePolicy: "
			       "stiffness matrix type already specificed"));
    }
    this->options.ks = p;
  } // end of SchemeBase::setStiffnessUpdatingPolicy

  void
  SchemeBase::completeInitialisation(void)
  {
    if(this->initialisationFinished){
      throw(std::runtime_error("MTest::completeInitialisation : "
			       "object already initialised"));
    }
    if(this->dimension==0u){
      this->setDefaultHypothesis();
    }
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
    // numerical parameters
    if(this->options.mSubSteps==-1){
      this->options.mSubSteps=10;
    }
    if(this->options.iterMax==-1){
      this->options.iterMax=100;
    }
    if(this->options.aa.get()!=nullptr){
      this->options.aa->initialize(this->getNumberOfUnknowns());
    }
    // prediction policy
    if(this->options.ppolicy==PredictionPolicy::UNSPECIFIEDPREDICTIONPOLICY){
      this->options.ppolicy=PredictionPolicy::NOPREDICTION;
    }
    // stiffness matrix type
    if(this->options.ktype==StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE){
      this->options.ktype = this->b->getDefaultStiffnessMatrixType();
    }
    // options selected
    if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
      auto& log = mfront::getLogStream();
      if(this->options.aa.get()!=nullptr){
	log << "** " << this->options.aa->getName()
	    << " acceleration algorithm selected\n";
      }
      if(this->options.ppolicy==PredictionPolicy::LINEARPREDICTION){
	log << "** using linear prediction\n";
      } else if(this->options.ppolicy==PredictionPolicy::ELASTICPREDICTION){
	log << "** prediction using elastic stiffness\n";
      } else if(this->options.ppolicy==PredictionPolicy::ELASTICPREDICTIONFROMMATERIALPROPERTIES){
	log << "** prediction using elastic stiffness computed from material properties\n";
      } else if(this->options.ppolicy==PredictionPolicy::TANGENTOPERATORPREDICTION){
	log << "** prediction using tangent operator\n";
      } else {
	if(this->options.ppolicy!=PredictionPolicy::NOPREDICTION){
	  throw(std::runtime_error("MTest::completeInitialisation : "
				   "internal error, unsupported "
				   "prediction policy"));
	}	    
	log << "** no prediction\n";
      }
    }
    // initialisation is complete
    this->initialisationFinished = true;
  } // end of SchemeBase::completeInitialisation
  
  void
  SchemeBase::declareVariable(const std::string& v,
			      const bool check)
  {
    if(find(this->vnames.begin(),this->vnames.end(),v)!=
       this->vnames.end()){
      if(check){
	throw(std::runtime_error("SchemeBase::declareVariable: "
				 "variable '"+v+"' already declared"));
      }
    } else {
      this->vnames.push_back(v);
    }
  }

  void
  SchemeBase::declareVariables(const std::vector<std::string>& v,
			       const bool check)
  {
    for(const auto& n : v){
      this->declareVariable(n,check);
    }
  }
  
  SchemeBase::~SchemeBase()
  {} // end of SchemeBase::~SchemeBase()
  
} // end of namespace mtest
