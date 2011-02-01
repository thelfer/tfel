/*!
 * \file   Mesh.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 mai 2007
 */

#ifndef _LIB_TFEL_MESH_IXX_
#define _LIB_TFEL_MESH_IXX_ 

namespace tfel
{

  namespace fe
  {

    template<typename Nodes,
	     typename Elements>
    class Mesh<Nodes,Elements,
		tfel::utilities::VectorRebind,
		tfel::utilities::VectorRebind>
      : public NodesManager<Nodes,tfel::utilities::VectorRebind>,
	public ElementsManager<Elements,tfel::utilities::VectorRebind>
    {
      typedef NodesManager<Nodes,tfel::utilities::VectorRebind>       NodesManagerType;
      typedef ElementsManager<Elements,tfel::utilities::VectorRebind> ElementsManagerType;
      typedef typename NodesManagerType::NodeList       NodeList;
      typedef typename ElementsManagerType::ElementList ElementList;
    public:

      template<typename T1>
      class iterator
      {
	typedef typename NodesManagerType::template iterator<T1>::type    NodeIterator;
	typedef typename ElementsManagerType::template iterator<T1>::type ElementsIterator;
      public:
	typedef typename tfel::meta::IF<tfel::meta::TLCountNbrOfT<T1,NodeList>::value==1u,
					NodeIterator,ElementsIterator>::type type;
      };

      template<typename T1>
      class reverse_iterator
      {
	typedef typename NodesManagerType::template reverse_iterator<T1>::type    NodeIterator;
	typedef typename ElementsManagerType::template reverse_iterator<T1>::type ElementsIterator;
      public:
	typedef typename tfel::meta::IF<tfel::meta::TLCountNbrOfT<T1,NodeList>::value==1u,
					NodeIterator,ElementsIterator>::type type;
      };
      
      template<typename T1>
      class const_iterator
      {
	typedef typename NodesManagerType::template const_iterator<T1>::type    NodeIterator;
	typedef typename ElementsManagerType::template const_iterator<T1>::type ElementsIterator;
      public:
	typedef typename tfel::meta::IF<tfel::meta::TLCountNbrOfT<T1,NodeList>::value==1u,
					NodeIterator,ElementsIterator>::type type;
      };

      template<typename T1>
      struct const_reverse_iterator
      {
	typedef typename NodesManagerType::template const_reverse_iterator<T1>::type NodeIterator;
	typedef typename ElementsManagerType::template const_reverse_iterator<T1>::type ElementsIterator;
      public:
	typedef typename tfel::meta::IF<tfel::meta::TLCountNbrOfT<T1,NodeList>::value==1u,
					NodeIterator,ElementsIterator>::type type;
      };

      using NodesManagerType::get;
      using NodesManagerType::clear;
      using NodesManagerType::empty;
      using NodesManagerType::size;
      using NodesManagerType::max_size;
      using NodesManagerType::capacity;
      using NodesManagerType::reserve;
      using NodesManagerType::resize;
      using NodesManagerType::push_back;
      using NodesManagerType::pop_back;
      using NodesManagerType::front; 
      using NodesManagerType::back;
      using NodesManagerType::swap;
      using NodesManagerType::insert;
      using NodesManagerType::begin;
      using NodesManagerType::end;
      using NodesManagerType::rbegin;
      using NodesManagerType::rend;      

      using ElementsManagerType::get;
      using ElementsManagerType::clear;
      using ElementsManagerType::empty;
      using ElementsManagerType::size;
      using ElementsManagerType::max_size;
      using ElementsManagerType::capacity;
      using ElementsManagerType::reserve;
      using ElementsManagerType::resize;
      using ElementsManagerType::push_back;
      using ElementsManagerType::pop_back;
      using ElementsManagerType::front; 
      using ElementsManagerType::back;
      using ElementsManagerType::swap;
      using ElementsManagerType::insert;
      using ElementsManagerType::begin;
      using ElementsManagerType::end;
      using ElementsManagerType::rbegin;
      using ElementsManagerType::rend;      

    };

  } // end of namespace fe

} // end of namespace tfel

#endif /* _LIB_TFEL_MESH_IXX */

