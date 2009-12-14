/*!
 * \file   NodesManager.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 mai 2007
 */

#ifndef _LIB_TFEL_NODESMANAGER_HXX_
#define _LIB_TFEL_NODESMANAGER_HXX_ 

#include<vector>
#include<list>

#include"Metaprogramming/TypeList.hxx"

namespace tfel{

  namespace fe{

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

    template<typename NodesList,
	     typename ContainerRebind = VectorRebind>
    struct NodesManager;

  } // end of namespace fe  

} // end of namespace tfel  

#include"FiniteElement/Node/NodesManager.ixx"

#endif /* _LIB_TFEL_NODESMANAGER_HXX */

