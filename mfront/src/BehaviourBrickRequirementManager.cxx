/*!
 * \file   BehaviourBrickRequirementManager.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   29 juin 2015
 */

#include<algorithm>
#include<stdexcept>
#include"MFront/BehaviourBrick/Provider.hxx"
#include"MFront/BehaviourBrick/RequirementManager.hxx"

namespace mfront{

  namespace bbrick{

    auto
    RequirementManager::getProviderIterator(const std::string& e) const
      -> decltype(this->providers.begin()){
      return std::find_if(this->providers.begin(),
			  this->providers.end(),[&e](const std::shared_ptr<Provider> p){
			    return p->getExternalName()==e;
			  });
    }

    auto
    RequirementManager::getProviderIterator(const std::string& e)
      -> decltype(this->providers.begin()){
      return std::find_if(this->providers.begin(),
			  this->providers.end(),[&e](const std::shared_ptr<Provider> p){
			    return p->getExternalName()==e;
			  });
    }

    const Provider&
    RequirementManager::getProvider(const std::string& e) const{
      const auto p = this->getProviderIterator(e);
      if(p==this->providers.end()){
	throw(std::runtime_error("RequirementManager::getProvider : "
				 "a provider for quantity '"+e+"' "
				 "declared"));
      }
      return *(*p);
    }
    
    void
    RequirementManager::check(const std::string& e) const{
      if(this->getProviderIterator(e)!=this->providers.end()){
	throw(std::runtime_error("RequirementManager::check : "
				 "a provider for quantity '"+e+"' "
				 "has already been declared"));
      }
    } // end of RequirementManager::check

    void
    RequirementManager::addMaterialPropertyProvider(const std::string& t,
						    const std::string& n,
						    const std::string& e,
						    const unsigned short s){
      this->check(e);
      this->providers.push_back(std::make_shared<MaterialPropertyProvider>(t,n,e,s));
    } // end of RequirementManager::addMaterialPropertyProvider

    void
    RequirementManager::addStateVariableProvider(const std::string& t,
						 const std::string& n,
						 const std::string& e,
						 const unsigned short s){
      this->check(e);
      this->providers.push_back(std::make_shared<StateVariableProvider>(t,n,e,s));
    } // end of RequirementManager::addStateVariableProvider

    void
    RequirementManager::addAuxiliaryStateVariableProvider(const std::string& t,
							  const std::string& n,
							  const std::string& e,
							  const unsigned short s){
      this->check(e);
      this->providers.push_back(std::make_shared<AuxiliaryStateVariableProvider>(t,n,e,s));
    } // end of RequirementManager::addAuxiliaryStateVariableProvider

    void
    RequirementManager::addExternalStateVariableProvider(const std::string& t,
							 const std::string& n,
							 const std::string& e,
							 const unsigned short s){
      this->check(e);
      this->providers.push_back(std::make_shared<ExternalStateVariableProvider>(t,n,e,s));
    } // end of RequirementManager::addExternalStateVariableProvider

    void
    RequirementManager::addIntegrationVariableProvider(const std::string& t,
						       const std::string& n,
						       const std::string& e,
						       const unsigned short s){
      this->check(e);
      this->providers.push_back(std::make_shared<IntegrationVariableProvider>(t,n,e,s));
    } // end of RequirementManager::addIntegrationVariableProvider
    
    void
    RequirementManager::addStaticVariableProvider(const std::string& t,
						  const std::string& n,
						  const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<StaticVariableProvider>(t,n,e));
    } // end of RequirementManager::addStaticVariableProvider

    void
    RequirementManager::addParameterProvider(const std::string& t,
					     const std::string& n,
					     const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<ParameterProvider>(t,n,e));
    } // end of RequirementManager::addParameterProvider
    
  }
}
