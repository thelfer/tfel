/*!
 * \file   include/TFEL/Utilities/ContainerRebind.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 mai 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_CONTAINERREBIND_HXX_
#define _LIB_TFEL_CONTAINERREBIND_HXX_ 

#include<vector>
#include<list>

namespace tfel
{

  namespace utilities
  {

    struct VectorRebind
    {
      template<typename T>
      struct Rebind
      {
	typedef std::vector<T> type;
      };
    };

    struct ListRebind
    {
      template<typename T>
      struct Rebind
      {
	typedef std::list<T> type;
      };
    };
    
  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_CONTAINERREBIND_HXX */

