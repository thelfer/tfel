/*!
 * \file   VectorTag.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 */

#include<string>
#include"TFEL/Math/Vector/VectorConcept.hxx"

namespace tfel{

  namespace math{

    const std::string 
    VectorTag::getName(void)
    {
      return std::string("VectorTag");
    } // end of getName()
    
  } // end of namespace math

} // end of namespace tfel
