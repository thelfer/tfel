/*!
 * \file   src/System/SignalHandler.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2007
 */

#include"TFEL/System/SignalHandler.hxx"

namespace tfel
{

  namespace system
  {

    FctSignalHandler *
    sigPtrFun(const FctSignalHandler::Fct f)
    {
      return new FctSignalHandler(f);
    } // end of sigPtrFun

    SignalHandler::~SignalHandler()
    {} // end of SignalHandler()::~SignalHandler(){

    FctSignalHandler::FctSignalHandler(const Fct f_)
      : f(f_)
    {} // end of FctSignalHandler::FctSignalHandler

    void
    FctSignalHandler::execute(const int s)
    {
      (*(this->f))(s);
    } // end of FctSignalHandler::execute

    FctSignalHandler::~FctSignalHandler()
    {} // end of FctSignalHandler::~FctSignalHandler

  } // end of system
  
} // end of tfel
