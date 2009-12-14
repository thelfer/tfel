/*!
 * \file   UMATException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2007
 */

#include"UMAT/UMATException.hxx"

namespace umat 
{

  const std::string 
  UMATException::getName(void){
    return "UMATException";
  } // end of UMATException::getName
  
  UMATException::UMATException(const std::string& s)
    : msg(s)
  {} // end of UMATException::UMATException
    
  const char* 
  UMATException::what (void) const throw()
  { 
    return msg.c_str(); 
  } // end of UMATException::what

  const std::string
  UMATException::getMsg(void) const throw()
  {
    return msg;
  } // end of UMATException::getMsg
  
  UMATException::~UMATException() throw()
  {} // end of UMATException::~UMATException

} // end of namespace umat
