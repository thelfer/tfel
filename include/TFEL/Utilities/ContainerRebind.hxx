/*!
 * \file   include/TFEL/Utilities/ContainerRebind.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 mai 2007
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

