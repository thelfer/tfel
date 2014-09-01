/*!
 * \file   mfront/src/UMATException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Utilities/ToString.hxx"
#include"MFront/UMAT/UMATException.hxx"

namespace umat 
{
  
  UMATException::UMATException(const std::string& s)
    : msg(s)
  {} // end of UMATException::UMATException
    
  UMATException::UMATException(const UMATException& e)
    : std::exception(),
      msg(e.msg)
  {} // end of UMATException::UMATException

  const char* 
  UMATException::what (void) const throw()
  { 
    return msg.c_str(); 
  } // end of UMATException::what

  std::string
  UMATException::getMsg(void) const throw()
  {
    return msg;
  } // end of UMATException::getMsg
  
  UMATException::~UMATException() throw()
  {} // end of UMATException::~UMATException

  UMATIntegrationFailed::UMATIntegrationFailed()
    : UMATException("behaviour integration failed")
  {} // end of UMATIntegrationFailed::UMATIntegrationFailed

  UMATIntegrationFailed::UMATIntegrationFailed(const std::string& m)
    : UMATException("behaviour integration failed : "+m)
  {} // end of UMATIntegrationFailed::UMATIntegrationFailed

  UMATIntegrationFailed::UMATIntegrationFailed(const UMATIntegrationFailed& e)
    : UMATException(e)
  {} // end of UMATIntegrationFailed::UMATIntegrationFailed

  UMATIntegrationFailed::~UMATIntegrationFailed() throw()
  {} // end of UMATIntegrationFailed::~UMATIntegrationFailed()

  UMATInvalidNTENSValue::UMATInvalidNTENSValue(const unsigned short N)
    : UMATException("Invalid tensor size declared '"+tfel::utilities::ToString(N)+"'")
  {} // end of UMATInvalidNTENSValue::UMATInvalidNTENSValue

  UMATInvalidNTENSValue::UMATInvalidNTENSValue(const UMATInvalidNTENSValue& e)
    : UMATException(e)
  {} // end of UMATInvalidNTENSValue::UMATInvalidNTENSValue

  UMATInvalidNTENSValue::~UMATInvalidNTENSValue() throw()
  {} // end of UMATInvalidNTENSValue::~UMATInvalidNTENSValue()

  UMATInvalidDimension::UMATInvalidDimension(const std::string& b,
					     const unsigned short N)
    : UMATException("''"+b+"' can't be used in "+tfel::utilities::ToString(N)+"D")
  {} // end of UMATInvalidDimension::UMATInvalidDimension

  UMATInvalidDimension::UMATInvalidDimension(const UMATInvalidDimension& e)
    : UMATException(e)
  {} // end of UMATInvalidDimension::UMATInvalidDimension

  UMATInvalidDimension::~UMATInvalidDimension() throw()
  {} // end of UMATInvalidDimension::~UMATInvalidDimension()

} // end of namespace umat

