/*!
 * \file   include/TFEL/System/SignalHandler.ixx
 * \brief
 * \author Thomas Helfer
 * \date   09 Nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SIGNALHANDLERIXX
#define LIB_TFEL_SIGNALHANDLERIXX

namespace tfel::system {

  template <typename Class>
  MemberSignalHandler<Class>::MemberSignalHandler(
      Class& c_, const typename MemberSignalHandler<Class>::Fct f_)
      : c(c_), f(f_) {}  // end of MemberSignalHandler::MemberSignalHandler

  template <typename Class>
  void MemberSignalHandler<Class>::execute(const int s) {
    (c.*f)(s);
  }  // end of MemberSignalHandler::execute

  template <typename Class>
  MemberSignalHandler<Class>::~MemberSignalHandler() {
  }  // end of MemberSignalHandler::~MemberSignalHandler

  template <typename Class>
  MemberSignalHandler<Class>* sigMemFun(
      Class& c, const typename MemberSignalHandler<Class>::Fct f) {
    return new MemberSignalHandler<Class>(c, f);
  }  // end of sigMemFun

}  // end of namespace tfel::system

#endif /* LIB_TFEL_SIGNALHANDLERIXX */
