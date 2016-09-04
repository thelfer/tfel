/*!
 * \file   mfront/src/EuroplexusException.cxx
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

#include"MFront/Europlexus/EuroplexusException.hxx"

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

namespace epx 
{
  
  EuroplexusException::EuroplexusException(const std::string& s)
    : msg(s)
  {} // end of EuroplexusException::EuroplexusException
    
  EuroplexusException::EuroplexusException(const EuroplexusException& e)
    : std::exception(),
      msg(e.msg)
  {} // end of EuroplexusException::EuroplexusException

  const char* 
  EuroplexusException::what () const noexcept
  { 
    return msg.c_str(); 
  } // end of EuroplexusException::what

  std::string
  EuroplexusException::getMsg() const noexcept
  {
    return msg;
  } // end of EuroplexusException::getMsg
  
  EuroplexusException::~EuroplexusException() noexcept
  {} // end of EuroplexusException::~EuroplexusException

  EuroplexusInvalidModellingHypothesis::EuroplexusInvalidModellingHypothesis(const char* b)
    : EuroplexusException("current modelling hypothesis is not supported by behaviour '"+
			  std::string(b)+"'")
  {}

  EuroplexusInvalidModellingHypothesis::~EuroplexusInvalidModellingHypothesis() noexcept
  {} // end of EuroplexusInvalidModellingHypothesis::~EuroplexusInvalidModellingHypothesis()
  
  EuroplexusInvalidNTENSValue::EuroplexusInvalidNTENSValue(const unsigned short N)
    : EuroplexusException("Invalid tensor size declared '"+
			  std::to_string(static_cast<unsigned int>(N))+"'")
  {} // end of EuroplexusInvalidNTENSValue::EuroplexusInvalidNTENSValue

  EuroplexusInvalidNTENSValue::EuroplexusInvalidNTENSValue(const EuroplexusInvalidNTENSValue& e)
    : EuroplexusException(e)
  {} // end of EuroplexusInvalidNTENSValue::EuroplexusInvalidNTENSValue

  EuroplexusInvalidNTENSValue::~EuroplexusInvalidNTENSValue() noexcept
  {} // end of EuroplexusInvalidNTENSValue::~EuroplexusInvalidNTENSValue()

  EuroplexusInvalidDimension::EuroplexusInvalidDimension(const std::string& b,
							 const unsigned short N)
    : EuroplexusException("''"+b+"' can't be used in "+
			  std::to_string(static_cast<unsigned int>(N))+"D")
  {} // end of EuroplexusInvalidDimension::EuroplexusInvalidDimension

  EuroplexusInvalidDimension::EuroplexusInvalidDimension(const EuroplexusInvalidDimension& e)
    : EuroplexusException(e)
  {} // end of EuroplexusInvalidDimension::EuroplexusInvalidDimension

  EuroplexusInvalidDimension::~EuroplexusInvalidDimension() noexcept
  {} // end of EuroplexusInvalidDimension::~EuroplexusInvalidDimension()

} // end of namespace epx

