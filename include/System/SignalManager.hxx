/*!
 * \file   SignalManager.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   09 Nov 2007
 */

#ifndef _LIB_TFEL_SIGNALMANAGER_HXX_
#define _LIB_TFEL_SIGNALMANAGER_HXX_ 

#include<map>
#include<string>

#include<signal.h>
#include"System/SignalHandler.hxx"

namespace tfel
{

  namespace system
  {
    
    struct SignalManager
    {
      static void
      printBackTrace(const int);
      static SignalManager&
      getSignalManager(void);
      unsigned short
      registerHandler(const int,SignalHandler *const,struct sigaction&);
      unsigned short
      registerHandler(const int,SignalHandler *const);
      void
      setSpecificHandler(const int,SignalHandler *const);
      void
      removeHandler(const unsigned short);
      ~SignalManager();
    private:
      static void
      callGdb(const int,const char* const);
      SignalManager();
      SignalManager(const SignalManager &);
      SignalManager&
      operator = (const SignalManager &);
      void eraseHandlers(void);
      static void treatAction(int);
      std::map<int,std::map<unsigned short,SignalHandler *> > callBacks;
      std::map<int,SignalHandler *> sHandlers;
      unsigned short handlerNbr;
    }; // end of struct SignalManager

  } // end of system
  
} // end of tfel

#endif /* _LIB_TFEL_SIGNALMANAGER_HXX */

