/*!
 * \file   mfront/include/MFront/Castem21MaterialPropertyInterface.hxx
 * \brief  This file declares the Castem21MaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   15/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEM21MATERIALPROPERTYINTERFACE_HXX
#define LIB_MFRONT_CASTEM21MATERIALPROPERTYINTERFACE_HXX

#include "MFront/CastemMaterialPropertyInterface.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT Castem21MaterialPropertyInterface
      : public CastemMaterialPropertyInterface {
    static std::string getName();
    //! \brief default constructor
    Castem21MaterialPropertyInterface();
    //! \brief destructor
    ~Castem21MaterialPropertyInterface() override;
  };  // end of MfrontCastemMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_CASTEM21MATERIALPROPERTYINTERFACE_HXX */
