/*! 
 * \file  mfront/mtest/MTestUmatBehaviourBase.cxx
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

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/MTestUmatBehaviourBase.hxx"

namespace mfront
{

  MTestUmatBehaviourBase::MTestUmatBehaviourBase(const tfel::material::ModellingHypothesis::Hypothesis h,
						 const std::string& l,
						 const std::string& b)
    : hypothesis(tfel::material::ModellingHypothesis::toString(h)),
      library(l),
      behaviour(b)
  {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    const vector<string>& hh =
      elm.getSupportedModellingHypotheses(l,b);
    if(find(hh.begin(),hh.end(),this->hypothesis)==hh.end()){
      string msg("MTestUmatBehaviourBase::MTestUmatBehaviourBase : "
		 "unsupported modelling hypothesis '"+this->hypothesis+"'."
		 "Supported modelling hypotheses are:");
      for(vector<string>::const_iterator ph=hh.begin();ph!=hh.end();++ph){
	msg += "\n'"+*ph+"'";
      }
      throw(runtime_error(msg));
    }
    this->type  = elm.getUMATBehaviourType(l,b);
    this->stype = elm.getUMATSymmetryType(l,b);
    if(this->stype>=2u){
      string msg("MTestUmatBehaviourBase::MTestUmatBehaviourBase : "
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
  MTestUmatBehaviourBase::setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy p) const
  {
    typedef tfel::system::ExternalLibraryManager ELM;    
    ELM& elm = ELM::getExternalLibraryManager();
    elm.setOutOfBoundsPolicy(this->library,this->behaviour,p);
  } // end of MTestUmatBehaviourBase::setOutOfBoundsPolicy
  
  tfel::material::MechanicalBehaviourBase::BehaviourType
  MTestUmatBehaviourBase::getBehaviourType(void) const
  {
    using namespace std;
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
    string msg("MTestUmatBehaviourBase::getBehaviourType : ");
    msg += "unsupported behaviour type";
    return MechanicalBehaviourBase::GENERALBEHAVIOUR;
  } // end of MTestUmatBehaviourBase::getBehaviourType

  unsigned short
  MTestUmatBehaviourBase::getDrivingVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->type==1){
      // small strain behaviours
      if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	 (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
		(h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 4u;
      } else if(MH::TRIDIMENSIONAL){
	return 6u;
      } else {
	string msg("MTestUmatBehaviourBase::getDrivingVariablesSize : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else if(this->type==2){
      // finite strain behaviours
      if((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	 (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
		(h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 5u;
      } else if(MH::TRIDIMENSIONAL){
	return 9u;
      } else {
	string msg("MTestUmatBehaviourBase::getDrivingVariablesSize : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else if(this->type==3){
      // cohesive zone models
      if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
	 (h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 2u;
      } else if(MH::TRIDIMENSIONAL){
	return 3u;
      } else {
	string msg("MTestUmatBehaviourBase::getDrivingVariablesSize : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }      
    } else {
      string msg("MTestUmatBehaviourBase::getDrivingVariablesSize : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    return 0u;
  } // end of MTestUmatBehaviourBase::getDrivingVariablesSize

  unsigned short
  MTestUmatBehaviourBase::getThermodynamicForcesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->type==1){
      // small strain behaviours
      if ((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	  (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
		(h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 4u;
      } else if(MH::TRIDIMENSIONAL){
	return 6u;
      } else {
	string msg("MTestUmatBehaviourBase::getThermodynamicForcesSize : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else if(this->type==2){
      // finite strain behaviours
      if ((h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)||
	  (h==MH::AXISYMMETRICALGENERALISEDPLANESTRESS)){
	return 3u;
      } else if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
		(h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 4u;
      } else if(MH::TRIDIMENSIONAL){
	return 6u;
      } else {
	string msg("MTestUmatBehaviourBase::getThermodynamicForcesSize : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else if(this->type==3){
      // cohesive zone models
      if((h==MH::PLANESTRAIN)||(h==MH::PLANESTRESS)||
	 (h==MH::GENERALISEDPLANESTRAIN)||(h==MH::AXISYMMETRICAL)){
	return 2u;
      } else if(MH::TRIDIMENSIONAL){
	return 3u;
      } else {
	string msg("MTestUmatBehaviourBase::getThermodynamicForcesSize : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }      
    } else {
      string msg("MTestUmatBehaviourBase::getThermodynamicForcesSize : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    return 0u;
  } // end of MTestUmatBehaviourBase::getThermodynamicForcesSize

  void
  MTestUmatBehaviourBase::getStensorComponentsSuffixes(std::vector<std::string>& c,
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
      string msg("MTestUmatBehaviourBase::getDrivingVariablesComponents : "
		 "unsupported modelling hypothesis");
      throw(runtime_error(msg));
    }
  } // end of MTestUmatBehaviourBase::getStensorComponentsSuffixes

  void
  MTestUmatBehaviourBase::getTensorComponentsSuffixes(std::vector<std::string>& c,
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
      string msg("MTestUmatBehaviourBase::getDrivingVariablesComponents : "
		 "unsupported modelling hypothesis");
      throw(runtime_error(msg));
    }
  } // end of MTestUmatBehaviourBase::getTensorComponentsSuffixes

  void
  MTestUmatBehaviourBase::getDrivingVariablesComponents(std::vector<std::string>& c,
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
	string msg("MTestUmatBehaviourBase::getDrivingVariablesComponents : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestUmatBehaviourBase::getDrivingVariablesComponents : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
  } // end of MTestUmatBehaviourBase::getDrivingVariablesComponents

  void
  MTestUmatBehaviourBase::getThermodynamicForcesComponents(std::vector<std::string>& c,
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
	string msg("MTestUmatBehaviourBase::getThermodynamicForcesComponents : "
		   "unsupported modelling hypothesis");
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestUmatBehaviourBase::getThermodynamicForcesComponents : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
  } // end of MTestUmatBehaviourBase::getThermodynamicForcesComponents

  unsigned short
  MTestUmatBehaviourBase::getDrivingVariableComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis h,
							      const std::string& cname) const
  {
    using namespace std;
    vector<string> c;
    vector<string>::const_iterator p;
    this->getDrivingVariablesComponents(c,h);
    p = find(c.begin(),c.end(),cname);
    if(p==c.end()){
      ostringstream msg;
      msg << "MTestUmatBehaviourBase::getDrivingVariableComponentPosition : "
	"component '" << cname << "' is not valid. Valid components are:\n";
      copy(c.begin(),c.end(),ostream_iterator<string>(msg," "));
      throw(runtime_error(msg.str()));
    }
    return static_cast<unsigned short>(p-c.begin());
  } // end of MTestUmatBehaviourBase::getDrivingVariableComponentPosition

  unsigned short
  MTestUmatBehaviourBase::getThermodynamicForceComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& cname) const
  {
    using namespace std;
    vector<string> c;
    vector<string>::const_iterator p;
    this->getThermodynamicForcesComponents(c,h);
    p = find(c.begin(),c.end(),cname);
    if(p==c.end()){
      ostringstream msg;
      msg << "MTestUmatBehaviourBase::getThermodynamicForceComponentPosition : "
	"component '" << cname << "' is not valid. Valid components are:\n";
      copy(c.begin(),c.end(),ostream_iterator<string>(msg," "));
      throw(runtime_error(msg.str()));
    }
    return static_cast<unsigned short>(p-c.begin());
  } // end of MTestUmatBehaviourBase::getThermodynamicForceComponentPosition

  unsigned short
  MTestUmatBehaviourBase::getSymmetryType(void) const
  {
    using namespace std;
    if(this->stype==0){
      return 0u;
    } else if(this->stype==1){
      return 1u;
    }
    string msg("MTestUmatBehaviourBase::MTestUmatBehaviourBase : "
	       "unsupported behaviour type "
	       "(neither isotropic nor orthotropic)");
    throw(runtime_error(msg));
    return 0u;
  } // end of MTestUmatBehaviourBase::getSymmetryType

  std::vector<std::string>
  MTestUmatBehaviourBase::getMaterialPropertiesNames(void) const
  {
    return this->mpnames;
  }

  std::vector<std::string>
  MTestUmatBehaviourBase::getInternalStateVariablesNames(void) const
  {
    return this->ivnames;
  }

  std::vector<std::string>
  MTestUmatBehaviourBase::getExternalStateVariablesNames(void) const
  {
    return this->evnames;
  }

  size_t
  MTestUmatBehaviourBase::getInternalStateVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const
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
	  string msg("MTestUmatBehaviourBase::getInternalStateVariablesSize : "
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
	  string msg("MTestUmatBehaviourBase::getInternalStateVariablesSize : "
		     "unsupported modelling hypothesis");
	  throw(runtime_error(msg));
	}
      } else {
	string msg("MTestUmatBehaviourBase::getInternalStateVariablesSize : "
		   "unsupported variable type");
	throw(runtime_error(msg));
      }
    }
    return s;
  } // end of MTestUmatBehaviourBase::getInternalStateVariablesSize

  std::vector<std::string>
  MTestUmatBehaviourBase::getInternalStateVariablesDescriptions(const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    vector<string> desc;
    vector<int>::const_iterator p;
    vector<string>::const_iterator pn;
    if(this->ivnames.size()!=this->ivtypes.size()){
      string msg("MTestUmatBehaviourBase::getInternalStateVariablesDescriptions : "
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
	    string msg("MTestUmatBehaviourBase::getInternalStateVariablesDescriptions : "
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
	    string msg("MTestUmatBehaviourBase::getInternalStateVariablesDescriptions : "
		       "invalid modelling hypothesis");
	    throw(runtime_error(msg));
	  }
	}
      } else {
	string msg("MTestUmatBehaviourBase::getInternalStateVariablesDescriptions : "
		   "unsupported variable type");
	throw(runtime_error(msg));
      }
    }
    return desc;
  } // end of MTestUmatBehaviourBase::getInternalStateVariablesDescriptions

  unsigned short
  MTestUmatBehaviourBase::getInternalStateVariableType(const std::string& n) const
  {
    using namespace std;
    vector<string>::const_iterator p;
    p=find(this->ivnames.begin(),this->ivnames.end(),n);
    if(p==this->ivnames.end()){
      string msg("MTestUmatBehaviourBase::getInternalStateVariableType : ");
      msg += "no internal variable named '"+n+"' declared";
      throw(runtime_error(msg));
    }
    if(this->ivnames.size()!=this->ivtypes.size()){
      string msg("MTestUmatBehaviourBase::getInternalStateVariableType : "
		 "the number of internal variables names and "
		 "the number of internal variables types do not match");
      throw(runtime_error(msg));
    }
    int t = this->ivtypes[p-this->ivnames.begin()];
    if(t==0){
      return 0u;
    } else if(t==1){
      return 1u;
    } else if(t==3){
      return 3u;
    } else {
      string msg("MTestUmatBehaviourBase::getInternalStateVariableType : ");
      msg += "unsupported internal variable type";
      throw(runtime_error(msg));
    }
    return 0u;
  }
  
  unsigned short
  MTestUmatBehaviourBase::getInternalStateVariablePosition(const tfel::material::ModellingHypothesis::Hypothesis h,
							   const std::string& n) const
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    vector<string>::const_iterator p;
    p=find(this->ivnames.begin(),this->ivnames.end(),n);
    if(p==this->ivnames.end()){
      string msg("MTestUmatBehaviourBase::getInternalStateVariablePosition : ");
      msg += "no internal variable named '"+n+"' declared";
      throw(runtime_error(msg));
    }
    if(this->ivnames.size()!=this->ivtypes.size()){
      string msg("MTestUmatBehaviourBase::getInternalStateVariablePosition : "
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
	  string msg("MTestUmatBehaviourBase::getInternalStateVariablePosition : "
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
	  string msg("MTestUmatBehaviourBase::getInternalStateVariablePosition : "
		     "invalid dimension");
	  throw(runtime_error(msg));
	}
      } else {
	string msg("MTestUmatBehaviourBase::getInternalStateVariablePosition : "
		   "unsupported internal variable type");
	throw(runtime_error(msg));
      }
      ++i;
    }
    return s;
  }
  
  void
  MTestUmatBehaviourBase::setParameter(const std::string& n,
				       const real v) const
  {
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    elm.setParameter(this->library,this->behaviour,this->hypothesis,n,v);
  } // end of MTestUmatBehaviourBase::setParameter

  void
  MTestUmatBehaviourBase::setIntegerParameter(const std::string& n,
					      const int v) const
  {
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    elm.setParameter(this->library,this->behaviour,this->hypothesis,n,v);
  } // end of MTestUmatBehaviourBase::setIntegerParameter

  void
  MTestUmatBehaviourBase::setUnsignedIntegerParameter(const std::string& n,
						      const unsigned short v) const
  {
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    elm.setParameter(this->library,this->behaviour,this->hypothesis,n,v);
  } // end of MTestUmatBehaviourBase::setUnsignedIntegerParameter

  void
  MTestUmatBehaviourBase::setOptionalMaterialPropertiesDefaultValues(MTestEvolutionManager&,
								     const MTestEvolutionManager&) const
  {} // end of MTestUmatBehaviourBase::setOptionalMaterialPropertiesDefaultValues

  

  void
  MTestUmatBehaviourBase::initializeTangentOperator(const MTestStiffnessMatrixType::mtype ktype,
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
	string msg("MTestAsterSmallStrainBehaviour::call_behaviour : "
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
	string msg("MTestAsterSmallStrainBehaviour::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    }
  } // end of MTestUmatBehaviourBase::initializeTangentOperator

  MTestUmatBehaviourBase::~MTestUmatBehaviourBase()
  {}
  
} // end of namespace mfront

