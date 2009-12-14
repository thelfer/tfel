/*!
 * \file   BasicOperations.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   06 nov 2006
 */

#include<string>
#include"Math/General/BasicOperations.hxx"

namespace tfel{

  namespace math{
 
    std::string
    OpPlus::getName(void){
      return std::string("+");
    }

    std::string
    OpMinus::getName(void){
      return std::string("-");
    }

    std::string
    OpMult::getName(void){
      return std::string("*");
    }

    std::string
    OpDiv::getName(void){
      return std::string("/");
    }

    std::string
    OpNeg::getName(void){
      return std::string("OpNeg");
    }

    std::string
    OpDotProduct::getName(void){
      return std::string("OpDotProduct");
    }

    std::string
    OpDiadicProduct::getName(void){
      return std::string("OpDiadicProduct");
    }

  } // end of namespace math  

} // end of namespace tfel  

