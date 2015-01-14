/*!
 * \file   mfront/src/CyranoException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
  CyranoException::what (void) const noexcept
  { 
    return msg.c_str(); 
  } // end of CyranoException::what

  std::string
  CyranoException::getMsg(void) const noexcept
  {
    return msg;
  } // end of CyranoException::getMsg
  
  CyranoException::~CyranoException() noexcept
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

  CyranoIntegrationFailed::~CyranoIntegrationFailed() noexcept
  {} // end of CyranoIntegrationFailed::~CyranoIntegrationFailed()

  CyranoInvalidNTENSValue::CyranoInvalidNTENSValue(const unsigned short N)
    : CyranoException("Invalid tensor size declared '"+tfel::utilities::ToString(N)+"'")
  {} // end of CyranoInvalidNTENSValue::CyranoInvalidNTENSValue

  CyranoInvalidNTENSValue::CyranoInvalidNTENSValue(const CyranoInvalidNTENSValue& e)
    : CyranoException(e)
  {} // end of CyranoInvalidNTENSValue::CyranoInvalidNTENSValue

  CyranoInvalidNTENSValue::~CyranoInvalidNTENSValue() noexcept
  {} // end of CyranoInvalidNTENSValue::~CyranoInvalidNTENSValue()

  CyranoInvalidDimension::CyranoInvalidDimension(const std::string& b,
					     const unsigned short N)
    : CyranoException("''"+b+"' can't be used in "+tfel::utilities::ToString(N)+"D")
  {} // end of CyranoInvalidDimension::CyranoInvalidDimension

  CyranoInvalidDimension::CyranoInvalidDimension(const CyranoInvalidDimension& e)
    : CyranoException(e)
  {} // end of CyranoInvalidDimension::CyranoInvalidDimension

  CyranoInvalidDimension::~CyranoInvalidDimension() noexcept
  {} // end of CyranoInvalidDimension::~CyranoInvalidDimension()

} // end of namespace cyrano

