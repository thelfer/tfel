/*!
 * \file   ST2toST2NegExprWithoutConstIterator.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_ST2TOST2_NEG_EXPRWITHOUTCONSTITERATOR_H_
#define _LIB_TFEL_ST2TOST2_NEG_EXPRWITHOUTCONSTITERATOR_H_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel{

  namespace math{

    template<typename A>
    class ST2toST2NegExprWithoutConstIterator
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
	return string("ST2toST2NegExprWithoutConstIterator<")+Name<A>::getName()+string(">");
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

      TFEL_MATH_INLINE ST2toST2NegExprWithoutConstIterator(const A& l)
	: a(l)
      {}

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned short i,const unsigned short j) const 
      {
	return -a(i,j);
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_ST2TOST2_NEG_EXPRWITHOUTCONSTITERATOR_H */

