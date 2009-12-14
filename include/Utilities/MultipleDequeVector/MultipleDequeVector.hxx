/*!
 * \file   MultipleDequeVector.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   03 May 2006
 */

#ifndef _LIB_TFEL_MULTIPLEDEQUEVECTOR_H_
#define _LIB_TFEL_MULTIPLEDEQUEVECTOR_H_ 

#include <algorithm>

#include "Config/TFELConfig.hxx"

#include "Metaprogramming/StaticAssert.hxx"
#include "Metaprogramming/EnableIf.hxx"
#include "Metaprogramming/TypeList.hxx"

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
  
      TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<T,tfel::meta::TypeList>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::TypeListElementsAreUnique<T>::cond));

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
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	void >::type 
      clear(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	bool >::type 
      empty(void) const;
    
      template<typename T1>
      TFEL_INLINE typename 
      tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::size_type 
      >::type size(void) const;

      template<typename T1>
      TFEL_INLINE typename 
      tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::size_type 
      >::type max_size(void) const;

      template<typename T1>
      TFEL_INLINE typename 
      tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::size_type 
      >::type capacity(void) const;

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	void >::type 
      reserve(typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::size_type s);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	void >::type 
      resize(typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::size_type s);
    
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	void >::type 
      resize(typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::size_type s, const T1 src);

      template<typename T1>
      TFEL_INLINE typename 
      tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	void >::type 
      push_back(const typename Transform<T1>::type& src);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	void >::type 
      pop_back(void);
        
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::reference >::type 
      front(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
			   typename Allocator::template rebind<T1>::other>::const_reference >::type 
      front(void) const;

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
			   typename Allocator::template rebind<T1>::other>::reference 
      >::type back(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	typename container<typename Transform<T1>::type,
			   typename Allocator::template rebind<T1>::other>::const_reference >::type 
      back(void) const;
    
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	void >::type 
      swap(container<typename Transform<T1>::type,
	             typename Allocator::template rebind<T1>::other>& src);
    
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	iterator<T1> >::type 
      insert(iterator<T1>,const T1&);

      template <typename T1,class InputIterator>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	void >::type 
      insert(iterator<T1>,InputIterator,InputIterator);
    
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	void >::type 
      insert(iterator<T1>,
	     typename container<typename Transform<T1>::type,
	     typename Allocator::template rebind<T1>::other>::size_type,
	     const T1&);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	iterator<T1> >::type 
      begin(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	iterator<T1> >::type 
      end(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	const_iterator<T1> >::type 
      begin(void) const;

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	const_iterator<T1> >::type 
      end(void) const;
        
      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	reverse_iterator<T1> >::type 
      rbegin(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	reverse_iterator<T1> >::type 
      rend(void);

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	const_reverse_iterator<T1> >::type 
      rbegin(void) const;

      template<typename T1>
      TFEL_INLINE typename tfel::meta::EnableIf<
	tfel::meta::TypeListCountNbrOfT<T1,T>::value==1, 
	const_reverse_iterator<T1> >::type 
      rend(void) const;

    };
    
  } // end of namespace utilities

} // end of namespace tfel

#include "Utilities/MultipleDequeVector/MultipleDequeVector.ixx"

#endif /* _LIB_TFEL_MULTIPLEDEQUEVECTOR_H */

