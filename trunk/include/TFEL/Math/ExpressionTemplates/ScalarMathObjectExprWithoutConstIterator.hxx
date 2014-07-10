/*!
 * \file   ScalarMathObjectExprWithoutConstIterator.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _MATH_SCALARMATHOBJECTEXPRWITHOUTCONSTITERATOR_LIB_
#define _MATH_SCALARMATHOBJECTEXPRWITHOUTCONSTITERATOR_LIB_ 1

#include <string>
#include<cstddef>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"

namespace tfel{

  namespace math {
    
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class ScalarMathObjectExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MathObjectConcept>::cond));
  
      typedef typename MathObjectTraits<B>::NumType   NumTypeB;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      const typename B::RunTimeProperties RTP;
      
      ScalarMathObjectExprWithoutConstIterator();

    public:
      
      typedef typename ComputeBinaryResult<A,NumTypeB,Op>::Handle NumType;
      typedef typename MathObjectTraits<B>::IndexType IndexType;

    protected:

      typedef const A first_arg;
      typedef const B second_arg;
      typedef typename B::RunTimeProperties RunTimeProperties;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE ScalarMathObjectExprWithoutConstIterator(const A l, const B& r)
	: a(l), b(r), RTP(r.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE 
      ScalarMathObjectExprWithoutConstIterator(const ScalarMathObjectExprWithoutConstIterator& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	return Op::apply(a,b(i));
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return Op::apply(a,b(i,j));
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return Op::apply(a,b(i,j,k));
      }

    public:

      TFEL_MATH_INLINE RunTimeProperties 
      getRunTimeProperties(void) const 
      {                                                         
	return RTP;                                             
      }                                                         
      
    };

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class MathObjectScalarExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename MathObjectTraits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;      

      MathObjectScalarExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,Op>::Handle NumType;
      typedef typename MathObjectTraits<A>::IndexType IndexType;
      
    protected:

      typedef const A first_arg;
      typedef const B second_arg;
      typedef typename A::RunTimeProperties RunTimeProperties;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE MathObjectScalarExprWithoutConstIterator(const A& l, const B r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE MathObjectScalarExprWithoutConstIterator(const MathObjectScalarExprWithoutConstIterator& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	return Op::apply(a(i),b);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return Op::apply(a(i,j),b);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return Op::apply(a(i,j,k),b);
      }
      
    public:

      TFEL_MATH_INLINE RunTimeProperties 
      getRunTimeProperties(void) const 
      {                                                         
	return RTP;                                             
      }                                                  

    };
      
  } // end of namespace math

} // end of namespace tfel

#ifndef NO_EXPRESSION_TEMPLATE_SPECIALISATION 
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExprWithoutConstIteratorSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _MATH_SCALARMATHOBJECTEXPRWITHOUTCONSTITERATOR_LIB_ */
