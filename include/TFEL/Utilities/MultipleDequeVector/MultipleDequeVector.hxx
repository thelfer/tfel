/*!
 * \file   include/TFEL/Utilities/MultipleDequeVector/MultipleDequeVector.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   03 May 2006
 */

#ifndef _LIB_TFEL_MULTIPLEDEQUEVECTOR_H_
#define _LIB_TFEL_MULTIPLEDEQUEVECTOR_H_ 

#include <algorithm>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/TypeList.hxx"

namespace tfel{
  
  namespace utilities{

    template<typename T, template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    class MultipleDequeVectorContainer_;

    template<typename T, template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator = std::allocator<int> >
    class MultipleDequeVectorContainer
      : public tfel::utilities::MultipleDequeVectorContainer_<T,Transform,container,Allocator>
    {

    private:
  
      TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<T,tfel::meta::TL>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::TLElementsAreUnique<T>::cond));

    public:

      template<typename T1>
      struct iterator;

      template<typename T1>
      struct reverse_iterator;
      
      template<typename T1>
      struct const_iterator;

      template<typename T1>
      struct const_reverse_iterator;
    
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	void >::type 
      clear(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	bool >::type 
      empty(void) const;
    
      template<typename T1>
      TFEL_INLINE typename 
      tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
			   typename Allocator::template rebind<T1>::other>::size_type 
      >::type size(void) const
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::size();
      }

      template<typename T1>
      TFEL_INLINE typename 
      tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::size_type 
      >::type max_size(void) const
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::max_size();
      }

      template<typename T1>
      TFEL_INLINE typename 
      tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::size_type 
      >::type capacity(void) const
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::capacity();
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	void >::type 
      reserve(typename container<typename Transform<T1>::type,
	      typename Allocator::template rebind<T1>::other>::size_type s)
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::reserve(s);
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	void >::type 
      resize(typename container<typename Transform<T1>::type,
	     typename Allocator::template rebind<T1>::other>::size_type s)
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::resize(s);
      }
      
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	void >::type 
      resize(typename container<typename Transform<T1>::type,
	     typename Allocator::template rebind<T1>::other>::size_type s, const T1 src)
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::resize(s,src);
      }

      template<typename T1>
      TFEL_INLINE typename 
      tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	void >::type 
      push_back(const typename Transform<T1>::type& src)
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::push_back(src);
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	void >::type 
      pop_back(void)
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::pop_back();
      }
        
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::reference >::type 
      front(void)
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::front();
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
			   typename Allocator::template rebind<T1>::other>::const_reference >::type 
      front(void) const
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::front();
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
			   typename Allocator::template rebind<T1>::other>::reference 
      >::type back(void)
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::back();
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
			   typename Allocator::template rebind<T1>::other>::const_reference >::type 
      back(void) const
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::back();
      }
    
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	void >::type 
      swap(container<typename Transform<T1>::type,
	             typename Allocator::template rebind<T1>::other>& src)
      {
	container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::swap(src);
      }
    
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	iterator<T1> >::type 
      insert(iterator<T1> pos,const T1& src)
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::insert(pos,src);
      }

      template <typename T1,class InputIterator>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	void >::type 
      insert(iterator<T1> pos,InputIterator f,InputIterator l)
      {
	return container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::insert(pos,f,l);
      }
    
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	void >::type 
      insert(iterator<T1> pos,
	     typename container<typename Transform<T1>::type,
	     typename Allocator::template rebind<T1>::other>::size_type n,
	     const T1& x)
      {
	container<typename Transform<T1>::type,
	typename Allocator::template rebind<T1>::other>::insert(pos,n,x);
      }

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	iterator<T1> >::type 
      begin(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	iterator<T1> >::type 
      end(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	const_iterator<T1> >::type 
      begin(void) const;

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	const_iterator<T1> >::type 
      end(void) const;
        
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	reverse_iterator<T1> >::type 
      rbegin(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	reverse_iterator<T1> >::type 
      rend(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	const_reverse_iterator<T1> >::type 
      rbegin(void) const;

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TLCountNbrOfT<T1,T>::value==1, 
	const_reverse_iterator<T1> >::type 
      rend(void) const;

    };
    
  } // end of namespace utilities

} // end of namespace tfel

#include"TFEL/Utilities/MultipleDequeVector/MultipleDequeVector.ixx"

#endif /* _LIB_TFEL_MULTIPLEDEQUEVECTOR_H */

