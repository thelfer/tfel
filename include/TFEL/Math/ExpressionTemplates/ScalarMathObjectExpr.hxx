/*!
 * \file   ScalarMathObjectExpr.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _MATH_SCALAR_MATHOBJECT_EXPR_LIB_
#define _MATH_SCALAR_MATHOBJECT_EXPR_LIB_ 1

#include <string>
#include<cstddef>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/TypeTraits/IsScalar.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/ScalarObjectRandomAccessConstIterator.hxx"

namespace tfel{

  namespace math {
    
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class ScalarMathObjectExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MathObjectConcept>::cond));
  
      typedef typename MathObjectTraits<B>::NumType   NumTypeB;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      const typename B::RunTimeProperties RTP;
      
      ScalarMathObjectExpr();

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

      TFEL_MATH_INLINE ScalarMathObjectExpr(const A l, const B& r)
	: a(l), b(r), RTP(r.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE ScalarMathObjectExpr(const ScalarMathObjectExpr& src)
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

      typedef ScalarObjectRandomAccessConstIterator<A,B,Op> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE RunTimeProperties 
      getRunTimeProperties(void) const 
      {                                                         
	return RTP;                                             
      }                                                         

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

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class MathObjectScalarExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename MathObjectTraits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;      

      MathObjectScalarExpr();

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

      TFEL_MATH_INLINE MathObjectScalarExpr(const A& l, const B r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE MathObjectScalarExpr(const MathObjectScalarExpr& src)
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

      typedef ObjectScalarRandomAccessConstIterator<A,B,Op> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE RunTimeProperties 
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
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExprSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _MATH_SCALAR_MATHOBJECT_EXPR_LIB_ */
