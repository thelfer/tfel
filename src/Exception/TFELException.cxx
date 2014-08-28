/*!
 * \file   src/Exception/TFELException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2007
 */

#include"TFEL/Exception/TFELException.hxx"

namespace tfel{

  namespace exception{
    
    std::string 
    TFELException::getName(void){
      return "TFELException";
    }

    TFELException::TFELException(const std::string& s)
      : msg(s)
    {}

    const char* 
    TFELException::what(void) const throw()
    { 
      return this->msg.c_str(); 
    }

    const std::string
    TFELException::getMsg(void) const
    {
      return this->msg;
    }
    
    TFELException::~TFELException() throw()
    {}

  } // end of namespace exception

} // end of namespace tfel

