/*!
 * \file   include/TFEL/Utilities/MultipleDequeVector/MultipleDequeVector.ixx
 * \brief  This file implements the MultipleDequeVector class.
 * \author Helfer Thomas
 * \date   31 mai 2006
 */

#ifndef _LIB_TFEL_MULTIPLEDEQUEVECTOR_IXX_
#define _LIB_TFEL_MULTIPLEDEQUEVECTOR_IXX_ 

#include"TFEL/Utilities/MultipleDequeVector/MultipleDequeVectorIterators.ixx"

namespace tfel{

  namespace utilities{

    /*
     * \brief Partial specialisation to end the recursion
     * \see   MultipleDequeVectorContainer_
     */
    template<template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    class MultipleDequeVectorContainer_<tfel::meta::TLE,
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
	       TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<T,tfel::meta::TL>::cond));
	     };
    
    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    TFEL_INLINE typename tfel::meta::EnableIf<
      tfel::meta::TLCountNbrOfT<T1,T>::value==1,
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
      tfel::meta::TLCountNbrOfT<T1,T>::value==1,
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
      tfel::meta::TLCountNbrOfT<T1,T>::value==1,
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
      tfel::meta::TLCountNbrOfT<T1,T>::value==1,
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
      tfel::meta::TLCountNbrOfT<T1,T>::value==1,
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
      tfel::meta::TLCountNbrOfT<T1,T>::value==1,
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
      tfel::meta::TLCountNbrOfT<T1,T>::value==1,
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
      tfel::meta::TLCountNbrOfT<T1,T>::value==1,
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
      tfel::meta::TLCountNbrOfT<T1,T>::value==1,
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
      tfel::meta::TLCountNbrOfT<T1,T>::value==1,
      typename MultipleDequeVectorContainer<T,Transform,container,Allocator>::template const_reverse_iterator<T1> >::type
    MultipleDequeVectorContainer<T,Transform,container,Allocator>::rend(void) const {
      return container<typename Transform<T1>::type,
      typename Allocator::template rebind<T1>::other>::rend();
    }

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_MULTIPLEDEQUEVECTOR_IXX */

