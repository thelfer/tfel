/*!
 * \file   mfront/src/AsterException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2007
 */

#include"TFEL/Utilities/ToString.hxx"
#include"MFront/Aster/AsterException.hxx"

namespace aster 
{
  
  AsterException::AsterException(const std::string& s)
    : msg(s)
  {} // end of AsterException::AsterException
    
  AsterException::AsterException(const AsterException& e)
    : std::exception(),
      msg(e.msg)
  {} // end of AsterException::AsterException

  const char* 
  AsterException::what (void) const throw()
  { 
    return msg.c_str(); 
  } // end of AsterException::what

  std::string
  AsterException::getMsg(void) const throw()
  {
    return msg;
  } // end of AsterException::getMsg
  
  AsterException::~AsterException() throw()
  {} // end of AsterException::~AsterException

  AsterInvalidNTENSValue::AsterInvalidNTENSValue(const unsigned short N)
    : AsterException("Invalid tensor size declared '"+tfel::utilities::ToString(N)+"'")
  {} // end of AsterInvalidNTENSValue::AsterInvalidNTENSValue

  AsterInvalidNTENSValue::AsterInvalidNTENSValue(const AsterInvalidNTENSValue& e)
    : AsterException(e)
  {} // end of AsterInvalidNTENSValue::AsterInvalidNTENSValue

  AsterInvalidNTENSValue::~AsterInvalidNTENSValue() throw()
  {} // end of AsterInvalidNTENSValue::~AsterInvalidNTENSValue()

  AsterInvalidDimension::AsterInvalidDimension(const std::string& b,
					     const unsigned short N)
    : AsterException("''"+b+"' can't be used in "+tfel::utilities::ToString(N)+"D")
  {} // end of AsterInvalidDimension::AsterInvalidDimension

  AsterInvalidDimension::AsterInvalidDimension(const AsterInvalidDimension& e)
    : AsterException(e)
  {} // end of AsterInvalidDimension::AsterInvalidDimension

  AsterInvalidDimension::~AsterInvalidDimension() throw()
  {} // end of AsterInvalidDimension::~AsterInvalidDimension()

} // end of namespace aster

