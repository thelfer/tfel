/*!
 * \file   include/TFEL/Math/ExpressionTemplates/MathObjectNegExprWithoutConstIterator.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_MATHOBJECTNEGEXPRWITHOUTCONSTITERATOR_H_
#define _LIB_TFEL_MATHOBJECTNEGEXPRWITHOUTCONSTITERATOR_H_ 

#include<cstddef>

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"

namespace tfel{

  namespace math{

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A>
    class MathObjectNegExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const typename A::RunTimeProperties RTP;

      struct invalid_argument;

      typedef typename MathObjectTraits<A>::NumType   NumTypeA;

      MathObjectNegExprWithoutConstIterator();

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

      TFEL_MATH_INLINE MathObjectNegExprWithoutConstIterator(const A& l)
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

      TFEL_MATH_INLINE RunTimeProperties 
      getRunTimeProperties(void) const 
      {                                                         
	return RTP;                                             
      }                                                         

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATHOBJECTNEGEXPRWITHOUTCONSTITERATOR_H */

