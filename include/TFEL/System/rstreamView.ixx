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

#ifndef LIB_TFEL_SYSTEM_RSTREAMVIEW_IXX
#define LIB_TFEL_SYSTEM_RSTREAMVIEW_IXX

namespace tfel::system {

  template <bool isBlocking>
  rstreamView<isBlocking>::rstreamView(const int id)
      : fd(id) {}  // end of rstreamView<isBlocking>::rstreamView

  template <bool isBlocking>
  int rstreamView<isBlocking>::getFileDescriptor() const {
    return this->fd;
  }  // end of rstreamView<isBlocking>::getFileDescriptor

}  // end of namespace tfel::system

#endif /* LIB_TFEL_SYSTEM_RSTREAMVIEW_IXX */
