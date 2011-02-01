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

#include"TFEL/Metaprogramming/TypeList.hxx"
#include"TFEL/Utilities/ContainerRebind.hxx"

namespace tfel{

  namespace fe{

    template<typename NodesList,
	     typename ContainerRebind = tfel::utilities::VectorRebind>
    struct NodesManager;

  } // end of namespace fe  

} // end of namespace tfel  

#include"TFEL/FiniteElement/Node/NodesManager.ixx"

#endif /* _LIB_TFEL_NODESMANAGER_HXX */

