/*!
 * \file   Node.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 mai 2007
 */

#ifndef _LIB_TFEL_NODE_IXX_
#define _LIB_TFEL_NODE_IXX_ 

namespace tfel
{
  namespace fe
  {

    template<unsigned short N,bool useQt,
	     typename Param>
    Node<N,useQt,Param>::Node()
    {}

    template<unsigned short N,bool useQt,
	     typename Param>
    Node<N,useQt,Param>::Node(const Node& src)
      : position(src.position)
    {}

    template<unsigned short N,bool useQt,
	     typename Param>
    Node<N,useQt,Param>::Node(const Node<N,useQt,Param>::PositionType& v)
      : position(v)
    {}

    template<unsigned short N,bool useQt,
	     typename Param>
    Node<N,useQt,Param>&
    Node<N,useQt,Param>::operator=(const Node<N,useQt,Param>& src)
    {
      if(this!=&src){
	this->position = src.position;
      }
      return *this;
    }

    template<unsigned short N,bool useQt,
	     typename Param>
    Node<N,useQt,Param>&
    Node<N,useQt,Param>::operator+=(const Node<N,useQt,Param>::PositionType& v)
    {
      this->position += v;
      return *this;
    }

    template<unsigned short N,bool useQt,
	     typename Param>
    const typename Node<N,useQt,Param>::PositionType&
    Node<N,useQt,Param>::getPosition(void) const
    {
      return this->position;
    }

    template<unsigned short N,bool useQt,
	     typename Param>
    const typename Node<N,useQt,Param>::length
    Node<N,useQt,Param>::getPosition(const unsigned short i) const
    {
      return this->position(i);
    }

    template<unsigned short N,bool useQt,
	     typename Param>
    Node<N,useQt,Param>::~Node()
    {}

  } // end of namespace fe

} // end of namespace tfel


#endif /* _LIB_TFEL_NODE_IXX */

