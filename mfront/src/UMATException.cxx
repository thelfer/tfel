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

#include"MFront/UMAT/UMATException.hxx"

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

namespace umat 
{
  
  UMATException::UMATException(const std::string& s)
    : msg(s)
  {} // end of UMATException::UMATException
    
  const char* 
  UMATException::what (void) const noexcept
  { 
    return msg.c_str(); 
  } // end of UMATException::what

  UMATException::~UMATException() noexcept
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

  UMATIntegrationFailed::~UMATIntegrationFailed() noexcept
  {} // end of UMATIntegrationFailed::~UMATIntegrationFailed()

  UMATInvalidNTENSValue::UMATInvalidNTENSValue(const unsigned short N)
    : UMATException("Invalid tensor size declared '"+
		    std::to_string(static_cast<unsigned int>(N))+"'")
  {} // end of UMATInvalidNTENSValue::UMATInvalidNTENSValue

  UMATInvalidNTENSValue::UMATInvalidNTENSValue(const UMATInvalidNTENSValue& e)
    : UMATException(e)
  {} // end of UMATInvalidNTENSValue::UMATInvalidNTENSValue

  UMATInvalidNTENSValue::~UMATInvalidNTENSValue() noexcept
  {} // end of UMATInvalidNTENSValue::~UMATInvalidNTENSValue()

  UMATInvalidDimension::UMATInvalidDimension(const std::string& b,
					     const unsigned short N)
    : UMATException("''"+b+"' can't be used in "+
		    std::to_string(static_cast<unsigned int>(N))+"D")
  {} // end of UMATInvalidDimension::UMATInvalidDimension

  UMATInvalidDimension::UMATInvalidDimension(const UMATInvalidDimension& e)
    : UMATException(e)
  {} // end of UMATInvalidDimension::UMATInvalidDimension

  UMATInvalidDimension::~UMATInvalidDimension() noexcept
  {} // end of UMATInvalidDimension::~UMATInvalidDimension()

} // end of namespace umat

