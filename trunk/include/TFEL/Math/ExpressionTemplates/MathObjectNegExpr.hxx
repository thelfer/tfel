/*!
 * \file   MathObjectNegExpr.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_MATHOBJECT_NEG_EXPR_H_
#define _LIB_TFEL_MATHOBJECT_NEG_EXPR_H_ 

#include<cstddef>

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/NegObjectRandomAccessConstIterator.hxx"

namespace tfel{

  namespace math{

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A>
    class MathObjectNegExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const typename A::RunTimeProperties RTP;

      struct invalid_argument;

      typedef typename MathObjectTraits<A>::NumType   NumTypeA;

    public:

      typedef typename ComputeUnaryResult<NumTypeA,OpNeg>::Handle NumType;
      typedef typename MathObjectTraits<A>::IndexType IndexType;

    protected:

      typedef const A first_arg;
      typedef invalid_argument second_arg;
      typedef typename A::RunTimeProperties RunTimeProperties;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TFEL_MATH_INLINE MathObjectNegExpr(const A& l)
	: a(l),RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE NumType
      operator()(const IndexType i) const 
      {
	return -a(i);
      }

      TFEL_MATH_INLINE NumType
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return -a(i,j);
      }

      TFEL_MATH_INLINE NumType
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return -a(i,j,k);
      }

    public:

      typedef NegObjectRandomAccessConstIterator<A> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE RunTimeProperties 
      getRunTimeProperties(void) const 
      {                                                         
	return RTP;                                             
      }                                                         

      TFEL_MATH_INLINE const_iterator begin(void) const                           	
      {											
	return const_iterator(a.begin());        					
      }											
 											
      TFEL_MATH_INLINE const_iterator end(void) const	                                
      {											
	return const_iterator(a.end());   						
      }											
											
      TFEL_MATH_INLINE const_reverse_iterator rbegin(void) const		        
      {											
	return const_reverse_iterator(end());						
      }											
											
      TFEL_MATH_INLINE const_reverse_iterator rend(void) const                          
      {											
	return const_reverse_iterator(begin());						
      }                                                                                 

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATHOBJECT_NEG_EXPR_H */

