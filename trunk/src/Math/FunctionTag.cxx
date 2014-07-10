/*!
 * \file   FunctionTag.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 */

#include<string>
#include"TFEL/Math/Function/FunctionConcept.hxx"

namespace tfel{

  namespace math{

    std::string 
    FunctionTag::getName(void)
    {
      return std::string("FunctionTag");
    } // end of getName()
    
  } // end of namespace math

} // end of namespace tfel
