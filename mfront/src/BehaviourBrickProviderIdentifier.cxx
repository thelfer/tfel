/*!
 * \file   BehaviourBrickProviderIdentifier.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   27 juin 2015
 */

#include<stdexcept>
#include"MFront/BehaviourBrick/ProviderIdentifier.hxx"

namespace mfront{

  namespace bbrick{

    std::string
    convertProviderIdentifierToString(const ProviderIdentifier& id){
      using namespace std;
      auto n = string{};
      switch(id){
      case ProviderIdentifier::MATERIALPROPERTY:
	n = "MaterialProperty";
	break;
      // case ProviderIdentifier::MATERIALLAW:
      // 	break;
      // 	n = "MaterialLaw";
      case ProviderIdentifier::STATEVARIABLE:
	n = "StateVariable";
	break;
      case ProviderIdentifier::AUXILIARYSTATEVARIABLE:
	n = "AuxiliaryStateVariable";
	break;
      case ProviderIdentifier::INTEGRATIONVARIABLE:
	n = "IntegrationVariable";
	break;
      case ProviderIdentifier::LOCALVARIABLE:
	n = "LocalVariable";
	break;
      case ProviderIdentifier::STATICVARIABLE:
	n = "StaticVariable";
	break;
      case ProviderIdentifier::PARAMETER:
	n = "Parameter";
	break;
      default:
	break;
      }
      if(n.empty()){
	throw(runtime_error("convertProviderIdentifierToString : "
			    "internal error (unsupported identifier)"));
      }
      return n;
    } // end of convertProviderIdentifierToString
    
  } // end of namespace bbrick

} // end of namespace mfront
