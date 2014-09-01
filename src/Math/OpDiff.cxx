/*!
 * \file   src/Math/OpDiff.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 fév 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */
#include<string>
#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"

namespace tfel{

  namespace math{
    
    std::string
    OpDiff::getName(void)
    {
      return "OpDiff";
    } // end of OpDiff

    namespace stdfunctions{

      TFELMATH_VISIBILITY_EXPORT OpDiff D TFEL_UNUSED_ATTRIBUTE;

    } // end of namespace stdfunctions
    
  } // end of namespace math

} // end of namespace tfel
