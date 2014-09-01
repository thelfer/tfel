/*!
 * \file   src/Math/BasicOperations.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   06 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<string>
#include"TFEL/Math/General/BasicOperations.hxx"

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

