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
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/UmatBehaviourBase.hxx"

namespace mtest
{

  UmatBehaviourDescription::UmatBehaviourDescription() = default;
  UmatBehaviourDescription::UmatBehaviourDescription(UmatBehaviourDescription&&) = default;
  UmatBehaviourDescription::UmatBehaviourDescription(const UmatBehaviourDescription&) = default;
  UmatBehaviourDescription&
  UmatBehaviourDescription::operator=(UmatBehaviourDescription&&) = default;
  UmatBehaviourDescription&
  UmatBehaviourDescription::operator=(const UmatBehaviourDescription&) = default;
  
  UmatBehaviourBase::UmatBehaviourBase(const Hypothesis h,
				       const std::string& l,
				       const std::string& b)
    : hypothesis(ModellingHypothesis::toString(h))
  {
    using namespace tfel::system;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    const auto& hh = elm.getSupportedModellingHypotheses(l,b);
    if(find(hh.begin(),hh.end(),this->hypothesis)==hh.end()){
      std::string msg("UmatBehaviourBase::UmatBehaviourBase : "
		      "unsupported modelling hypothesis '"+this->hypothesis+"'. "
		      "Supported modelling hypotheses are:");
      for(const auto& vh : hh){
	msg += "\n'"+vh+"'";
      }
      throw(std::runtime_error(msg));
    }
    this->library   = l;
    this->behaviour = b;
    this->type  = elm.getUMATBehaviourType(l,b);
    this->stype = elm.getUMATSymmetryType(l,b);
    if(this->stype>=2u){
      throw(std::runtime_error("UmatBehaviourBase::UmatBehaviourBase : "
			       "unsupported behaviour type "
			       "(neither isotropic nor orthotropic)"));
    }
    this->ivnames = elm.getUMATInternalStateVariablesNames(l,b,this->hypothesis);
    this->ivtypes = elm.getUMATInternalStateVariablesTypes(l,b,this->hypothesis);
    this->evnames = elm.getUMATExternalStateVariablesNames(l,b,this->hypothesis);
    this->evnames.insert(this->evnames.begin(),"Temperature");
  }

  UmatBehaviourBase::UmatBehaviourBase(const UmatBehaviourDescription& umb,
				       const Hypothesis h)
    : UmatBehaviourDescription(umb),
      hypothesis(ModellingHypothesis::toString(h))
  {
    if(this->stype>=2u){
      throw(std::runtime_error("UmatBehaviourBase::UmatBehaviourBase : "
			       "unsupported behaviour type "
			       "(neither isotropic nor orthotropic)"));
    }
  }
  
  UmatBehaviourBase::Hypothesis
  UmatBehaviourBase::getHypothesis() const{
    return ModellingHypothesis::fromString(this->hypothesis);
  } // end of UmatBehaviourBase::getHypothesis
  
  void
  UmatBehaviourBase::setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy p) const
  {
    auto& elm = tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    elm.setOutOfBoundsPolicy(this->library,this->behaviour,p);
  } // end of UmatBehaviourBase::setOutOfBoundsPolicy
  
  tfel::material::MechanicalBehaviourBase::BehaviourType
  UmatBehaviourBase::getBehaviourType() const
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
  UmatBehaviourBase::getDrivingVariablesSize() const
  {
    const auto h = this->getHypothesis();
    if(this->type==1){
      // small strain behaviours
      if((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	 (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==ModellingHypothesis::PLANESTRAIN)||
		(h==ModellingHypothesis::PLANESTRESS)||
		(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
		(h==ModellingHypothesis::AXISYMMETRICAL)){
	return 4u;
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	return 6u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getDrivingVariablesSize : "
				 "unsupported modelling hypothesis"));
      }
    } else if(this->type==2){
      // finite strain behaviours
      if((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	 (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==ModellingHypothesis::PLANESTRAIN)||
		(h==ModellingHypothesis::PLANESTRESS)||
		(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
		(h==ModellingHypothesis::AXISYMMETRICAL)){
	return 5u;
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	return 9u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getDrivingVariablesSize : "
				 "unsupported modelling hypothesis"));
      }
    } else if(this->type==3){
      // cohesive zone models
      if((h==ModellingHypothesis::PLANESTRAIN)||
	 (h==ModellingHypothesis::PLANESTRESS)||
	 (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
	 (h==ModellingHypothesis::AXISYMMETRICAL)){
	return 2u;
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
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
  UmatBehaviourBase::getThermodynamicForcesSize() const
  {
    const auto h = this->getHypothesis();
    if(this->type==1){
      // small strain behaviours
      if ((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	  (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==ModellingHypothesis::PLANESTRAIN)||
		(h==ModellingHypothesis::PLANESTRESS)||
		(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
		(h==ModellingHypothesis::AXISYMMETRICAL)){
	return 4u;
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	return 6u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getThermodynamicForcesSize : "
				 "unsupported modelling hypothesis"));
      }
    } else if(this->type==2){
      // finite strain behaviours
      if ((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	  (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==ModellingHypothesis::PLANESTRAIN)||
		(h==ModellingHypothesis::PLANESTRESS)||
		(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
		(h==ModellingHypothesis::AXISYMMETRICAL)){
	return 4u;
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	return 6u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getThermodynamicForcesSize : "
				 "unsupported modelling hypothesis"));
      }
    } else if(this->type==3){
      // cohesive zone models
      if((h==ModellingHypothesis::PLANESTRAIN)||
	 (h==ModellingHypothesis::PLANESTRESS)||
	 (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
	 (h==ModellingHypothesis::AXISYMMETRICAL)){
	return 2u;
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	return 3u;
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getThermodynamicForcesSize : "
				 "unsupported modelling hypothesis"));
      }      
    }
    throw(std::runtime_error("UmatBehaviourBase::getThermodynamicForcesSize : "
			     "unsupported behaviour type"));
  } // end of UmatBehaviourBase::getThermodynamicForcesSize

  std::vector<std::string>
  UmatBehaviourBase::getStensorComponentsSuffixes() const
  {
    const auto h = this->getHypothesis();
    std::vector<std::string> c;						  
    if((h==ModellingHypothesis::TRIDIMENSIONAL)||
       (h==ModellingHypothesis::PLANESTRAIN)||
       (h==ModellingHypothesis::PLANESTRESS)||
       (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      c.push_back("XX");
      c.push_back("YY");
      c.push_back("ZZ");
      c.push_back("XY");
      if(h==ModellingHypothesis::TRIDIMENSIONAL){
	c.push_back("XZ");
	c.push_back("YZ");
      }
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	       (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
      c.push_back("RR");
      c.push_back("ZZ");
      c.push_back("TT");
      if(h==ModellingHypothesis::AXISYMMETRICAL){
	c.push_back("RZ");
      }
    } else {
      throw(std::runtime_error("UmatBehaviourBase::getDrivingVariablesComponents : "
			       "unsupported modelling hypothesis"));
    }
    return c;
  } // end of UmatBehaviourBase::getStensorComponentsSuffixes

  std::vector<std::string>
  UmatBehaviourBase::getTensorComponentsSuffixes() const
  {
    const auto h = this->getHypothesis();
    std::vector<std::string> c;						  
    if((h==ModellingHypothesis::TRIDIMENSIONAL)||
       (h==ModellingHypothesis::PLANESTRAIN)||
       (h==ModellingHypothesis::PLANESTRESS)||
       (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      c.push_back("XX");
      c.push_back("YY");
      c.push_back("ZZ");
      c.push_back("XY");
      c.push_back("YX");
      if(h==ModellingHypothesis::TRIDIMENSIONAL){
	c.push_back("XZ");
	c.push_back("ZX");
	c.push_back("YZ");
	c.push_back("ZY");
      }
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	       (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
      c.push_back("RR");
      c.push_back("ZZ");
      c.push_back("TT");
      if(h==ModellingHypothesis::AXISYMMETRICAL){
	c.push_back("RZ");
	c.push_back("ZR");
      }
    } else {
      throw(std::runtime_error("UmatBehaviourBase::getDrivingVariablesComponents : "
			       "unsupported modelling hypothesis"));
    }
    return c;
  } // end of UmatBehaviourBase::getTensorComponentsSuffixes

  std::vector<std::string>
  UmatBehaviourBase::getDrivingVariablesComponents() const
  {
    using namespace std;
    const auto h = this->getHypothesis();
    vector<string> c;
    if(this->type==1){
      const auto exts = this->getStensorComponentsSuffixes();
      for(const auto& e : exts){
	c.push_back("E"+e);
      }
    } else if(this->type==2){
      const auto exts = this->getTensorComponentsSuffixes();
      for(const auto& e : exts){
	c.push_back("F"+e);
      }
    } else if(this->type==3){
      if((h==ModellingHypothesis::TRIDIMENSIONAL)||
	 (h==ModellingHypothesis::PLANESTRAIN)||
	 (h==ModellingHypothesis::PLANESTRESS)||
	 (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	c.push_back("Un");
	if(h==ModellingHypothesis::TRIDIMENSIONAL){
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
    return c;
  } // end of UmatBehaviourBase::getDrivingVariablesComponents

  std::vector<std::string>
  UmatBehaviourBase::getThermodynamicForcesComponents() const
  {
    using namespace std;
    const auto h = this->getHypothesis();
    vector<string> c;							   
    if((this->type==1)||(this->type==2)){
      const auto exts = this->getStensorComponentsSuffixes();
      for(const auto& e : exts){
	c.push_back("S"+e);
      }
    } else if(this->type==3){
      if((h==ModellingHypothesis::TRIDIMENSIONAL)||
	 (h==ModellingHypothesis::PLANESTRAIN)||
	 (h==ModellingHypothesis::PLANESTRESS)||
	 (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	c.push_back("Tn");
	if(h==ModellingHypothesis::TRIDIMENSIONAL){
	  c.push_back("Tt1");
	  c.push_back("Tt2");
	} else {
	  c.push_back("Tt");
	}
      } else {
	throw(runtime_error("UmatBehaviourBase::getThermodynamicForcesComponents : "
			    "unsupported modelling hypothesis"));
      }
    } else {
      throw(runtime_error("UmatBehaviourBase::getThermodynamicForcesComponents : "
			  "unsupported behaviour type"));
    }
    return c;
  } // end of UmatBehaviourBase::getThermodynamicForcesComponents

  unsigned short
  UmatBehaviourBase::getDrivingVariableComponentPosition(const std::string& cname) const
  {
    using namespace std;
    const auto c = this->getDrivingVariablesComponents();
    auto p = find(c.begin(),c.end(),cname);
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
  UmatBehaviourBase::getThermodynamicForceComponentPosition(const std::string& cname) const
  {
    using namespace std;
    const auto c = this->getThermodynamicForcesComponents();
    auto p = find(c.begin(),c.end(),cname);
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
  UmatBehaviourBase::getSymmetryType() const
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
  UmatBehaviourBase::getMaterialPropertiesNames() const
  {
    return this->mpnames;
  }

  std::vector<std::string>
  UmatBehaviourBase::getInternalStateVariablesNames() const
  {
    return this->ivnames;
  }

  std::vector<std::string>
  UmatBehaviourBase::getExternalStateVariablesNames() const
  {
    return this->evnames;
  }

  size_t
  UmatBehaviourBase::getInternalStateVariablesSize() const
  {
    const auto h = this->getHypothesis();    
    size_t s = 0;
    for(const auto& t : this->ivtypes){
      if(t==0){
	s+=1;
      } else if(t==1){
	if((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	   (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	  s+=3;
	} else if((h==ModellingHypothesis::AXISYMMETRICAL)||
		  (h==ModellingHypothesis::PLANESTRESS)||
		  (h==ModellingHypothesis::PLANESTRAIN)||
		  (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	  s+=4;
	} else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	  s+=6;
	} else {
	  throw(std::runtime_error("UmatBehaviourBase::getInternalStateVariablesSize: "
				   "unsupported modelling hypothesis"));
	}
      } else if(t==3){
	if((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	   (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	  s+=3;
	} else if((h==ModellingHypothesis::AXISYMMETRICAL)||
		  (h==ModellingHypothesis::PLANESTRESS)||
		  (h==ModellingHypothesis::PLANESTRAIN)||
		  (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	  s+=5;
	} else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	  s+=9;
	} else {
	  throw(std::runtime_error("UmatBehaviourBase::getInternalStateVariablesSize: "
				   "unsupported modelling hypothesis"));
	}
      } else {
	throw(std::runtime_error("UmatBehaviourBase::getInternalStateVariablesSize: "
				 "unsupported variable type"));
      }
    }
    return s;
  } // end of UmatBehaviourBase::getInternalStateVariablesSize

  std::vector<std::string>
  UmatBehaviourBase::getInternalStateVariablesDescriptions() const
  {
    using namespace std;
    const auto h = this->getHypothesis();    
    vector<string> desc;
    vector<int>::const_iterator p;
    vector<string>::const_iterator pn;
    if(this->ivnames.size()!=this->ivtypes.size()){
      throw(runtime_error("UmatBehaviourBase::getInternalStateVariablesDescriptions : "
			  "internal error (the number of internal state variables names "
			  "dont match the number of internal state variables types)"));
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
	if(!((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	     (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS))){
	  if((h==ModellingHypothesis::AXISYMMETRICAL)||(h==ModellingHypothesis::PLANESTRESS)||
	     (h==ModellingHypothesis::PLANESTRAIN)||(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	    desc.push_back("fourth  component of internal variable '"+*pn+"'");
	  } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
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
	if(!((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	     (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS))){
	  if((h==ModellingHypothesis::AXISYMMETRICAL)||(h==ModellingHypothesis::PLANESTRESS)||
	     (h==ModellingHypothesis::PLANESTRAIN)||(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	    desc.push_back("fourth  component of internal variable '"+*pn+"'");
	    desc.push_back("fifth   component of internal variable '"+*pn+"'");
	  } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
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
  UmatBehaviourBase::getInternalStateVariablePosition(const std::string& n) const
  {
    using namespace std;
    const auto h = this->getHypothesis();    
    auto p=find(this->ivnames.begin(),this->ivnames.end(),n);
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
	if((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	   (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	  s+=3;
	} else if((h==ModellingHypothesis::AXISYMMETRICAL)||(h==ModellingHypothesis::PLANESTRESS)||
		  (h==ModellingHypothesis::PLANESTRAIN)||(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	  s+=4;
	} else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	  s+=6;
	} else {
	  string msg("UmatBehaviourBase::getInternalStateVariablePosition : "
		     "invalid dimension");
	  throw(runtime_error(msg));
	}
      } else if(t==3){
	if((h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	   (h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	  s+=3;
	} else if((h==ModellingHypothesis::AXISYMMETRICAL)||(h==ModellingHypothesis::PLANESTRESS)||
		  (h==ModellingHypothesis::PLANESTRAIN)||(h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
	  s+=5;
	} else if(h==ModellingHypothesis::TRIDIMENSIONAL){
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
  UmatBehaviourBase::initializeTangentOperator(tfel::math::matrix<real>& D,
					       const StiffnessMatrixType ktype,
					       const bool b) const
  {
    if(b){
      if((ktype==StiffnessMatrixType::NOSTIFFNESS)||
	 (ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES)){
	// do nothing
      } else if(ktype==StiffnessMatrixType::ELASTIC){
	D(0,0) = real(1);
      } else if(ktype==StiffnessMatrixType::SECANTOPERATOR){
	D(0,0) = real(2);
      } else if(ktype==StiffnessMatrixType::TANGENTOPERATOR){
	D(0,0) = real(3);
      } else if(ktype==StiffnessMatrixType::CONSISTENTTANGENTOPERATOR){
	D(0,0) = real(4);
      } else {
	throw(std::runtime_error("UmatBehaviourBase::call_behaviour : "
				 "invalid or unspecified stiffness matrix type"));
      }
    } else {
      if(ktype==StiffnessMatrixType::ELASTIC){
	D(0,0) = real(-1);
      } else if(ktype==StiffnessMatrixType::SECANTOPERATOR){
	D(0,0) = real(-2);
      } else if(ktype==StiffnessMatrixType::TANGENTOPERATOR){
	D(0,0) = real(-3);
      } else {
	throw(std::runtime_error("UmatBehaviourBase::call_behaviour : "
				 "invalid or unspecified stiffness matrix type"));
      }
    }
  } // end of UmatBehaviourBase::initializeTangentOperator

  bool
  UmatBehaviourBase::doPackagingStep(CurrentState&,
				     BehaviourWorkSpace&) const
  {
    return true;
  } // end of UmatBehaviourBase::doPackagingStep
  
  UmatBehaviourBase::~UmatBehaviourBase()
  {}
  
} // end of namespace mtest
