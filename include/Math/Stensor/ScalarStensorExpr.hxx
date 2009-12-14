/*!
 * \file   ScalarStensorExpr.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _MATH_SCALAR_STENSOR_EXPR_LIB_
#define _MATH_SCALAR_STENSOR_EXPR_LIB_ 1

#include <string>

#include "Config/TFELConfig.hxx"

#include "Metaprogramming/StaticAssert.hxx"
#include "Metaprogramming/Implements.hxx"
#include "TypeTraits/IsScalar.hxx"
#include "TypeTraits/IsTemporary.hxx"
#include "Utilities/Name.hxx"
#include "Math/General/ResultType.hxx"
#include "Math/General/BasicOperations.hxx"
#include "Math/General/ScalarObjectRandomAccessConstIterator.hxx"
#include "Math/Stensor/StensorConcept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B, class Op>
    class ScalarStensorExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,StensorConcept>::cond));
      
      typedef typename StensorTraits<B>::NumType   NumTypeB;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
    public:
      
      typedef typename ComputeBinaryResult<A,NumTypeB,Op>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ScalarStensorExpr<")+Name<A>::getName()+string(",")
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

      TFEL_MATH_INLINE ScalarStensorExpr(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE ScalarStensorExpr(const ScalarStensorExpr<A,B,Op>& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i) const 
      {
	return Op::apply(a,b(i));
      }

    public:

      typedef ScalarObjectRandomAccessConstIterator<A,B,Op> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a,b.begin());
      }

      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a,b.end());
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

    template<typename A, typename B, class Op>
    class StensorScalarExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,StensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename StensorTraits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,Op>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorScalarExpr<")+Name<A>::getName()+string(",")
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

      TFEL_MATH_INLINE StensorScalarExpr(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE StensorScalarExpr(const StensorScalarExpr& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i) const 
      {
	return Op::apply(a(i),b);
      }

    public:

      typedef ObjectScalarRandomAccessConstIterator<A,B,Op> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b);
      }

      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a.end(),b);
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

#ifndef NO_EXPRESSION_TEMPLATE_SPECIALISATION 
#include"Math/Stensor/ScalarStensorExprSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _MATH_SCALAR_STENSOR_EXPR_LIB_ */
