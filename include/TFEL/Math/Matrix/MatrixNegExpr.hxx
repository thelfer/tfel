/*!
 * \file   MatrixNegExpr.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_MATRIX_NEG_EXPR_H_
#define _LIB_TFEL_MATRIX_NEG_EXPR_H_ 

#include<cstddef>

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/NegObjectRandomAccessConstIterator.hxx"
#include"TFEL/Math/Matrix/MatrixConcept.hxx"

namespace tfel{

  namespace math{

    template<typename A>
    class MatrixNegExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MatrixConcept>::cond));
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const typename A::RunTimeProperties RTP;

      struct invalid_argument;

      typedef typename MatrixTraits<A>::NumType   NumTypeA;
      typedef typename MatrixTraits<A>::IndexType IndexType;

    public:

      typedef typename ComputeUnaryResult<NumTypeA,OpNeg>::Handle NumType;

    protected:

      typedef typename A::RunTimeProperties RunTimeProperties;
      typedef A first_arg;
      typedef invalid_argument second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TFEL_MATH_INLINE MatrixNegExpr(const A& l)
	: a(l), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE const NumType
      operator()(const IndexType i, const IndexType j) const 
      {
	return -a(i,j);
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

#endif /* _LIB_TFEL_MATRIX_NEG_EXPR_H */

