/*!
 * \file   MaterialException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2007
 */

#include"TFEL/Material/MaterialException.hxx"

namespace tfel{
  
  namespace material {

    MaterialException::MaterialException(const std::string& s)
      :  tfel::exception::TFELException(s)
    {}
    
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

    std::string 
    OutOfBoundsException::getName(void){
      return "OutOfBoundsException";
    }

    OutOfBoundsException::OutOfBoundsException(const std::string& s)
      :  tfel::material::MaterialException(s)
    {}
  
  } // end of namespace material

} // end of namespace tfel

