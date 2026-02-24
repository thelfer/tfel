/*!
 * \file   mfront/src/GenericMaterialPropertyInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/05/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/GenericMaterialPropertyInterface.hxx"

namespace mfront {

  std::string GenericMaterialPropertyInterface::getName() { return "generic"; }

  GenericMaterialPropertyInterface::GenericMaterialPropertyInterface() =
      default;

  GenericMaterialPropertyInterface::TypesDescription
  GenericMaterialPropertyInterface::getTypesDescription() const {
    auto types = TypesDescription{};
    types.real_type = "mfront_gmp_real";
    types.integer_type = "mfront_gmp_size_type";
    types.out_of_bounds_policy_type = "mfront_gmp_OutOfBoundsPolicy";
    types.output_status_type = "mfront_gmp_OutputStatus";
    return types;
  }  // end of getTypesDescription

  std::vector<std::string>
  GenericMaterialPropertyInterface::getInterfaceHeaderFiles() const {
    return {"MFront/GenericMaterialProperty/Types.h",
            "MFront/GenericMaterialProperty/MaterialProperty.h"};
  }  // end of getInterfaceHeaderFiles

  std::string GenericMaterialPropertyInterface::getInterfaceName() const {
    return GenericMaterialPropertyInterface::getName();
  }  // end of getInterfaceName

  std::string GenericMaterialPropertyInterface::getInterfaceNameInCamelCase()
      const {
    return "Generic";
  }  // end of getInterfaceNameInCamelCase

  std::string GenericMaterialPropertyInterface::getInterfaceNameInUpperCase()
      const {
    return "GENERIC";
  }  // end of getInterfaceNameInUpperCase

  std::string
  GenericMaterialPropertyInterface::getOutOfBoundsPolicyEnumerationPrefix()
      const {
    return "GENERIC_MATERIALPROPERTY";
  }  // end of getOutOfBoundsPolicyEnumerationPrefix

  GenericMaterialPropertyInterface::~GenericMaterialPropertyInterface() =
      default;

}  // end of namespace mfront
