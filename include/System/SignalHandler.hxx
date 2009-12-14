/*!
 * \file   SignalHandler.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   09 Nov 2007
 */

#ifndef _LIB_TFEL_SIGNALHANDLER_HXX_
#define _LIB_TFEL_SIGNALHANDLER_HXX_ 

namespace tfel
{

  namespace system
  {
    
    struct SignalHandler
    {
      virtual void execute(const int) = 0;
      virtual ~SignalHandler();
    }; // end of struct SignalHandler
    
    struct FctSignalHandler
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
    
    FctSignalHandler *
    sigPtrFun(const FctSignalHandler::Fct);

    template<typename Class>
    MemberSignalHandler<Class> *
    sigMemFun(Class&,const typename MemberSignalHandler<Class>::Fct);

  } // end of system
  
} // end of tfel

#include"System/SignalHandler.ixx"

#endif /* _LIB_TFEL_SIGNALHANDLER_HXX */

