/*!
 * \file   MultipleDequeVector.ixx
 * \brief  This file implements the MultipleDequeVector class.
 * \author Helfer Thomas
 * \date   31 mai 2006
 */

#ifndef _LIB_TFEL_MULTIPLEDEQUEVECTOR_IXX_
#define _LIB_TFEL_MULTIPLEDEQUEVECTOR_IXX_ 

#include"Utilities/MultipleDequeVector/MultipleDequeVectorIterators.ixx"

namespace tfel{

  namespace utilities{

    /*!
     * \brief Partial specialisation to end the recursion
     * \see   MultipleDequeVectorContainer_
     */
    template<template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    class MultipleDequeVectorContainer_<tfel::meta::TypeListEndType,
					Transform,
					container,
					Allocator>
    {}; // end of class MultipleDequeVectorContainer_
    
    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    class MultipleDequeVectorContainer_
      : public container<typename Transform<typename T::Current>::type,
			 typename Allocator::template rebind<typename T::Current>::other>,
	public MultipleDequeVectorContainer_<typename T::Next,Transform,
					     container,Allocator>
    {
      /*!
       * A simple check
       * \see TFEL_STATIC_ASSERT
       */
      TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<T,tfel::meta::TypeList>::cond));
    };
    
    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      void >::type
    MultipleDequeVectorContainer<T,Transform,container,Allocator>::clear(void){
      container<typename Transform<T1>::type,
	        typename Allocator::template rebind<T1>::other>::clear();
    }
    
    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      bool >::type
    MultipleDequeVectorContainer<T,Transform,container,Allocator>::empty(void) const{
      return container<typename Transform<T1>::type,
	               typename Allocator::template rebind<T1>::other>::empty();
    }
  
    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename 
    tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::size_type >::type
    MultipleDequeVectorContainer<T,Transform,container,Allocator>::size(void) const
    {
      return container<typename Transform<T1>::type,
	               typename Allocator::template rebind<T1>::other>::size();
    }

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename 
    tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::size_type >::type
    MultipleDequeVectorContainer<T,Transform,container,Allocator>::max_size(void) const{
      return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::max_size();
  }

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename 
    tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::size_type >::type
    MultipleDequeVectorContainer<T,Transform,container,Allocator>::capacity(void) const{
      return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::capacity();
  }

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      void >::type
    MultipleDequeVectorContainer<T,Transform,container,Allocator>::reserve(typename container<typename Transform<T1>::type,
									   typename Allocator::template rebind<T1>::other>::size_type s){
      return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::reserve(s);
  }

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      void >::type
    MultipleDequeVectorContainer<T,Transform,container,Allocator>::resize(typename container<typename Transform<T1>::type,
									  typename Allocator::template rebind<T1>::other>::size_type s){
      return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::resize(s);
    }
    
    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      void >::type
      MultipleDequeVectorContainer<T,Transform,container,Allocator>::resize(typename container<typename Transform<T1>::type,
									    typename Allocator::template rebind<T1>::other>::size_type s,const T1 src){
      return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::resize(s,src);
    }

template<typename T,
	 template<class> class Transform,
	 template<class,class> class container,
	 typename Allocator>
template<typename T1>
TFEL_INLINE typename 
tfel::meta::EnableIf<
  tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
  void >::type
MultipleDequeVectorContainer<T,Transform,container,Allocator>::push_back(const typename Transform<T1>::type& src){
  return container<typename Transform<T1>::type,
    typename Allocator::template rebind<T1>::other>::push_back(src);
}

  template<typename T,
	   template<class> class Transform,
	   template<class,class> class container,
	   typename Allocator>
  template<typename T1>
  TFEL_INLINE typename tfel::meta::EnableIf<
    tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
    void >::type
  MultipleDequeVectorContainer<T,Transform,container,Allocator>::pop_back(void){
    return container<typename Transform<T1>::type,
      typename Allocator::template rebind<T1>::other>::pop_back();
  }
        
    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::reference >::type
			 MultipleDequeVectorContainer<T,Transform,container,Allocator>::front(void){
      return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::front();
    }

template<typename T,
	 template<class> class Transform,
	 template<class,class> class container,
	 typename Allocator>
template<typename T1>
TFEL_INLINE typename tfel::meta::EnableIf<
  tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
  typename container<typename Transform<T1>::type,
		     typename Allocator::template rebind<T1>::other>::const_reference >::type
MultipleDequeVectorContainer<T,Transform,container,Allocator>::front(void) const {
  return container<typename Transform<T1>::type,
    typename Allocator::template rebind<T1>::other>::front();
}

  template<typename T,
	   template<class> class Transform,
	   template<class,class> class container,
	   typename Allocator>
  template<typename T1>
  TFEL_INLINE typename tfel::meta::EnableIf<
    tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
    typename container<typename Transform<T1>::type,
		       typename Allocator::template rebind<T1>::other>::reference >::type
  MultipleDequeVectorContainer<T,Transform,container,Allocator>::back(void){
    return container<typename Transform<T1>::type,
      typename Allocator::template rebind<T1>::other>::back();
  }

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::const_reference >::type
			 MultipleDequeVectorContainer<T,Transform,container,Allocator>::back(void) const {
      return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::back();
    }
    
    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      void >::type
      MultipleDequeVectorContainer<T,Transform,container,Allocator>::swap(container<typename Transform<T1>::type,
									  typename Allocator::template rebind<T1>::other>& src)
    {
      container<typename Transform<T1>::type,
	        typename Allocator::template rebind<T1>::other>::swap(src);
    }
    
  template<typename T,
	   template<class> class Transform,
	   template<class,class> class container,
	   typename Allocator>
  template<typename T1>
  TFEL_INLINE typename 
  tfel::meta::EnableIf<
    tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
    typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template iterator<T1> >::type
  MultipleDequeVectorContainer<T,Transform,container,Allocator>::insert(typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template iterator<T1> pos, const T1& src){
    return container<typename Transform<T1>::type,
      typename Allocator::template rebind<T1>::other>::insert(pos,src);
  }

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1,class InputIterator>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      void >::type
      MultipleDequeVectorContainer<T,Transform,container,Allocator>::insert(typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template iterator<T1> pos,InputIterator f, InputIterator l){
      return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::insert(pos,f,l);
    }
    
template<typename T,
	 template<class> class Transform,
	 template<class,class> class container,
	 typename Allocator>
template<typename T1>
TFEL_INLINE typename tfel::meta::EnableIf<
  tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
  void >::type
MultipleDequeVectorContainer<T,Transform,container,Allocator>::insert(typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template iterator<T1> pos, typename container<typename Transform<T1>::type,
								      typename Allocator::template rebind<T1>::other>::size_type n, const T1& x){
  container<typename Transform<T1>::type,
    typename Allocator::template rebind<T1>::other>::insert(pos,n,x);
}

  template<typename T,
	   template<class> class Transform,
	   template<class,class> class container,
	   typename Allocator>
  template<typename T1>
  TFEL_INLINE typename 
  tfel::meta::EnableIf<
    tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
    typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template iterator<T1> >::type
  MultipleDequeVectorContainer<T,Transform,container,Allocator>::begin(void){
    return iterator<T1>(container<typename Transform<T1>::type,
			typename Allocator::template rebind<T1>::other>::begin());
  }

template<typename T,
	 template<class> class Transform,
	 template<class,class> class container,
	 typename Allocator>
template<typename T1>
TFEL_INLINE typename 
tfel::meta::EnableIf<
  tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
  typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template iterator<T1> >::type
MultipleDequeVectorContainer<T,Transform,container,Allocator>::end(void){
  return container<typename Transform<T1>::type,
  typename Allocator::template rebind<T1>::other>::end();
}

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename 
    tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template const_iterator<T1> >::type
      MultipleDequeVectorContainer<T,Transform,container,Allocator>::begin(void) const {
      return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::begin();
    }

template<typename T,
	 template<class> class Transform,
	 template<class,class> class container,
	 typename Allocator>
template<typename T1>
TFEL_INLINE typename 
tfel::meta::EnableIf<
  tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
  typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template const_iterator<T1> >::type
MultipleDequeVectorContainer<T,Transform,container,Allocator>::end(void) const {
  return container<typename Transform<T1>::type,
    typename Allocator::template rebind<T1>::other>::end();
}
        
  template<typename T,
	   template<class> class Transform,
	   template<class,class> class container,
	   typename Allocator>
  template<typename T1>
  TFEL_INLINE typename 
  tfel::meta::EnableIf<
    tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
    typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template reverse_iterator<T1> >::type
  MultipleDequeVectorContainer<T,Transform,container,Allocator>::rbegin(void)
  {
    return container<typename Transform<T1>::type,
                     typename Allocator::template rebind<T1>::other>::rbegin();
  }

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
      typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template reverse_iterator<T1> >::type
      MultipleDequeVectorContainer<T,Transform,container,Allocator>::rend(void){
      return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::rend();
    }

template<typename T,
	 template<class> class Transform,
	 template<class,class> class container,
	 typename Allocator>
template<typename T1>
TFEL_INLINE typename tfel::meta::EnableIf<
  tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
  typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template const_reverse_iterator<T1> >::type
MultipleDequeVectorContainer<T,Transform,container,Allocator>::rbegin(void) const {
  return container<typename Transform<T1>::type,
    typename Allocator::template rebind<T1>::other>::rbegin();
}

  template<typename T,
	   template<class> class Transform,
	   template<class,class> class container,
	   typename Allocator>
  template<typename T1>
  TFEL_INLINE typename 
  tfel::meta::EnableIf<
    tfel::meta::TypeListCountNbrOfT<T1,T>::value==1,
    typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template const_reverse_iterator<T1> >::type
  MultipleDequeVectorContainer<T,Transform,container,Allocator>::rend(void) const {
    return container<typename Transform<T1>::type,
      typename Allocator::template rebind<T1>::other>::rend();
}

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_MULTIPLEDEQUEVECTOR_IXX */

