/*!
 * \file   UMATException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2007
 */

#include"Utilities/ToString.hxx"
#include"UMAT/UMATException.hxx"

namespace umat 
{
  
  UMATException::UMATException(const std::string& s)
    : msg(s)
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

  UMATInvalidNTENSValue::UMATInvalidNTENSValue(const unsigned short N)
    : UMATException("Invalid tensor size declared '"+tfel::utilities::ToString(N)+"'")
  {} // end of UMATInvalidNTENSValue::UMATInvalidNTENSValue

  UMATInvalidNTENSValue::~UMATInvalidNTENSValue() throw()
  {} // end of UMATInvalidNTENSValue::~UMATInvalidNTENSValue()

  UMATInvalidDimension::UMATInvalidDimension(const std::string& b,
					     const unsigned short N)
    : UMATException("''"+b+"' can't be used in "+tfel::utilities::ToString(N)+"D")
  {} // end of UMATInvalidDimension::UMATInvalidDimension

  UMATInvalidDimension::~UMATInvalidDimension() throw()
  {} // end of UMATInvalidDimension::~UMATInvalidDimension()

} // end of namespace umat

