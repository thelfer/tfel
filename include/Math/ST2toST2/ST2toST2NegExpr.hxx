/*!
 * \file   ST2toST2NegExpr.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_ST2TOST2_NEG_EXPR_H_
#define _LIB_TFEL_ST2TOST2_NEG_EXPR_H_ 

#include"Metaprogramming/StaticAssert.hxx"
#include"Metaprogramming/Implements.hxx"
#include"Metaprogramming/InvalidType.hxx"
#include"TypeTraits/IsTemporary.hxx"
#include"Math/General/NegObjectRandomAccessConstIterator.hxx"
#include"Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel{

  namespace math{

    template<typename A>
    class ST2toST2NegExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ST2toST2Concept>::cond));
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;

      struct invalid_argument;

      typedef typename ST2toST2Traits<A>::NumType   NumTypeA;

    public:

      typedef typename ComputeUnaryResult<NumTypeA,OpNeg>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ST2toST2NegExpr<")+Name<A>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef invalid_argument second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned short size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TFEL_MATH_INLINE ST2toST2NegExpr(const A& l)
	: a(l)
      {}

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned short i,const unsigned short j) const 
      {
	return -a(i,j);
      }

    public:

      typedef NegObjectRandomAccessConstIterator<A> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
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

#endif /* _LIB_TFEL_ST2TOST2_NEG_EXPR_H */

