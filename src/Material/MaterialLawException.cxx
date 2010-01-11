/*!
 * \file   MaterialLawException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2007
 */

#include "MaterialLaw/MaterialLawException.hxx"

namespace tfel{
  
  namespace materiallaw {

    MaterialLawException::MaterialLawException(const std::string& s)
      :  tfel::exception::TFELException(s)
    {}
    
    const std::string 
    MaterialLawException::getName(void){
      return "MaterialLawException";
    }
    
    const std::string 
    DivergenceException::getName(void){
      return "DivergenceException";
    }

    DivergenceException::DivergenceException(const std::string& s)
      :  tfel::materiallaw::MaterialLawException(s)
    {}

    const std::string 
    OutOfBoundsException::getName(void){
      return "OutOfBoundsException";
    }

    OutOfBoundsException::OutOfBoundsException(const std::string& s)
      :  tfel::materiallaw::MaterialLawException(s)
    {}
  
  } // end of namespace materiallaw

} // end of namespace tfel

