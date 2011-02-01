/*!
 * \file   Mesh.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 mai 2007
 */

#ifndef _LIB_TFEL_MESH_HXX_
#define _LIB_TFEL_MESH_HXX_ 

#include"TFEL/Utilities/ContainerRebind.hxx"
#include"TFEL/FiniteElement/NodesManager.hxx"
#include"TFEL/FiniteElement/ElementsManager.hxx"

namespace tfel
{

  namespace fe
  {

    template<typename NodeList,
	     typename ElementList,
	     typename NodesContainerRebind   = tfel::utilities::VectorRebind,
	     typename ElementContainerRebind = tfel::utilities::VectorRebind>
    struct Mesh;

  } // end of namespace fe

} // end of namespace tfel

#include"TFEL/FiniteElement/Mesh/Mesh.ixx"

#endif /* _LIB_TFEL_MESH_HXX */
