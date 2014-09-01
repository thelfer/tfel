/*!
 * \file   src/Math/Id.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
