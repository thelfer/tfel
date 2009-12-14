/*!
 * \file   StensorStensorExprWithoutConstIterator.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _MATH_STENSOR_STENSOR_EXPRWITHOUTCONSTITERATOR_LIB_
#define _MATH_STENSOR_STENSOR_EXPRWITHOUTCONSTITERATOR_LIB_ 1

#include"Config/TFELConfig.hxx"
#include"Metaprogramming/StaticAssert.hxx"
#include"Metaprogramming/Implements.hxx"
#include"TypeTraits/Promote.hxx"
#include"TypeTraits/IsTemporary.hxx"
#include"Math/General/ResultType.hxx"
#include"Math/General/BasicOperations.hxx"
#include"Math/Stensor/StensorConcept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B, class Op>
    class StensorStensorExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,StensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,StensorConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,Op>::Result Result;
      typedef StensorTraits<Result> traits;

      typedef typename StensorTraits<A>::NumType NumTypeA;
      typedef typename StensorTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,Op>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorStensorExprWithoutConstIterator<")+Name<A>::getName()+string(",")
	  +Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned short size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TFEL_MATH_INLINE StensorStensorExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE StensorStensorExprWithoutConstIterator(const StensorStensorExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i) const 
      {
	return Op::apply(a(i),b(i));
      }

    };
        
  } // end of namespace math

} // end of namespace tfel

#ifndef NO_EXPRESSION_TEMPLATE_SPECIALISATION 
#include"Math/Stensor/StensorStensorExprWithoutConstIteratorSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _MATH_STENSOR_STENSOR_EXPRWITHOUTCONSTITERATOR_LIB_ */
