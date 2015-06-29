/*!
 * \file   BehaviourBrickProvider.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   26 juin 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 * <!-- Local IspellDict: english -->
 */

#include<sstream>
#include<algorithm>
#include<stdexcept>
#include"MFront/BehaviourBrick/Requirement.hxx"
#include"MFront/BehaviourBrick/Provider.hxx"

namespace mfront{

  namespace bbrick{
  
    Provider::~Provider() = default;

    bool ProviderBase::handleRequirement(const Requirement& r) const
    {
      using namespace std;
      if(this->getExternalName()!=r.name){
	return false;
      }
      if(this->getArraySize()!=r.asize){
	ostringstream msg;
	msg << "ProviderBase::handleRequirement : "
	    << "unmatched array size for requirement '"
	    << this->getExternalName() <<"' "
	    << "(requirement array size is " << r.asize << " and "
	    << " provided variable array size is "
	    << this->getArraySize() << ")";
	throw(runtime_error(msg.str()));
      }
      // check for type
      
      // check if provider is allowed
      const auto id = this->getIdentifier();
      if(find(r.aproviders.begin(),r.aproviders.end(),id)==
	 r.aproviders.end()){
	throw(runtime_error("ProviderBase::handleRequirement : "
			    "a provider of type '"+
			    convertProviderIdentifierToString(id)+
			    "' is not allowed for "
			    "requirement '"+r.name+"'"));
      }
      return true;
    } // end of ProviderBase::handleRequirement
    
    ProviderBase::~ProviderBase() = default;
    
    StandardProvider::StandardProvider(const std::string& t,
				       const std::string& n,
				       const std::string& e,
				       const unsigned short s)
      : type(t),
	name(n),
	ename(e),
	asize(s)
    {} // end of StandardProvider::StandardProvider

    std::string StandardProvider::getVariableType(void) const {
      return this->type;
    }
    
    std::string StandardProvider::getExternalName(void) const {
      return this->ename;
    }
    
    unsigned short StandardProvider::getArraySize(void) const{
      return this->asize; 
    }
    
    StandardProvider::~StandardProvider() = default;

    ProviderIdentifier
    MaterialPropertyProvider::getIdentifier(void) const{
      return ProviderIdentifier::MATERIALPROPERTY;
    } // end of MaterialPropertyProvider::getIdentifier

    MaterialPropertyProvider::~MaterialPropertyProvider() = default;

    // std::string
    // MaterialLawProvider::getVariableType(void) const{
    //   return this->type;
    // } // end of MaterialLawProvider::getVariableType

    // std::string
    // MaterialLawProvider::getExternalName(void) const{
    //   return this->name;
    // }
    
    // unsigned short
    // MaterialLawProvider::getArraySize(void) const{
    //   return 1u;
    // }
    
    // ProviderIdentifier
    // MaterialLawProvider::getIdentifier(void) const{
    //   return ProviderIdentifier::MATERIALLAW;
    // } // end of MaterialLawProvider::getIdentifier

    // MaterialLawProvider::~MaterialLawProvider() = default;
    
    ProviderIdentifier
    StateVariableProvider::getIdentifier(void) const{
      return ProviderIdentifier::STATEVARIABLE;
    }
    
    StateVariableProvider::~StateVariableProvider() = default;
    
    ProviderIdentifier
    AuxiliaryStateVariableProvider::getIdentifier(void) const{
      return ProviderIdentifier::AUXILIARYSTATEVARIABLE;
    } // end of AuxiliaryStateVariableProvider::getIdentifier

    AuxiliaryStateVariableProvider::~AuxiliaryStateVariableProvider() = default;
    
    ProviderIdentifier
    IntegrationVariableProvider::getIdentifier(void) const{
      return ProviderIdentifier::INTEGRATIONVARIABLE;
    } // end of IntegrationVariableProvider::getIdentifier

    IntegrationVariableProvider::~IntegrationVariableProvider() = default;
    
    ProviderIdentifier
    LocalVariableProvider::getIdentifier(void) const{
      return ProviderIdentifier::LOCALVARIABLE;
    } // end of LocalVariableProvider::getIdentifier

    LocalVariableProvider::~LocalVariableProvider() = default;

    StaticVariableProvider::StaticVariableProvider(const std::string& t,
					 const std::string& n,
					 const std::string& e)
      : type(t),
	name(n),
	ename(e)
    {} // end of StaticVariableProvider::StaticVariableProvider

    
    std::string StaticVariableProvider::getVariableType(void) const {
      return this->type;
    }
    
    std::string StaticVariableProvider::getExternalName(void) const {
      return this->ename;
    }
    
    unsigned short StaticVariableProvider::getArraySize(void) const{
      return 1u; 
    }
    
    ProviderIdentifier
    StaticVariableProvider::getIdentifier(void) const{
      return ProviderIdentifier::STATICVARIABLE;
    } // end of StaticVariableProvider::getIdentifier

    StaticVariableProvider::~StaticVariableProvider() = default;

    ParameterProvider::ParameterProvider(const std::string& t,
					 const std::string& n,
					 const std::string& e)
      : type(t),
	name(n),
	ename(e)
    {} // end of ParameterProvider::ParameterProvider

    std::string ParameterProvider::getVariableType(void) const {
      return this->type;
    }
    
    std::string ParameterProvider::getExternalName(void) const {
      return this->ename;
    }
    
    unsigned short ParameterProvider::getArraySize(void) const{
      return 1u; 
    }
    
    ProviderIdentifier
    ParameterProvider::getIdentifier(void) const{
      return ProviderIdentifier::PARAMETER;
    } // end of ParameterProvider::getIdentifier

    ParameterProvider::~ParameterProvider() = default;
    
  } // end of namespace bbrick
  
} // end of namespace mfront
