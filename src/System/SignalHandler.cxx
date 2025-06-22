/*!
 * \file   src/System/SignalHandler.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/System/SignalHandler.hxx"

namespace tfel::system {

  FctSignalHandler* sigPtrFun(const FctSignalHandler::Fct f) {
    return new FctSignalHandler(f);
  }  // end of sigPtrFun

  SignalHandler::~SignalHandler() = default;

  FctSignalHandler::FctSignalHandler(const Fct f_)
      : f(f_) {}  // end of FctSignalHandler::FctSignalHandler

  void FctSignalHandler::execute(const int s) {
    (*(this->f))(s);
  }  // end of FctSignalHandler::execute

  FctSignalHandler::~FctSignalHandler() = default;

}  // end of namespace tfel::system
