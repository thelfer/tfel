/*!
 * \file   src/Exception/TFELException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

