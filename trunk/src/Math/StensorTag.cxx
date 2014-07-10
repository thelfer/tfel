/*!
 * \file   StensorTag.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 */

#include<string>
#include"TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel{

  namespace math{

    std::string 
    StensorTag::getName(void)
    {
      return std::string("StensorTag");
    } // end of getName()
    
  } // end of namespace math

} // end of namespace tfel
