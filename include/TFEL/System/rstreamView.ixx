/*!
 * \file   include/TFEL/System/rstreamView.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   18 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_RSTREAMVIEWIXX
#define LIB_TFEL_RSTREAMVIEWIXX

namespace tfel {

  namespace system {

    template <bool isBlocking>
    TFEL_INLINE2 rstreamView<isBlocking>::rstreamView(const int id)
        : fd(id) {}  // end of rstreamView<isBlocking>::rstreamView

    template <bool isBlocking>
    TFEL_INLINE2 int rstreamView<isBlocking>::getFileDescriptor() const {
      return this->fd;
    }  // end of rstreamView<isBlocking>::getFileDescriptor

  }  // end of namespace system

}  // end of namespace tfel

#endif /* LIB_TFEL_RSTREAMVIEWIXX */
