/*!
 * \file   include/TFEL/Math/ExpressionTemplates/MathObjectMathObjectExpr.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_MATHOBJECT_MATHOBJECT_EXPR_LIB_
#define _LIB_TFEL_MATH_MATHOBJECT_MATHOBJECT_EXPR_LIB_ 1

#include<cstddef>
#include<iterator>
#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"
#include"TFEL/Math/General/ObjectObjectRandomAccessConstIterator.hxx"

namespace tfel{

  namespace math {
    
    /*!
     * \param  MathObjectConcept implemented by objects of type A and B
     * \param  MathObjectTraits, traits used to describe objects of type A and B
     */
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class MathObjectMathObjectExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MathObjectConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,Op>::Result Result;
      typedef MathObjectTraits<Result> traits;

      typedef typename MathObjectTraits<A>::NumType NumTypeA;
      typedef typename MathObjectTraits<B>::NumType NumTypeB;
  
      static constexpr bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static constexpr bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename std::conditional<IsATemporary,const A,const A&>::type a;
      typename std::conditional<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;

      MathObjectMathObjectExpr();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,Op>::Handle NumType;
      typedef typename traits::IndexType IndexType;
      
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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE MathObjectMathObjectExpr(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE MathObjectMathObjectExpr(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE MathObjectMathObjectExpr(const MathObjectMathObjectExpr<MathObjectConcept,
									       MathObjectTraits,A,B,Op>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE NumType
      operator()(const IndexType i) const 
      {
	return Op::apply(a(i),b(i));
      }

      TFEL_MATH_INLINE NumType
      operator()(const IndexType i, 
		 const IndexType j) const 
      {
	return Op::apply(a(i,j),b(i,j));
      }

      TFEL_MATH_INLINE NumType
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return Op::apply(a(i,j,k),b(i,j,k));
      }

    public:
      
      typedef ObjectObjectRandomAccessConstIterator<A,B,Op> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      TFEL_MATH_INLINE
      const_iterator
      begin(void) const
      {
	return const_iterator(a.begin(),b);
      }

      TFEL_MATH_INLINE
      const_iterator
      end(void) const
      {
	return const_iterator(a.end(),b);
      }

      TFEL_MATH_INLINE
      const_reverse_iterator
      rbegin(void) const
      {
	return const_reverse_iterator(end());
      }

      TFEL_MATH_INLINE
      const_reverse_iterator
      rend(void) const
      {
	return const_reverse_iterator(begin());
      }

    };

  } // end of namespace math

} // end of namespace tfel

#ifndef NO_EXPRESSION_TEMPLATE_SPECIALISATION 
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExprSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _LIB_TFEL_MATH_MATHOBJECT_MATHOBJECT_EXPR_LIB_ */
