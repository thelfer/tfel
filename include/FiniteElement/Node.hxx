/*!
 * \file   Node.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 mai 2007
 */

#ifndef _LIB_TFEL_NODE_HXX_
#define _LIB_TFEL_NODE_HXX_ 

#include"Config/TFELTypes.hxx"

namespace tfel
{
  namespace fe
  {
    
    namespace internals
    {

      template<unsigned short N>
      struct NodeDefaultParameters
	: protected tfel::config::Types<N,double,false>
      {};
      
    } // end of namespace internals

    template<unsigned short N,
	     bool useQt=false,
             class NodeParameters=tfel::fe::internals::NodeDefaultParameters<N> >
    class Node
      : public NodeParameters
    {
      //! a simple alias
      typedef typename NodeParameters::length       length;
      typedef typename NodeParameters::PositionType PositionType;

    public:

      Node();

      Node(const Node&);

      Node(const PositionType&);
      
      Node&
      operator=(const Node&);

      Node&
      operator+=(const PositionType&);
      
      const PositionType&
      getPosition(void) const;

      const length
      getPosition(const unsigned short) const;

      virtual
      ~Node();

    private:
      
      // position of the node
      PositionType position;

    };

  } // end of namespace fe

} // end of namespace tfel

#include"FiniteElement/Node/Node.ixx"

#endif /* _LIB_TFEL_NODE_HXX */

