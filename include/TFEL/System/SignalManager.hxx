/*!
 * \file   include/TFEL/System/SignalManager.hxx
 * \brief
 * \author Helfer Thomas
 * \date   09 Nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SIGNALMANAGER_HXX_
#define LIB_TFEL_SIGNALMANAGER_HXX_

#include <map>
#include <string>

#include <signal.h>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/SignalHandler.hxx"

namespace tfel {

  namespace system {

    struct TFELSYSTEM_VISIBILITY_EXPORT SignalManager {
      static void printBackTrace(const int);
      static SignalManager &getSignalManager(void);
      unsigned short registerHandler(const int,
                                     SignalHandler *const,
                                     struct sigaction &);
      unsigned short registerHandler(const int, SignalHandler *const);
      void setSpecificHandler(const int, SignalHandler *const);
      void removeHandler(const unsigned short);
      ~SignalManager();

     private:
      TFEL_VISIBILITY_LOCAL
      static void callGdb(const int, const char *const);
      TFEL_VISIBILITY_LOCAL
      SignalManager();
      TFEL_VISIBILITY_LOCAL
      SignalManager(const SignalManager &);
      TFEL_VISIBILITY_LOCAL
      SignalManager &operator=(const SignalManager &);
      TFEL_VISIBILITY_LOCAL
      void eraseHandlers(void);
      TFEL_VISIBILITY_LOCAL
      static void treatAction(int);
      std::map<int, std::map<unsigned short, SignalHandler *>> callBacks;
      std::map<int, SignalHandler *> sHandlers;
      unsigned short handlerNbr;
    };  // end of struct SignalManager

  }  // namespace system

}  // namespace tfel

#endif /* LIB_TFEL_SIGNALMANAGER_HXX_ */
