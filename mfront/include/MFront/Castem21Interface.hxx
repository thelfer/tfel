/*!
 * \file   mfront/include/MFront/Castem21Interface.hxx
 * \brief  This file declares the Castem21Interface class
 * \author Thomas Helfer
 * \date   23/02/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEM_21_FFTP_INTERFACE_HXX
#define LIB_MFRONT_CASTEM_21_FFTP_INTERFACE_HXX

#include "MFront/CastemInterface.hxx"

namespace mfront {

  /*!
   * \brief Interface for behaviours for use in the `Cast3M` finite element
   * solver for versions higher than `Cast3M` `2021`.
   */
  struct Castem21Interface : public CastemInterface {
    //! \brief return the name of the interface
    static std::string getName();
    //! \brief default constructor
    Castem21Interface();
    //
    std::string getInterfaceVersion() const override;
    //! \brief destructor
    ~Castem21Interface() override;

   protected:
    //
    std::vector<BehaviourMaterialProperty> getDefaultMaterialPropertiesList(
        const BehaviourDescription &, const Hypothesis) const override;
    //
    std::string getMaterialPropertiesOffsetForBehaviourTraits(
        const BehaviourDescription &) const override;
  };  // end of struct Castem21Interface

}  // end of namespace mfront

#endif /* LIB_MFRONT_CASTEM_21_FFTP_INTERFACE_HXX */
