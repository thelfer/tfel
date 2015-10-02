/*! 
 * \file  mfront/mtest/UmatBehaviourBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<sstream>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MTest/UmatBehaviourBase.hxx"

namespace mfront
{

  UmatBehaviourBase::UmatBehaviourBase(const tfel::material::ModellingHypothesis::Hypothesis h,
						 const std::string& l,
						 const std::string& b)
    : hypothesis(tfel::material::ModellingHypothesis::toString(h)),
      library(l),
      behaviour(b)
  {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    const auto& hh = elm.getSupportedModellingHypotheses(l,b);
    if(find(hh.begin(),hh.end(),this->hypothesis)==hh.end()){
      string msg("UmatBehaviourBase::UmatBehaviourBase : "
		 "unsupported modelling hypothesis '"+this->hypothesis+"'."
		 "Supported modelling hypotheses are:");
      for(const auto& vh : hh){
	msg += "\n'"+vh+"'";
      }
      throw(runtime_error(msg));
    }
    this->type  = elm.getUMATBehaviourType(l,b);
    this->stype = elm.getUMATSymmetryType(l,b);
    if(this->stype>=2u){
      string msg("UmatBehaviourBase::UmatBehaviourBase : "
		 "unsupported behaviour type "
		 "(neither isotropic nor orthotropic)");
      throw(runtime_error(msg));
    }
    this->ivnames = elm.getUMATInternalStateVariablesNames(l,b,this->hypothesis);
    this->ivtypes = elm.getUMATInternalStateVariablesTypes(l,b,this->hypothesis);
    this->evnames = elm.getUMATExternalStateVariablesNames(l,b,this->hypothesis);
    this->evnames.insert(this->evnames.begin(),"Temperature");
  }

  void
  UmatBehaviourBase::setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy p) const
  {
    typedef tfel::system::ExternalLibraryManager ELM;    
    ELM& elm = ELM::getExternalLibraryManager();
    elm.setOutOfBoundsPolicy(this->library,this->behaviour,p);
  } // end of UmatBehaviourBase::setOutOfBoundsPolicy
  
  tfel::material::MechanicalBehaviourBase::BehaviourType
  UmatBehaviourBase::getBehaviourType(void) const
  {
    using namespace tfel::material;
    switch(this->type){
    case 0:
      return MechanicalBehaviourBase::GENERALBEHAVIOUR;
    case 1:
      return MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR;
    case 2:
      return MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR;
    case 3:
      return MechanicalBehaviourBase::COHESIVEZONEMODEL;
    }
    throw(std::runtime_error("UmatBehaviourBase::getBehaviourType : "
			     "unsupported behaviour type"));
  } // end of UmatBehaviourBase::getBehaviourType

  unsigned short
  UmatBehaviourBase::getDrivingVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    typedef tfel::material::ModellingHypothesis MH;
    if(this->type==1){
      // small strain behaviours
      if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	 (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
		(h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 4u;
      } else if(h==MH::TRIDIMENSIONAL){
	return 6u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getDrivingVariablesSize : "
				 "unsupported modelling hypothesis"));
      }
    } else if(this->type==2){
      // finite strain behaviours
      if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	 (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
		(h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 5u;
      } else if(h==MH::TRIDIMENSIONAL){
	return 9u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getDrivingVariablesSize : "
				 "unsupported modelling hypothesis"));
      }
    } else if(this->type==3){
      // cohesive zone models
      if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
	 (h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 2u;
      } else if(h==MH::TRIDIMENSIONAL){
	return 3u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getDrivingVariablesSize : "
				 "unsupported modelling hypothesis"));
      }      
    } 
    throw(std::runtime_error("UmatBehaviourBase::getDrivingVariablesSize : "
			     "unsupported behaviour type"));
  } // end of UmatBehaviourBase::getDrivingVariablesSize

  unsigned short
  UmatBehaviourBase::getThermodynamicForcesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    typedef tfel::material::ModellingHypothesis MH;
    if(this->type==1){
      // small strain behaviours
      if ((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	  (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
		(h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 4u;
      } else if(h==MH::TRIDIMENSIONAL){
	return 6u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getThermodynamicForcesSize : "
				 "unsupported modelling hypothesis"));
      }
    } else if(this->type==2){
      // finite strain behaviours
      if ((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	  (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
		(h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 4u;
      } else if(h==MH::TRIDIMENSIONAL){
	return 6u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getThermodynamicForcesSize : "
				 "unsupported modelling hypothesis"));
      }
    } else if(this->type==3){
      // cohesive zone models
      if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
	 (h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 2u;
      } else if(h==MH::TRIDIMENSIONAL){
	return 3u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getThermodynamicForcesSize : "
				 "unsupported modelling hypothesis"));
      }      
    }
    throw(std::runtime_error("UmatBehaviourBase::getThermodynamicForcesSize : "
			     "unsupported behaviour type"));
  } // end of UmatBehaviourBase::getThermodynamicForcesSize

  void
  UmatBehaviourBase::getStensorComponentsSuffixes(std::vector<std::string>& c,
						       const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    c.clear();
    if((h==MH::TRIDIMENSIONAL)||
       (h==MH::PLANESTRAIN)||
       (h==MH::PLANESTRESS)||
       (h==MH::GENERALISEDPLANESTRAIN)){
      c.push_back("XX");
      c.push_back("YY");
      c.push_back("ZZ");
      c.push_back("XY");
      if(h==MH::TRIDIMENSIONAL){
	c.push_back("XZ");
	c.push_back("YZ");
      }
    } else if ((h==MH::AXISYMMETRICAL)||
	       (h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	       (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
      c.push_back("RR");
      c.push_back("ZZ");
      c.push_back("TT");
      if(h==MH::AXISYMMETRICAL){
	c.push_back("RZ");
      }
    } else {
      string msg("UmatBehaviourBase::getDrivingVariablesComponents : "
		 "unsupported modelling hypothesis");
      throw(runtime_error(msg));
    }
  } // end of UmatBehaviourBase::getStensorComponentsSuffixes

  void
  UmatBehaviourBase::getTensorComponentsSuffixes(std::vector<std::string>& c,
						      const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    c.clear();
    if((h==MH::TRIDIMENSIONAL)||
       (h==MH::PLANESTRAIN)||
       (h==MH::PLANESTRESS)||
       (h==MH::GENERALISEDPLANESTRAIN)){
      c.push_back("XX");
      c.push_back("YY");
      c.push_back("ZZ");
      c.push_back("XY");
      c.push_back("YX");
      if(h==MH::TRIDIMENSIONAL){
	c.push_back("XZ");
	c.push_back("ZX");
	c.push_back("YZ");
	c.push_back("ZY");
      }
    } else if ((h==MH::AXISYMMETRICAL)||
	       (h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	       (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
      c.push_back("RR");
      c.push_back("ZZ");
      c.push_back("TT");
      if(h==MH::AXISYMMETRICAL){
	c.push_back("RZ");
	c.push_back("ZR");
      }
    } else {
      string msg("UmatBehaviourBase::getDrivingVariablesComponents : "
		 "unsupported modelling hypothesis");
      throw(runtime_error(msg));
    }
  } // end of UmatBehaviourBase::getTensorComponentsSuffixes

  void
  UmatBehaviourBase::getDrivingVariablesComponents(std::vector<std::string>& c,
							const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    c.clear();
    if(this->type==1){
      vector<string> exts;
      vector<string>::const_iterator pe;
      this->getStensorComponentsSuffixes(exts,h);
      for(pe=exts.begin();pe!=exts.end();++pe){
	c.push_back("E"+*pe);
      }
    } else if(this->type==2){
      vector<string> exts;
      vector<string>::const_iterator pe;
      this->getTensorComponentsSuffixes(exts,h);
      for(pe=exts.begin();pe!=exts.end();++pe){
	c.push_back("F"+*pe);
      }
    } else if(this->type==3){
      if((h==MH::TRIDIMENSIONAL)||
	 (h==MH::PLANESTRAIN)||
	 (h==MH::PLANESTRESS)||
	 (h==MH::GENERALISEDPLANESTRAIN)){
	c.push_back("Un");
	if(h==MH::TRIDIMENSIONAL){
	  c.push_back("Ut1");
	  c.push_back("Ut2");
	} else {
	  c.push_back("Ut");
	}
      } else {
	string msg("UmatBehaviourBase::getDrivingVariablesComponents : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else {
      string msg("UmatBehaviourBase::getDrivingVariablesComponents : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
  } // end of UmatBehaviourBase::getDrivingVariablesComponents

  void
  UmatBehaviourBase::getThermodynamicForcesComponents(std::vector<std::string>& c,
							   const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    c.clear();
    if((this->type==1)||
       (this->type==2)){
      vector<string> exts;
      vector<string>::const_iterator pe;
      this->getStensorComponentsSuffixes(exts,h);
      for(pe=exts.begin();pe!=exts.end();++pe){
	c.push_back("S"+*pe);
      }
    } else if(this->type==3){
      if((h==MH::TRIDIMENSIONAL)||
	 (h==MH::PLANESTRAIN)||
	 (h==MH::PLANESTRESS)||
	 (h==MH::GENERALISEDPLANESTRAIN)){
	c.push_back("Tn");
	if(h==MH::TRIDIMENSIONAL){
	  c.push_back("Tt1");
	  c.push_back("Tt2");
	} else {
	  c.push_back("Tt");
	}
      } else {
	string msg("UmatBehaviourBase::getThermodynamicForcesComponents : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else {
      string msg("UmatBehaviourBase::getThermodynamicForcesComponents : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
  } // end of UmatBehaviourBase::getThermodynamicForcesComponents

  unsigned short
  UmatBehaviourBase::getDrivingVariableComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis h,
							      const std::string& cname) const
  {
    using namespace std;
    vector<string> c;
    vector<string>::const_iterator p;
    this->getDrivingVariablesComponents(c,h);
    p = find(c.begin(),c.end(),cname);
    if(p==c.end()){
      ostringstream msg;
      msg << "UmatBehaviourBase::getDrivingVariableComponentPosition : "
	"component '" << cname << "' is not valid. Valid components are:\n";
      copy(c.begin(),c.end(),ostream_iterator<string>(msg," "));
      throw(runtime_error(msg.str()));
    }
    return static_cast<unsigned short>(p-c.begin());
  } // end of UmatBehaviourBase::getDrivingVariableComponentPosition

  unsigned short
  UmatBehaviourBase::getThermodynamicForceComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& cname) const
  {
    using namespace std;
    vector<string> c;
    vector<string>::const_iterator p;
    this->getThermodynamicForcesComponents(c,h);
    p = find(c.begin(),c.end(),cname);
    if(p==c.end()){
      ostringstream msg;
      msg << "UmatBehaviourBase::getThermodynamicForceComponentPosition : "
	"component '" << cname << "' is not valid. Valid components are:\n";
      copy(c.begin(),c.end(),ostream_iterator<string>(msg," "));
      throw(runtime_error(msg.str()));
    }
    return static_cast<unsigned short>(p-c.begin());
  } // end of UmatBehaviourBase::getThermodynamicForceComponentPosition

  unsigned short
  UmatBehaviourBase::getSymmetryType(void) const
  {
    if(this->stype==0){
      return 0u;
    } else if(this->stype==1){
      return 1u;
    }
    throw(std::runtime_error("UmatBehaviourBase::UmatBehaviourBase : "
			     "unsupported behaviour type "
			     "(neither isotropic nor orthotropic)"));
  } // end of UmatBehaviourBase::getSymmetryType

  std::vector<std::string>
  UmatBehaviourBase::getMaterialPropertiesNames(void) const
  {
    return this->mpnames;
  }

  std::vector<std::string>
  UmatBehaviourBase::getInternalStateVariablesNames(void) const
  {
    return this->ivnames;
  }

  std::vector<std::string>
  UmatBehaviourBase::getExternalStateVariablesNames(void) const
  {
    return this->evnames;
  }

  size_t
  UmatBehaviourBase::getInternalStateVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    vector<int>::const_iterator p;
    size_t s = 0;
    for(p=this->ivtypes.begin();p!=this->ivtypes.end();++p){
      if(*p==0){
	s+=1;
      } else if(*p==1){
	if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	   (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	  s+=3;
	} else if((h==MH::AXISYMMETRICAL)||(h==MH::PLANESTRESS)||
		  (h==MH::PLANESTRAIN)||(h==MH::GENERALISEDPLANESTRAIN)){
	  s+=4;
	} else if(h==MH::TRIDIMENSIONAL){
	  s+=6;
	} else {
	  string msg("UmatBehaviourBase::getInternalStateVariablesSize : "
		     "unsupported modelling hypothesis");
	  throw(runtime_error(msg));
	}
      } else if(*p==3){
	if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	   (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	  s+=3;
	} else if((h==MH::AXISYMMETRICAL)||(h==MH::PLANESTRESS)||
		  (h==MH::PLANESTRAIN)||(h==MH::GENERALISEDPLANESTRAIN)){
	  s+=5;
	} else if(h==MH::TRIDIMENSIONAL){
	  s+=9;
	} else {
	  string msg("UmatBehaviourBase::getInternalStateVariablesSize : "
		     "unsupported modelling hypothesis");
	  throw(runtime_error(msg));
	}
      } else {
	string msg("UmatBehaviourBase::getInternalStateVariablesSize : "
		   "unsupported variable type");
	throw(runtime_error(msg));
      }
    }
    return s;
  } // end of UmatBehaviourBase::getInternalStateVariablesSize

  std::vector<std::string>
  UmatBehaviourBase::getInternalStateVariablesDescriptions(const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    vector<string> desc;
    vector<int>::const_iterator p;
    vector<string>::const_iterator pn;
    if(this->ivnames.size()!=this->ivtypes.size()){
      string msg("UmatBehaviourBase::getInternalStateVariablesDescriptions : "
		 "internal error (the number of internal state variables names "
		 "dont match the number of internal state variables types)");
      throw(runtime_error(msg));
    }
    for(p=this->ivtypes.begin(),pn=this->ivnames.begin();
	p!=this->ivtypes.end();++p,++pn){
      if(*p==0){
	desc.push_back(*pn);
      } else if(*p==1){
	// symmetric tensor
	desc.push_back("first  component of internal variable '"+*pn+"'");
	desc.push_back("second component of internal variable '"+*pn+"'");
	desc.push_back("third  component of internal variable '"+*pn+"'");
	if(!((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	     (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS))){
	  if((h==MH::AXISYMMETRICAL)||(h==MH::PLANESTRESS)||
	     (h==MH::PLANESTRAIN)||(h==MH::GENERALISEDPLANESTRAIN)){
	    desc.push_back("fourth  component of internal variable '"+*pn+"'");
	  } else if(h==MH::TRIDIMENSIONAL){
	    desc.push_back("fourth  component of internal variable '"+*pn+"'");
	    desc.push_back("fifth   component of internal variable '"+*pn+"'");
	    desc.push_back("sixth   component of internal variable '"+*pn+"'");
	  } else {
	    string msg("UmatBehaviourBase::getInternalStateVariablesDescriptions : "
		       "invalid modelling hypothesis");
	    throw(runtime_error(msg));
	  }
	}
      } else if(*p==3){
	// unsymmetric tensor
	desc.push_back("first  component of internal variable '"+*pn+"'");
	desc.push_back("second component of internal variable '"+*pn+"'");
	desc.push_back("third  component of internal variable '"+*pn+"'");
	if(!((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	     (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS))){
	  if((h==MH::AXISYMMETRICAL)||(h==MH::PLANESTRESS)||
	     (h==MH::PLANESTRAIN)||(h==MH::GENERALISEDPLANESTRAIN)){
	    desc.push_back("fourth  component of internal variable '"+*pn+"'");
	    desc.push_back("fifth   component of internal variable '"+*pn+"'");
	  } else if(h==MH::TRIDIMENSIONAL){
	    desc.push_back("fourth  component of internal variable '"+*pn+"'");
	    desc.push_back("fifth   component of internal variable '"+*pn+"'");
	    desc.push_back("sixth   component of internal variable '"+*pn+"'");
	    desc.push_back("seventh component of internal variable '"+*pn+"'");
	    desc.push_back("eighth  component of internal variable '"+*pn+"'");
	    desc.push_back("ninth   component of internal variable '"+*pn+"'");
	  } else {
	    string msg("UmatBehaviourBase::getInternalStateVariablesDescriptions : "
		       "invalid modelling hypothesis");
	    throw(runtime_error(msg));
	  }
	}
      } else {
	string msg("UmatBehaviourBase::getInternalStateVariablesDescriptions : "
		   "unsupported variable type");
	throw(runtime_error(msg));
      }
    }
    return desc;
  } // end of UmatBehaviourBase::getInternalStateVariablesDescriptions

  unsigned short
  UmatBehaviourBase::getInternalStateVariableType(const std::string& n) const
  {
    auto p=find(this->ivnames.begin(),this->ivnames.end(),n);
    if(p==this->ivnames.end()){
      throw(std::runtime_error("UmatBehaviourBase::getInternalStateVariableType: "
			       "no internal variable named '"+n+"' declared"));
    }
    if(this->ivnames.size()!=this->ivtypes.size()){
      throw(std::runtime_error("UmatBehaviourBase::getInternalStateVariableType: "
			       "the number of internal variables names and "
			       "the number of internal variables types do not match"));
    }
    const auto t = this->ivtypes[p-this->ivnames.begin()];
    if(t==0){
      return 0u;
    } else if(t==1){
      return 1u;
    } else if(t==3){
      return 3u;
    }
    throw(std::runtime_error("UmatBehaviourBase::getInternalStateVariableType : "
			     "unsupported internal variable type"));
  }
  
  unsigned short
  UmatBehaviourBase::getInternalStateVariablePosition(const tfel::material::ModellingHypothesis::Hypothesis h,
							   const std::string& n) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    vector<string>::const_iterator p;
    p=find(this->ivnames.begin(),this->ivnames.end(),n);
    if(p==this->ivnames.end()){
      string msg("UmatBehaviourBase::getInternalStateVariablePosition : ");
      msg += "no internal variable named '"+n+"' declared";
      throw(runtime_error(msg));
    }
    if(this->ivnames.size()!=this->ivtypes.size()){
      string msg("UmatBehaviourBase::getInternalStateVariablePosition : "
		 "the number of internal variables names and "
		 "the number of internal variables types do not match");
      throw(runtime_error(msg));
    }
    vector<string>::size_type i  = 0;
    vector<string>::size_type ie = p-this->ivnames.begin();
    unsigned short s = 0;
    while(i!=ie){
      int t = this->ivtypes[i];
      if(t==0){
	s += 1;
      } else if(t==1){
	if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	   (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	  s+=3;
	} else if((h==MH::AXISYMMETRICAL)||(h==MH::PLANESTRESS)||
		  (h==MH::PLANESTRAIN)||(h==MH::GENERALISEDPLANESTRAIN)){
	  s+=4;
	} else if(h==MH::TRIDIMENSIONAL){
	  s+=6;
	} else {
	  string msg("UmatBehaviourBase::getInternalStateVariablePosition : "
		     "invalid dimension");
	  throw(runtime_error(msg));
	}
      } else if(t==3){
	if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	   (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	  s+=3;
	} else if((h==MH::AXISYMMETRICAL)||(h==MH::PLANESTRESS)||
		  (h==MH::PLANESTRAIN)||(h==MH::GENERALISEDPLANESTRAIN)){
	  s+=5;
	} else if(h==MH::TRIDIMENSIONAL){
	  s+=9;
	} else {
	  string msg("UmatBehaviourBase::getInternalStateVariablePosition : "
		     "invalid dimension");
	  throw(runtime_error(msg));
	}
      } else {
	string msg("UmatBehaviourBase::getInternalStateVariablePosition : "
		   "unsupported internal variable type");
	throw(runtime_error(msg));
      }
      ++i;
    }
    return s;
  }

  void
  UmatBehaviourBase::setParameter(const std::string& n,
				       const real v) const
  {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    elm.setParameter(this->library,this->behaviour,this->hypothesis,n,v);
  } // end of UmatBehaviourBase::setParameter

  void
  UmatBehaviourBase::setIntegerParameter(const std::string& n,
					 const int v) const
  {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    elm.setParameter(this->library,this->behaviour,this->hypothesis,n,v);
  } // end of UmatBehaviourBase::setIntegerParameter

  void
  UmatBehaviourBase::setUnsignedIntegerParameter(const std::string& n,
						 const unsigned short v) const
  {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    elm.setParameter(this->library,this->behaviour,this->hypothesis,n,v);
  } // end of UmatBehaviourBase::setUnsignedIntegerParameter

  void
  UmatBehaviourBase::setOptionalMaterialPropertiesDefaultValues(EvolutionManager&,
								     const EvolutionManager&) const
  {} // end of UmatBehaviourBase::setOptionalMaterialPropertiesDefaultValues

  

  void
  UmatBehaviourBase::initializeTangentOperator(const MTestStiffnessMatrixType::mtype ktype,
						    const bool b) const
  {
    using namespace std;
    if(b){
      if((ktype==MTestStiffnessMatrixType::NOSTIFFNESS)||
	 (ktype==MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES)){
	// do nothing
      } else if(ktype==MTestStiffnessMatrixType::ELASTIC){
	this->D(0,0) = real(1);
      } else if(ktype==MTestStiffnessMatrixType::SECANTOPERATOR){
	this->D(0,0) = real(2);
      } else if(ktype==MTestStiffnessMatrixType::TANGENTOPERATOR){
	this->D(0,0) = real(3);
      } else if(ktype==MTestStiffnessMatrixType::CONSISTENTTANGENTOPERATOR){
	this->D(0,0) = real(4);
      } else {
	string msg("AsterSmallStrainBehaviour::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    } else {
      if(ktype==MTestStiffnessMatrixType::ELASTIC){
	this->D(0,0) = real(-1);
      } else if(ktype==MTestStiffnessMatrixType::SECANTOPERATOR){
	this->D(0,0) = real(-2);
      } else if(ktype==MTestStiffnessMatrixType::TANGENTOPERATOR){
	this->D(0,0) = real(-3);
      } else {
	string msg("AsterSmallStrainBehaviour::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    }
  } // end of UmatBehaviourBase::initializeTangentOperator

  UmatBehaviourBase::~UmatBehaviourBase()
  {}
  
} // end of namespace mfront

