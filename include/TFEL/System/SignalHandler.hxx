/*!
 * \file   include/TFEL/System/SignalHandler.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   09 Nov 2007
 */

#ifndef _LIB_TFEL_SIGNALHANDLER_HXX_
#define _LIB_TFEL_SIGNALHANDLER_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace system
  {
    
    struct TFELSYSTEM_VISIBILITY_EXPORT SignalHandler
    {
      virtual void execute(const int) = 0;
      virtual ~SignalHandler();
    }; // end of struct SignalHandler
    
    struct TFELSYSTEM_VISIBILITY_EXPORT FctSignalHandler
      : public SignalHandler
    {
      typedef void (*Fct)(const int);
      FctSignalHandler(const Fct);
      void execute(const int);
      ~FctSignalHandler();
    private:
      const Fct f;
    }; // end of struct FctSignalHandler

    template<typename Class>
    struct MemberSignalHandler
      : public SignalHandler
    {
      typedef void (Class:: *Fct)(const int);
      MemberSignalHandler(Class&,const Fct);
      void execute(const int);
      ~MemberSignalHandler();
    private:
      Class &c;
      const Fct f;
    }; // end of struct MemberSignalHandler
    
    TFELSYSTEM_VISIBILITY_EXPORT
    FctSignalHandler *
    sigPtrFun(const FctSignalHandler::Fct);

    template<typename Class>
    MemberSignalHandler<Class> *
    sigMemFun(Class&,const typename MemberSignalHandler<Class>::Fct);

  } // end of system
  
} // end of tfel

#include"TFEL/System/SignalHandler.ixx"

#endif /* _LIB_TFEL_SIGNALHANDLER_HXX */

