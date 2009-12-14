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
#include"Utilities/ContainerRebind.hxx"

namespace tfel{

  namespace fe{

    template<typename NodesList,
	     typename ContainerRebind = tfel::utilities::VectorRebind>
    struct NodesManager;

  } // end of namespace fe  

} // end of namespace tfel  

#include"FiniteElement/Node/NodesManager.ixx"

#endif /* _LIB_TFEL_NODESMANAGER_HXX */

