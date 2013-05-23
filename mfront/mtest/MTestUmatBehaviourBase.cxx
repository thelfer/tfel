/*! 
 * \file  MTestUmatBehaviourBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#include<cmath>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/UMAT/UMAT.hxx"
#include"MFront/MTestUmatBehaviourBase.hxx"
#include"MFront/UMAT/UMATComputeStiffnessTensor.hxx"

namespace mfront
{

  MTestUmatBehaviourBase::MTestUmatBehaviourBase(const std::string& l,
					 const std::string& b)
  {
    using namespace std;
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    this->type = elm.getUMATBehaviourType(l,b);
    if(this->type>=2u){
      string msg("MTestUmatBehaviourBase::MTestUmatBehaviourBase : "
		 "unsupported behaviour type "
		 "(neither isotropic nor orthotropic)");
      throw(runtime_error(msg));
    }
    this->ivnames = elm.getUMATInternalStateVariablesNames(l,b);
    this->ivtypes = elm.getUMATInternalStateVariablesTypes(l,b);
    this->evnames = elm.getUMATExternalStateVariablesNames(l,b);
    this->evnames.insert(this->evnames.begin(),"Temperature");
  }

  unsigned short
  MTestUmatBehaviourBase::getBehaviourType(void) const
  {
    using namespace std;
    if(this->type==0){
      return 0u;
    } else if(this->type==1){
      return 1u;
    }
    string msg("MTestUmatBehaviourBase::MTestUmatBehaviourBase : "
	       "unsupported behaviour type "
	       "(neither isotropic nor orthotropic)");
    throw(runtime_error(msg));
    return 0u;
  } // end of MTestUmatBehaviourBase::getBehaviourType

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
  MTestUmatBehaviourBase::getInternalStateVariablesSize(const unsigned short d) const
  {
    using namespace std;
    vector<int>::const_iterator p;
    size_t s = 0;
    for(p=this->ivtypes.begin();p!=this->ivtypes.end();++p){
      if(*p==0){
	s+=1;
      } else if(*p==1){
	if(d==1){
	  s+=3;
	} else if(d==2){ 
	  s+=4;
	} else if(d==3){ 
	  s+=6;
	} else {
	  string msg("MTestUmatBehaviourBase::getInternalStateVariablesSize : "
		     "invalid space dimension");
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
  MTestUmatBehaviourBase::getInternalStateVariablesDescriptions(const unsigned short d) const
  {
    using namespace std;
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
	desc.push_back("first  component of internal variable '"+*pn+"'");
	desc.push_back("second component of internal variable '"+*pn+"'");
	desc.push_back("third  component of internal variable '"+*pn+"'");
	if(d!=1){
	  if(d==2){ 
	    desc.push_back("fourth  component of internal variable '"+*pn+"'");
	  } else if(d==3){ 
	    desc.push_back("fourth  component of internal variable '"+*pn+"'");
	    desc.push_back("fifth   component of internal variable '"+*pn+"'");
	    desc.push_back("sixth   component of internal variable '"+*pn+"'");
	  } else {
	    string msg("MTestUmatBehaviourBase::getInternalStateVariablesDescriptions : "
		       "invalid space dimension");
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
    } else {
      string msg("MTestUmatBehaviourBase::getInternalStateVariableType : ");
      msg += "unsupported internal variable type";
      throw(runtime_error(msg));
    }
    return 0u;
  }
  
  unsigned short
  MTestUmatBehaviourBase::getInternalStateVariablePosition(const unsigned short d,
							   const std::string& n) const
  {
    using namespace std;
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
	if(d==1u){
	  s += 3u;
	} else if(d==2u){
	  s += 4u;
	} else if(d==3u){
	  s += 6u;
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

  MTestUmatBehaviourBase::~MTestUmatBehaviourBase()
  {}
  
} // end of namespace mfront

