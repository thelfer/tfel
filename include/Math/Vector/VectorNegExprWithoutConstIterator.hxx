/*!
 * \file   VectorNegExprWithoutConstIterator.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_VECTORNEGEXPRWITHOUTCONSTITERATOR_H_
#define _LIB_TFEL_VECTORNEGEXPRWITHOUTCONSTITERATOR_H_ 

#include"Metaprogramming/StaticAssert.hxx"
#include"Metaprogramming/Implements.hxx"
#include"Metaprogramming/InvalidType.hxx"
#include"TypeTraits/IsInvalid.hxx"
#include"TypeTraits/IsTemporary.hxx"
#include"Math/Vector/VectorConcept.hxx"

namespace tfel{

  namespace math{

    template<typename A>
    class VectorNegExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const typename A::RunTimeProperties RTP;

      struct invalid_argument;

      typedef typename VectorTraits<A>::NumType   NumTypeA;
      typedef typename VectorTraits<A>::IndexType IndexType;

      VectorNegExprWithoutConstIterator();

    public:

      typedef typename ComputeUnaryResult<NumTypeA,OpNeg>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorNegExprWithoutConstIterator<")+Name<A>::getName()+string(">");
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
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TFEL_MATH_INLINE VectorNegExprWithoutConstIterator(const A& l)
	: a(l),RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE const NumType
      operator()(const IndexType i) const 
      {
	return -a(i);
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

#endif /* _LIB_TFEL_VECTORNEGEXPRWITHOUTCONSTITERATOR_H */

