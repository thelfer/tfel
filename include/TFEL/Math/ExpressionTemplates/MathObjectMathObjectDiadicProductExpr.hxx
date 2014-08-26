/*!
 * \file   MathObjectMathObjectDiadicProductExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 avr 2008
 */

#ifndef _LIB_MATHOBJECTMATHOBJECTDIADICPRODUCTEXPR_HXX_
#define _LIB_MATHOBJECTMATHOBJECTDIADICPRODUCTEXPR_HXX_ 

#include<string>
#include<cstddef>
#include<iterator>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"

namespace tfel{

  namespace math {
    
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B>
    class TFEL_VISIBILITY_LOCAL MathObjectMathObjectDiadicProductExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MathObjectConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpDiadicProduct>::Result Result;

      typedef typename MathObjectTraits<A>::IndexType AIndexType;
      typedef typename MathObjectTraits<B>::IndexType BIndexType;

      typedef typename MathObjectTraits<A>::NumType NumTypeA;
      typedef typename MathObjectTraits<B>::NumType NumTypeB;

      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      MathObjectMathObjectDiadicProductExpr();

    public:

      typedef typename Result::RunTimeProperties RunTimeProperties; 
      typedef typename MathObjectTraits<Result>::IndexType IndexType;
      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;
	    
    protected:

      typedef const A first_arg;
      typedef const B second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef AIndexType     size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      const RunTimeProperties RTP;

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE MathObjectMathObjectDiadicProductExpr(const A& l, const B& r)
	: a(l), b(r),
	  RTP(l.getRunTimeProperties(),r.getRunTimeProperties())
      {}
#else
      TFEL_MATH_INLINE MathObjectMathObjectDiadicProductExpr(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE MathObjectMathObjectDiadicProductExpr(const MathObjectMathObjectDiadicProductExpr& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE NumType 
      operator()(const AIndexType i,
		 const BIndexType j) const 
      {
	return a(i)*b(j);
      }

    public:
      
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return this->RTP;
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_MATHOBJECTMATHOBJECTDIADICPRODUCTEXPR_HXX */

