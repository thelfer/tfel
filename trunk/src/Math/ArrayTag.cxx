/*!
 * \file   ArrayTag.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 */

#include<string>
#include"TFEL/Math/Array/ArrayConcept.hxx"

namespace tfel{

  namespace math{

    std::string 
    ArrayTag::getName(void)
    {
      return std::string("ArrayTag");
    } // end of getName()
    
  } // end of namespace math

} // end of namespace tfel
