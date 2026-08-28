/*!
 * \file   mfront/include/MFront/Castem21ModelInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEM21MODELINTERFACE_HXX
#define LIB_MFRONT_CASTEM21MODELINTERFACE_HXX

#include "MFront/CastemModelInterface.hxx"

namespace mfront {

  struct Castem21ModelInterface final : public CastemModelInterface {
    //! \return the name of the interface
    static std::string getName();
    //! \brief constructor
    Castem21ModelInterface();
    //! \brief destructor
    ~Castem21ModelInterface() override;
  };  // end of struct CastemModelInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_CASTEMMODELINTERFACE_HXX */
