/*!
 * \file   ScalarST2toST2ExprWithoutConstIterator.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _MATH_SCALAR_ST2TOST2_EXPRWITHOUTCONSTITERATOR_LIB_
#define _MATH_SCALAR_ST2TOST2_EXPRWITHOUTCONSTITERATOR_LIB_ 1

#include <string>

#include "Config/TFELConfig.hxx"

#include "Metaprogramming/StaticAssert.hxx"
#include "Metaprogramming/Implements.hxx"
#include "TypeTraits/IsScalar.hxx"
#include "TypeTraits/IsTemporary.hxx"
#include "Utilities/Name.hxx"
#include "Math/General/ResultType.hxx"
#include "Math/General/BasicOperations.hxx"
#include "Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B, class Op>
    class ScalarST2toST2ExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ST2toST2Concept>::cond));
      
      typedef typename ST2toST2Traits<B>::NumType   NumTypeB;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
    public:
      
      typedef typename ComputeBinaryResult<A,NumTypeB,Op>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ScalarST2toST2ExprWithoutConstIterator<")+Name<A>::getName()+string(",")
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

      TFEL_MATH_INLINE ScalarST2toST2ExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE 
      ScalarST2toST2ExprWithoutConstIterator(const ScalarST2toST2ExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i,const unsigned short j) const 
      {
	return Op::apply(a,b(i,j));
      }

    };

    template<typename A, typename B, class Op>
    class ST2toST2ScalarExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ST2toST2Concept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename ST2toST2Traits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,Op>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ST2toST2ScalarExprWithoutConstIterator<")+Name<A>::getName()+string(",")
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

      TFEL_MATH_INLINE ST2toST2ScalarExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE 
      ST2toST2ScalarExprWithoutConstIterator(const ST2toST2ScalarExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i,const unsigned short j) const 
      {
	return Op::apply(a(i,j),b);
      }

    };
        
  } // end of namespace math

} // end of namespace tfel

#ifndef NO_EXPRESSION_TEMPLATE_SPECIALISATION 
#include"Math/ST2toST2/ScalarST2toST2ExprWithoutConstIteratorSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _MATH_SCALAR_ST2TOST2_EXPRWITHOUTCONSTITERATOR_LIB_ */
