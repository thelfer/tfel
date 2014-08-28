/*!
 * \file   src/Math/CompositeTag.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 */

#include<string>
#include"TFEL/Math/Composite/CompositeConcept.hxx"

namespace tfel{

  namespace math{

    const std::string 
    CompositeTag::getName(void)
    {
      return std::string("CompositeTag");
    } // end of getName()
    
  } // end of namespace math

} // end of namespace tfel
