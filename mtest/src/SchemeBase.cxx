/*!
 * \file   mtest/src/SchemeBase.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   02 oct. 2015
 */

#include<sstream>

#include"TFEL/System/ExternalLibraryManager.hxx"

#include"MTest/Behaviour.hxx"
#ifdef HAVE_CASTEM
#include"MTest/UmatSmallStrainBehaviour.hxx"
#include"MTest/UmatFiniteStrainBehaviour.hxx"
#include"MTest/UmatCohesiveZoneModel.hxx"
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
#include"MTest/AsterSmallStrainBehaviour.hxx"
#include"MTest/AsterFiniteStrainBehaviour.hxx"
#include"MTest/AsterCohesiveZoneModel.hxx"
#endif /* HAVE_ASTER  */
#ifdef HAVE_CYRANO
#include"MTest/CyranoBehaviour.hxx"
#endif /* HAVE_CYRANO  */
#include"MTest/AccelerationAlgorithmFactory.hxx"
#include"MTest/CastemAccelerationAlgorithm.hxx"
#include"MTest/SchemeBase.hxx"

namespace mtest{

  void
  SchemeBase::setTimes(const std::vector<real>& t)
  {
    if(!this->times.empty()){
      throw(std::runtime_error("SchemeBase::setTimes : "
			       "times already defined"));
    }
    this->times=t;
  } // end of SchemeBase::setTimes

  void
  SchemeBase::setMaximumNumberOfIterations(const unsigned int i)
  {
    if(this->iterMax!=-1){
      throw(std::runtime_error("SchemeBase::setMaximumNumberOfIterations : "
			       "the maximum number of iterations "
			       "has already been declared"));
    }
    if(i==0){
      throw(std::runtime_error("SchemeBase::setMaximumNumberOfIterations : "
			       "invalid number of iterations"));
    }
    this->iterMax = static_cast<int>(i);
  }

  void
  SchemeBase::setMaximumNumberOfSubSteps(const unsigned int i)
  {
    if(this->mSubSteps!=-1){
      throw(std::runtime_error("SchemeBase::setMaximumNumberOfSubSteps : "
			       "the maximum number of sub steps "
			       "has already been declared"));
    }
    if(i==0){
      throw(std::runtime_error("SchemeBase::setMaximumNumberOfSubSteps : "
			       "invalid number of sub steps"));
    }
    this->mSubSteps = static_cast<int>(i);
  }
  
  void
  SchemeBase::setDescription(const std::string& d)
  {
    if(!this->description.empty()){
      throw(std::runtime_error("SchemeBase::setDescription : "
			       "description already set."));
    }
    this->description = d;
  } // end of SchemeBase::setDescription

  void
  SchemeBase::setAuthor(const std::string& a)
  {
    if(!this->author.empty()){
      throw(std::runtime_error("SchemeBase::setAuthor : "
			       "author already set."));
    }
    this->author = a;
  } // end of SchemeBase::setAuthor

  void
  SchemeBase::setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy p)
  {
    if(this->b.get()==0){
      throw(std::runtime_error("SchemeBase::setOutOfBoundsPolicy: "
			       "no behaviour defined"));
    }
    this->b->setOutOfBoundsPolicy(p);
  }
  
  void
  SchemeBase::setDate(const std::string& d)
  {
    if(!this->date.empty()){
      throw(std::runtime_error("SchemeBase::setDate : "
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
      throw(std::runtime_error("SchemeBase::setBehaviour : "
			       "behaviour already defined"));
    }
#ifdef HAVE_CASTEM
    if(i=="umat"){
      auto& elm = ELM::getExternalLibraryManager();
      const unsigned short type = elm.getUMATBehaviourType(l,f);
      if(type==1u){
	this->b = shared_ptr<Behaviour>(new UmatSmallStrainBehaviour(this->hypothesis,l,f));
      } else if(type==2u){
	this->b = shared_ptr<Behaviour>(new UmatFiniteStrainBehaviour(this->hypothesis,l,f));
      } else if(type==3u){
	this->b = shared_ptr<Behaviour>(new UmatCohesiveZoneModel(this->hypothesis,l,f));
      } else {
	ostringstream msg;
	msg << "SchemeBase::setBehaviour : "
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
	msg << "SchemeBase::setBehaviour : "
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
      throw(runtime_error("SchemeBase::setBehaviour : "
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
	for(auto s=0;s!=sexts.size();++s){
	  const string& vn = n+sexts[s];
	  this->declareVariable(vn,true);
	  this->ivfullnames.push_back(vn);
	}
      } else if(t==3){
	//! suffixes f stensor components
	const auto& exts = this->b->getTensorComponentsSuffixes(this->hypothesis);
	for(auto s=0;s!=exts.size();++s){
	  const string& vn = n+exts[s];
	  this->declareVariable(vn,true);
	  this->ivfullnames.push_back(vn);
	}
      } else {
	throw(runtime_error("SchemeBase::setBehaviour : "
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
      string msg("SchemeBase::setParameter : ");
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
      string msg("SchemeBase::setIntegerParameter : ");
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
      string msg("SchemeBase::setUnsignedIntegerParameter : ");
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
      throw(std::runtime_error("SchemeBase::setModellingHypothesis : "
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
      throw(std::runtime_error("SchemeBase::setModellingHypothesis : "
			       "unsupported hypothesis '"+h+"'"));
    }
  } // end of SchemeBase::setModellingHypothesis

  tfel::material::ModellingHypothesis::Hypothesis
  SchemeBase::getModellingHypothesis() const
  {
    if(this->dimension==0){
      throw(std::runtime_error("SchemeBase::getModellingHypothesis : "
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
    if(this->aa.get()!=nullptr){
      throw(std::runtime_error("SchemeBase::setAccelerationAlgorithm : "
			       "acceleration algorithm already set"));
    }
    auto& f = AccelerationAlgorithmFactory::getAccelerationAlgorithmFactory();
    this->aa = f.getAlgorithm(a);
  }

  void
  SchemeBase::setAccelerationAlgorithmParameter(const std::string& p,
					   const std::string& v)
  {
    if(this->aa.get()==nullptr){
      throw(std::runtime_error("SchemeBase::setAccelerationAlgorithmParameter : "
			       "no acceleration algorithm set"));
    }
    this->aa->setParameter(p,v);
  }

  void
  SchemeBase::setPredictionPolicy(const SchemeBase::PredictionPolicy p)
  {
    if(this->ppolicy!=UNSPECIFIEDPREDICTIONPOLICY){
      throw(std::runtime_error("SchemeBase::setPredictionPolicy : "
			       "prediction policy already declared"));
    }
    this->ppolicy = p;
  } // end of SchemeBase::setPredictionPolicy

  void
  SchemeBase::setStiffnessMatrixType(const StiffnessMatrixType::mtype k)
  {
    if(this->ktype!=StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE){
      throw(std::runtime_error("SchemeBase::setStiffnessMatrixType : "
			       "stiffness matrix type already specificed"));
    }
    this->ktype = k;
  }

  void
  SchemeBase::setUseCastemAccelerationAlgorithm(const bool ucaa)
  {
    if(ucaa){
      if(this->aa.get()!=nullptr){
	throw(std::runtime_error("SchemeBase::setUseCastemAccelerationAlgorithm : "
				 "an algorithm was already set"));
      }
      this->aa = std::shared_ptr<AccelerationAlgorithm>(new CastemAccelerationAlgorithm);
    }
    this->useCastemAcceleration = ucaa;
  }
  
  void
  SchemeBase::setCastemAccelerationTrigger(const int i)
  {
    if(!this->useCastemAcceleration){
      throw(std::runtime_error("SchemeBase::setCastemAccelerationTrigger : "
			       "the castem acceleration algorithm has "
			       "not been set using the "
			       "@UseCast3mAccelerationAlgorithm keyword. "
			       "If the Cast3M acceleration algorithm "
			       "was specified using the "
			       "@AccelerationAlgorithm keyword, please "
			       "use the @AccelerationAlgorithmParameter "
			       "keyword to specify the acceleration trigger."));
    }
    if(this->aa.get()==nullptr){
      throw(std::runtime_error("SchemeBase::setCastemAccelerationTrigger : "
			       "internal error"));
    }
    std::ostringstream nb;
    nb << i;
    this->aa->setParameter("AccelerationTrigger",nb.str());
  }


  void
  SchemeBase::setCastemAccelerationPeriod(const int p)
  {
    if(!this->useCastemAcceleration){
      throw(std::runtime_error("SchemeBase::setCastemAccelerationPeriod : "
			       "the castem acceleration algorithm has not "
			       "been set using the "
			       "@UseCast3mAccelerationAlgorithm keyword. "
			       "If the Cast3M acceleration algorithm was "
			       "specified using the @AccelerationAlgorithm "
			       "keyword, please use the "
			       "@AccelerationAlgorithmParameter keyword to "
			       "specify the acceleration period."));
    }
    if(this->aa.get()==nullptr){
      throw(std::runtime_error("SchemeBase::setCastemAccelerationPeriod : "
			       "internal error"));
    }
    std::ostringstream nb;
    nb << p;
    this->aa->setParameter("AccelerationPeriod",nb.str());
  }
  
  void
  SchemeBase::setStiffnessUpdatingPolicy(const SchemeBase::StiffnessUpdatingPolicy p)
  {
    if(this->ks!=SchemeBase::UNSPECIFIEDSTIFFNESSUPDATINGPOLICY){
      throw(std::runtime_error("SchemeBase::setStiffnessUpdatePolicy : "
			       "stiffness matrix type already specificed"));
    }
    this->ks = p;
  } // end of SchemeBase::setStiffnessUpdatingPolicy
  
  void
  SchemeBase::declareVariable(const std::string& v,
			      const bool check)
  {
    if(find(this->vnames.begin(),this->vnames.end(),v)!=
       this->vnames.end()){
      if(check){
	throw(std::runtime_error("SchemeBase::declareVariable : "
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
