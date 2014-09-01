/*!
 * \file   src/Math/ST2toST2Tag.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<string>
#include"TFEL/Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel{

  namespace math{

    std::string 
    ST2toST2Tag::getName(void)
    {
      return std::string("ST2toST2Tag");
    } // end of getName()
    
  } // end of namespace math

} // end of namespace tfel
