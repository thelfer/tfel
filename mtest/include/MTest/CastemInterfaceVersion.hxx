/*!
 * \file   include/MTest/CastemInterfaceVersion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   26/02/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CASTEMINTERFACEVERSION_HXX
#define LIB_MTEST_CASTEMINTERFACEVERSION_HXX

namespace mtest {

  //! \brief describe the version of the interface used
  enum struct CastemInterfaceVersion {
    //! \brief version of the interface for versions prior to 2021
    LEGACY_CASTEM_INTERFACE_VERSION,
    //! \brief version of the interface for versions starting with 2021
    CASTEM_INTERFACE_VERSION_2021
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMINTERFACEVERSION_HXX */
