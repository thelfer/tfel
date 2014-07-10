/*!
 * \file   Id.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Jan 2007
 */

#include"TFEL/Math/Function/Id.hxx"
#include"TFEL/Math/Function/Cst.hxx"

namespace tfel{

  namespace math{

    TFELMATH_VISIBILITY_EXPORT FctId id TFEL_UNUSED_ATTRIBUTE;
    
    std::string FctId::getName(void){
      return "FctId";
    }

    Cst<1,1>
    derivate(const FctId)
    {
      return Cst<1,1>();
    }

  } // end of namespace tfel

} // end of namespace tfel
