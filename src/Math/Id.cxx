/*!
 * \file   Id.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Jan 2007
 */

#include"Math/Function/Id.hxx"

namespace tfel{

  namespace math{

    std::string FctId::getName(void){
      return "FctId";
    }

    Cst<1>
    derivate(const FctId)
    {
      return Cst<1>();
    }

  } // end of namespace tfel

} // end of namespace tfel
