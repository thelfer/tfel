/*!
 * \file   ArrayNegExpr.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_ARRAY_NEG_EXPR_H_
#define _LIB_TFEL_ARRAY_NEG_EXPR_H_ 

#include "Metaprogramming/IsSuperClassOf.hxx"
#include "TypeTraits/IsTemporary.hxx"
#include "Math/General/RunTimeCheck.hxx"
#include "Math/General/NegObjectRandomAccessConstIterator.hxx"
#include "Math/Array/ArrayConcept.hxx"

namespace tfel{

  namespace math{

    template<typename A>
    class ArrayNegExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::IsSuperClassOf<ArrayConcept<A>,A>::cond));
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const typename A::RunTimeProperties RTP;

      typedef typename ArrayTraits<A>::NumType   NumTypeA;

      struct invalid_argument;

    public:

      typedef typename ComputeUnaryResult<NumTypeA,OpNeg>::Handle NumType;

      static const std::string getName(void){
	return std::string("ArrayNegExpr<")+tfel::utilities::Name<A>::getName()+std::string(">");
      }

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

      TFEL_MATH_INLINE ArrayNegExpr(const A& l)
	: a(l), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE ArrayNegExpr(const ArrayNegExpr& src)
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

      typedef NegObjectRandomAccessConstIterator<A> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties getRunTimeProperties(void) const
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

#endif /* _LIB_TFEL_ARRAY_NEG_EXPR_H */

