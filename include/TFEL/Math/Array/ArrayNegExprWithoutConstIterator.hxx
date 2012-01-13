/*!
 * \file   ArrayNegExprWithoutConstIterator.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_ARRAY_NEG_EXPRWITHOUTCONSTITERATOR_H_
#define _LIB_TFEL_ARRAY_NEG_EXPRWITHOUTCONSTITERATOR_H_ 

#include<cstddef>

#include"TFEL/Metaprogramming/IsSuperClassOf.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/Array/ArrayConcept.hxx"

namespace tfel{

  namespace math{

    template<typename A>
    class ArrayNegExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::IsSuperClassOf<ArrayConcept<A>,A>::cond));
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const typename A::RunTimeProperties RTP;

      typedef typename ArrayTraits<A>::NumType   NumTypeA;

      struct invalid_argument;

    public:

      typedef typename ComputeUnaryResult<NumTypeA,OpNeg>::Handle NumType;

    protected:

      typedef A first_arg;
      typedef invalid_argument second_arg;
      typedef typename A::RunTimeProperties RunTimeProperties;
      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned int   size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TFEL_MATH_INLINE ArrayNegExprWithoutConstIterator(const A& l)
	: a(l), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE 
      ArrayNegExprWithoutConstIterator(const ArrayNegExprWithoutConstIterator& src)
	: a(src.a), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i) const 
      {
	return -a(i);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,
		 const unsigned int j) const 
      {
	return -a(i,j);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,
		 const unsigned int j,
		 const unsigned int k) const 
      {
	return -a(i,j,k);
      }

    public:

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_ARRAY_NEG_EXPRWITHOUTCONSTITERATOR_H */

