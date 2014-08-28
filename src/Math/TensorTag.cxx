/*!
 * \file   src/Math/TensorTag.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 */

#include<string>
#include"TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel{

  namespace math{

    std::string 
    TensorTag::getName(void)
    {
      return std::string("TensorTag");
    } // end of getName()
    
  } // end of namespace math

} // end of namespace tfel
