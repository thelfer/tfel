/*!
 * \file   MathException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2007
 */

#include"TFEL/Math/MathException.hxx"

namespace tfel{

  namespace math{

    const std::string 
    MathException::getName(void){
      return "MathException";
    }
    
    MathException::MathException(const std::string& s)
      :  tfel::exception::TFELException(s)
    {}

    const std::string 
    MathRunTimeException::getName(void){
      return "MathRunTimeException";
    }

    MathRunTimeException::MathRunTimeException(const std::string& s)
      :  MathException(s)
    {}

    const std::string 
    MathDivergenceException::getName(void){
      return "MathDivergenceException";
    }

    MathDivergenceException::MathDivergenceException(const std::string& s)
      :  MathException(s)
    {}

  } // end of namespace math

} // end of namespace tfel

