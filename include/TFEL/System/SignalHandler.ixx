/*!
 * \file   SignalHandler.ixx
 * \brief    
 * \author Helfer Thomas
 * \date   09 Nov 2007
 */

#ifndef _LIB_TFEL_SIGNALHANDLER_IXX_
#define _LIB_TFEL_SIGNALHANDLER_IXX_ 

namespace tfel
{

  namespace system
  {

    template<typename Class>
    MemberSignalHandler<Class>::MemberSignalHandler(Class& c_,
						    const typename MemberSignalHandler<Class>::Fct f_)
      : c(c_),
	f(f_)
    {} // end of MemberSignalHandler::MemberSignalHandler

    template<typename Class>
    void
    MemberSignalHandler<Class>::execute(const int s)
    {
      (c.*f)(s);
    } // end of MemberSignalHandler::execute

    template<typename Class>
    MemberSignalHandler<Class>::~MemberSignalHandler()
    {} // end of MemberSignalHandler::~MemberSignalHandler

    template<typename Class>
    MemberSignalHandler<Class> *
    sigMemFun(Class& c,const typename MemberSignalHandler<Class>::Fct f)
    {
      return new MemberSignalHandler<Class>(c,f);
    } // end of sigMemFun

  } // end of system
  
} // end of tfel

#endif /* _LIB_TFEL_SIGNALHANDLER_IXX_ */

