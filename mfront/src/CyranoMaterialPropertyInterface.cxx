/*!
 * \file   mfront/src/CyranoMaterialPropertyInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   06/05/2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/CyranoMaterialPropertyInterface.hxx"

namespace mfront {

  std::string CyranoMaterialPropertyInterface::getName() { return "cyrano"; }

  CyranoMaterialPropertyInterface::CyranoMaterialPropertyInterface() = default;

  CyranoMaterialPropertyInterface::TypesDescription
  CyranoMaterialPropertyInterface::getTypesDescription() const {
    auto types = TypesDescription{};
    types.real_type = "CyranoRealType";
    types.integer_type = "CyranoIntegerType";
    types.out_of_bounds_policy_type = "CyranoOutOfBoundsPolicy";
    types.output_status_type = "CyranoOutputStatus";
    return types;
  }  // end of getTypesDescription

  std::vector<std::string>
  CyranoMaterialPropertyInterface::getInterfaceHeaderFiles() const {
    return {"Cyrano/MaterialProperty.hxx"};
  }  // end of getInterfaceHeaderFiles

  std::string CyranoMaterialPropertyInterface::getInterfaceName() const {
    return CyranoMaterialPropertyInterface::getName();
  }  // end of getInterfaceName

  std::string CyranoMaterialPropertyInterface::getInterfaceNameInCamelCase()
      const {
    return "Cyrano";
  }  // end of getInterfaceNameInCamelCase

  std::string CyranoMaterialPropertyInterface::getInterfaceNameInUpperCase()
      const {
    return "CYRANO";
  }  // end of getInterfaceNameInUpperCase

  std::string
  CyranoMaterialPropertyInterface::getOutOfBoundsPolicyEnumerationPrefix()
      const {
    return this->getInterfaceNameInUpperCase();
  }  // end of getOutOfBoundsPolicyEnumerationPrefix

  CyranoMaterialPropertyInterface::~CyranoMaterialPropertyInterface() = default;

}  // end of namespace mfront
