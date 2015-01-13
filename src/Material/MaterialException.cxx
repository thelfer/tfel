/*!
 * \file   src/Material/MaterialException.cxx
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

#include"TFEL/Material/MaterialException.hxx"

namespace tfel{
  
  namespace material {

    MaterialException::MaterialException(const std::string& s)
      :  tfel::exception::TFELException(s)
    {}

    MaterialException::~MaterialException() throw()
    {} // end of MaterialException::~MaterialException
    
    std::string 
    MaterialException::getName(void){
      return "MaterialException";
    }
    
    std::string 
    DivergenceException::getName(void){
      return "DivergenceException";
    }

    DivergenceException::DivergenceException(const std::string& s)
      :  tfel::material::MaterialException(s)
    {}

    DivergenceException::~DivergenceException() throw()
    {} // end of DivergenceException::~DivergenceException

    std::string 
    OutOfBoundsException::getName(void){
      return "OutOfBoundsException";
    }

    OutOfBoundsException::OutOfBoundsException(const std::string& s)
      :  tfel::material::MaterialException(s)
    {}
  
    OutOfBoundsException::~OutOfBoundsException() throw()
    {} // end of OutOfBoundsException::~OutOfBoundsException

  } // end of namespace material

} // end of namespace tfel

