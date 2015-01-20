/*!
 * \file   mfront/src/AsterException.cxx
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
  AsterException::what (void) const noexcept
  { 
    return msg.c_str(); 
  } // end of AsterException::what

  std::string
  AsterException::getMsg(void) const noexcept
  {
    return msg;
  } // end of AsterException::getMsg
  
  AsterException::~AsterException() noexcept
  {} // end of AsterException::~AsterException

  AsterInvalidNTENSValue::AsterInvalidNTENSValue(const unsigned short N)
    : AsterException("Invalid tensor size declared '"+
		     std::to_string(static_cast<unsigned int>(N))+"'")
  {} // end of AsterInvalidNTENSValue::AsterInvalidNTENSValue

  AsterInvalidNTENSValue::AsterInvalidNTENSValue(const AsterInvalidNTENSValue& e)
    : AsterException(e)
  {} // end of AsterInvalidNTENSValue::AsterInvalidNTENSValue

  AsterInvalidNTENSValue::~AsterInvalidNTENSValue() noexcept
  {} // end of AsterInvalidNTENSValue::~AsterInvalidNTENSValue()

  AsterInvalidDimension::AsterInvalidDimension(const std::string& b,
					     const unsigned short N)
    : AsterException("''"+b+"' can't be used in "+
		     std::to_string(static_cast<unsigned int>(N))+"D")
  {} // end of AsterInvalidDimension::AsterInvalidDimension

  AsterInvalidDimension::AsterInvalidDimension(const AsterInvalidDimension& e)
    : AsterException(e)
  {} // end of AsterInvalidDimension::AsterInvalidDimension

  AsterInvalidDimension::~AsterInvalidDimension() noexcept
  {} // end of AsterInvalidDimension::~AsterInvalidDimension()

} // end of namespace aster

