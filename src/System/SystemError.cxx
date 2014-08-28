/*!
 * \file   src/System/SystemError.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   07 Nov 2007
 */

#include"TFEL/System/SystemError.hxx"

namespace tfel
{
  namespace system
  {

    SystemError::SystemError(const std::string& msg_)
      : tfel::exception::TFELException(msg_)
    {} // end of SystemError::SystemError

  } // end of namespace system

} // end of namespace tfel
