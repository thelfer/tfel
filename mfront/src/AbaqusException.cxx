/*!
 * \file   mfront/src/AbaqusException.cxx
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

#include"MFront/Abaqus/AbaqusException.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99)
#include<sstream>
namespace std{
  template<typename T>
  std::string to_string(const T& v){
    std::ostringstream s;
    s << v;
    return s.str();
  }
}
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace abaqus 
{
  
  AbaqusException::AbaqusException(const std::string& s)
    : msg(s)
  {} // end of AbaqusException::AbaqusException
    
  AbaqusException::AbaqusException(const AbaqusException& e)
    : std::exception(),
      msg(e.msg)
  {} // end of AbaqusException::AbaqusException

  const char* 
  AbaqusException::what (void) const noexcept
  { 
    return msg.c_str(); 
  } // end of AbaqusException::what

  std::string
  AbaqusException::getMsg(void) const noexcept
  {
    return msg;
  } // end of AbaqusException::getMsg
  
  AbaqusException::~AbaqusException() noexcept
  {} // end of AbaqusException::~AbaqusException

  AbaqusInvalidNTENSValue::AbaqusInvalidNTENSValue(const unsigned short N)
    : AbaqusException("Invalid tensor size declared '"+
		     std::to_string(static_cast<unsigned int>(N))+"'")
  {} // end of AbaqusInvalidNTENSValue::AbaqusInvalidNTENSValue

  AbaqusInvalidNTENSValue::AbaqusInvalidNTENSValue(const AbaqusInvalidNTENSValue& e)
    : AbaqusException(e)
  {} // end of AbaqusInvalidNTENSValue::AbaqusInvalidNTENSValue

  AbaqusInvalidNTENSValue::~AbaqusInvalidNTENSValue() noexcept
  {} // end of AbaqusInvalidNTENSValue::~AbaqusInvalidNTENSValue()

  AbaqusInvalidDimension::AbaqusInvalidDimension(const std::string& b,
					     const unsigned short N)
    : AbaqusException("''"+b+"' can't be used in "+
		     std::to_string(static_cast<unsigned int>(N))+"D")
  {} // end of AbaqusInvalidDimension::AbaqusInvalidDimension

  AbaqusInvalidDimension::AbaqusInvalidDimension(const AbaqusInvalidDimension& e)
    : AbaqusException(e)
  {} // end of AbaqusInvalidDimension::AbaqusInvalidDimension

  AbaqusInvalidDimension::~AbaqusInvalidDimension() noexcept
  {} // end of AbaqusInvalidDimension::~AbaqusInvalidDimension()

} // end of namespace abaqus

