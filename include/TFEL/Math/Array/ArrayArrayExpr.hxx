/*!
 * \file   ArrayArrayExpr.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _TFEL_MATH_ARRAY_ARRAY_EXPR_LIB_
#define _TFEL_MATH_ARRAY_ARRAY_EXPR_LIB_ 1

#include <string>
#include<cstddef>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"

#include"TFEL/TypeTraits/IsTemporary.hxx"

#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"
#include"TFEL/Math/General/ObjectObjectRandomAccessConstIterator.hxx"
#include"TFEL/Math/Array/ArrayConcept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B, class Op>
    class ArrayArrayExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT(ArrayTraits<A>::dimension==ArrayTraits<B>::dimension);
      TFEL_STATIC_ASSERT((tfel::meta::IsSameType<typename A::RunTimeProperties,
		                            typename B::RunTimeProperties>::cond));

      typedef typename ComputeBinaryResult<A,B,Op>::Result Result;
      typedef ArrayTraits<Result> traits;

      typedef typename ArrayTraits<A>::NumType NumTypeA;
      typedef typename ArrayTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
      const typename A::RunTimeProperties RTP;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,Op>::Handle NumType;

    protected:

      typedef A first_arg;
      typedef B second_arg;

      typedef typename A::RunTimeProperties RunTimeProperties;
      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned int   size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	
      
#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE ArrayArrayExpr(const A& l, const B& r)
	: a(l), b(r), 
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE ArrayArrayExpr(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}
#endif

      TFEL_MATH_INLINE ArrayArrayExpr(const ArrayArrayExpr& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i) const 
      {
	return Op::apply(a(i),b(i));
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i, 
		 const unsigned int j) const 
      {
	return Op::apply(a(i,j),b(i,j));
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,
		 const unsigned int j,
		 const unsigned int k) const 
      {
	return Op::apply(a(i,j,k),b(i,j,k));
      }

    public:
      
      typedef ObjectObjectRandomAccessConstIterator<A,B,Op> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

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
#include"TFEL/Math/Array/ArrayArrayExprSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _TFEL_MATH_ARRAY_ARRAY_EXPR_LIB_ */
