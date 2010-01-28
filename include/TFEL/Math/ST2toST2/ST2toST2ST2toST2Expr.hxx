/*!
 * \file   ST2toST2ST2toST2Expr.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _MATH_ST2TOST2_ST2TOST2_EXPR_LIB_
#define _MATH_ST2TOST2_ST2TOST2_EXPR_LIB_ 1

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/Promote.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/ObjectObjectRandomAccessConstIterator.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B, class Op>
    class ST2toST2ST2toST2Expr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ST2toST2Concept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ST2toST2Concept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,Op>::Result Result;
      typedef ST2toST2Traits<Result> traits;

      typedef typename ST2toST2Traits<A>::NumType NumTypeA;
      typedef typename ST2toST2Traits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,Op>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ST2toST2ST2toST2Expr<")+Name<A>::getName()+string(",")
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

      TFEL_MATH_INLINE ST2toST2ST2toST2Expr(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE ST2toST2ST2toST2Expr(const ST2toST2ST2toST2Expr& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i,const unsigned short j) const 
      {
	return Op::apply(a(i,j),b(i,j));
      }

    public:

      typedef ObjectObjectRandomAccessConstIterator<A,B,Op> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b.begin());
      }
 
      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a.end(),b.end());
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
#include"TFEL/Math/ST2toST2/ST2toST2ST2toST2ExprSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _MATH_ST2TOST2_ST2TOST2_EXPR_LIB_ */
