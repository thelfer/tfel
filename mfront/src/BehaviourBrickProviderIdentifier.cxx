/*!
 * \file   BehaviourBrickProviderIdentifier.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27 juin 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/ProviderIdentifier.hxx"

namespace mfront {

  namespace bbrick {

    std::string convertProviderIdentifierToString(
        const ProviderIdentifier& id) {
      auto n = std::string{};
      switch (id) {
        case ProviderIdentifier::MATERIALPROPERTY:
          n = "MaterialProperty";
          break;
        // case ProviderIdentifier::MATERIALLAW:
        // 	break;
        // 	n = "MaterialLaw";
        case ProviderIdentifier::INTEGRATIONVARIABLE:
          n = "IntegrationVariable";
          break;
        case ProviderIdentifier::AUXILIARYSTATEVARIABLE:
          n = "AuxiliaryStateVariable";
          break;
        case ProviderIdentifier::EXTERNALSTATEVARIABLE:
          n = "ExternalStateVariable";
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
      tfel::raise_if(n.empty(),
                     "convertProviderIdentifierToString: "
                     "internal error (unsupported identifier)");
      return n;
    }  // end of convertProviderIdentifierToString

  }  // end of namespace bbrick

}  // end of namespace mfront
