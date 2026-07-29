/*!
 * \file   mfront/include/MFront/GenericMaterialPropertyInterface.hxx
 * \brief  This file declares the GenericMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   04/05/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICMATERIALPROPERTYINTERFACE_HXX
#define LIB_MFRONT_GENERICMATERIALPROPERTYINTERFACE_HXX

#include "MFront/GenericMaterialPropertyInterfaceBase.hxx"

namespace mfront {

  //! \brief generic interface for material properties
  struct GenericMaterialPropertyInterface
      : public GenericMaterialPropertyInterfaceBase {
    //! \return the interface name
    static std::string getName();
    //! \brief default constructor
    GenericMaterialPropertyInterface();
    //! \brief destructor
    ~GenericMaterialPropertyInterface() override;

   protected:
    [[nodiscard]] TypesDescription getTypesDescription() const override;
    [[nodiscard]] std::vector<std::string> getInterfaceHeaderFiles()
        const override;
    [[nodiscard]] std::string getInterfaceName() const override;
    [[nodiscard]] std::string getInterfaceNameInCamelCase() const override;
    [[nodiscard]] std::string getInterfaceNameInUpperCase() const override;
    [[nodiscard]] std::string getOutOfBoundsPolicyEnumerationPrefix()
        const override;
  };  // end of GenericMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICMATERIALPROPERTYINTERFACE_HXX */
