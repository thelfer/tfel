/*!
 * \file   mfront/include/MFront/CyranoMaterialPropertyInterface.hxx
 * \brief  This file declares the CyranoMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANOMATERIALPROPERTYINTERFACE_HXX
#define LIB_MFRONT_CYRANOMATERIALPROPERTYINTERFACE_HXX

#include "MFront/GenericMaterialPropertyInterfaceBase.hxx"

namespace mfront {

  /*!
   * \brief material properties' interface for EDF' `Cyrano` fuel
   * performance code.
   */
  struct CyranoMaterialPropertyInterface
      : public GenericMaterialPropertyInterfaceBase {
    //! \return the interface name
    static std::string getName();
    //! \brief default constructor
    CyranoMaterialPropertyInterface();
    //! \brief destructor
    ~CyranoMaterialPropertyInterface() override;

   protected:
    TypesDescription getTypesDescription() const override;
    std::vector<std::string> getInterfaceHeaderFiles() const override;
    std::string getInterfaceName() const override;
    std::string getInterfaceNameInCamelCase() const override;
    std::string getInterfaceNameInUpperCase() const override;
    std::string getOutOfBoundsPolicyEnumerationPrefix() const override;
  };  // end of CyranoMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_CYRANOMATERIALPROPERTYINTERFACE_HXX */
