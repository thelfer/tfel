/*!
 * \file   CyranoException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#include"TFEL/Utilities/ToString.hxx"
#include"MFront/Cyrano/CyranoException.hxx"

namespace cyrano 
{
  
  CyranoException::CyranoException(const std::string& s)
    : msg(s)
  {} // end of CyranoException::CyranoException
    
  CyranoException::CyranoException(const CyranoException& e)
    : std::exception(),
      msg(e.msg)
  {} // end of CyranoException::CyranoException

  const char* 
  CyranoException::what (void) const throw()
  { 
    return msg.c_str(); 
  } // end of CyranoException::what

  std::string
  CyranoException::getMsg(void) const throw()
  {
    return msg;
  } // end of CyranoException::getMsg
  
  CyranoException::~CyranoException() throw()
  {} // end of CyranoException::~CyranoException

  CyranoIntegrationFailed::CyranoIntegrationFailed()
    : CyranoException("behaviour integration failed")
  {} // end of CyranoIntegrationFailed::CyranoIntegrationFailed

  CyranoIntegrationFailed::CyranoIntegrationFailed(const std::string& m)
    : CyranoException("behaviour integration failed : "+m)
  {} // end of CyranoIntegrationFailed::CyranoIntegrationFailed

  CyranoIntegrationFailed::CyranoIntegrationFailed(const CyranoIntegrationFailed& e)
    : CyranoException(e)
  {} // end of CyranoIntegrationFailed::CyranoIntegrationFailed

  CyranoIntegrationFailed::~CyranoIntegrationFailed() throw()
  {} // end of CyranoIntegrationFailed::~CyranoIntegrationFailed()

  CyranoInvalidNTENSValue::CyranoInvalidNTENSValue(const unsigned short N)
    : CyranoException("Invalid tensor size declared '"+tfel::utilities::ToString(N)+"'")
  {} // end of CyranoInvalidNTENSValue::CyranoInvalidNTENSValue

  CyranoInvalidNTENSValue::CyranoInvalidNTENSValue(const CyranoInvalidNTENSValue& e)
    : CyranoException(e)
  {} // end of CyranoInvalidNTENSValue::CyranoInvalidNTENSValue

  CyranoInvalidNTENSValue::~CyranoInvalidNTENSValue() throw()
  {} // end of CyranoInvalidNTENSValue::~CyranoInvalidNTENSValue()

  CyranoInvalidDimension::CyranoInvalidDimension(const std::string& b,
					     const unsigned short N)
    : CyranoException("''"+b+"' can't be used in "+tfel::utilities::ToString(N)+"D")
  {} // end of CyranoInvalidDimension::CyranoInvalidDimension

  CyranoInvalidDimension::CyranoInvalidDimension(const CyranoInvalidDimension& e)
    : CyranoException(e)
  {} // end of CyranoInvalidDimension::CyranoInvalidDimension

  CyranoInvalidDimension::~CyranoInvalidDimension() throw()
  {} // end of CyranoInvalidDimension::~CyranoInvalidDimension()

} // end of namespace cyrano

