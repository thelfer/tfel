/*!
 * \file   LambdaTag.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 */

#include<string>
#include"TFEL/Math/LambdaExpression/LambdaConcept.hxx"

namespace tfel{

  namespace math{

    std::string 
    LambdaTag::getName(void)
    {
      return std::string("LambdaTag");
    } // end of getName()
    
  } // end of namespace math

} // end of namespace tfel
