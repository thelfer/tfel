/*!
 * \file   ElementsManager.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 mai 2007
 */

#ifndef _LIB_TFEL_ELEMENTSMANAGER_IXX_
#define _LIB_TFEL_ELEMENTSMANAGER_IXX_ 

namespace tfel
{

  namespace fe
  {

    namespace internals
    {
      
      template<typename Element,
	       typename ContainerRebind>
      struct ElementsContainer
	: protected ContainerRebind::template Rebind<Element>::type
      {};

      template<typename CurrentElement,typename NextElement,
	       typename ContainerRebind>
      struct ElementsContainer<tfel::meta::TLNode<CurrentElement,NextElement>,
			       ContainerRebind>
	: protected ElementsContainer<CurrentElement,ContainerRebind>,
	  protected ElementsContainer<NextElement,ContainerRebind>
      {};
      
      template<typename ContainerRebind>
      struct ElementsContainer<tfel::meta::TLE,ContainerRebind>
      {};
      
    } // end of namspace internals

    template<typename Element>
    class ElementsManager<Element,tfel::utilities::VectorRebind>
      : public tfel::fe::internals::ElementsContainer<Element,tfel::utilities::VectorRebind>
    {

    protected:

      typedef typename tfel::meta::GenerateTypeList<Element>::type NList;
      typedef typename tfel::meta::IF<tfel::meta::IsSubClassOf<Element,tfel::meta::TL>::cond,
				      Element,NList>::type ElementList;

    public:

      template<typename T1>
      struct iterator
      {
	typedef typename std::vector<T1>::iterator type;
      };

      template<typename T1>
      struct reverse_iterator
      {
	typedef typename std::vector<T1>::reverse_iterator type;
      };
      
      template<typename T1>
      struct const_iterator
      {
	typedef typename std::vector<T1>::const_iterator type;
      };

      template<typename T1>
      struct const_reverse_iterator
      {
	typedef typename std::vector<T1>::const_reverse_iterator type;
      };

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	const T1&>::type 
      get(const typename std::vector<T1>::size_type i) const
      {
	return std::vector<T1>::operator[](i);
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	T1&>::type 
      get(const typename std::vector<T1>::size_type i)
      {
	return std::vector<T1>::operator[](i);
      }
    
      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	void >::type 
      clear(void)
      {
	std::vector<T1>::clear();
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	const bool >::type 
      empty(void) const
      {
	return std::vector<T1>::empty();
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename std::vector<T1>::size_type 
      >::type size(void) const
      {
	return std::vector<T1>::size();
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename std::vector<T1>::size_type 
      >::type max_size(void) const
      {
	return std::vector<T1>::max_size();
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename std::vector<T1>::size_type 
      >::type capacity(void) const
      {
	return std::vector<T1>::capacity();
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	void >::type 
      reserve(const typename std::vector<T1>::size_type s)
      {
	std::vector<T1>::reserve(s);
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	void >::type 
      resize(const typename std::vector<T1>::size_type s)
      {
	std::vector<T1>::resize(s);
      }
    
      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	void >::type 
      resize(const typename std::vector<T1>::size_type s, const T1& src)
      {
	std::vector<T1>::resize(s,src);
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	void >::type 
      push_back(const T1& src)
      {
	std::vector<T1>::push_back(src);
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	void >::type 
      pop_back(void)
      {
	std::vector<T1>::pop_back();
      }
        
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename std::vector<T1>::reference >::type 
      front(void)
      {
	return 	std::vector<T1>::front();
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename std::vector<T1>::const_reference >::type 
      front(void) const
      {
	return std::vector<T1>::front();
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename std::vector<T1>::reference 
      >::type back(void)
      {
	return std::vector<T1>::back();
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename std::vector<T1>::const_reference >::type 
      back(void) const
      {
	return std::vector<T1>::back();
      }
    
      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	void >::type 
      swap(std::vector<T1>& src)
      {
	std::vector<T1>::swap(src);
      }
    
      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	iterator<T1> >::type 
      insert(const iterator<T1> pos, const T1& src)
      {
	return std::vector<T1>::insert(pos,src);
      }

      template <typename T1,class InputIterator>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	void >::type 
      insert(const iterator<T1> pos,const InputIterator f,const InputIterator l)
      {
	std::vector<T1>::insert(pos,f,l);
      }
    
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	void >::type 
      insert(const iterator<T1> pos,
	     const typename std::vector<T1>::size_type n,
	     const T1& x)
      {
	std::vector<T1>::insert(pos,n,x);
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename iterator<T1>::type
      >::type 
      begin(void)
      {
	return std::vector<T1>::begin();
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename iterator<T1>::type
      >::type 
      end(void)
      {
	return std::vector<T1>::end();
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename const_iterator<T1>::type
      >::type 
      begin(void) const
      {
	return std::vector<T1>::begin();
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename const_iterator<T1>::type
      >::type 
      end(void) const
      {
	return std::vector<T1>::end();
      }
        
      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename reverse_iterator<T1>::type
      >::type 
      rbegin(void)
      {
	return std::vector<T1>::rbegin();
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename reverse_iterator<T1>::type
      >::type 
      rend(void)
      {
	return std::vector<T1>::rend();
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename const_reverse_iterator<T1>::type
      >::type 
      rbegin(void) const
      {
	return std::vector<T1>::rbegin();
      }

      template<typename T1>
      TFEL_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,ElementList>::value==1, 
	typename const_reverse_iterator<T1>::type
      >::type 
      rend(void) const
      {
	return std::vector<T1>::rbegin();
      }
    };

  } // end of namespace fe  

} // end of namespace tfel  

#endif /* _LIB_TFEL_ELEMENTSMANAGER_IXX */

