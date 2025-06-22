/*!
 * \file   include/TFEL/System/SignalHandler.hxx
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

#ifndef LIB_TFEL_SIGNALHANDLER_HXX
#define LIB_TFEL_SIGNALHANDLER_HXX

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace system {

    struct TFELSYSTEM_VISIBILITY_EXPORT SignalHandler {
      virtual void execute(const int) = 0;
      virtual ~SignalHandler();
    };  // end of struct SignalHandler

    struct TFELSYSTEM_VISIBILITY_EXPORT FctSignalHandler
        : public SignalHandler {
      typedef void (*Fct)(const int);
      FctSignalHandler(const Fct);
      virtual void execute(const int) override final;
      virtual ~FctSignalHandler();

     private:
      const Fct f;
    };  // end of struct FctSignalHandler

    template <typename Class>
    struct MemberSignalHandler : public SignalHandler {
      typedef void (Class::*Fct)(const int);
      MemberSignalHandler(Class &, const Fct);
      virtual void execute(const int) override final;
      ~MemberSignalHandler();

     private:
      Class &c;
      const Fct f;
    };  // end of struct MemberSignalHandler

    TFELSYSTEM_VISIBILITY_EXPORT
    FctSignalHandler *sigPtrFun(const FctSignalHandler::Fct);

    template <typename Class>
    MemberSignalHandler<Class> *sigMemFun(
        Class &, const typename MemberSignalHandler<Class>::Fct);

  }  // namespace system

}  // namespace tfel

#include "TFEL/System/SignalHandler.ixx"

#endif /* LIB_TFEL_SIGNALHANDLER_HXX */
