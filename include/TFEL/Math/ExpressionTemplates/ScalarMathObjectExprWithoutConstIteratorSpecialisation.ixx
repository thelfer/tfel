/*!
 * \file   ScalarMathObjectExprWithoutConstIteratorSpecialisation.ixx
 * \brief  This file partially specialises the ScalarMathObjectExprWithoutConstIterator and MathObjectScalarExprWithoutConstIterator classes for usual operations.
 * \see    NO_EXPRESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_SCALARMATHOBJECTEXPRWITHOUTCONSTITERATORSPECIALISATION_I_
#define _LIB_TFEL_SCALARMATHOBJECTEXPRWITHOUTCONSTITERATORSPECIALISATION_I_ 

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A,typename B>
    class ScalarMathObjectExprWithoutConstIterator<MathObjectConcept,MathObjectTraits,A,B,OpMult>
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
      
      typedef typename ComputeBinaryResult<A,NumTypeB,OpMult>::Handle NumType;
      typedef typename MathObjectTraits<B>::IndexType IndexType;

    protected:

      typedef A first_arg;
      typedef B second_arg;
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

      TFEL_MATH_INLINE ScalarMathObjectExprWithoutConstIterator(const ScalarMathObjectExprWithoutConstIterator& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	return a*b(i);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return a*b(i,j);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return a*b(i,j,k);
      }

    public:

      TFEL_MATH_INLINE RunTimeProperties 
      getRunTimeProperties(void) const 
      {                                                         
	return RTP;                                             
      }                                                         
      
    };

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A,typename B>
    class MathObjectScalarExprWithoutConstIterator<MathObjectConcept,MathObjectTraits,A,B,OpMult>
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

      typedef typename ComputeBinaryResult<NumTypeA,B,OpMult>::Handle NumType;
      typedef typename MathObjectTraits<A>::IndexType IndexType;
      
    protected:

      typedef A first_arg;
      typedef B second_arg;
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
	return a(i)*b;
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return a(i,j)*b;
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return a(i,j,k)*b;
      }
      
    public:

      TFEL_MATH_INLINE RunTimeProperties 
      getRunTimeProperties(void) const 
      {                                                         
	return RTP;                                             
      }                                                  
      
    };

    /*
     * \brief Partial Specialisation for OpDiv.
     */
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A,typename B>
    class MathObjectScalarExprWithoutConstIterator<MathObjectConcept,MathObjectTraits,A,B,OpDiv>
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

      typedef typename ComputeBinaryResult<NumTypeA,B,OpDiv>::Handle NumType;
      typedef typename MathObjectTraits<A>::IndexType IndexType;
      
    protected:

      typedef A first_arg;
      typedef B second_arg;
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
	return a(i)/b;
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return a(i,j)/b;
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return a(i,j,k)/b;
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


#endif /* _LIB_TFEL_SCALARMATHOBJECTEXPRWITHOUTCONSTITERATORSPECIALISATION_I_ */

