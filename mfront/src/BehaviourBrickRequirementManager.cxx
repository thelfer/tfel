/*!
 * \file   BehaviourBrickRequirementManager.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   29 juin 2015
 */

#include<utility>
#include<algorithm>
#include<stdexcept>
#include"MFront/VariableDescription.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/BehaviourData.hxx"
#include"MFront/BehaviourBrick/Provider.hxx"
#include"MFront/BehaviourBrick/Requirement.hxx"
#include"MFront/BehaviourBrick/RequirementManager.hxx"

namespace mfront{

  namespace bbrick{

    RequirementManager::RequirementManager() = default;
    
    RequirementManager::RequirementManager(const mfront::BehaviourData& bd){
      for(const auto& v: bd.getMaterialProperties()){
	this->addMaterialPropertyProvider(v,bd.getExternalName(v.name));
      }
      for(const auto& v: bd.getIntegrationVariables()){
	this->addIntegrationVariableProvider(v,bd.getExternalName(v.name));
      }
      for(const auto& v: bd.getAuxiliaryStateVariables()){
	this->addAuxiliaryStateVariableProvider(v,bd.getExternalName(v.name));
      }
      for(const auto& v: bd.getExternalStateVariables()){
	this->addExternalStateVariableProvider(v,bd.getExternalName(v.name));
      }
      for(const auto& v: bd.getLocalVariables()){
	this->addLocalVariableProvider(v,bd.getExternalName(v.name));
      }
      for(const auto& v: bd.getStaticVariables()){
	this->addStaticVariableProvider(v,bd.getExternalName(v.name));
      }
      for(const auto& v: bd.getParameters()){
	if(v.type=="real"){
	  this->addParameterProvider(v,bd.getExternalName(v.name));
	}
      }
    }

    static void
    throwIfRequirementIsAlreadyDefined(const std::vector<std::shared_ptr<Requirement>>& rqs,
				       const Requirement& r)
    {
      auto test = [&r](const std::shared_ptr<Requirement>& rr){
	return rr->name == r.name;
      };
      if(std::find_if(rqs.begin(),rqs.end(),test)!=rqs.end()){
	throw(std::runtime_error("RequirementManager::addRequirement : "
				 "requirement '"+r.name+"' already registred"));
      }
    }
    
    void RequirementManager::addRequirement(const Requirement& r){
      throwIfRequirementIsAlreadyDefined(this->requirements,r);
      this->requirements.push_back(std::shared_ptr<Requirement>(new Requirement(r)));
    }

    void RequirementManager::addRequirement(Requirement&& r){
      throwIfRequirementIsAlreadyDefined(this->requirements,r);
      this->requirements.push_back(std::shared_ptr<Requirement>(new Requirement(std::move(r))));
    }
    
    std::vector<std::shared_ptr<Provider>>::const_iterator
    RequirementManager::getProviderIterator(const std::string& e) const{
      return std::find_if(this->providers.begin(),
			  this->providers.end(),[&e](const std::shared_ptr<Provider> p){
			    return p->getExternalName()==e;
			  });
    }

    std::vector<std::shared_ptr<Provider>>::iterator
    RequirementManager::getProviderIterator(const std::string& e){
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
    RequirementManager::addMaterialPropertyProvider(const mfront::VariableDescription& v,
						    const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<MaterialPropertyProvider>(v,e));
    }
    
    void
    RequirementManager::addMaterialPropertyProvider(const std::string& t,
						    const std::string& n,
						    const std::string& e,
						    const unsigned short s){
      this->check(e);
      this->providers.push_back(std::make_shared<MaterialPropertyProvider>(t,n,e,s));
    } // end of RequirementManager::addMaterialPropertyProvider

    void
    RequirementManager::addAuxiliaryStateVariableProvider(const VariableDescription& v,
							  const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<AuxiliaryStateVariableProvider>(v,e));
    } // end of RequirementManager::addAuxiliaryStateVariableProvider
    
    void
    RequirementManager::addAuxiliaryStateVariableProvider(const std::string& t,
							  const std::string& n,
							  const std::string& e,
							  const unsigned short s){
      this->check(e);
      this->providers.push_back(std::make_shared<AuxiliaryStateVariableProvider>(t,n,e,s));
    } // end of RequirementManager::addAuxiliaryStateVariableProvider

    void
    RequirementManager::addExternalStateVariableProvider(const mfront::VariableDescription& v,
							 const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<ExternalStateVariableProvider>(v,e));
    } // end of RequirementManager::addExternalStateVariableProvider

    void
    RequirementManager::addExternalStateVariableProvider(const std::string& t,
							 const std::string& n,
							 const std::string& e,
							 const unsigned short s){
      this->check(e);
      this->providers.push_back(std::make_shared<ExternalStateVariableProvider>(t,n,e,s));
    } // end of RequirementManager::addExternalStateVariableProvider

    void
    RequirementManager::addIntegrationVariableProvider(const mfront::VariableDescription& v,
						       const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<IntegrationVariableProvider>(v,e));
    } // end of RequirementManager::addIntegrationVariableProvider
    
    void
    RequirementManager::addIntegrationVariableProvider(const std::string& t,
						       const std::string& n,
						       const std::string& e,
						       const unsigned short s){
      this->check(e);
      this->providers.push_back(std::make_shared<IntegrationVariableProvider>(t,n,e,s));
    } // end of RequirementManager::addIntegrationVariableProvider
    
    void
    RequirementManager::addStaticVariableProvider(const mfront::StaticVariableDescription& v,
						  const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<StaticVariableProvider>(v,e));
    } // end of RequirementManager::addStaticVariableProvider

    void
    RequirementManager::addStaticVariableProvider(const std::string& t,
						  const std::string& n,
						  const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<StaticVariableProvider>(t,n,e));
    } // end of RequirementManager::addStaticVariableProvider

    void
    RequirementManager::addParameterProvider(const mfront::VariableDescription& v,
					     const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<ParameterProvider>(v,e));
    } // end of RequirementManager::addParameterProvider
    
    void
    RequirementManager::addParameterProvider(const std::string& t,
					     const std::string& n,
					     const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<ParameterProvider>(t,n,e));
    } // end of RequirementManager::addParameterProvider

    void
    RequirementManager::addLocalVariableProvider(const mfront::VariableDescription& v,
						 const std::string& e){
      this->check(e);
      this->providers.push_back(std::make_shared<LocalVariableProvider>(v,e));
    } // end of RequirementManager::addLocalVariableProvider
    
    void
    RequirementManager::addLocalVariableProvider(const std::string& t,
						 const std::string& n,
						 const std::string& e,
						 const unsigned short s){
      this->check(e);
      this->providers.push_back(std::make_shared<LocalVariableProvider>(t,n,e,s));
    } // end of RequirementManager::addLocalVariableProvider

    RequirementManager::~RequirementManager() = default;
    
  }
}
