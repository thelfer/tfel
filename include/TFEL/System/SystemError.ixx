/*!
 * \file   include/TFEL/System/SystemError.ixx
 * \brief
 * \author Thomas Helfer
 * \date   06 Nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEMERRORIXX
#define LIB_TFEL_SYSTEMERRORIXX

namespace tfel::system {

  template <int N>
  PosixError<N>::PosixError(const std::string& msg_)
      : SystemError(msg_) {}  // end of PosixError::PosixError

}  // end of namespace tfel::system

#endif /* LIB_TFEL_SYSTEMERRORIXX */
